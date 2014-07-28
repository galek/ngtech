//Copyright © 2014 Sony Computer Entertainment America LLC. See License.txt.

using System;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.Threading;
using System.Windows.Forms;

using Sce.Atf;
using Sce.Atf.Applications;
using Sce.Atf.Perforce;

namespace CodeEditor
{
    /// <summary>
    /// This is a code editor sample application.
    /// This editor uses the ActiproSoftare SyntaxEditor to provide an editing Control.
    /// It provides language syntax sensitive editing for plain text, C#, Lua, Squirrel,
    /// Python, XML, COLLADA and Cg files.
    /// For more information, see https://github.com/SonyWWS/ATF/wiki/ATF-Code-Editor-Sample. </summary>
    public class CEForm
    {
        public CEForm()
        {
            var catalog = new TypeCatalog(
                typeof(SettingsService),                // persistent settings and user preferences dialog
                typeof(StatusService),                  // status bar at bottom of main Form
                typeof(CommandService),                 // handles commands in menus and toolbars
                typeof(ControlHostService),             // docking control host
                typeof(WindowLayoutService),            // multiple window layout support
                typeof(WindowLayoutServiceCommands),    // window layout commands
                typeof(FileDialogService),              // standard Windows file dialogs
                typeof(AutoDocumentService),            // opens documents from last session, or creates a new document, on startup
                typeof(Outputs),                        // service that provides static methods for writing to IOutputWriter objects.
                typeof(OutputService),                  // rich text box for displaying error and warning messages. Implements IOutputWriter.
                typeof(RecentDocumentCommands),         // standard recent document commands in File menu
                typeof(StandardFileCommands),           // standard File menu commands for New, Open, Save, SaveAs, Close
                typeof(StandardFileExitCommand),        // standard File exit menu command
                typeof(HelpAboutCommand),               // Help -> About command
                typeof(AtfUsageLogger),                 // logs computer info to an ATF server
                typeof(CrashLogger),                    // logs unhandled exceptions to an ATF server
                typeof(UnhandledExceptionService),      // catches unhandled exceptions, displays info, and gives user a chance to save
                typeof(ContextRegistry),                // central context registry with change notification
                typeof(DocumentRegistry),               // central document registry with change notification
                typeof(MainWindowTitleService),         // tracks document changes and updates main form title
                typeof(TabbedControlSelector),          // enable ctrl-tab selection of documents and controls within the app
                typeof(DefaultTabCommands),             // provides the default commands related to document tab Controls
                typeof(Editor),                         // code editor component
                typeof(PythonService),                  // scripting service for automated tests
                typeof(ScriptConsole),                  // provides a dockable command console for entering Python commands
                typeof(AtfScriptVariables),             // exposes common ATF services as script variables
                typeof(AutomationService),              // provides facilities to run an automated script using the .NET remoting service
                typeof(PerforceService),                // Perforce plugin
                typeof(SourceControlCommands),          // source control commmands to interact with Perforce plugin
                typeof(SourceControlContext)            // source control context component
                );

            container = new CompositionContainer(catalog);

            var toolStripContainer = new ToolStripContainer();
            toolStripContainer.Dock = DockStyle.Fill;

            mainForm = new MainForm(toolStripContainer);

            mainForm.Text = "Code Editor";

            var batch = new CompositionBatch();
            batch.AddPart(mainForm);
            batch.AddPart(new WebHelpCommands("https://github.com/SonyWWS/ATF/wiki/ATF-Code-Editor-Sample".Localize()));
            container.Compose(batch);

            var tabCommands = container.GetExportedValue<DefaultTabCommands>();
            tabCommands.IsDocumentControl = controlInfo => controlInfo.Client is Editor;

            container.InitializeAll();
        }


        ~CEForm()
        {
            container.Dispose();
        }
        public void Show()
        {
            // Show the main form and start message handling. The main Form Load event provides a final chance
            //  for components to perform initialization and configuration.
            mainForm.ShowDialog();
        }
        private CompositionContainer container;
        private MainForm mainForm;
    }
}