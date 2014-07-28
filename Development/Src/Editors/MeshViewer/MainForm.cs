using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace MeshViewer
{
    public partial class MainForm : Form
    {
        private EngineCLR.EngineCLR engine = null;
        private CodeEditor.CEForm mCF = null;

        public MainForm()
        {
            InitializeComponent();
        }

        public void DoUpdate()
        {
            Application.DoEvents();
            if (engine != null)
                engine.Update();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            this.engine = new EngineCLR.EngineCLR(this.splitContainer1.Panel1.Handle.ToInt32());
            engine.EngineInit();
            mCF = new CodeEditor.CEForm();
        }

        private void codeEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mCF == null)
                mCF = new CodeEditor.CEForm();
            mCF.Show();
        }
    }
}
