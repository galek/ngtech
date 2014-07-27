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
    public partial class Form1 : Form
    {
        private EngineCLR.EngineCLR engine = null;

        public Form1()
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

        }
    }
}
