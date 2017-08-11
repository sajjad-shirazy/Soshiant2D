using System;
using System.Collections.Generic;
using System.Text;


using System.ComponentModel;

using Soshiant2DEditor;
using Soshiant2DEditor.Editor_Types;


namespace Soshiant2DEditor.Engine_Types
{
    [DefaultPropertyAttribute("نام")]
    [Serializable]
    public class Sprite : System.ICloneable
    {
        protected int _ID;
        protected double _x;
        protected double _y;
        protected double _scale;
        protected double _rotation;
        protected int _alpha;
        protected int _row;
        protected int _col;
        protected int _totalFrames;
        protected int _width;
        protected int _height;
        protected int _anim_timer;
        protected int _isRev;
        protected double _xmov;
        protected double _ymov;
        protected int _move_timer;
        public ResourceMember _image;
        protected string _name;
        protected string _layer;

        public List<Shape> shapesList;
        public string script;
        protected bool _isPhysical;
        protected bool _isStatic;
        protected float _mass;
        protected float _inertia;
        protected float _friction;
        protected float _elastic;
        public EntityTypes type;

         
        
        [CategoryAttribute("ظاهر"), DescriptionAttribute("شماره شناسایی منحصر به فرد . تنها با این شماره می شود به اسپریت های موتور سوشیانت دسترسی پیدا کرد ."), ReadOnlyAttribute(true)]
        public int شماره_شناسایی
        {
            get
            {
                return _ID;
            }
            set
            {
                _ID = value;
            }
        }

        [CategoryAttribute("ظاهر"), DescriptionAttribute("نام اسپریت")]
        public string نام
        {
            get
            {
                return _name;
            }
            set
            {
                _name = value;
            }
        }

        [CategoryAttribute("ظاهر"), DescriptionAttribute("لایه ای که اسپریت در آن نمایش داده می شود")] 
        public string لایه
        {
            get
            {
                return _layer;
            }
            set
            {
                //check For Lyer Is true;
                _layer = value;
            }
        }

        [CategoryAttribute("ظاهر"), DescriptionAttribute("موقعیت اسپریت در بردار x ها .")]
        public double x
        {
            get
            {
                return _x;
            }
            set
            {
                _x = value;
            }
        }
        [CategoryAttribute("ظاهر"), DescriptionAttribute("موقعیت اسپریت در بردار y ها.")]
        public double y
        {
            get
            {
                return _y;
            }
            set
            {
                _y = value;
            }
        }
        [CategoryAttribute("ظاهر"), DescriptionAttribute("مقیاس اسپریت . در حالت پیشفرض با مقدار 1.0 اسپریت به صورت غادی نمایش داده می شود .")]
        public double مقیاس
        {
            get
            {
                return _scale;
            }
            set
            {
                _scale = value;
            }
        }
        [CategoryAttribute("ظاهر"), DescriptionAttribute("زاویه چرخش اسپریت")]
        public double زاویه_چرخش
        {
            get
            {
                return _rotation;
            }
            set
            {
                _rotation = value;
            }
        }
        [CategoryAttribute("ظاهر"), DescriptionAttribute("اسپریت شفافیت")]
        public int شفافیت
        {
            get
            {
                return _alpha;
            }
            set
            {
                _alpha = value;
            }
        }
        [CategoryAttribute("پویانما"), DescriptionAttribute("تعداد سطر ها / برای بافت های چند جزئی")]
        public int سطر
        {
            get
            {
                return _row;
            }
            set
            {
                _row = value;
            }
        }
        [CategoryAttribute("پویانما"), DescriptionAttribute("تعداد ستون ها / برای بافت های چند جزئی")]
        public int ستون
        {
            get
            {
                return _col;
            }
            set
            {
                _col = value;
            }
        }
        [CategoryAttribute("پویانما"), DescriptionAttribute("تعداد کل فریم ها / برای بافت های چند جزئی")]
        public int تعدادفریم
        {
            get
            {
                return _totalFrames;
            }
            set
            {
                _totalFrames = value;
            }
        }
        [CategoryAttribute("ظاهر"), DescriptionAttribute("عرض اسپریت / برای بافت های چند جزئی عرض اجزا در نظر گرفته شود .")]
        public int عرض
        {
            get
            {
                return _width;
            }
            set
            {
                _width = value;
            }
        }
        [CategoryAttribute("ظاهر"), DescriptionAttribute("ارتفاع اسپریت / برای بافت های چند جزئی ارتفاع اجزائ در نظر گرفته شود .")]
        public int ارتفاع
        {
            get
            {
                return _height;
            }
            set
            {
                _height = value;
            }
        }
        [CategoryAttribute("پویانما"), DescriptionAttribute("قبل از نمایش فریم بعد بر روی صفحه نمایش به این میزان بر حسب میلی ثانیه توقف می کند .")]
        public int توقف_در_هر_فریم
        {
            get
            {
                return _anim_timer;
            }
            set
            {
                _anim_timer = value;
            }
        }
        [CategoryAttribute("پویانما"), DescriptionAttribute("آیا پویانمایی به طور معکوس انجام شود ؟")]
        public int معکوس
        {
            get
            {
                return _isRev;
            }
            set
            {
                _isRev = value;
            }
        }
        [CategoryAttribute("حرکت"), DescriptionAttribute("در هر بار بروز رسانی به این میزان اسپریت در راستای X های جابجا می شود .")]
        public double XMove
        {
            get
            {
                return _xmov;
            }
            set
            {
                _xmov = value;
            }
        }
        [CategoryAttribute("حرکت"), DescriptionAttribute("در هر بار بروز رسانی به این میزان اسپریت در راستای Y های جابجا می شود .")]
        public double YMove
        {
            get
            {
                return _ymov;
            }
            set
            {
                _ymov = value;
            }
        }
        [CategoryAttribute("حرکت"), DescriptionAttribute("حرکت اسپریت تا قبل از نمایش در موقعیت بعدی به این مقدار بر حسب میلی ثانیه توقف می کند.")]
        public int توقف_حرکت_در_هر_فریم
        {
            get
            {
                return _move_timer;
            }
            set
            {
                _move_timer = value;
            }
        }
        [CategoryAttribute("بافت"), DescriptionAttribute("فایل بافتی که در موقعیت اسپریت نمایش داده می شود ."), ReadOnlyAttribute(true)]
        public string بافت
        {
            get
            {
                return _image.نام;
            }
            set
            {
                _image.نام = value;
            }
        }

        [CategoryAttribute("فیزیک"), DescriptionAttribute("دارای خواص فیزیکی هست ؟"), ReadOnlyAttribute(true)]
        public bool دارای_خواص_فیزیکی
        {
            get
            {
                return _isPhysical;
            }
            set
            {
                _isPhysical = value;
            }
        }

        [CategoryAttribute("فیزیک"), DescriptionAttribute("آیا شیء ایستا هست ؟"), ReadOnlyAttribute(true)]
        public bool ایستایی
        {
            get
            {
                return _isStatic;
            }
            set
            {
                _isStatic = value;
            }
        }

        [CategoryAttribute("فیزیک"), DescriptionAttribute("جرم اسپریت"), ReadOnlyAttribute(true)]
        public float جرم
        {
            get
            {
                return _mass;
            }
            set
            {
                _mass = value;
            }
        }

        [CategoryAttribute("فیزیک"), DescriptionAttribute("اینرسی اسپریت")]
        public float اینرسی
        {
            get
            {
                return _inertia;
            }
            set
            {
                _inertia = value;
            }
        }
        [CategoryAttribute("فیزیک"), DescriptionAttribute("ضريب اصطكاك بين 0.0 تا 1.0")]
        public float ضريب_اصطكاك
        {
            get
            {
                return _friction;
            }
            set
            {
                _friction = value;
            }
        }

        [CategoryAttribute("فیزیک"), DescriptionAttribute("ضريب كشسان بين 0.0 تا 1.0")]
        public float ضريب_كشسان
        {
            get
            {
                return _elastic;
            }
            set
            {
                _elastic = value;
            }
        }

        public Sprite()
        {
            _x = 0; _y = 0;
            _scale = 1.0f;
            _rotation = 0;
            _row = 1; _ID = 0;
            _col = 1; _totalFrames = 1;
            _anim_timer = 0;
            _isRev = 1;
            _xmov = 0.0f;
            _ymov = 0.0f;
            _move_timer = 0;

            _image = null;
            _name = "";
            script = "";

            shapesList = new List<Shape>();

        }

        public void AddShape(Shape shape)
        {
            shapesList.Add(shape);
        }

        public void RemoveShape(int id)
        {
            for (int i = 0; i < shapesList.Count; i++)
            {
                if (shapesList[i].id == id)
                {
                    shapesList.RemoveAt(i);
                    break;
                }
            }
        }

        public object Clone()
        {
            return MemberwiseClone();
        }

        public int getLineShapeCount()
        {
            int output = 0;
            for (int i = 0; i < shapesList.Count; i++)
            {
                if (shapesList[i].type == ShapeTypes.LINE)
                    output++;
            }
            return output;
        }
        public int getCircleShapeCount()
        {
            int output = 0;
            for (int i = 0; i < shapesList.Count; i++)
            {
                if (shapesList[i].type == ShapeTypes.CIRCLE)
                    output++;
            }
            return output;
        }
        public int getPolygonShapeCount()
        {
            int output = 0;
            for (int i = 0; i < shapesList.Count; i++)
            {
                if (shapesList[i].type == ShapeTypes.POLYGON)
                    output++;
            }
            return output;
        }

        virtual public void ExportTo(string path)
        {
            string content = "";

            content += "--sprite...\n";

            content += "name = \"" + this.نام + "\" ;\n";
            content += "image = \"" + this.بافت + "\" ;\n";
            content += "x = " + this.x.ToString() + " ;\n";
            content += "y = " + this.y.ToString() + " ;\n";
            content += "height = " + this.ارتفاع.ToString() + " ;\n";
            content += "width = " + this.عرض.ToString() + " ;\n";
            content += "rotation = " + this.زاویه_چرخش.ToString() + " ;\n";
            content += "scale = " + this.مقیاس.ToString() + " ;\n";
            content += "alpha = " + this.شفافیت.ToString() + " ;\n";
            content += "isPhysical = " + this.دارای_خواص_فیزیکی.ToString().ToLower() + " ;\n";
            content += "isStatic = " + this.ایستایی.ToString().ToLower() + " ;\n";
            content += "mass = " + this.جرم.ToString() + " ;\n";
            content += "inertia = " + this.اینرسی.ToString() + " ;\n";
            content += "friction = " + this.ضريب_اصطكاك.ToString() + " ;\n";
            content += "elastic = " + this.ضريب_كشسان.ToString() + " ;\n";
            content += "-- Animation Attributes ...\n";
            content += "anim_timer = " + this.توقف_در_هر_فریم.ToString() + " ;\n";
            content += "totalFrames = " + this.تعدادفریم.ToString() + " ;\n";
            content += "isRev = " + this.معکوس.ToString() + " ;\n";
            content += "rows = " + this.سطر.ToString() + " ;\n";
            content += "cols = " + this.ستون.ToString() + " ;\n";

            
            if (this.دارای_خواص_فیزیکی)
            {
                content += "--Shapes...\n";
                //////////////////////////////////////////////////////////////////////
                //LineShapes
                content += "    --LineShapes...\n";

                content += "    lineShapesCount = " + this.getLineShapeCount().ToString() + " ;\n";
                int n = 0;
                for (int k = 0; k < this.shapesList.Count; k++)
                {
                    if (this.shapesList[k].type == ShapeTypes.LINE)
                    {
                        content += "        --lineShape_" + n.ToString() + "...\n";
                        LineShape lineShape = (LineShape)this.shapesList[k];
                        content += "        lineShape_" + n.ToString() + "_aX = " + lineShape.aX.ToString() + " ;\n";
                        content += "        lineShape_" + n.ToString() + "_aY = " + lineShape.aY.ToString() + " ;\n";
                        content += "        lineShape_" + n.ToString() + "_bX = " + lineShape.bX.ToString() + " ;\n";
                        content += "        lineShape_" + n.ToString() + "_bY = " + lineShape.bY.ToString() + " ;\n";
                        content += "        lineShape_" + n.ToString() + "_thickness = " + lineShape.thickness.ToString() + " ;\n";
                        n++;
                    }
                }
                content += "    --End of LineShapes...\n";
                /////////////////////////////////////////////////////////////////////////////
                //CircleShapes
                content += "    --CircleShapes...\n";

                content += "    circleShapesCount = " + this.getCircleShapeCount().ToString() + " ;\n";
                n = 0;
                for (int k = 0; k < this.shapesList.Count; k++)
                {
                    if (this.shapesList[k].type == ShapeTypes.CIRCLE)
                    {
                        content += "        --circleShape_" + n.ToString() + "...\n";
                        CircleShape circleShape = (CircleShape)this.shapesList[k];
                        content += "        circleShape_" + n.ToString() + "_x = " + circleShape.x.ToString() + " ;\n";
                        content += "        circleShape_" + n.ToString() + "_y = " + circleShape.y.ToString() + " ;\n";
                        content += "        circleShape_" + n.ToString() + "_r = " + circleShape.r.ToString() + " ;\n";
                        n++;
                    }
                }
                content += "    --End of CircleShapes...\n";
                /////////////////////////////////////////////////////////////////////////////
                //CircleShapes
                content += "    -- PolyGonShapes...\n";

                content += "    polyGonShapesCount = " + this.getPolygonShapeCount().ToString() + " ;\n";
                n = 0;
                //////
                //
                //

                content += "    --End of PolyGonShapes...\n";

            }
            content += "--End of sprite...\n";
            content += "\n--Functions ...\n";
            content += this.script;
            content += "\n--End of Functions ...";
            System.IO.File.WriteAllText(path, content);
        }
    }
}
