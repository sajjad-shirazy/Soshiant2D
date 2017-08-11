using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using System.Threading;

using Soshiant2DEditor.Engine_Types;

namespace Soshiant2DEditor.Editor_Types
{
    public partial class ParticleEditor : Form
    {
        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void InitEngine(Int32 hWnd, int width, int height, int depth, bool isFullscrean);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void Render();

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void CloseEngine();

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void SetBackColor(int R, int G, int B);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void SetActivLayer(int ID);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern int AddLayer(char[] name, double z);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void EditorMouseDown(int x, int y);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void UpdateEditorPositionBase(int x, int y);

        //Particel Editing Methods
        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern int AddParticleSystem(char[] name, double x, double y, int count, int mnpl, int mxpl, int isRep, int scr, int scg, int scb, int sa, int ecr, int ecg, int ecb, int ea, float mnScale, float mxScale, float acc_x, float acc_y, float grv_x, float grv_y, float mnVel, float mxVel, char[] imagePath);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void ResetParticleSystem(int ID, char[] name, double x, double y, int count, int mnpl, int mxpl, int isRep, int scr, int scg, int scb, int sa, int ecr, int ecg, int ecb, int ea, float mnScale, float mxScale, float acc_x, float acc_y, float grv_x, float grv_y, float mnVel, float mxVel, char[] imagePath);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void ResetParticleSystemPos(int ID,int x,int y);

        [DllImport("Copy of Soshiant2D.dll", SetLastError = true)]
        static extern void ResetParticleSystemImage(int ID, char[] imagePath);


        Particle particle;
        Thread Engine_Rendere_Thread;
        MainForm p_mainForm;

        EditeMods editeMod;
        DorwState drowState;

        //Rendering Panel X mouse and Y mouse
        int xMouse;
        int yMouse;

        bool isDraging;

        int image_width;
        int image_height;

        int editor_x_base = 0;
        int editor_y_base = 0;
        int base_selection_x = 0;
        int base_selection_y = 0;

        int task = 1;
        bool initing = false;

        int clientParticleID = -1;

        public ParticleEditor(MainForm p_mainForm)
        {
            InitializeComponent();
            this.p_mainForm = p_mainForm;
            ParticleEditor_VisibleChanged(null, null);
            drowState = DorwState.NON;
            particle = new Particle();
        }

        private void ParticleEditor_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible)
            {
                //new Particle ...
                //toolStripButton4_Click(null, null);
                this.ResteComboBox();
                this.Play();
            }
        }
        void ResteComboBox()
        {
            comboBox1.Items.Clear();
            comboBox2.Items.Clear();
            string[] texList = this.p_mainForm.sequence.resMan.GetResourceNameList(ResourceTypes.TEXTURE) ;
            string[] layerList = this.p_mainForm.sequence.partMan.GetLayersNameList();
            for (int i = 0; i < texList.Length; i++)
            {
                comboBox1.Items.Add(texList[i]);
            }
            for (int i = 0; i < layerList.Length; i++)
            {
                comboBox2.Items.Add(layerList[i]);
            }
        }
        public void Play()
        {
            Pouse();
            Engine_Rendere_Thread = new Thread(new ThreadStart(Renderer));
            Engine_Rendere_Thread.Start();
        }
        public void Pouse()
        {
            if (Engine_Rendere_Thread != null)
                Engine_Rendere_Thread.Abort();
        }
        void Renderer()
        {
            while (true)
            {
                Render();
            }
        }

        private void ParticleEditor_Load(object sender, EventArgs e)
        {
            InitEngine(panel1.Handle.ToInt32(), panel1.Width, panel1.Height, 16, false);

            SetActivLayer(AddLayer("L0".ToCharArray(), 0));

            Play();
            
            //Initing Fields
            InitFields();
            
        }

        private void Color_toolStripButton1_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                SetBackColor(colorDialog1.Color.R, colorDialog1.Color.G, colorDialog1.Color.B);
            }
        }

        private void ParticleEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Pouse();
            CloseEngine();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Cancel Button
            this.Visible = false;
            this.Pouse();
        }

        void InitFields()
        {
            ResteComboBox();
            text_name.Text = "particle_" + p_mainForm.sequence.partMan.particleCounter++.ToString(); ;
            particle.نام = text_name.Text;
            text_x.Text = (p_mainForm.sequence.engine_width / 2).ToString();
            text_y.Text = (p_mainForm.sequence.engine_height / 2).ToString();
            text_count.Value = particle.تعداد_ذرات;
            text_minParticleLife.Value = particle.كمينه_زندگي_ذرات;
            text_maxParticleLife.Value = particle.بيشينه_زندگي_ذرات;
            text_isRep.Checked = particle.تكرار;
            //Apearance
            text_startCol_r.Value = (int)particle.رنگ_شروع_قرمز;
            text_startCol_g.Value = (int)particle.رنگ_شروع_سبز;
            text_startCol_b.Value = (int)particle.رنگ_شروع_آبي;

            text_endCol_r.Value = (int)particle.رنگ_پايان_قرمز;
            text_endCol_g.Value = (int)particle.رنگ_پايان_سبز;
            text_endCol_b.Value = (int)particle.رنگ_پايان_آبي;

            text_startAlpha.Value = (int)particle.شفافيت_آغاز_ذرات;
            text_endAlpha.Value = (int)particle.شفافيت_پايان_ذرات;

            text_minSclae.Value = (int)particle.مقياس_كمينه;
            text_maxScale.Value = (int)particle.مقياس_بيشينه;

            text_acc_x.Text = particle.شتاب_x.ToString();
            text_acc_y.Text = particle.شتاب_y.ToString();

            text_gravity_x.Text = particle.گرانش_x.ToString();
            text_gravity_y.Text = particle.گرانش_y.ToString();

            text_minVel.Text = particle.كمينه_سرعت_شروع.ToString();
            text_maxVel.Text = particle.بيشينه_سرعت_شروع.ToString();

            ResetColorBoxes();

            toolStripStatusLabel_xbace.Text = "موقعيت " + " x " + " مبنا :" + "0";
            toolStripStatusLabel_ybace.Text = "| " + "موقعيت " + " y " + " مبنا :" + "0";
        }

        void ResetColorBoxes()
        {
            pictureBox1.BackColor = System.Drawing.Color.FromArgb((int)text_startCol_r.Value, (int)text_startCol_g.Value, (int)text_startCol_b.Value);
            pictureBox2.BackColor = System.Drawing.Color.FromArgb((int)text_endCol_r.Value, (int)text_endCol_g.Value, (int)text_endCol_b.Value);
        }

        private void text_count_ValueChanged(object sender, EventArgs e)
        {            
            if (initing)
            {
                particle.لایه = comboBox2.SelectedItem.ToString();
                text_name.Text = particle.نام;
                particle.تعداد_ذرات = (int)text_count.Value;
                particle.كمينه_زندگي_ذرات = (int)text_minParticleLife.Value;
                particle.بيشينه_زندگي_ذرات = (int)text_maxParticleLife.Value;
                particle.تكرار = text_isRep.Checked;
                //Apearance=.
                particle.رنگ_شروع_قرمز = (int)text_startCol_r.Value;
                particle.رنگ_شروع_سبز = (int)text_startCol_g.Value;
                particle.رنگ_شروع_آبي = (int)text_startCol_b.Value;

                particle.رنگ_پايان_قرمز = (int)text_endCol_r.Value;
                particle.رنگ_پايان_سبز = (int)text_endCol_g.Value;
                particle.رنگ_پايان_آبي = (int)text_endCol_b.Value;

                particle.شفافيت_آغاز_ذرات = (int)text_startAlpha.Value;
                particle.شفافيت_پايان_ذرات = (int)text_endAlpha.Value;

                particle.مقياس_كمينه = (int)text_minSclae.Value;
                particle.مقياس_بيشينه = (int)text_maxScale.Value;

                particle.شتاب_x = float.Parse(text_acc_x.Text);
                particle.شتاب_y = float.Parse(text_acc_y.Text);

                particle.گرانش_x = float.Parse(text_gravity_x.Text);
                particle.گرانش_y = float.Parse(text_gravity_y.Text);

                particle.كمينه_سرعت_شروع = float.Parse(text_minVel.Text);
                particle.بيشينه_سرعت_شروع = float.Parse(text_maxVel.Text);

                ResetColorBoxes();
                ResetParticleSystem(particle.شماره_شناسایی, particle.نام.ToCharArray(), panel1.Width / 2, panel1.Height / 2, particle.تعداد_ذرات, particle.كمينه_زندگي_ذرات, particle.بيشينه_زندگي_ذرات, 1, particle.رنگ_شروع_قرمز, particle.رنگ_شروع_سبز, particle.رنگ_شروع_آبي, particle.شفافيت_آغاز_ذرات, particle.رنگ_پايان_قرمز, particle.رنگ_پايان_سبز, particle.رنگ_پايان_آبي, particle.شفافيت_پايان_ذرات, particle.مقياس_كمينه, particle.مقياس_بيشينه, particle.شتاب_x, particle.شتاب_y, particle.گرانش_x, particle.گرانش_y, particle.كمينه_سرعت_شروع, particle.بيشينه_سرعت_شروع, particle._image.نشانی.ToCharArray());
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //Preview btn
            if (particle != null && CheckFields())
            {
                particle._image = p_mainForm.sequence.resMan.Find(comboBox1.SelectedItem.ToString());
                if (particle._image != null && !initing)
                {
                    particle.شماره_شناسایی = AddParticleSystem(particle.نام.ToCharArray(), panel1.Width/2, panel1.Height/2, particle.تعداد_ذرات, particle.كمينه_زندگي_ذرات, particle.بيشينه_زندگي_ذرات, 1, particle.رنگ_شروع_قرمز, particle.رنگ_شروع_سبز, particle.رنگ_شروع_آبي, particle.شفافيت_آغاز_ذرات, particle.رنگ_پايان_قرمز, particle.رنگ_پايان_سبز, particle.رنگ_پايان_آبي, particle.شفافيت_پايان_ذرات, particle.مقياس_كمينه, particle.مقياس_بيشينه, particle.شتاب_x, particle.شتاب_y, particle.گرانش_x, particle.گرانش_y, particle.كمينه_سرعت_شروع, particle.بيشينه_سرعت_شروع, particle._image.نشانی.ToCharArray());
                    particle.لایه = comboBox2.SelectedItem.ToString();
                    particle.x = p_mainForm.sequence.engine_width / 2;
                    particle.y = p_mainForm.sequence.engine_height / 2;
                    text_ID.Text = particle.شماره_شناسایی.ToString();
                    button2.Enabled = true;
                    initing = true;
                }
            }
        }
        bool CheckFields()
        {
            if (comboBox1.SelectedItem == null)
            {
                comboBox1.SelectedIndex = 0;
            }
            if (comboBox2.SelectedItem == null)
            {
                comboBox2.SelectedIndex = 0;
            }
            bool res = (
                text_x.Text != "" &&
                text_y.Text != "" &&
                text_name.Text != "" &&
                comboBox1.SelectedItem.ToString() != "" &&
                comboBox2.SelectedItem.ToString() != ""
            );
            if (!res)
                MessageBox.Show("لطفا جاهای خالی را به طور کامل پر کنید");
            return res;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
               text_startCol_r.Value = int.Parse(colorDialog1.Color.R.ToString());
               text_startCol_g.Value = int.Parse(colorDialog1.Color.G.ToString());
               text_startCol_b.Value = int.Parse(colorDialog1.Color.B.ToString());
               pictureBox1.BackColor = colorDialog1.Color;
            }
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                text_endCol_r.Value = int.Parse(colorDialog1.Color.R.ToString());
                text_endCol_g.Value = int.Parse(colorDialog1.Color.G.ToString());
                text_endCol_b.Value = int.Parse(colorDialog1.Color.B.ToString());
                pictureBox2.BackColor = colorDialog1.Color;
            }
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            isDraging = true;

            EditorMouseDown(e.X, e.Y);
            base_selection_x = e.X;
            base_selection_y = e.Y;

            if (e.Button == MouseButtons.Left)
            {
                if (drowState == DorwState.NON)
                {
                    while (drowState != DorwState.FINISHED)
                    {
                        //
                        //
                        //
                        Application.DoEvents();
                    }
                    drowState = DorwState.NON;
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                this.Cursor = System.Windows.Forms.Cursors.NoMove2D;
                while (isDraging)
                {
                    UpdateEditorPositionBase(xMouse, yMouse);

                    editor_x_base -= base_selection_x - xMouse;
                    editor_y_base -= base_selection_y - yMouse;
                    base_selection_x = xMouse;
                    base_selection_y = yMouse;

                    toolStripStatusLabel_xbace.Text = "موقعيت " + " x " + " مبنا :" + editor_x_base.ToString();
                    toolStripStatusLabel_ybace.Text = "| " + "موقعيت " + " y " + " مبنا :" + editor_y_base.ToString();

                    Application.DoEvents();
                }
                this.Cursor = System.Windows.Forms.Cursors.Default;
            }
            else if (e.Button == MouseButtons.Right)
            {
                while (isDraging)
                {
                    ResetParticleSystem(particle.شماره_شناسایی, particle.نام.ToCharArray(), xMouse, yMouse, particle.تعداد_ذرات, particle.كمينه_زندگي_ذرات, particle.بيشينه_زندگي_ذرات, 1, particle.رنگ_شروع_قرمز, particle.رنگ_شروع_سبز, particle.رنگ_شروع_آبي, particle.شفافيت_آغاز_ذرات, particle.رنگ_پايان_قرمز, particle.رنگ_پايان_سبز, particle.رنگ_پايان_آبي, particle.شفافيت_پايان_ذرات, particle.مقياس_كمينه, particle.مقياس_بيشينه, particle.شتاب_x, particle.شتاب_y, particle.گرانش_x, particle.گرانش_y, particle.كمينه_سرعت_شروع, particle.بيشينه_سرعت_شروع, particle._image.نشانی.ToCharArray());
                    Application.DoEvents();
                }
            }
        }
        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            isDraging = false;
            if (drowState == DorwState.NON)
                drowState = DorwState.CLICKWAITNIG;
            else if (editeMod != EditeMods.POLYGON && drowState == DorwState.CLICKWAITNIG)
                drowState = DorwState.FINISHED;
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            //updating x,y mouse
            xMouse = e.X;
            yMouse = e.Y;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (initing)
            {
                particle._image = p_mainForm.sequence.resMan.Find(comboBox1.SelectedItem.ToString());
                ResetParticleSystemImage(particle.شماره_شناسایی, particle._image.نشانی.ToCharArray());
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //if (this.task == 1)
            //{
            //    //Add to main Device
            //    SetSpriteShape();
            if (p_mainForm.sequence.partMan.FindLayer(this.particle.لایه).IsUnique(particle.نام))
            {
                this.particle.type = EntityTypes.Particle;
                p_mainForm.sequence.partMan.AddParticleSystem((Particle)particle.Clone(),true);
            }
            else
            {
                MessageBox.Show("Particle :" + "نام انتخاب شده تکراری است");
            }
            //}
            //else if (this.task == 2)
            //{
            //    p_mainForm.sequence.partMan.p_activLayer.Find(this.clientSpriteID).script = this.sprite.script;
            //}

            this.Visible = false;
            this.Pouse();
        }



    }
}
