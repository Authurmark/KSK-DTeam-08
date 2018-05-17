using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MaterialSkin.Controls;
using MaterialSkin.Animations;
using MaterialSkin;

namespace K08.UI_Control
{
    public partial class MainPanelControl : Form
    {
        public MainPanelControl()
        {
            InitializeComponent();
            pnActiveProcess.Height = btManualControl.Height;
            pnActiveProcess.Top = btManualControl.Top;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            
        }

        private void btManualControl_Click(object sender, EventArgs e)
        {
            pnActiveProcess.Height = btManualControl.Height;
            pnActiveProcess.Top = btManualControl.Top;
        }

        private void btResetStartPoint_Click(object sender, EventArgs e)
        {
            pnActiveProcess.Height = btResetStartPoint.Height;
            pnActiveProcess.Top = btResetStartPoint.Top;
        }

        private void btResetHomeProcess_Click(object sender, EventArgs e)
        {
            pnActiveProcess.Height = btResetHomeProcess.Height;
            pnActiveProcess.Top = btResetHomeProcess.Top;
        }

        private void btResetCheckerProcess_Click(object sender, EventArgs e)
        {

            pnActiveProcess.Height = btResetCheckerProcess.Height;
            pnActiveProcess.Top = btResetCheckerProcess.Top;
        }

        private void btScanHoleProcess_Click(object sender, EventArgs e)
        {

            pnActiveProcess.Height = btScanHoleProcess.Height;
            pnActiveProcess.Top = btScanHoleProcess.Top;
        }

        private void btCheckingProcess_Click(object sender, EventArgs e)
        {

            pnActiveProcess.Height = btCheckingProcess.Height;
            pnActiveProcess.Top = btCheckingProcess.Top;
        }

        private void pnWorkingParameter_Paint(object sender, PaintEventArgs e)
        {

        }

        private void button12_Click(object sender, EventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void pnDetectHole_Paint(object sender, PaintEventArgs e)
        {

        }

        private void metroToggle1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
