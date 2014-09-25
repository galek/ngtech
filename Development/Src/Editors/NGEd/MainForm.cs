using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace NGEd
{
    public partial class MainForm : Form
    {
        private EngineCLR.EngineCLR engine = null;
        private GraphNodes.LogicEditorForm mLogicEditor = null;

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
            engine.Resize(this.Width, this.Height);
            mLogicEditor = new GraphNodes.LogicEditorForm();
        }

        private void codeEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void splitContainer1_Panel1_Resize(object sender, EventArgs e)
        {
            if (this.engine != null) this.engine.Resize(this.Width, this.Height);
        }

        private void splitContainer1_Panel1_MouseDown(object sender, MouseEventArgs e)
        {
            var grab = this.engine.isGrabbed();
            this.engine.SetGrabbed(!grab);
        }
        private void splitContainer1_Panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
                MouseRightBActions(sender, e);
        }
        private void MouseRightBActions(object sender, MouseEventArgs e)
        {
            var grab = this.engine.isGrabbed();
            if (grab)
                CameraActions(1, e.X - splitContainer1.Width / 2, e.Y - splitContainer1.Height / 2);

        }
        private void splitContainer1_Panel1_MouseUp(object sender, MouseEventArgs e)
        {
            this.engine.SetGrabbed(false);
        }

        private void CameraActions(int _action, int x, int y)
        {
            switch (_action)
            {
                case 1:
                    this.engine.CameraSetDirection(x, y);
                    break;
                case 2:
                    //Nick:TODO("Actions")
                    break;
                default:
                    //Nick:TODO("Actions")
                    break;
            }
        }

        private void playStandalloneToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void simulateToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.engine.PauseEngine(!simulateToolStripMenuItem.Checked);
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.InitialDirectory = "../";
            openFileDialog1.Filter = "Mesh files (*.nggf)|*.nggf|All files (*.*)|*.*";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                this.engine.LoadEngineFormat(openFileDialog1.SafeFileName); 
            }
        }

        private void logicEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mLogicEditor == null)
                mLogicEditor = new GraphNodes.LogicEditorForm();
            mLogicEditor.ShowDialog();
        }
    }
}