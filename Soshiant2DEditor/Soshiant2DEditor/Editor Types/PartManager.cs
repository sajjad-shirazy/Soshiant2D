using System;
using System.Collections.Generic;
using System.Text;

using System.Windows.Forms;
using Soshiant2DEditor.Engine_Types;
using System.Runtime.InteropServices;

namespace Soshiant2DEditor.Editor_Types
{
    public enum PartTypes
    {
        SPRITE,
        PARTCLE
    }
    [Serializable]
    public class PartManager
    {
        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int AddLayer(char[] name, double z);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void ResetLayers(char[] name, double z);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void SetActivLayer(int ID);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int AddSprite(char[] name, double x, double y, double scale, double rotation, int alpha, int row, int col, int totalFrame, int width, int height, int anim_timer, int isRev, double xmov, double ymov, int mov_timer, char[] imagePath, bool isPhysical, bool isStatic, float mass, float inertia);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int GetSpriteWidth(int ID);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int GetSpriteHeight(int ID);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        public static extern void ResetSprite(int ID, char[] name, double x, double y, double scale, double rotation, int alpha, int width, int height, int row, int col, int totalFrame, int isRev, int anim_timer, double xmov, double ymov, int mov_timer);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void DeleteSprite(int ID);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void SetFocusLayer(int ID);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void SetPivotLayer(int ID);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void SetSelectedSprite(int ID);

        //shape Editing
        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void setActiveShape(int id);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void setShapeVisible(int id, bool value);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int AddLineShape(float ax, float ay, float bx, float by,float r);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void ResetLineShape(float ax, float ay, float bx, float by,float r);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int AddCircleShape(float x, float y, float r);

        //Particel Editing Methods
        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern int AddParticleSystem(char[] name, double x, double y, int count, int mnpl, int mxpl, int isRep, int scr, int scg, int scb, int sa, int ecr, int ecg, int ecb, int ea, float mnScale, float mxScale, float acc_x, float acc_y, float grv_x, float grv_y, float mnVel, float mxVel, char[] imagePath);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void ResetParticleSystem(int ID, char[] name, double x, double y, int count, int mnpl, int mxpl, int isRep, int scr, int scg, int scb, int sa, int ecr, int ecg, int ecb, int ea, float mnScale, float mxScale, float acc_x, float acc_y, float grv_x, float grv_y, float mnVel, float mxVel, char[] imagePath);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void ResetParticleSystemPos(int ID, int x, int y);

        [DllImport("Soshiant2D.dll", SetLastError = true)]
        static extern void ResetParticleSystemImage(int ID, char[] imagePath);

        [NonSerialized()]
        public TreeView p_treeView;
        [NonSerialized()]
        public MainForm p_mainForm;
        public List<Layer> layers;
        public Layer p_activLayer;
        public int spriteCounter = 0;
        public int particleCounter = 0;

        public PartManager(TreeView p_treeView, MainForm p_mainForm)
        {
            this.p_treeView = p_treeView;
            this.p_mainForm = p_mainForm;
            spriteCounter = 0;

            layers = new List<Layer>();

            p_treeView.ImageList = p_mainForm.imageList1;
            p_treeView.Nodes.Add(p_mainForm.sequence.name);
            p_treeView.Nodes[0].ImageIndex = 0;
            //treeView.Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.5f);

            p_treeView.Nodes[0].Nodes.Add("لایه ها");
            p_treeView.Nodes[0].Nodes[0].ImageIndex = 5;
            p_treeView.Nodes[0].Nodes[0].SelectedImageIndex = 5;
            //treeView.Nodes[0].Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[0].ForeColor = System.Drawing.Color.DimGray;

            p_treeView.Nodes[0].Nodes.Add("رابط های کاربر");
            p_treeView.Nodes[0].Nodes[1].ImageIndex = 8;
            p_treeView.Nodes[0].Nodes[1].SelectedImageIndex = 8;
            //treeView.Nodes[0].Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[0].ForeColor = System.Drawing.Color.DimGray;

            AddNewLayer("layer_0", 0);
            _SetActiveLayer("layer_0");

            
        }

        public void Reload(TreeView p_treeView, MainForm p_mainForm)
        {
            this.p_treeView = p_treeView;
            this.p_mainForm = p_mainForm;

            p_treeView.ImageList = p_mainForm.imageList1;
            p_treeView.Nodes.Add(p_mainForm.sequence.name);
            p_treeView.Nodes[0].ImageIndex = 0;
            //treeView.Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.5f);

            p_treeView.Nodes[0].Nodes.Add("لایه ها");
            p_treeView.Nodes[0].Nodes[0].ImageIndex = 5;
            p_treeView.Nodes[0].Nodes[0].SelectedImageIndex = 5;
            //treeView.Nodes[0].Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[0].ForeColor = System.Drawing.Color.DimGray;

            p_treeView.Nodes[0].Nodes.Add("رابط های کاربر");
            p_treeView.Nodes[0].Nodes[1].ImageIndex = 8;
            p_treeView.Nodes[0].Nodes[1].SelectedImageIndex = 8;
            //treeView.Nodes[0].Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[0].ForeColor = System.Drawing.Color.DimGray;

            for (int i = 0; i < layers.Count; i++)
            {
                this.AddNewLayer(layers[i],false);

                for (int j = 0; j < layers[i].sprites.Count; j++)
                {
                    if (layers[i].sprites[j].type == EntityTypes.Sprite)
                        this.AddSprite(layers[i].sprites[j], false);
                    else if (layers[i].sprites[j].type == EntityTypes.Particle)
                        this.AddParticleSystem((Particle)layers[i].sprites[j], false);
                }
            }
            p_treeView.ExpandAll();

        }

        public void AddNewLayer(string name, double z)
        {
            int nodIndex = p_treeView.Nodes[0].Nodes[0].Nodes.Count;
            if (name == "")
            {
                name = "layer_" + p_treeView.Nodes[0].Nodes[0].Nodes.Count.ToString();
            }
            if (!IsLayerNameUnique(name))
            {
                MessageBox.Show("نام لایه تکراری است");
                return;
            }
            layers.Add(new Layer(AddLayer(name.ToCharArray(), z), name, 0, 0, z, nodIndex));

            p_treeView.Nodes[0].Nodes[0].Nodes.Add(name);
            p_treeView.Nodes[0].Nodes[0].Nodes[nodIndex].ImageIndex = 5;
            p_treeView.Nodes[0].Nodes[0].Nodes[nodIndex].SelectedImageIndex = 5;
        }
        public void AddNewLayer(Layer layer, bool add2Layers)
        {
            layer.nodeIndex = p_treeView.Nodes[0].Nodes[0].Nodes.Count;
            if (!IsLayerNameUnique(layer.نام))
            {
                MessageBox.Show("نام لایه تکراری است");
                return;
            }
            layer.شماره_شناسایی = AddLayer(layer.نام.ToCharArray(), layer.عمق);
            if (add2Layers)
                layers.Add(layer);

            p_treeView.Nodes[0].Nodes[0].Nodes.Add(layer.نام);
            p_treeView.Nodes[0].Nodes[0].Nodes[layer.nodeIndex].ImageIndex = 5;
            p_treeView.Nodes[0].Nodes[0].Nodes[layer.nodeIndex].SelectedImageIndex = 5;
        }

        public string[] GetLayersNameList()
        {
            string[] ret = new string[p_treeView.Nodes[0].Nodes[0].Nodes.Count];
            for (int i = 0; i < p_treeView.Nodes[0].Nodes[0].Nodes.Count; i++)
            {
                ret[i] = p_treeView.Nodes[0].Nodes[0].Nodes[i].Text;
            }
            return ret;
        }

        public int GetLayersCont()
        {
            return layers.Count;
        }

        public int AddSprite(Sprite sprite,bool add2Layer)
        {
            if (_SetActiveLayer(sprite.لایه) == -1)
            {
                MessageBox.Show("لایه فعال پیدا نشد");
                return -1;
            }
            string path = Application.StartupPath + "\\" + sprite._image.نشانی;
            int id = AddSprite(sprite.نام.ToCharArray(), sprite.x, sprite.y, sprite.مقیاس, sprite.زاویه_چرخش, sprite.شفافیت, sprite.سطر, sprite.ستون, sprite.تعدادفریم, sprite.عرض, sprite.ارتفاع, sprite.توقف_در_هر_فریم, sprite.معکوس, sprite.XMove, sprite.YMove, sprite.توقف_حرکت_در_هر_فریم, path.ToCharArray(), sprite.دارای_خواص_فیزیکی, sprite.ایستایی, sprite.جرم, sprite.اینرسی);

            sprite.شماره_شناسایی = id;
            p_mainForm.propertyGrid1.SelectedObject = sprite;
            SetSelectedSprite(id);

            for (int i = 0; i < sprite.shapesList.Count; i++)
            {
                if (sprite.shapesList[i].type == ShapeTypes.LINE)
                {
                    LineShape shape = (LineShape)sprite.shapesList[i];
                    shape.id = AddLineShape(shape.aX, shape.aY, shape.bX, shape.bY, shape.thickness);
                }
                else if (sprite.shapesList[i].type == ShapeTypes.CIRCLE)
                {
                    CircleShape shape = (CircleShape)sprite.shapesList[i];
                    shape.id = AddCircleShape(shape.x, shape.y, shape.r);
                }

                //setShapeVisible(sprite.shapesList[i].id, true);
            }

            if (add2Layer)
                this.p_activLayer.AddSprite(sprite);

            p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes.Add(sprite.نام);
            p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes.Count - 1].ImageIndex = 6;
            p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes.Count - 1].SelectedImageIndex = 6;

            p_mainForm.ConsolPrint(sprite.نام + "   به صحنه اضافه شد .");

            return id;
        }
        public int AddParticleSystem(Particle particle, bool add2Layer)
        {
            if (_SetActiveLayer(particle.لایه) == -1)
            {
                MessageBox.Show("لایه فعال پیدا نشد");
                return -1;
            }
            particle.شماره_شناسایی = AddParticleSystem(particle.نام.ToCharArray(), particle.x, particle.y, particle.تعداد_ذرات, particle.كمينه_زندگي_ذرات, particle.بيشينه_زندگي_ذرات, 1, particle.رنگ_شروع_قرمز, particle.رنگ_شروع_سبز, particle.رنگ_شروع_آبي, particle.شفافيت_آغاز_ذرات, particle.رنگ_پايان_قرمز, particle.رنگ_پايان_سبز, particle.رنگ_پايان_آبي, particle.شفافيت_پايان_ذرات, particle.مقياس_كمينه, particle.مقياس_بيشينه, particle.شتاب_x, particle.شتاب_y, particle.گرانش_x, particle.گرانش_y, particle.كمينه_سرعت_شروع, particle.بيشينه_سرعت_شروع, particle._image.نشانی.ToCharArray());
            //
            p_mainForm.propertyGrid1.SelectedObject = particle;
            SetSelectedSprite(particle.شماره_شناسایی);

            //for (int i = 0; i < sprite.shapesList.Count; i++)
            //{
            //    if (sprite.shapesList[i].type == ShapeTypes.LINE)
            //    {
            //        LineShape shape = (LineShape)sprite.shapesList[i];
            //        shape.id = AddLineShape(shape.aX, shape.aY, shape.bX, shape.bY, shape.thickness);
            //    }
            //    else if (sprite.shapesList[i].type == ShapeTypes.CIRCLE)
            //    {
            //        CircleShape shape = (CircleShape)sprite.shapesList[i];
            //        shape.id = AddCircleShape(shape.x, shape.y, shape.r);
            //    }

            //    //setShapeVisible(sprite.shapesList[i].id, true);
            //}

            if (add2Layer)
                this.p_activLayer.AddSprite(particle);

            p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes.Add(particle.نام);
            p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes.Count - 1].ImageIndex = 7;
            p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes[p_activLayer.nodeIndex].Nodes.Count - 1].SelectedImageIndex = 7;

            p_mainForm.ConsolPrint(particle.نام + "   به صحنه اضافه شد .");

            return particle.شماره_شناسایی;
        }

        public void RemovLayer()
        {
            RemoveLayerNod(p_activLayer.name);
            layers.Remove(p_activLayer);
        }

        public void Select(TreeNodeMouseClickEventArgs e)
        {
            switch (e.Node.Level)
            {
                case 0:
                    p_mainForm.propertyGrid1.SelectedObject = p_mainForm.sequence;
                    break;
                case 2:
                    if (e.Node.Parent.Index == 0)
                    {
                        //In Layers cat
                        _SetActiveLayer(e.Node.Text);
                        p_mainForm.propertyGrid1.SelectedObject = p_activLayer;
                    }
                    else
                    {
                        //In GUI cat
                    }
                    break;
                case 3:
                    if (e.Node.Parent.Parent.Index == 0)
                    {
                        //In Layers cat
                        //select an Sprite or Particle
                        _SetActiveLayer(e.Node.Parent.Text);
                        p_mainForm.selectedSprite = p_activLayer.Find(e.Node.Text);
                        p_mainForm.selectedSpriteID = p_mainForm.selectedSprite.شماره_شناسایی;
                        SetSelectedSprite(p_mainForm.selectedSpriteID);
                        p_mainForm.propertyGrid1.SelectedObject = p_mainForm.selectedSprite;
                    }
                    else
                    {
                        //In GUI cat
                    }
                    break;
            }
        }

        //Forl List Finder Metod
        bool IsActiveLayer(Layer layer)
        {
            if (layer == p_activLayer)
                return true;
            else
                return false;
        }

        bool IsLayerNameUnique(string name)
        {
            for (int i = 0; i < p_treeView.Nodes[0].Nodes[0].Nodes.Count; i++)
            {
                if (p_treeView.Nodes[0].Nodes[0].Nodes[i].Text == name)
                    return false;
            }
            return true;
        }

        void RemoveLayerNod(string name)
        {
            for (int i = 0; i < p_treeView.Nodes[0].Nodes[0].Nodes.Count; i++)
            {
                if (p_treeView.Nodes[0].Nodes[0].Nodes[i].Text == name)
                {
                    p_treeView.Nodes[0].Nodes[0].Nodes[i].Remove();
                    return;
                }
            }
        }

        public int _SetActiveLayer(string name)
        {
            p_activLayer = null;
            for (int i = 0; i < layers.Count; i++)
            {
                if (layers[i].name == name)
                {
                    p_activLayer = layers[i];
                    SetActivLayer(p_activLayer.id);
                    //p_mainForm.ConsolPrint("تغییر لایه فعال به : " + p_activLayer.name);
                    p_mainForm.toolStripStatusLabel_ActivLayer.Text = "لایه فعال : [" + p_activLayer.name+']';

                    return p_activLayer.id;
                }
            }
            return -1;
        }

        public void _ResetSprite(int ID, char[] name, double x, double y, double scale, double rotation, int alpha, int width, int height, int row, int col, int totalFrame, int isRev, int anim_timer, double xmov, double ymov, int mov_timer)
        {
            ResetSprite(ID, name, x, y, scale, rotation, alpha, width, height, row, col, totalFrame, isRev, anim_timer, xmov, ymov, mov_timer);
        }

        public void _ResetParticleSystem(int ID, char[] name, double x, double y, int count, int mnpl, int mxpl, int isRep, int scr, int scg, int scb, int sa, int ecr, int ecg, int ecb, int ea, float mnScale, float mxScale, float acc_x, float acc_y, float grv_x, float grv_y, float mnVel, float mxVel, char[] imagePath)
        {
            ResetParticleSystem(ID, name, x, y, count, mnpl, mxpl, isRep, scr, scg, scb, sa, ecr, ecg, ecb, ea, mnScale, mxScale, acc_x, acc_y, grv_x, grv_y, mnVel, mxVel, imagePath);
        }

        public void _ResetLayers(char[] name, double z)
        {
            ResetLayers(name, z);
        }

        public Layer FindLayer(string name)
        {
            for (int i = 0; i < layers.Count; i++)
            {
                if (layers[i].name == name)
                {
                    return layers[i];
                }
            }
            return null;
        }

        public void _SetFocusLayer(string name)
        {
            p_mainForm.sequence.focusLayer = name;
            SetFocusLayer(FindLayer(name).id);
        }

        public void _SetPivotLayer(string name)
        {
            p_mainForm.sequence.pivotLayer = name;
            SetPivotLayer(FindLayer(name).id);
        }
        public void _SetCameraLookAt(string name)
        {
            p_mainForm.sequence.cameraLookAt = name;
        }
        public void RemoveSprite(int id)
        {
            if (p_activLayer != null)
            {
                for (int i = 0; i < this.p_activLayer.sprites.Count; i++)
                {
                    if (this.p_activLayer.sprites[i].شماره_شناسایی == id)
                    {
                        if (MessageBox.Show("آیا مطمئن هستید ؟", "", MessageBoxButtons.YesNo) == DialogResult.Yes)
                        {
                            DeleteSprite(id);
                            this.p_activLayer.sprites.RemoveAt(i);
                            this.ResetTree();
                        }
                    }
                }
            }
            else
            {
                MessageBox.Show("Editor :: no active layer is selected");
            }
        }
        void ResetTree()
        {
            p_treeView.Nodes.Clear();
            p_treeView.ImageList = p_mainForm.imageList1;
            p_treeView.Nodes.Add(p_mainForm.sequence.name);
            p_treeView.Nodes[0].ImageIndex = 0;
            //treeView.Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.5f);

            p_treeView.Nodes[0].Nodes.Add("لایه ها");
            p_treeView.Nodes[0].Nodes[0].ImageIndex = 5;
            p_treeView.Nodes[0].Nodes[0].SelectedImageIndex = 5;
            //treeView.Nodes[0].Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[0].ForeColor = System.Drawing.Color.DimGray;

            p_treeView.Nodes[0].Nodes.Add("رابط های کاربر");
            p_treeView.Nodes[0].Nodes[1].ImageIndex = 8;
            p_treeView.Nodes[0].Nodes[1].SelectedImageIndex = 8;
            //treeView.Nodes[0].Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[0].ForeColor = System.Drawing.Color.DimGray;

            for (int i = 0; i < layers.Count; i++)
            {
                int nodIndex = p_treeView.Nodes[0].Nodes[0].Nodes.Count;
                p_treeView.Nodes[0].Nodes[0].Nodes.Add(layers[i].name);
                p_treeView.Nodes[0].Nodes[0].Nodes[nodIndex].ImageIndex = 5;
                p_treeView.Nodes[0].Nodes[0].Nodes[nodIndex].SelectedImageIndex = 5;

                for (int j = 0; j < layers[i].sprites.Count; j++)
                {
                    p_treeView.Nodes[0].Nodes[0].Nodes[i].Nodes.Add(layers[i].sprites[j].نام);
                    p_treeView.Nodes[0].Nodes[0].Nodes[i].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes[i].Nodes.Count - 1].ImageIndex = 6;
                    p_treeView.Nodes[0].Nodes[0].Nodes[i].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes[i].Nodes.Count - 1].SelectedImageIndex = 6;
                }
            }
            p_treeView.ExpandAll();
        }
    }
}