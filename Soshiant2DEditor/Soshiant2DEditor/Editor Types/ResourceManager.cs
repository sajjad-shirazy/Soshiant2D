using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Soshiant2DEditor.Editor_Types
{
    [Serializable]
    public class ResourceManager
    {
        [NonSerialized()]
        public TreeView p_treeView;
        [NonSerialized()]
        public MainForm p_mainForm;
        List<ResourceMember> members;
        public ResourceManager(TreeView inTree,MainForm inMf)
        {
            p_mainForm = inMf;
            p_treeView = inTree;
            members = new List<ResourceMember>();

            this.Reload(inTree, inMf);
        }

        public void Reload(TreeView p_treeView, MainForm p_mainForm)
        {
            this.p_treeView = p_treeView;
            this.p_mainForm = p_mainForm;

            p_treeView.ImageList = p_mainForm.imageList1;
            p_treeView.Nodes.Add(p_mainForm.sequence.name);
            p_treeView.Nodes[0].ImageIndex = 0;
            //treeView.Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.5f);

            p_treeView.Nodes[0].Nodes.Add("بافت ها");
            p_treeView.Nodes[0].Nodes[0].ImageIndex = 1;
            p_treeView.Nodes[0].Nodes[0].SelectedImageIndex = 1;
            //treeView.Nodes[0].Nodes[0].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[0].ForeColor = System.Drawing.Color.DimGray;

            p_treeView.Nodes[0].Nodes.Add("صدا ها");
            p_treeView.Nodes[0].Nodes[1].ImageIndex = 1;
            p_treeView.Nodes[0].Nodes[1].SelectedImageIndex = 1;
            //treeView.Nodes[0].Nodes[1].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[1].ForeColor = System.Drawing.Color.DimGray;

            p_treeView.Nodes[0].Nodes.Add("انواع دیگر");
            p_treeView.Nodes[0].Nodes[2].ImageIndex = 1;
            p_treeView.Nodes[0].Nodes[2].SelectedImageIndex = 1;
            //treeView.Nodes[0].Nodes[2].NodeFont = new System.Drawing.Font("tahoma", 9.0f);
            //treeView.Nodes[0].Nodes[2].ForeColor = System.Drawing.Color.DimGray;

            for (int i = 0; i < members.Count; i++)
            {
                if (members[i].نوع == ResourceTypes.TEXTURE)
                {
                    p_treeView.Nodes[0].Nodes[0].Nodes.Add(members[i].نام);
                    p_treeView.Nodes[0].Nodes[0].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes.Count - 1].ImageIndex = 2;
                    p_treeView.Nodes[0].Nodes[0].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes.Count - 1].SelectedImageIndex = 2;
                }
                else if (members[i].نوع == ResourceTypes.SOUND)
                {
                    p_treeView.Nodes[0].Nodes[1].Nodes.Add(members[i].نام);
                    p_treeView.Nodes[0].Nodes[1].Nodes[p_treeView.Nodes[0].Nodes[1].Nodes.Count - 1].ImageIndex = 3;
                    p_treeView.Nodes[0].Nodes[1].Nodes[p_treeView.Nodes[0].Nodes[1].Nodes.Count - 1].SelectedImageIndex = 3;
                }
                else
                {
                    p_treeView.Nodes[0].Nodes[2].Nodes.Add(members[i].نام);
                    p_treeView.Nodes[0].Nodes[2].Nodes[p_treeView.Nodes[0].Nodes[2].Nodes.Count - 1].ImageIndex = 4;
                    p_treeView.Nodes[0].Nodes[2].Nodes[p_treeView.Nodes[0].Nodes[2].Nodes.Count - 1].SelectedImageIndex = 4;
                }

            }

            p_treeView.ExpandAll();
        }

        public void AddNewMember()
        {
            ResourceMember temp;
            OpenFileDialog openFileDialog1=new OpenFileDialog();
            openFileDialog1.Filter = "Windows bitmap (BMP) file format |*.bmp|" + "Joint Photographics Experts Group (JPEG) compressed file format |*.jpg|" + "Truevision (Targa, or TGA) image file format |*.tga|" + "Portable Network Graphics (PNG) file format |*.png|" + "DirectDraw surface (DDS) file format |*.dds|" + "Portable pixmap (PPM) file format |*.ppm|" + "Windows device-independent bitmap (DIB) file format |*.dib|" + "High dynamic range (HDR) file format |*.hdr|" + "Portable float map file format |*.pfm|" + " All files (*.*) |*.*";
            openFileDialog1.FilterIndex = 4;
            openFileDialog1.Title = "انتخاب فایل";
            openFileDialog1.Multiselect = true;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                for (int ii = 0; ii < openFileDialog1.FileNames.Length; ii++)
                {
                    string name = System.IO.Path.GetFileName(openFileDialog1.FileNames[ii]);
                    ResourceTypes rf_type = GetType(name);
                    //if (Exist(name)) MessageBox.Show("hehehehe");

                    if (rf_type == ResourceTypes.TEXTURE)
                    {
                        if (!System.IO.File.Exists(Application.StartupPath + "\\res\\textures\\" + name))
                            System.IO.File.Copy(openFileDialog1.FileNames[ii], Application.StartupPath + "\\res\\textures\\" + name);
                        p_treeView.Nodes[0].Nodes[0].Nodes.Add(name);
                        p_treeView.Nodes[0].Nodes[0].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes.Count - 1].ImageIndex = 2;
                        p_treeView.Nodes[0].Nodes[0].Nodes[p_treeView.Nodes[0].Nodes[0].Nodes.Count - 1].SelectedImageIndex = 2;
                        temp = new ResourceMember(name, "res\\textures\\" + name, rf_type);
                    }
                    else if (rf_type == ResourceTypes.SOUND)
                    {
                        if (!System.IO.File.Exists(Application.StartupPath + "\\res\\Sounds\\" + name))
                            System.IO.File.Copy(openFileDialog1.FileNames[ii], Application.StartupPath + "\\res\\Sounds\\" + name);
                        p_treeView.Nodes[0].Nodes[1].Nodes.Add(name);
                        p_treeView.Nodes[0].Nodes[1].Nodes[p_treeView.Nodes[0].Nodes[1].Nodes.Count - 1].ImageIndex = 3;
                        p_treeView.Nodes[0].Nodes[1].Nodes[p_treeView.Nodes[0].Nodes[1].Nodes.Count - 1].SelectedImageIndex = 3;
                        temp = new ResourceMember(name,"res\\Sounds\\" + name, rf_type);
                    }
                    else
                    {
                        if (!System.IO.File.Exists(Application.StartupPath + "\\res\\Others\\" + name))
                            System.IO.File.Copy(openFileDialog1.FileNames[ii], Application.StartupPath + "\\res\\Others\\" + name);
                        p_treeView.Nodes[0].Nodes[2].Nodes.Add(name);
                        p_treeView.Nodes[0].Nodes[2].Nodes[p_treeView.Nodes[0].Nodes[2].Nodes.Count - 1].ImageIndex = 4;
                        p_treeView.Nodes[0].Nodes[2].Nodes[p_treeView.Nodes[0].Nodes[2].Nodes.Count - 1].SelectedImageIndex = 4;
                        temp = new ResourceMember(name, "res\\Others\\" + name, rf_type);
                    }

                    members.Add(temp);
                    p_mainForm.ConsolPrint(name + "   به منابع اضافه شد .");
                }
            }
        }

        //public bool Exist(string name)
        //{
            //ResourceMember temp = new ResourceMember(name, GetType(name));
            //return members.Contains(temp);
        //}

        ResourceTypes GetType(string filePath)
        {
            string format = System.IO.Path.GetExtension(filePath).ToLower();

            if (format == ".jpg" || format == ".jpg" || format == ".bmp" || format == ".png" || format == ".tga" || format == ".dds" || format == ".hdr" || format == ".ppm" || format == ".dib" || format == ".pfm")
            {
                return ResourceTypes.TEXTURE;
            }
            else if (format == ".wmv" || format == ".ogg" || format == ".mp3")
            {
                return ResourceTypes.SOUND;
            }
            else
            {
                return ResourceTypes.OTHERS;
            }
        }

        public string[] GetResourceNameList(ResourceTypes rt)
        {
            //string[] ret=new string[p_treeView.Nodes[0].Nodes[0].Nodes.Count];
            //for (int i = 0; i < p_treeView.Nodes[0].Nodes[0].Nodes.Count; i++)
            //{
            //    ret[i] = p_treeView.Nodes[0].Nodes[0].Nodes[i].Text;
            //}
            //return ret;
            int index = 0;
            string[] ret = new string[GetResourcesCount(rt)];
            for (int i = 0; i < members.Count; i++)
            {
                if (members[i].نوع == rt)
                    ret[index++] = members[i].نام;
            }
            return ret;
        }

        public string[] GetResourcePathList(ResourceTypes rt)
        {
            int index = 0;
            string[] ret = new string[GetResourcesCount(rt)];
            for (int i = 0; i < members.Count; i++)
            {
                if (members[i].نوع == rt)
                    ret[index++] = members[i].نشانی;
            }
            return ret;
        }
        public int GetResourcesCount(ResourceTypes rt)
        {
            int output = 0;
            for (int i = 0; i < members.Count; i++)
            {
                if (members[i].نوع == rt)
                    output++;
            }
            return output;
        }

        public ResourceMember Find(string name)
        {
            for (int i = 0; i < members.Count; i++)
            {
                if (members[i].نام == name)
                {
                    return members[i];
                }
            }
            return null;
        }

        public void ExportTo(string path)
        {
            string content = "";
            content += "--Library Items ...\n";
            content += "--Texture Library Items ...\n";
            //Texture Library Items ...
            int TLC = this.GetResourcesCount(ResourceTypes.TEXTURE);
            string[] textureNames = this.GetResourceNameList(ResourceTypes.TEXTURE);
            string[] texturePaths = this.GetResourcePathList(ResourceTypes.TEXTURE);
            content += "texturesCount = " + TLC.ToString() + " ;\n";
            for (int i = 0; i < TLC; i++)
            {
                content += "    texture" + i.ToString() + "_name = \"" + textureNames[i] + "\" ;\n";
                texturePaths[i] = texturePaths[i].Replace("\\", "\\\\");
                content += "    texture" + i.ToString() + "_path = \"" + texturePaths[i] + "\" ;\n";
            }
            content += "--End of Texture Library Items ...\n";
            //Sound Library Items ...
            content += "--Sound Library Items ...\n";
            //Texture Library Items ...
            int SLC = this.GetResourcesCount(ResourceTypes.SOUND);
            string[] soundNames = this.GetResourceNameList(ResourceTypes.SOUND);
            string[] soundPaths = this.GetResourcePathList(ResourceTypes.SOUND);
            content += "soundsCount = " + SLC.ToString() + " ;\n";
            for (int i = 0; i < SLC; i++)
            {
                content += "    sound" + i.ToString() + "_name = \"" + soundNames[i] + "\" ;\n";
                soundPaths[i] = soundPaths[i].Replace("\\", "\\\\");
                content += "    sound" + i.ToString() + "_path = \"" + soundPaths[i] + "\" ;\n";
            }
            content += "--End of Sound Library Items ...\n";
            content += "--End of Library Items ...\n";
            System.IO.File.WriteAllText(path, content);
        }



    }
}
