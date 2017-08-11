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
    public partial class SpriteEditor : Form
    {
        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void InitEngine(Int32 hWnd, int width, int height, int depth, bool isFullscrean);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void Render();

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void CloseEngine();

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void SetBackColor(int R, int G, int B);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern int SelectSprite(int x, int y);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern bool IsSelectedSprite();

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void UpdateSprite(int ID, int x, int y);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void SetPause(bool choice);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void UpdateEditorPositionBase(int x, int y);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void EditorMouseDown(int x, int y);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern int AddLayer(char[] name, double z);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void SetActivLayer(int ID);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern int AddSprite(char[] name, double x, double y, double scale, double rotation, int alpha, int row, int col, int totalFrame, int width, int height, int anim_timer, int isRev, double xmov, double ymov, int mov_timer, char[] imagePath, bool isPhysical, bool isStatic, float mass, float inertia);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern int GetSpriteWidth(int ID);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern int GetSpriteHeight(int ID);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void ResetSprite(int ID, char[] name, double x, double y, double scale, double rotation, int alpha, int width, int height, int row, int col, int totalFrame, int isRev, int anim_timer, double xmov, double ymov, int mov_timer, bool isPhysical, bool isStatic, float mass, float inertia);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void ResetSpriteImage(int ID, char[] imagePath);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void DeleteSprite(int ID);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void SetSelectedSprite(int ID);
        //
        //Shape Metods
        //
        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void setActiveShape(int id);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void setShapeVisible(int id, bool value);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern int AddLineShape(float ax, float ay, float bx, float by, float r);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void ResetLineShape(float ax, float ay, float bx, float by, float r);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern int AddCircleShape(float x, float y, float r);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void ResetCircleShape(float x, float y, float r);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void RemoveShape(int id);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void SwitchEditeMode(bool value);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern bool SelectPoint(int x, int y);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void UpdatePoint(int x, int y);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void GetLineProperties(int id, float[] carry);

        [DllImport("SpriteEditor.dll", SetLastError = true)]
        static extern void GetCircleProperties(int id, float[] carry);



        Thread Engine_Rendere_Thread;
        MainForm p_mainForm;
        Sprite sprite;

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

        int clientSpriteID = -1;



        public SpriteEditor(MainForm p_mainForm)
        {
            InitializeComponent();
            this.p_mainForm = p_mainForm;
            SpriteEditor_VisibleChanged(null, null);
            drowState = DorwState.NON;
        }

        private void SpriteEditor_Load(object sender, EventArgs e)
        {
            InitEngine(panel1.Handle.ToInt32(), panel1.Width, panel1.Height, 16, false);

            SetActivLayer(AddLayer("L0".ToCharArray(), 0));

            SetPause(true);

            Play();

            //Initing Fields
            InitFields();

        }

        void Renderer()
        {
            while (true)
            {
                Render();
            }
        }

        private void Color_toolStripButton1_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                SetBackColor(colorDialog1.Color.R, colorDialog1.Color.G, colorDialog1.Color.B);
            }
        }

        void InitFields()
        {
            this.ResteComboBox();
            text_name.Text = "sprite_" + p_mainForm.sequence.partMan.spriteCounter++.ToString(); ;
            text_x.Text = (p_mainForm.sequence.engine_width / 2).ToString();
            text_y.Text = (p_mainForm.sequence.engine_height / 2).ToString();
            text_scale.Text = "1.0";
            text_rotation.Text = "0";
            text_alpha.Text = "255";
            text_width.Text = "0";
            text_height.Text = "0";
            text_row.Value = 1;
            text_col.Value = 1;
            text_anim_timer.Value = 0;
            text_totalframes.Value = 1;
            text_isRev.Checked = false;
            text_xmov.Text = "0";
            text_ymov.Text = "0";
            text_mov_timer.Value = 0;

            //
            this.checkBox_noPhysics.Checked = true;

            toolStripStatusLabel_xbace.Text = "موقعيت " + " x " + " مبنا :" + "0";
            toolStripStatusLabel_ybace.Text = "| " + "موقعيت " + " y " + " مبنا :" + "0";
        }

        void ResteComboBox()
        {
            comboBox1.Items.Clear();
            comboBox2.Items.Clear();
            string[] texList = this.p_mainForm.sequence.resMan.GetResourceNameList(ResourceTypes.TEXTURE);
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

        bool RefreshSprite()
        {
            if (!CheckFields())
                return false;
            if (sprite == null)
            {
                sprite = new Sprite();
                sprite.نام = text_name.Text;
                sprite.x = double.Parse(text_x.Text);
                sprite.y = double.Parse(text_y.Text);
                sprite.مقیاس = double.Parse(text_scale.Text);
                sprite.زاویه_چرخش = double.Parse(text_rotation.Text);
                sprite.شفافیت = int.Parse(text_alpha.Text);
                sprite.عرض = int.Parse(text_width.Text);
                sprite.ارتفاع = int.Parse(text_height.Text);
                sprite.سطر = (int)text_row.Value;
                sprite.ستون = (int)text_col.Value;
                sprite.توقف_در_هر_فریم = (int)text_anim_timer.Value;
                sprite.تعدادفریم = (int)text_totalframes.Value;
                if (text_isRev.Checked)
                    sprite.معکوس = -1;
                else
                    sprite.معکوس = 1;
                sprite.XMove = double.Parse(text_xmov.Text);
                sprite.YMove = double.Parse(text_ymov.Text);
                sprite.توقف_حرکت_در_هر_فریم = (int)text_mov_timer.Value;
                sprite.لایه = comboBox2.SelectedItem.ToString();
                sprite._image = p_mainForm.sequence.resMan.Find(comboBox1.SelectedItem.ToString());
                if (sprite.بافت == null)
                {
                    MessageBox.Show("   ! خطا : بافت انتخاب شده یافت نشد   " + '\n' + '\n' + "... لطفا این خرابی را به ما گزارش دهید " + '\n' + comboBox1.SelectedItem.ToString());
                    return false;
                }
                //NO Includes in Editor
                sprite.دارای_خواص_فیزیکی = !checkBox_noPhysics.Checked;
                sprite.ایستایی = checkBox_isStaticObject.Checked;
                sprite.جرم = float.Parse(text_mass.Text);
                sprite.اینرسی = float.Parse(text_inertia.Text);
                sprite.ضريب_اصطكاك = float.Parse(text_friction.Text);
                sprite.ضريب_كشسان = float.Parse(text_elastic.Text);


                text_ID.Text = AddSprite(sprite.نام.ToCharArray(), 0, 0, sprite.مقیاس, sprite.زاویه_چرخش, sprite.شفافیت, sprite.سطر, sprite.ستون, sprite.تعدادفریم, sprite.عرض, sprite.ارتفاع, sprite.توقف_در_هر_فریم, sprite.معکوس, sprite.XMove, sprite.YMove, sprite.توقف_حرکت_در_هر_فریم, sprite._image.نشانی.ToCharArray(), sprite.دارای_خواص_فیزیکی, sprite.ایستایی, sprite.جرم, sprite.اینرسی).ToString();
                //MessageBox.Show(text_ID.Text);

                sprite.شماره_شناسایی = int.Parse(text_ID.Text);

                image_height = GetSpriteHeight(int.Parse(text_ID.Text));
                image_width = GetSpriteWidth(int.Parse(text_ID.Text));


                text_width.Text = image_width.ToString();
                text_height.Text = image_height.ToString();


                return true;
            }
            else
            {
                sprite.نام = text_name.Text;
                sprite.x = double.Parse(text_x.Text);
                sprite.y = double.Parse(text_y.Text);
                sprite.مقیاس = double.Parse(text_scale.Text);
                sprite.زاویه_چرخش = double.Parse(text_rotation.Text);
                sprite.شفافیت = int.Parse(text_alpha.Text);
                sprite.عرض = int.Parse(text_width.Text);
                sprite.ارتفاع = int.Parse(text_height.Text);
                sprite.سطر = (int)text_row.Value;
                sprite.ستون = (int)text_col.Value;
                sprite.توقف_در_هر_فریم = (int)text_anim_timer.Value;
                sprite.تعدادفریم = (int)text_totalframes.Value;
                if (text_isRev.Checked)
                    sprite.معکوس = -1;
                else
                    sprite.معکوس = 1;
                sprite.XMove = double.Parse(text_xmov.Text);
                sprite.YMove = double.Parse(text_ymov.Text);
                sprite.توقف_حرکت_در_هر_فریم = (int)text_mov_timer.Value;
                sprite.لایه = comboBox2.SelectedItem.ToString();
                sprite._image = p_mainForm.sequence.resMan.Find(comboBox1.SelectedItem.ToString());
                if (sprite.بافت == null)
                {
                    MessageBox.Show("   ! خطا : بافت انتخاب شده یافت نشد   " + '\n' + '\n' + "... لطفا این خرابی را به ما گزارش دهید " + '\n' + comboBox1.SelectedItem.ToString());
                    return false;
                }
                //NO Includes in Editor
                sprite.دارای_خواص_فیزیکی = !checkBox_noPhysics.Checked;
                sprite.ایستایی = checkBox_isStaticObject.Checked;
                sprite.جرم = float.Parse(text_mass.Text);
                sprite.اینرسی = float.Parse(text_inertia.Text);
                sprite.ضريب_اصطكاك = float.Parse(text_friction.Text);
                sprite.ضريب_كشسان = float.Parse(text_elastic.Text);

                sprite.شماره_شناسایی = int.Parse(text_ID.Text);

                ResetSprite(sprite.شماره_شناسایی, sprite.نام.ToCharArray(), 0, 0, sprite.مقیاس, sprite.زاویه_چرخش, sprite.شفافیت, sprite.عرض, sprite.ارتفاع, sprite.سطر, sprite.ستون, sprite.تعدادفریم, sprite.معکوس, sprite.توقف_در_هر_فریم, sprite.XMove, sprite.YMove, sprite.توقف_حرکت_در_هر_فریم, sprite.دارای_خواص_فیزیکی, sprite.ایستایی, sprite.جرم, sprite.اینرسی);

                return true;
            }
        }

        void RefreshPanel()
        {
            text_name.Text = sprite.نام;
            text_x.Text = sprite.x.ToString();
            text_y.Text = sprite.y.ToString();
            text_scale.Text = sprite.مقیاس.ToString();
            text_rotation.Text = sprite.زاویه_چرخش.ToString();
            text_alpha.Text = sprite.شفافیت.ToString();
            text_width.Text = sprite.عرض.ToString();
            text_height.Text = sprite.ارتفاع.ToString();
            text_row.Value = sprite.سطر;
            text_col.Value = sprite.ستون;
            text_anim_timer.Value = sprite.توقف_در_هر_فریم;
            text_totalframes.Value = sprite.تعدادفریم;
            text_xmov.Text = sprite.XMove.ToString();
            text_ymov.Text = sprite.YMove.ToString();
            text_mov_timer.Value = sprite.توقف_حرکت_در_هر_فریم;

            comboBox1.SelectedIndex = comboBox1.Items.IndexOf(sprite.بافت);
            comboBox2.SelectedIndex = comboBox2.Items.IndexOf(sprite.لایه);

            checkBox_isStaticObject.Checked = sprite.ایستایی;
            checkBox_noPhysics.Checked = !sprite.دارای_خواص_فیزیکی;
            text_mass.Text = sprite.جرم.ToString();
            text_inertia.Text = sprite.اینرسی.ToString();
            text_friction.Text = sprite.ضريب_اصطكاك.ToString();
            text_elastic.Text = sprite.ضريب_كشسان.ToString();

            text_ID.Text = sprite.شماره_شناسایی.ToString();
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
                text_scale.Text != "" &&
                text_rotation.Text != "" &&
                text_alpha.Text != "" &&
                text_width.Text != "" &&
                text_height.Text != "" &&
                text_row.Value >= 1 &&
                text_col.Value >= 1 &&
                text_totalframes.Value >= 1 &&
                text_xmov.Text != "" &&
                text_ymov.Text != "" &&
                text_elastic.Text != "" &&
                text_friction.Text != "" &&
                comboBox1.SelectedItem.ToString() != "" &&
                comboBox2.SelectedItem.ToString() != ""
            );
            if (!res)
                MessageBox.Show("لطفا جاهای خالی را به طور کامل پر کنید");
            return res;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (RefreshSprite())
            {
                button2.Enabled = true;

                SetSelectedSprite(sprite.شماره_شناسایی);

                //initing tools
                if (!checkBox_noPhysics.Checked)
                {
                    //
                    //
                    //if sprite is previewing in panel enable physics Editing tools
                    this.خطToolStripMenuItem.Enabled = true;
                    this.دایرهToolStripMenuItem.Enabled = true;
                    this.چندضلعیToolStripMenuItem.Enabled = true;
                    toolStripButton_edite.Enabled = true;
                }
            }

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Cancel Button
            this.Visible = false;
            this.Pouse();
        }

        public void Pouse()
        {
            if (Engine_Rendere_Thread != null)
                Engine_Rendere_Thread.Abort();
        }
        public void Play()
        {
            Pouse();
            Engine_Rendere_Thread = new Thread(new ThreadStart(Renderer));
            Engine_Rendere_Thread.Start();
        }

        private void SpriteEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Pouse();
            CloseEngine();
        }

        private void SpriteEditor_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible)
            {
                //new Sprite ...
                toolStripButton4_Click(null, null);
                this.ResteComboBox();
                this.Play();
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
                if (editeMod == EditeMods.LINE)
                {
                    if (drowState == DorwState.NON)
                    {
                        //Create a new Line Shape and Activ it
                        setActiveShape(_AddLineShape(e.X, e.Y, 100, 100, 0));
                        this.Cursor = System.Windows.Forms.Cursors.Cross;
                        while (drowState != DorwState.FINISHED)
                        {
                            ResetLineShape((float)(e.X - editor_x_base), (float)(e.Y - editor_y_base), (float)(xMouse - editor_x_base), (float)(yMouse - editor_y_base), 0);
                            Application.DoEvents();
                        }
                        //seting thickness
                        LineThickness LT = new LineThickness();
                        float tt = LT.MyShowDialog();
                        ResetLineShape((float)(e.X - editor_x_base), (float)(e.Y - editor_y_base), (float)(xMouse - editor_x_base), (float)(yMouse - editor_y_base), tt);

                        this.Cursor = System.Windows.Forms.Cursors.Default;
                        drowState = DorwState.NON;
                    }
                }
                else if (editeMod == EditeMods.CIRCLE)
                {
                    if (drowState == DorwState.NON)
                    {
                        //Create a new Line Shape and Activ it
                        setActiveShape(_AddCircleShape(e.X - editor_x_base, e.Y - editor_y_base, 50));
                        this.Cursor = System.Windows.Forms.Cursors.Cross;
                        while (drowState != DorwState.FINISHED)
                        {
                            ResetCircleShape((float)(e.X - editor_x_base), (float)(e.Y - editor_y_base), (float)(xMouse - e.X));
                            Application.DoEvents();
                        }
                        this.Cursor = System.Windows.Forms.Cursors.Default;
                        drowState = DorwState.NON;
                    }
                }
                else if (editeMod == EditeMods.EDITOR)
                {
                    if (SelectPoint(e.X, e.Y))
                    {
                        this.Cursor = System.Windows.Forms.Cursors.SizeAll;

                        drowState = DorwState.CLICKWAITNIG;

                        while (drowState != DorwState.FINISHED)
                        {
                            UpdatePoint(xMouse, yMouse);
                            Application.DoEvents();
                        }
                        this.Cursor = System.Windows.Forms.Cursors.Default;
                        drowState = DorwState.NON;
                    }
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                //editeMod = EditeMods.NON;
                خطToolStripMenuItem.Checked = false;
                دایرهToolStripMenuItem.Checked = false;
                چندضلعیToolStripMenuItem.Checked = false;
                //toolStripButton_edite.Checked= false;
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
            //changing Image Selected
            if (sprite != null)
            {
                if (comboBox1.SelectedItem.ToString() != "")
                {
                    sprite._image = p_mainForm.sequence.resMan.Find(comboBox1.SelectedItem.ToString());
                    ResetSpriteImage(sprite.شماره_شناسایی, sprite._image.نشانی.ToCharArray());
                    text_width.Text = GetSpriteWidth(sprite.شماره_شناسایی).ToString();
                    text_height.Text = GetSpriteHeight(sprite.شماره_شناسایی).ToString();
                }
            }
        }

        void _ResteSprite()
        {
            //
        }

        private void text_name_TextChanged(object sender, EventArgs e)
        {
            //changing Texts
            if (sprite != null && !initing)
                if (CheckFields())
                {
                    RefreshSprite();
                    ///////////////////////////////
                }
        }

        private void checkBox_noPhysics_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_noPhysics.Checked)
            {
                this.groupBox7.Enabled = false;

                this.خطToolStripMenuItem.Enabled = false;
                this.دایرهToolStripMenuItem.Enabled = false;
                this.چندضلعیToolStripMenuItem.Enabled = false;
                toolStripButton_edite.Enabled = false;
            }
            else
            {
                this.groupBox7.Enabled = true;
                if (button2.Enabled)
                {
                    //
                    //
                    //if sprite is previewing in panel enable physics Editing tools
                    this.خطToolStripMenuItem.Enabled = true;
                    this.دایرهToolStripMenuItem.Enabled = true;
                    this.چندضلعیToolStripMenuItem.Enabled = true;
                    toolStripButton_edite.Enabled = true;
                }
            }
        }

        private void checkBox_isStaticObject_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_isStaticObject.Checked)
            {
                this.text_inertia.Enabled = false;
                this.text_mass.Enabled = false;
            }
            else
            {
                this.text_inertia.Enabled = true;
                this.text_mass.Enabled = true;
            }
        }

        private void toolStripButton_Line_Click(object sender, EventArgs e)
        {
            خطToolStripMenuItem.Checked = true;
            دایرهToolStripMenuItem.Checked = false;
            چندضلعیToolStripMenuItem.Checked = false;
            toolStripButton_edite.Checked = false;

            this.editeMod = EditeMods.LINE;
            drowState = DorwState.NON;

        }

        private void toolStripButton_Circle_Click(object sender, EventArgs e)
        {
            خطToolStripMenuItem.Checked = false;
            دایرهToolStripMenuItem.Checked = true;
            چندضلعیToolStripMenuItem.Checked = false;
            toolStripButton_edite.Checked = false;

            this.editeMod = EditeMods.CIRCLE;
            drowState = DorwState.NON;
        }

        private void toolStripButton_Polygon_Click(object sender, EventArgs e)
        {
            خطToolStripMenuItem.Checked = false;
            دایرهToolStripMenuItem.Checked = false;
            چندضلعیToolStripMenuItem.Checked = true;
            toolStripButton_edite.Checked = false;

            this.editeMod = EditeMods.POLYGON;
            drowState = DorwState.NON;
        }

        int _AddLineShape(float ax, float ay, float bx, float by, float r)
        {
            int id = AddLineShape(ax, ay, bx, by, r);

            listView1.Items.Add(id.ToString(), 0);

            setShapeVisible(id, true);
            return id;
        }

        int _AddCircleShape(float x, float y, float r)
        {
            int id = AddCircleShape(x, y, r);

            listView1.Items.Add(id.ToString(), 1);
            setShapeVisible(id, true);

            return id;
        }

        private void listView1_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count > 0)
            {
                setActiveShape(int.Parse(listView1.SelectedItems[0].Text));
                if (editeMod == EditeMods.EDITOR)
                {
                    SwitchEditeMode(true);
                }
                else
                {
                    editeMod = EditeMods.NON;
                }
            }

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("مطمئن هستید ؟", "حذف اشکال", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                int cnt = listView1.SelectedItems.Count;
                for (int i = 0; i < cnt; i++)
                {
                    RemoveShape(int.Parse(listView1.SelectedItems[0].Text));

                    listView1.Items.RemoveAt(listView1.SelectedItems[0].Index);
                }
            }
        }

        private void toolStripButton_edite_Click(object sender, EventArgs e)
        {
            if (editeMod == EditeMods.EDITOR)
            {
                toolStripButton_edite.Checked = false;
                editeMod = EditeMods.NON;
                SwitchEditeMode(false);
            }
            else
            {
                editeMod = EditeMods.EDITOR;
                SwitchEditeMode(true);
                toolStripButton_edite.Checked = true;

            }
        }

        private void listView1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
                toolStripButton1_Click(null, null);
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            //lock editor position bace
            EditorMouseDown(editor_x_base, editor_y_base);
            UpdateEditorPositionBase(0, 0);

            editor_x_base = 0;
            editor_y_base = 0;

            toolStripStatusLabel_xbace.Text = "موقعيت " + " x " + " مبنا :" + editor_x_base.ToString();
            toolStripStatusLabel_ybace.Text = "| " + "موقعيت " + " y " + " مبنا :" + editor_y_base.ToString();

        }

        private void SetSpriteShape()
        {
            sprite.shapesList.Clear();
            for (int i = 0; i < listView1.Items.Count; i++)
            {
                if (listView1.Items[i].ImageIndex == 0)
                {
                    float[] carry = new float[5];
                    GetLineProperties(int.Parse(listView1.Items[i].Text), carry);

                    LineShape shape = new LineShape();
                    shape.type = ShapeTypes.LINE;
                    shape.aX = carry[0];
                    shape.aY = carry[1];
                    shape.bX = carry[2];
                    shape.bY = carry[3];
                    shape.thickness = carry[4];

                    sprite.AddShape(shape);
                }
                else if (listView1.Items[i].ImageIndex == 1)
                {
                    float[] carry = new float[3];
                    GetCircleProperties(int.Parse(listView1.Items[i].Text), carry);

                    CircleShape shape = new CircleShape();
                    shape.type = ShapeTypes.CIRCLE;
                    shape.x = carry[0];
                    shape.y = carry[1];
                    shape.r = carry[2];

                    sprite.AddShape(shape);
                }
                else
                {
                    //Polygon
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //دکمه اضاف
            if (this.task == 1)
            {
                if (p_mainForm.sequence.partMan.FindLayer(this.sprite.لایه).IsUnique(sprite.نام))
                {
                    //Add to main Device
                    SetSpriteShape();
                    this.sprite.type = EntityTypes.Sprite;
                    p_mainForm.sequence.partMan.AddSprite((Sprite)sprite.Clone(),true);
                }
                else
                {
                    MessageBox.Show("Sprite :" + "نام انتخاب شده تکراری است");
                }
            }
            else if (this.task == 2)
            {
                p_mainForm.sequence.partMan.p_activLayer.Find(this.clientSpriteID).script = this.sprite.script;
            }

            this.Visible = false;
            this.Pouse();
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            if (sprite != null)
            {
                this.Pouse();
                InitFields();
                DeleteSprite(sprite.شماره_شناسایی);
                listView1.Clear();
                sprite = null;
                //RefreshSprite();
                this.Play();
            }
        }

        public void setTask(int choos, Sprite sprite)
        {
            if (choos == 1)
            {
                // 1 means new Sprite
                this.task = 1;
                button3.Enabled = true;
                button2.Enabled = false;
                button2.Text = "اضاف";
                comboBox2.Enabled = true;
                this.clientSpriteID = -1;
            }
            else if (choos == 2)
            {
                // 2 means Editing an existing Sprite
                this.task = 2;
                comboBox2.Enabled = false;
                button3.Enabled = false;
                button2.Enabled = true;
                button2.Text = "ذخيره";
                //clearing scene
                this.clientSpriteID = sprite.شماره_شناسایی;
                toolStripButton4_Click(null, null);

                if (sprite != null)
                {
                    initing = true;
                    this.sprite = sprite;
                    Reload();
                    RefreshPanel();
                    initing = false;
                }
            }
        }

        public void Reload()
        {
            sprite.شماره_شناسایی = AddSprite(sprite.نام.ToCharArray(), 0, 0, sprite.مقیاس, sprite.زاویه_چرخش, sprite.شفافیت, sprite.سطر, sprite.ستون, sprite.تعدادفریم, sprite.عرض, sprite.ارتفاع, sprite.توقف_در_هر_فریم, sprite.معکوس, sprite.XMove, sprite.YMove, sprite.توقف_حرکت_در_هر_فریم, sprite._image.نشانی.ToCharArray(), sprite.دارای_خواص_فیزیکی, sprite.ایستایی, sprite.جرم, sprite.اینرسی);
            
            SetSelectedSprite(sprite.شماره_شناسایی);
            for (int k = 0; k < sprite.shapesList.Count; k++)
            {
                if (sprite.shapesList[k].type == ShapeTypes.LINE)
                {
                    LineShape shape = (LineShape)sprite.shapesList[k];
                    shape.id = AddLineShape(shape.aX, shape.aY, shape.bX, shape.bY, shape.thickness);
                    listView1.Items.Add(shape.id.ToString(), 0);
                }
                else if (sprite.shapesList[k].type == ShapeTypes.CIRCLE)
                {
                    CircleShape shape = (CircleShape)sprite.shapesList[k];
                    shape.id = AddCircleShape(shape.x, shape.y, shape.r);
                    listView1.Items.Add(shape.id.ToString(),1);
                }

                //setShapeVisible(sprite.shapesList[i].id, true);
            }
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            if (this.sprite != null)
            {
                ScriptEditor scriptEditor = new ScriptEditor(this.sprite);
                scriptEditor.ShowDialog();
            }

        }
    }





















    public enum EditeMods
    {
        NON,
        LINE,
        CIRCLE,
        POLYGON,
        EDITOR
    }

    public enum DorwState
    {
        NON,
        CLICKWAITNIG,
        FINISHED
    }
}
