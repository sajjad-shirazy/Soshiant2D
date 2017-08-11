using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Soshiant2DEditor.Engine_Types;
using Soshiant2DEditor.Editor_Types;

namespace Soshiant2DEditor
{
    public partial class CameraSetings : Form
    {
        PartManager p_partMan;
        public CameraSetings(PartManager p_partMan)
        {
            InitializeComponent();
            this.p_partMan = p_partMan;
            ResteComboBox();           
        }

        void ResteComboBox()
        {
            comboBox1.Items.Clear();
            comboBox2.Items.Clear();
            comboBox3.Items.Clear();
            string[] layerList = p_partMan.GetLayersNameList();
            for (int i = 0; i < layerList.Length; i++)
            {
                comboBox1.Items.Add(layerList[i]);
                comboBox2.Items.Add(layerList[i]);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (   comboBox1.SelectedIndex != -1
                && comboBox2.SelectedIndex != -1
                && comboBox3.SelectedIndex != -1)
            {
                p_partMan._SetPivotLayer(comboBox1.SelectedItem.ToString());
                p_partMan._SetFocusLayer(comboBox2.SelectedItem.ToString());
                p_partMan._SetCameraLookAt(comboBox3.SelectedItem.ToString());
                this.Close();
            }
            else
            {
                MessageBox.Show("لطفا همه گزینه ها را انتخاب کنید");
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            comboBox3.Items.Clear();
            Layer parent = p_partMan.FindLayer(comboBox2.SelectedItem.ToString());
            for (int i = 0; i < parent.sprites.Count; i++)
            {
                comboBox3.Items.Add(parent.sprites[i].نام);
            }
        }
    }
}
