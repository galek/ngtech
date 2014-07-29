//Copyright � 2014 Sony Computer Entertainment America LLC. See License.txt.

using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Text;
using System.Windows.Forms;

using Perforce.P4;


namespace Sce.Atf.Perforce
{
    /// <summary>
    /// Manages connection to Perforce server connections, using the P4API.NET</summary>
    public class ConnectionManager : IDisposable
    {
        /// <summary>
        /// Connection changed event</summary>
        public event EventHandler ConnectionChanged;
        /// <summary>
        /// Login canceled event</summary>
        public event EventHandler LoginCanceled;

        /// <summary>
        /// Gets or sets a string indicating recent connections to Perforce server</summary>
        public string ConnectionHistory
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                int numItems = 0;
                foreach (string connection in m_recentConnections)
                {
                    ++numItems;
                    if (numItems > MaxConnections)
                        break;
                    sb.Append(connection);
                    sb.Append(Environment.NewLine);
                }
                return sb.ToString();
            }

            set
            {
                StringReader strReader = new StringReader(value);
                string line = strReader.ReadLine();

                while (line != null)
                {
                    if (m_recentConnections.Count < MaxConnections)
                        if (m_recentConnections.IndexOf(line) == -1)
                            m_recentConnections.Add(line);
                    line = strReader.ReadLine();
                }
            }
        }

        /// <summary>
        /// Gets or sets a string list of recent connections to Perforce server</summary>
        public List<string> RecentConnections
        {
            get { return m_recentConnections; }
            set { m_recentConnections = value; }
        }

        /// <summary>
        /// Gets or sets a string indicating the default connection to Perforce server</summary>
        public string DefaultConnection
        {
            get { return m_defaultConnection; }
            set { m_defaultConnection = value; }
        }

        /// <summary>
        /// Shows an error message to the user</summary>
        /// <param name="message">Error message</param>
        protected void ShowErrorMessage(string message)
        {
            Outputs.WriteLine(OutputMessageType.Warning, message);
        }


        /// <summary> 
        /// Gets or sets whether the source control server is connected</summary>
        public bool IsConnected
        {
            get
            {
                lock (m_lock)
                {
                    if (!m_connectionInitialized || m_connection == null)
                        return false;
                    return m_connection.Status == ConnectionStatus.Connected;
                }
            }          
        }

        /// <summary>
        /// Gets or sets whether the source control service should throw exceptions 
        /// caused by run time errors from the server</summary>
        /// <remarks>The default value is false (turnoff exceptions)</remarks>
        public bool ThrowExceptions { get; set; }

        internal bool InitializeConnection(string selectedConnection = null)
        {
            if (m_invalidPlatform || m_connecting || m_configuring)
                return false;
            if (!m_connectionInitialized)
            {
                string[] connectionConfig = null;
                if (!string.IsNullOrWhiteSpace(selectedConnection))
                    connectionConfig = ExtractConnectionParts(selectedConnection);
                else if (!string.IsNullOrWhiteSpace(DefaultConnection))
                    connectionConfig = ExtractConnectionParts(DefaultConnection);
                if (connectionConfig != null && connectionConfig.Length == 3)
                {
                    Server server = new Server(new ServerAddress(connectionConfig[0]));
                    
                    lock (m_lock)
                    {
                        m_repository = new Repository(server);
                        m_connection = m_repository.Connection;
                        m_connection.UserName = connectionConfig[1];
                        m_connection.Client = new Client();
                        m_connection.Client.Name = connectionConfig[2];
                    }

                    if (ValidateConnection(true))
                    {
                        m_connectionInitialized = true;
                    }
                }
                
            }
            return m_connectionInitialized;
        }

        internal void DestroyConnection()
        {
            lock (m_lock)
            {
                if (m_connection != null)
                    m_connection.Disconnect(null);
            }
            m_connectionInitialized = false;
        }

        /// <summary>
        /// Creates a new Changelist and add add a new change to it</summary>
        /// <param name="description">Description of change</param>
        /// <returns>New Changelist with added change</returns>
        public Changelist CreateChangelist(string description)
        {
            Changelist cl = new Changelist();
            cl.Description = description;
            lock (m_lock)
            {
                cl.ClientId = m_connection.Client.Name;
                cl = m_repository.CreateChangelist(cl);
            }
            return cl;
        }

        /// <summary>
        /// Gets the current connection</summary>
        public  string CurrentConnection
        {
            get
            {
                lock (m_lock)
                {
                    if (m_connection == null)
                        return string.Empty;
                    return GetConnectionConfig(m_connection);
                }
            }
        }

        /// <summary>
        /// Returns an enumeration of user names for the given server's repository</summary>
        /// <param name="serverAddress">Server</param>
        /// <returns>Enumeration of user names</returns>
        public  IEnumerable<string> GetUsers(string serverAddress)
        {
            var result = new List<string>();

            try
            {

                Server server = new Server(new ServerAddress(serverAddress));
                var repository = new Repository(server);
                repository.Connection.Connect(null); // this call ensures internal P4Server is set for repository.Connection , otherwise GetUsers() call will fail

                Options opts = new Options(UsersCmdFlags.IncludeAll, -1);
                foreach (var user in repository.GetUsers(opts))
                    result.Add(user.Id);

            }
            catch (P4Exception ex)
            {
                switch (ex.ErrorLevel)
                {
                    case ErrorSeverity.E_WARN:
                        Outputs.WriteLine(OutputMessageType.Warning, ex.Message);
                        break;
                    case ErrorSeverity.E_FAILED:
                        Outputs.WriteLine(OutputMessageType.Error, ex.Message);
                        break;
                    case ErrorSeverity.E_INFO:
                        Outputs.WriteLine(OutputMessageType.Info, ex.Message);
                        break;
                }
                if (ThrowExceptions)
                    throw;
            }
            catch (Exception ex)
            {
                Outputs.WriteLine(OutputMessageType.Error, ex.Message);
                if (ThrowExceptions)
                    throw;
            }
          
            return result;
        }

        /// <summary>
        /// Gets list of workspaces for a given server and user</summary>
        /// <param name="serverAddress">Server address</param>
        /// <param name="userId">User ID</param>
        /// <returns>List of workspaces</returns>
        public IEnumerable<string> GetWorkspaces(string serverAddress, string userId)
        {
  
            var result = new List<string>();
            try
            {

                Server server = new Server(new ServerAddress(serverAddress));
                var repository = new Repository(server);
                repository.Connection.Connect(null); // this call ensures internal P4Server is set for repository.Connection , otherwise GetUsers() call will fail
                if (CheckLogin(repository.Connection))
                {
                    Options opts = new Options();
                    opts.Add("-u", userId); //The -u user flag lists client workspaces that are owned by the  specified user. 

                    foreach (var client in repository.GetClients(opts))
                        result.Add(client.Name);
                }

            }
            catch (P4Exception ex)
            {
                switch (ex.ErrorLevel)
                {
                    case ErrorSeverity.E_WARN:
                        Outputs.WriteLine(OutputMessageType.Warning, ex.Message);
                        break;
                    case ErrorSeverity.E_FAILED:
                        Outputs.WriteLine(OutputMessageType.Error, ex.Message);
                        break;
                    case ErrorSeverity.E_INFO:
                        Outputs.WriteLine(OutputMessageType.Info, ex.Message);
                        break;
                }
                if (ThrowExceptions)
                    throw;
            }
          
            return result;
        }

        /// <summary>
        /// Sets up server/client connection information</summary>
        /// <returns>True iff information set up</returns>
        public bool ConfigureConnection()
        {
            bool result = false;
            var dlg = new Connections(this);
            if ((MainForm != null) && (MainForm.Icon != null))
                dlg.Icon = MainForm.Icon;
            DialogResult dr = dlg.ShowDialog(MainForm);
            if (dr == DialogResult.OK)
            {
                string oldConnection = CurrentConnection;

                lock (m_lock)
                {
                    if (m_connection != null)
                    {
                        m_connection.Dispose();
                        m_connection = null;
                    }
                }
                m_connectionInitialized = false;
                m_configuring = true; // nullify InitializeConnection() 

                string[] connectionConfig = ExtractConnectionParts(dlg.ConnectionSelected);
                if (connectionConfig != null && connectionConfig.Length == 3)
                {
                    Server server = new Server(new ServerAddress(connectionConfig[0]));
                    lock (m_lock)
                    {
                        m_repository = new Repository(server);
                        m_connection = m_repository.Connection;
                        m_connection.UserName = connectionConfig[1];
                        m_connection.Client = new Client();
                        m_connection.Client.Name = connectionConfig[2];
                    }
                }

                if (ValidateConnection(true))// a valid connection
                {
                    m_connectionInitialized = true;

                    if (dlg.UseAsDefaultConnection)
                        DefaultConnection = CurrentConnection;

                    int index = RecentConnections.IndexOf(CurrentConnection);
                    if (index != -1)
                        RecentConnections.RemoveAt(index);
                    RecentConnections.Insert(0, CurrentConnection);
                    if (CurrentConnection != oldConnection)
                        OnConnectionChanged(EventArgs.Empty);        
                }          
            }
            else if (dr == DialogResult.Cancel)
            {
                
            }
            m_configuring = false;
            return result;
        }

        /// <summary>
        /// Connects to the Perforce server and logs in, if there is not already a connection.</summary>
        /// <returns>Returns true if a connection was established and the log-in was successful and false otherwise.</returns>
        internal bool ValidateConnection(bool checkLogin)
        {
            if (m_invalidPlatform || m_connecting)
                return false;
            lock (m_lock)
            {
                if (m_connection == null)
                    return false;
                if (m_connection.Status == ConnectionStatus.Disconnected)
                    m_connection.Connect(null); // try to connect
                if (m_connection.Status == ConnectionStatus.Disconnected)
                    return false;
            }

            bool result = true;
            if (checkLogin) //verifies that login -s does not have errors
            {
                m_connecting = true;
                bool checkLoginSucceeded;
                lock (m_lock)
                    checkLoginSucceeded = CheckLogin(m_connection);
                if (!checkLoginSucceeded)
                {
                    OnLoginCanceled(EventArgs.Empty);
                    result = false;
                }
            }
            m_connecting = false;
            return result;
        }

        // return false if user canceled login
        private  bool CheckLogin(Connection connection)
        {
            bool result = true;
            bool requireLogin = false;
            try
            {
                var cmd = new P4Command(connection, "login", true, "-s");
                cmd.Run(null);
            }
            catch (P4Exception ex)
            {
                if (ex.ErrorLevel >= ErrorSeverity.E_FAILED) //"Perforce password (P4PASSWD) invalid or unset.\n"}    int
                    requireLogin = true;
            }

            LoginDialog loginDialog = null;

            while (requireLogin)
            {
                if (loginDialog == null)
                {
                    loginDialog = new LoginDialog();
                    string message = string.Format("For user {0} with workspace {1} on server {2}.".Localize(),
                        connection.UserName, connection.Client.Name, connection.Server.Address.Uri);

                    loginDialog.SetConnectionLabel(message);
                    if ((MainForm != null) && (MainForm.Icon != null))
                        loginDialog.Icon = MainForm.Icon;
                }

                DialogResult dr = loginDialog.ShowDialog(MainForm);
                if (dr == DialogResult.OK)
                {
                    try
                    {
                        var credential = connection.Login(loginDialog.Password);
                        requireLogin = credential == null;

                    }
                    catch (P4Exception e)
                    {
                        ShowErrorMessage(e.Message);
                        if (ThrowExceptions)
                            throw;
                    }
                }
                else
                {
                    result = false;
                    break;
                }
            }
            return result;
        }

       
        /// <summary>
        /// Raises ConnectionChanged event</summary>
        /// <param name="e">Event arguments</param>
        protected virtual void OnConnectionChanged(EventArgs e)
        {
            ConnectionChanged.Raise(this, e);
        }

        /// <summary>
        /// Raises LoginCanceled event</summary>
        /// <param name="e">Event arguments</param>
        protected virtual void OnLoginCanceled(EventArgs e)
        {
            LoginCanceled.Raise(this, e);
        }

        private static string GetConnectionConfig(Connection c)
        {
            if (c == null)
                return "";

            return c.Server.Address + "," + c.UserName + "," + c.Client.Name;
        }

        // return server, user, and workspace
        private string[] ExtractConnectionParts(string connectionConfig)
        {
            char[] separator = new char[1] { ',' };
            string[] tokens = connectionConfig.Split(separator);
            if (tokens.Length == 3)
            {
                if (tokens[2].EndsWith(DefaultConnectionMarker))
                    tokens[2] = tokens[2].Substring(0, tokens[2].Length - DefaultConnectionMarker.Length);
               
            }
            return tokens;
        }
        /// <summary>
        /// Gets or sets default connection marker</summary>
        public string DefaultConnectionMarker
        {
            get { return m_defaultConnectionMarker; }
            set { m_defaultConnectionMarker = value; }
        }


        internal P4Command CreateCommand(string command, params string[] args)
        {
            lock (m_lock)
                return new P4Command(m_connection, command, true, args);
        }

        internal string UserName
        {
            get
            {
                lock (m_lock)
                    return m_connection.UserName;
            }
        }

#pragma warning disable 649 // Field is never assigned to and will always have its default value null

        /// <summary>
        /// Gets or sets main form</summary>
        [Import(AllowDefault = true)] // optional service
        protected Form MainForm { get; set; }

#pragma warning restore 649

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or
        /// resetting unmanaged resources</summary>
        public void Dispose()
        {
            lock (m_lock)
            {
                if (m_connection != null)
                {
                    if (m_connection.Status == ConnectionStatus.Connected)
                    {
                        m_connection.Disconnect();
                        m_connection.Dispose();
                        m_connection = null;
                    }
                }
            }
        }

        private const int MaxConnections = 8;

        private string m_defaultConnectionMarker = " (Default)";

        private List<string> m_recentConnections = new List<string>();
        private string m_defaultConnection;

        //All access to m_connection and m_repository must be through this lock.
        // P4API.NET does not support multi-threaded access to the same "connection".
        // http://www.perforce.com/perforce/doc.current/user/p4api.netnotes.txt
        private object m_lock = new object();
        private Connection m_connection; //lock m_lock before using!
        private Repository m_repository; //lock m_lock before using!

        private bool m_invalidPlatform = false;
        private bool m_connectionInitialized; //whether or not ValidateConnection() succeeded
        private bool m_connecting = false; //true if we're attempting to connect, to avoid reentrancy while showing a dialog
        private bool m_configuring; //true if we're attempting to configure from dialog, to avoid connection attempts during this time
    }
}
