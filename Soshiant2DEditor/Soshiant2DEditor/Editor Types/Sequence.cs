using System;
using System.Collections.Generic;
using System.Text;
using Soshiant2DEditor.Engine_Types;
using System.Drawing;

using System.ComponentModel;

namespace Soshiant2DEditor.Editor_Types
{
    [DefaultPropertyAttribute("نام")]
    [Serializable]
    public class Sequence
    {
        public string name = "";
        public string path = "";
        public string designer = "";
        public string comments = "";
        public PartManager partMan;
        public ResourceManager resMan;
        public int engine_width;
        public int engine_height;
        public int engine_depth;
        public Color backColor;
        public string focusLayer;
        public string pivotLayer;
        public string cameraLookAt;


        public Sequence()
        {
            this.backColor = Color.Black;
        }

        
        [CategoryAttribute("خصوصیات"), DescriptionAttribute("نام فصل")]
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

        [CategoryAttribute("خصوصیات"), DescriptionAttribute("طراح فصل")]
        public string طراح
        {
            get
            {
                return designer;
            }
            set
            {
                designer = value;
            }
        }

        [CategoryAttribute("خصوصیات"), DescriptionAttribute("توضیحات فصل")]
        public string توضیحات
        {
            get
            {
                return comments;
            }
            set
            {
                comments = value;
            }
        }

        [CategoryAttribute("خصوصیات"), DescriptionAttribute("نشانی فصل"), ReadOnlyAttribute(true)]
        public string نشانی
        {
            get
            {
                return path;
            }
            set
            {
                path = value;
            }
        }



        public void ExportTo(string path)
        {
            
            string lib_ext = ".lib.lua";
            string lyr_ext = ".lyr.lua";
            string content = "";

            content += "name = \"" + this.name + "\" ;\n";
            content += "designer = \"" + this.designer + "\" ;\n";
            content += "backColor_r = " + this.backColor.R.ToString() + " ;\n";
            content += "backColor_g = " + this.backColor.G.ToString() + " ;\n";
            content += "backColor_b = " + this.backColor.B.ToString() + " ;\n";
            content += "isPhysical = true ;\n";
            content += "focusLayer = \"" + this.focusLayer+ "\" ;\n";
            content += "pivotLayer = \"" + this.pivotLayer + "\" ;\n";
            content += "cameraLookAt = \"" + this.cameraLookAt + "\" ;\n";

            content += "library = \"seq" + lib_ext + "\" ;\n";
            this.resMan.ExportTo(path + "\\seq" + lib_ext);

            content += "\n";

            //Layers Items
            content += "--Layers Items ...\n";
            content += "layersCount = " + this.partMan.layers.Count.ToString() + " ;\n";
            for (int i = 0; i < this.partMan.layers.Count; i++)
            {
                this.partMan.layers[i].ExportTo(path + "\\" + "layer_" + i.ToString() + lyr_ext);
            }
            content += "--End of Layers Items ...\n";
            //GUI ITEMS
            //
            //
            content += "--GUI Items ...\n";
            content += "\n";
            content += "--End of GUI Items ...\n";



            //Writing File
            System.IO.File.WriteAllText(path + "\\seq.seq.lua", content);
        }
    }
}
