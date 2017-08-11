using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Soshiant2DEditor.Engine_Types;

namespace Soshiant2DEditor.Editor_Types
{
    public partial class LineThickness : Form
    {
        public LineThickness()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        public float MyShowDialog()
        {
            this.ShowDialog();
            return (float)numericUpDown1.Value;
        }
    }
}
