using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Soshiant2DEditor.Engine_Types;
using Soshiant2DEditor.Editor_Types;

using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;
using System.IO;

using System.Runtime.InteropServices;
using System.Threading;

namespace Soshiant2DEditor
{    
    public partial class MainForm : Form
    {
        //Main Engine Functions ....
        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void InitEngine(Int32 hWnd , int width , int height , int depth , bool isFullscrean);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void ResizeEngine(int width, int height);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void Render();

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void CloseEngine();

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int GetRealFPS();

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int GetCoreFPS();

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void SetBackColor(int R, int G, int B);       
        
        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int SelectSprite(int x, int y);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern bool IsSelectedSprite();

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void UpdateSprite(int ID, int x, int y);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern  void SetPause(bool choice);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void UpdateEditorPositionBase(int x, int y);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void EditorMouseDown(int x, int y);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int GetSpriteX(int ID);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int GetSpriteY(int ID);


        Thread Engine_Rendere_Thread;
        SpriteEditor spriteEditor;
        ParticleEditor particleEditor;
        public Sequence sequence;

        //is Sprite Editor Inited ?
        public bool sp_inited;

        //Consol Seprator
        string cnsl_sep;

        //is in draging ?
        bool isDraging;
        public int selectedSpriteID;
        public Sprite selectedSprite;

        //Rendering Panel X mouse and Y mouse
        int xMouse;
        int yMouse;
        int base_select_x;
        int base_select_y;

        public MainForm()
        {
            InitializeComponent();

            cnsl_sep = "  :: > ";
            isDraging = false;

            toolStripButton1.Enabled = false;
            toolStripButton2.Enabled = false;
            toolStripButton3.Enabled = false;
            toolStripButton4.Enabled = false;
            toolStripButton6.Enabled = false;
            toolStripButton7.Enabled = false;
            toolStripButton9.Enabled = false;
            toolStripButton13.Enabled = false;
            toolStripDropDownButton1.Enabled = false;

            splitContainer1.Panel1.ContextMenuStrip = contextMenuStrip1;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            ConsolPrint("بسم الله الرحمن الرحیم");
            ConsolPrint("شما خودتان را درست کنید ، کشورتان درست خواهد شد . امام خمینی (ره)");
            ConsolPrint("اجرای ویرایشگر ...");

            NewSegvence();
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (Engine_Rendere_Thread != null)
                Engine_Rendere_Thread.Abort();
            CloseEngine();
        }

        void NewSegvence()
        {
            if (sequence == null)
            {                
                Begin begin_panel = new Begin();
                begin_panel.ShowDialog();
                if (begin_panel.Accepted)
                {
                    sequence = new Sequence();
                    ConsolPrint("شروع فصل جدید" );
                    sequence.engine_width = begin_panel.Device_width;
                    sequence.engine_height = begin_panel.Device_height;
                    sequence.engine_depth = begin_panel.Device_depth;

                    sequence.name = begin_panel.seq_Name;
                    sequence.designer = begin_panel.seq_Designer_Name;
                    sequence.comments = begin_panel.seq_Comments;
                    //
                    //
                    //
                    //Initing Soshiant2D Engine
                    RunEngine();

                    //resource mangare initing ....

                    sequence.partMan = new PartManager(treeView_partman, this);
                    sequence.resMan = new ResourceManager(treeView_resman, this);
                }
            }
            else
            {
                //Agar rooye fasle jadidi klik konad va fasl dar hale pardazesh bashad
                MessageBox.Show("شما باید خارج شوید / در دست ساخت");
            }
        }

        void RunEngine()
        {
            InitEngine(this.splitContainer1.Panel1.Handle.ToInt32(), sequence.engine_width, sequence.engine_height, sequence.engine_depth, false);
            ConsolPrint("دستگاه ساخته شد : " + "دستگیره پنجره [" + this.splitContainer1.Panel1.Handle.ToInt32().ToString() + "]");
            //ResizeEngine(this.Width,this.Height);

            toolStripButton8_Click(null, null);
            this.Play();
            //toolStripButton8_Click(null, null);

            //Fixing panel Size
            toolStripButton7_Click(null, null);

            toolStripButton1.Enabled = true;
            toolStripButton2.Enabled = true;
            toolStripButton3.Enabled = true;
            toolStripButton4.Enabled = true;
            toolStripButton6.Enabled = true;
            toolStripButton7.Enabled = true;
            toolStripButton13.Enabled = true;
            toolStripDropDownButton1.Enabled = true;
        }

        void Renderer()
        {
            int wait = 0;
            while (true)
            {
                UpdateScrean();

                if (wait++ > 500)
                {
                    if (Real_framerate_LBL.Visible)
                        Real_framerate_LBL.Text = GetRealFPS().ToString();
                    if (Core_framerate_LBL.Visible)
                        Core_framerate_LBL.Text = GetCoreFPS().ToString();
                    wait = 0;
                }
            }
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            //runinig game
            System.Diagnostics.Process Proc = new System.Diagnostics.Process();
            Proc.StartInfo.FileName = "game.exe";
            Proc.Start();
        }

        private void فصلجدیدToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewSegvence();
        }

        private void splitContainer1_Panel1_Resize(object sender, EventArgs e)
        {
            this.Device_size_LBL.Text = splitContainer1.Panel1.Width.ToString() + " x " + splitContainer1.Panel1.Height.ToString();
        }

        private void toolStripButton7_Click(object sender, EventArgs e)
        {
            //Fixing panel Size
            splitContainer1.SplitterDistance = sequence.engine_height;
            MainForm_splitContainer1.SplitterDistance = MainForm_splitContainer1.Width - sequence.engine_width - 4;
        }

        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            //clear Bot of consol
            Consol_textbox.Text = "";
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            //Add new Sprite BTN
            if (spriteEditor == null)
            {
                spriteEditor = new SpriteEditor(this);
                spriteEditor.Show();
            }
            spriteEditor.Visible = true;
            spriteEditor.Play();

            spriteEditor.setTask(1,null);
        }

        //
        //
        //
        //DLL Exported Functions
        public void _SetBackColor( int R, int G, int B)
        {
            SetBackColor(R,G,B);
        }
        ////////////////////////////////////////////////////////////////////
        public void Pouse()
        {
            Engine_Rendere_Thread.Abort();
        }
        public void Play()
        {
            Engine_Rendere_Thread = new Thread(new ThreadStart(Renderer));
            Engine_Rendere_Thread.Start();
        }

        public void UpdateScrean()
        {
            //for panels to render when moves
            Render();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            //َAdd BtN of resource tree view
            sequence.resMan.AddNewMember();
        }

        public void ConsolPrint(string str)
        {
            Consol_textbox.Text += System.DateTime.Now.TimeOfDay.ToString() + cnsl_sep + str +'\n';
        }

        private void tabPage2_Enter(object sender, EventArgs e)
        {
            //onFocosing to resource manger panel ...
            //this.treeView_resman.ExpandAll();
        }

        private void splitContainer1_Panel1_MouseDown(object sender, MouseEventArgs e)
        {
            //Rendering Panel Mouse Down ...
            bool draged = false;
            isDraging = true;
            base_select_x = e.X;
            base_select_y = e.Y;
            //
            EditorMouseDown(e.X,e.Y);

           if (e.Button == MouseButtons.Left)
                //if (false)
            {
                selectedSpriteID = SelectSprite((int)(e.X * ((float)sequence.engine_width) / (float)splitContainer1.Panel1.Width), (int)(e.Y * ((float)sequence.engine_height) / (float)splitContainer1.Panel1.Height));
                selectedSprite = sequence.partMan.p_activLayer.Find(selectedSpriteID);
                bool isSelected = (selectedSpriteID != -1);
                if (isSelected)
                {
                    //MessageBox.Show("Selected ... in draging //// id : "+id.ToString());
                    int x = 0;int y = 0;
                    int xm = xMouse;
                    int ym = yMouse;
                    while (isDraging)
                    {
                        x = (int)(xMouse * ((float)sequence.engine_width) / (float)splitContainer1.Panel1.Width);
                        y = (int)(yMouse * ((float)sequence.engine_height) / (float)splitContainer1.Panel1.Height);

                        UpdateSprite(selectedSpriteID, x-selectedSprite.عرض/2, y-selectedSprite.ارتفاع/2);
                        draged = true;
                        Application.DoEvents();
                    }
                    if (draged)
                    {
                        selectedSprite = sequence.partMan.p_activLayer.Find(selectedSpriteID);
                        selectedSprite.x += x-xm ;
                        selectedSprite.y += y-ym ;

                        propertyGrid1.SelectedObject = selectedSprite;
                    }
                }
                else
                {
                    propertyGrid1.SelectedObject = null;
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                this.Cursor = System.Windows.Forms.Cursors.NoMove2D;
                while (isDraging)
                {
                    int x = (int)(xMouse * ((float)sequence.engine_width) / (float)splitContainer1.Panel1.Width);
                    int y = (int)(yMouse * ((float)sequence.engine_height) / (float)splitContainer1.Panel1.Height);

                    UpdateEditorPositionBase(x,y);
                    draged = true;
                    Application.DoEvents();
                }
                this.Cursor = System.Windows.Forms.Cursors.Default;
            }
        }

        private void splitContainer1_Panel1_MouseUp(object sender, MouseEventArgs e)
        {
            //Rendering Panel Mouse UP ...
            isDraging = false;
            //MessageBox.Show("FREEE");
        }

        private void splitContainer1_Panel1_MouseMove(object sender, MouseEventArgs e)
        {
            //updating x,y mouse
            xMouse = e.X;
            yMouse = e.Y;
        }

        private void toolStripButton9_Click(object sender, EventArgs e)
        {
            if (this.sequence != null)
            {
                //Rendering play Boutom
                SetPause(false);
                toolStripButton9.Enabled = false;
                toolStripButton8.Enabled = true;
            }
        }

        private void toolStripButton8_Click(object sender, EventArgs e)
        {
            if (this.sequence != null)
            {
                //Rendering pousing Boutom
                SetPause(true);
                toolStripButton9.Enabled = true;
                toolStripButton8.Enabled = false;
            }
        }

        private void propertyGrid1_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            if (propertyGrid1.SelectedObject is Particle)
            {
                Particle particle = propertyGrid1.SelectedObject as Particle;
                sequence.partMan._ResetParticleSystem(particle.شماره_شناسایی, particle.نام.ToCharArray(), particle.x, particle.y, particle.تعداد_ذرات, particle.كمينه_زندگي_ذرات, particle.بيشينه_زندگي_ذرات, 1, particle.رنگ_شروع_قرمز, particle.رنگ_شروع_سبز, particle.رنگ_شروع_آبي, particle.شفافيت_آغاز_ذرات, particle.رنگ_پايان_قرمز, particle.رنگ_پايان_سبز, particle.رنگ_پايان_آبي, particle.شفافيت_پايان_ذرات, particle.مقياس_كمينه, particle.مقياس_بيشينه, particle.شتاب_x, particle.شتاب_y, particle.گرانش_x, particle.گرانش_y, particle.كمينه_سرعت_شروع, particle.بيشينه_سرعت_شروع, particle._image.نشانی.ToCharArray());
            }
            else if (propertyGrid1.SelectedObject is Sprite)
            {
                sequence.partMan._ResetSprite(selectedSprite.شماره_شناسایی, selectedSprite.نام.ToCharArray(), selectedSprite.x, selectedSprite.y, selectedSprite.مقیاس, selectedSprite.زاویه_چرخش, selectedSprite.شفافیت, selectedSprite.عرض, selectedSprite.ارتفاع, selectedSprite.سطر, selectedSprite.ستون, selectedSprite.تعدادفریم, selectedSprite.معکوس, selectedSprite.توقف_در_هر_فریم, selectedSprite.XMove, selectedSprite.YMove, selectedSprite.توقف_حرکت_در_هر_فریم);
            }
            else if (propertyGrid1.SelectedObject is Layer)
            {
                sequence.partMan._ResetLayers(sequence.partMan.p_activLayer.name.ToCharArray(), sequence.partMan.p_activLayer.z);
            }
        }

        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            MessageBox.Show("");
            if (e.KeyCode == Keys.Delete && selectedSpriteID != -1)
            {
                //DeleteSprite(selectedSpriteID);
                //spritesList.Remove(selectedSprite);
            }
        }

        private void لایهToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //PartManager Add Lyer Button
            NewLayerForm nlf = new NewLayerForm(sequence.partMan);
            nlf.ShowDialog();
        }

        private void treeView_partman_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            //partMan tree view clicking event
            sequence.partMan.Select(e);
        }

        private void دوربینToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //doorbin settings butom:
            CameraSetings cs = new CameraSetings(this.sequence.partMan);
            cs.ShowDialog();

        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(spriteEditor!=null)
                spriteEditor.Close();
            if (particleEditor != null)
                particleEditor.Close();
        }

        public void AddSprite(Sprite sprite)
        {
            //
        }

        void Save()
        {
            BinaryFormatter formatter = new BinaryFormatter();
            FileStream outFS = new System.IO.FileStream(sequence.path, FileMode.OpenOrCreate, FileAccess.Write);
            formatter.Serialize(outFS, sequence);
            outFS.Close();
        }
        void Open(string path)
        {
            if (sequence == null)
            {
                BinaryFormatter reader = new BinaryFormatter();
                FileStream inFS = new System.IO.FileStream(path, FileMode.Open, FileAccess.Read);
                this.sequence = (Sequence)reader.Deserialize(inFS);
                inFS.Close();

                sequence.path = path;
                ConsolPrint("بارگزاري فصل " + sequence.name + " از [" + sequence.path + "].");

                RunEngine();


                SetBackColor(sequence.backColor.R, sequence.backColor.G, sequence.backColor.B);

                sequence.resMan.Reload(this.treeView_resman, this);
                sequence.partMan.Reload(this.treeView_partman, this);
            }
            else
            {
                MessageBox.Show("در دست ساخت");
            }
        }

        private void ذخیرهکنToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (sequence != null)
            {
                //Saving sequence
                if (sequence.path == "")
                    ذخيرهدرجايديگرToolStripMenuItem_Click(null, null);
                this.Save();
            }
        }

        private void saveToolStripButton1_Click(object sender, EventArgs e)
        {
            ذخیرهکنToolStripMenuItem_Click(null, null);
        }

        private void ذخيرهدرجايديگرToolStripMenuItem_Click(object sender, EventArgs e)
        {
            sequence.path = Application.StartupPath + "\\" + sequence.name + ".sohiant";
            saveFileDialog1.FileName = sequence.path;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                sequence.path = saveFileDialog1.FileName;
            }
        }

        private void بازکنToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //open From File ...
            openFileDialog1.FileName = Application.StartupPath;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Open(openFileDialog1.FileName);
            }
        }

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            بازکنToolStripMenuItem_Click(sender, e);
        }

        private void newToolStripButton1_Click(object sender, EventArgs e)
        {
            فصلجدیدToolStripMenuItem_Click(sender, e);
        }

        private void صدورToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Exporting Sequence
            folderBrowserDialog1.SelectedPath = Application.StartupPath;
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                System.IO.Directory.CreateDirectory(folderBrowserDialog1.SelectedPath + "\\" + sequence.name);
                sequence.ExportTo(folderBrowserDialog1.SelectedPath + "\\" + sequence.name);
            }
        }

        private void Editing_Sprite_Click(object sender, EventArgs e)
        {
            if (selectedSpriteID != -1)
            {
                if (spriteEditor == null)
                {
                    spriteEditor = new SpriteEditor(this);
                    spriteEditor.Show();
                }
                spriteEditor.Visible = true;
                spriteEditor.Play();
                spriteEditor.setTask(2,(Sprite)selectedSprite.Clone());
            }
        }

        private void Delete_Sprite_Click(object sender, EventArgs e)
        {
            if (selectedSpriteID != -1)
            {
                sequence.partMan.RemoveSprite(selectedSpriteID);
            }
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            //runinig particle Editor ...
            if (particleEditor == null)
            {
                particleEditor = new ParticleEditor(this);
                particleEditor.Show();
            }
            particleEditor.Visible = true;
            //particleEditor.Play();

            //particleEditor.setTask(1, null);
        }

        private void Color_toolStripButton1_Click_1(object sender, EventArgs e)
        {
            ColorDialog colorDialog1 = new ColorDialog();
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                SetBackColor(colorDialog1.Color.R, colorDialog1.Color.G, colorDialog1.Color.B);
                this.sequence.backColor = colorDialog1.Color;
            }

        }

        private void بیادجولویهمهToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (selectedSpriteID != -1)
            {
                this.sequence.partMan.p_activLayer.setSpriteOnFront(selectedSprite.نام);
            }
        }        
    }
}
