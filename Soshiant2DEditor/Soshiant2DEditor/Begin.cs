using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Soshiant2DEditor
{
    public partial class Begin : Form
    {
        public string seq_Name;
        public string seq_Designer_Name;
        public string seq_Comments;

        public int Device_width;
        public int Device_height;
        public int Device_depth;

        public bool Accepted;

        public Begin()
        {
            InitializeComponent();
            Accepted = false;
            comboBox1.SelectedIndex = 0;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (CheckField())
            {
                Device_width  = Int32.Parse(textBox1.Text);
                Device_height = Int32.Parse(textBox2.Text);
                if (comboBox1.SelectedIndex == 0)
                    Device_depth = 16;
                else
                    Device_depth = 32;

                seq_Name = textBox4.Text;
                seq_Designer_Name = textBox3.Text;
                seq_Comments = textBox5.Text;

                Accepted = true;
                this.Close();
            }
            else
                MessageBox.Show("لطفا همه جعبه های ورودی پر شود ");
        }

        bool CheckField()
        {
            bool result = false;
            if (textBox1.Text != "" && textBox2.Text != "" && textBox3.Text != "" && textBox4.Text != "" && comboBox1.SelectedIndex!=-1)
                result = true;
            //
            return result;
        }
    }
}
