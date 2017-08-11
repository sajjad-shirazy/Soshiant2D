using System;
using System.Collections.Generic;
using System.Text;

using System.ComponentModel;

namespace Soshiant2DEditor.Engine_Types
{
    [DefaultPropertyAttribute("نام")]
    [Serializable]
    public class Layer
    {
        public List<Sprite> sprites;
        public double x, y, z;
        public string name;
        public int id;
        //Tree View Nod Index ...
        public int nodeIndex;

        [CategoryAttribute("خصوصیات"), DescriptionAttribute("شماره شناسایی منحصر به فرد . تنها با این شماره می شود به لایه های موتور سوشیانت دسترسی پیدا کرد ."), ReadOnlyAttribute(true)]
        public int شماره_شناسایی
        {
            get
            {
                return id;
            }
            set
            {
                id = value;
            }
        }
        [CategoryAttribute("خصوصیات"), DescriptionAttribute("نام لایه")]
        public string نام
        {
            get
            {
                return name;
            }
            set
            {
                name = value;
            }
        }
        [CategoryAttribute("خصوصیات"), DescriptionAttribute("عمق لایه")]
        public double عمق
        {
            get
            {
                return z;
            }
            set
            {
                z = value;
            }
        }


        public Layer(int id, string name, double x, double y, double z, int nodeIndex)
        {
            this.id = id;
            this.name = name;
            this.x = x;
            this.y = y;
            this.z = z;
            this.nodeIndex = nodeIndex;
            sprites = new List<Sprite>();
        }

        public void AddSprite(Sprite sprite)
        {
            this.sprites.Add(sprite);
        }

        public bool IsUnique(string name)
        {
            for (int i = 0; i < sprites.Count; i++)
            {
                if (sprites[i].نام == name)
                    return false;
            }
            return true;
        }

        public Sprite Find(int ID)
        {
            for (int i = 0; i < sprites.Count; i++)
            {
                if (sprites[i].شماره_شناسایی == ID)
                    return sprites[i];
            }
            return null;
        }
        public Sprite Find(string name)
        {
            for (int i = 0; i < sprites.Count; i++)
            {
                if (sprites[i].نام== name)
                    return sprites[i];
            }
            return null;
        }
        public void setSpriteOnFront(string name)
        {
            Sprite temp = this.Find(name);
            for (int i = 0; i < sprites.Count; i++)
            {
                if (sprites[i].نام == name)
                {
                    sprites[i] = sprites[sprites.Count - 1];
                    sprites[sprites.Count - 1] = temp;
                }
            }
        }

        public void ExportTo(string path)
        {
            string spt_ext = ".spt.lua";
            string prt_ext = ".prt.lua";

            string content = "";

            content += "--layer Items ...\n";

            content += "name = \"" + this.name + "\" ;\n";
            content += "depth = " + this.z + " ;\n";
            content += "spritesCount = " + this.sprites.Count.ToString() + " ;\n";

            content += "sprites = {} ;\n";
            string parent = System.IO.Path.GetDirectoryName(path);
            for (int j = 0; j < this.sprites.Count; j++)
            {
                if (sprites[j].type == EntityTypes.Sprite)
                {
                    content += "    sprite_" + j.ToString() + "_type = \"" + sprites[j].type.ToString() + "\" ;\n";
                    this.sprites[j].ExportTo(parent + "\\" + this.name + "_sprite" + "_" + j.ToString() + spt_ext);
                }
                else if (sprites[j].type == EntityTypes.Particle)
                {
                    content += "    sprite_" + j.ToString() + "_type = \"" + sprites[j].type.ToString() + "\" ;\n";
                    this.sprites[j].ExportTo(parent + "\\" + this.name + "_particle" + "_" + j.ToString() + prt_ext);
                }
                //if (sprites[j].type == EntityTypes.Sprite)
                //{
                //    content += "    sprites[" + j.ToString() + "] = {Path = \"" + this.name + "_sprite" + "_" + j.ToString() + spt_ext + "\" , Type = \"" + sprites[j].type.ToString() + "\"} ;\n";
                //    this.sprites[j].ExportTo(parent + "\\" + this.name + "_sprite" + "_" + j.ToString() + spt_ext);
                //}
                //else if (sprites[j].type == EntityTypes.Particle)
                //{
                //    content += "    sprites[" + j.ToString() + "] = {Path = \"" + this.name + "_particle" + "_" + j.ToString() + prt_ext + "\" , Type = \"" + sprites[j].type.ToString() + "\"} ;\n";
                //    this.sprites[j].ExportTo(parent + "\\" + this.name + "_particle" + "_" + j.ToString() + prt_ext);
                //}
            }

            content += "--End of layer...\n";
            
            System.IO.File.WriteAllText(path, content);
        }
    }
    public enum EntityTypes
    {
        Sprite,
        Particle,
        Gui
    }
}
