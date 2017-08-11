using System;
using System.Collections.Generic;
using System.Text;

using System.ComponentModel;

namespace Soshiant2DEditor.Editor_Types
{
    public enum ResourceTypes
    {
        TEXTURE,
        SOUND,
        OTHERS
    }

    [DefaultPropertyAttribute("نام")]
    [Serializable]
    public class ResourceMember
    {
        private string name;
        private string path;
        private ResourceTypes type;

        [DescriptionAttribute("نام منبع")]
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
        [DescriptionAttribute("نوع منبع")]
        public ResourceTypes نوع
        {
            get
            {
                return type;
            }
            set
            {
                type = value;
            }

        }
        [DescriptionAttribute("نشانی منبع")]
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

        public ResourceMember(string name,string path, ResourceTypes type)
        {
            this.name = name;
            this.path = path;
            this.type = type;
        }
    }
}
