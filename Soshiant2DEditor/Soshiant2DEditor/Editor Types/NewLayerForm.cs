using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Soshiant2DEditor.Editor_Types
{
    public partial class NewLayerForm : Form
    {
        PartManager p_partMan;
        public NewLayerForm(PartManager p_partmanager)
        {
            InitializeComponent();
            this.p_partMan = p_partmanager;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            p_partMan.AddNewLayer(textBox1.Text,double.Parse(textBox2.Text));
            this.Close();
        }
    }
}
