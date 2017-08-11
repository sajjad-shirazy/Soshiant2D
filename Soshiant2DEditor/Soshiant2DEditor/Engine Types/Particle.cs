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
    public class Particle :Sprite 
    {

		int _count;

		int _startColor_R;
        int _startColor_G;
        int _startColor_B;

        int _endColor_R;
        int _endColor_G;
        int _endColor_B;

        int _startAlpha;
        int _endAlpha;

		int _minParticleLifeTime;
		int _maxParticleLifeTime;
        float _minStartSpeed;
        float _maxStartSpeed;
		float _acceleration_x;
        float _acceleration_y;
        float _gravity_x;
		float _gravity_y;
		float _minScale;
		float _maxScale;
		bool _repeat ;

        [CategoryAttribute("ويژگي هاي دستگاه ذرات"), DescriptionAttribute("تعداد ذرات")]
        public int تعداد_ذرات
        {
            get
            {
                return _count;
            }
            set
            {
                _count = value;
            }
        }
        [CategoryAttribute("ويژگي هاي دستگاه ذرات"), DescriptionAttribute("كينه مدت زندگي ذرات")]
        public int كمينه_زندگي_ذرات
        {
            get
            {
                return _minParticleLifeTime;
            }
            set
            {
                _minParticleLifeTime = value;
            }
        }
        [CategoryAttribute("ويژگي هاي دستگاه ذرات"), DescriptionAttribute("بيشينه مدت زندگي ذرات")]
        public int بيشينه_زندگي_ذرات
        {
            get
            {
                return _maxParticleLifeTime;
            }
            set
            {
                _maxParticleLifeTime = value;
            }
        }
        [CategoryAttribute("ويژگي هاي دستگاه ذرات"), DescriptionAttribute("پخش مكرر ذرات")]
        public bool تكرار
        {
            get
            {
                return _repeat;
            }
            set
            {
                _repeat = value;
            }
        }
        [CategoryAttribute("بافت"), DescriptionAttribute("فایل بافتی که براي ذرات نمایش داده می شود ."), ReadOnlyAttribute(true)]
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
        [CategoryAttribute("رنگ"), DescriptionAttribute("رنگ شروع ذرات .")]
        public int رنگ_شروع_قرمز
        {
            get
            {
                return _startColor_R;
            }
            set
            {
                _startColor_R = value;
            }
        }
        [CategoryAttribute("رنگ"), DescriptionAttribute("رنگ شروع ذرات .")]
        public int رنگ_شروع_سبز
        {
            get
            {
                return _startColor_G;
            }
            set
            {
                _startColor_G = value;
            }
        }
        [CategoryAttribute("رنگ"), DescriptionAttribute("رنگ شروع ذرات")]
        public int رنگ_شروع_آبي
        {
            get
            {
                return _startColor_B;
            }
            set
            {
                _startColor_B = value;
            }
        }
        [CategoryAttribute("رنگ"), DescriptionAttribute("رنگ پايان ذرات .")]
        public int رنگ_پايان_قرمز
        {
            get
            {
                return _endColor_R;
            }
            set
            {
                _endColor_R = value;
            }
        }
        [CategoryAttribute("رنگ"), DescriptionAttribute("رنگ پايان ذرات .")]
        public int رنگ_پايان_سبز
        {
            get
            {
                return _endColor_G;
            }
            set
            {
                _endColor_G = value;
            }
        }
        [CategoryAttribute("رنگ"), DescriptionAttribute("رنگ پايان ذرات")]
        public int رنگ_پايان_آبي
        {
            get
            {
                return _endColor_B;
            }
            set
            {
                _endColor_B = value;
            }
        }
        [CategoryAttribute("شفافيت"), DescriptionAttribute("شفافيت آغاز ذرات")]
        public int شفافيت_آغاز_ذرات
        {
            get
            {
                return _startAlpha;
            }
            set
            {
                _startAlpha = value;
            }
        }
        [CategoryAttribute("شفافيت"), DescriptionAttribute("شفافيت پايان ذرات")]
        public int شفافيت_پايان_ذرات
        {
            get
            {
                return _endAlpha;
            }
            set
            {
                _endAlpha = value;
            }
        }
        [CategoryAttribute("مقياس"), DescriptionAttribute("مقياس كمينه ذرات")]
        public float مقياس_كمينه
        {
            get
            {
                return _minScale;
            }
            set
            {
                _minScale = value;
            }
        }
        [CategoryAttribute("مقياس"), DescriptionAttribute("مقياس بيشينه ذرات")]
        public float مقياس_بيشينه
        {
            get
            {
                return _maxScale;
            }
            set
            {
                _maxScale = value;
            }
        }
        [CategoryAttribute("شتاب ذرات"), DescriptionAttribute("شتاب ذرات در راستاي حركت")]
        public float شتاب_x
        {
            get
            {
                return _acceleration_x;
            }
            set
            {
                _acceleration_x = value;
            }
        }
        [CategoryAttribute("شتاب ذرات"), DescriptionAttribute("شتاب ذرات در راستاي حركت")]
        public float شتاب_y
        {
            get
            {
                return _acceleration_y;
            }
            set
            {
                _acceleration_y = value;
            }
        }
        [CategoryAttribute("گرانش"), DescriptionAttribute("گرانش محيط")]
        public float گرانش_x
        {
            get
            {
                return _gravity_x;
            }
            set
            {
                _gravity_x = value;
            }
        }
        [CategoryAttribute("گرانش"), DescriptionAttribute("گرانش محيط")]
        public float گرانش_y
        {
            get
            {
                return _gravity_y;
            }
            set
            {
                _gravity_y = value;
            }
        }
        [CategoryAttribute("سرعت شروع"), DescriptionAttribute("كمينه سرعت شروع ذرات")]
        public float كمينه_سرعت_شروع
        {
            get
            {
                return _minStartSpeed;
            }
            set
            {
                _minStartSpeed = value;
            }
        }
        [CategoryAttribute("سرعت شروع"), DescriptionAttribute("بيشينه سرعت شروع ذرات")]
        public float بيشينه_سرعت_شروع
        {
            get
            {
                return _maxStartSpeed;
            }
            set
            {
                _maxStartSpeed = value;
            }
        }



        void setStartColor(int R, int G, int B)
        {
            _startColor_R = R;
            _startColor_G = G;
            _startColor_B = B;
        }
        void setEndColor(int R, int G, int B)
        {
            _endColor_R = R;
            _endColor_G = G;
            _endColor_B = B;
        }












        public Particle()
        {
            _x = 0; _y = 0;

            _image = null;
            _name = "";
            _count = 500;
            setStartColor(255, 162, 0);
            setEndColor(255, 6, 0);
            _minParticleLifeTime = 1;
            _maxParticleLifeTime = 500;
            _minStartSpeed = 0.01f;
            _maxStartSpeed = 2.0f;
            _acceleration_x = 0.96f;
            _acceleration_y = 0.96f;
            _gravity_x = 0.0f;
            _gravity_y = -0.04f;
            _minScale = 0.5f;
            _maxScale = 1.0f;
            _repeat = true;
            _startAlpha = 255;
            _endAlpha = 0;

        }

        //public object Clone()
        //{
        //    return MemberwiseClone();
        //}

        override public void ExportTo(string path)
        {
            string content = "";

            content += "--Particle...\n";

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

            //Particle Attributes
            content += "--Particle Attributes...\n";
            content += "count = " + this.تعداد_ذرات.ToString() + " ;\n";
            content += "startColor_R = " + this.رنگ_شروع_قرمز.ToString() + " ;\n";
            content += "startColor_G = " + this.رنگ_شروع_سبز.ToString() + " ;\n";
            content += "startColor_B = " + this.رنگ_شروع_آبي.ToString() + " ;\n";
            content += "endColor_R = " + this.رنگ_پايان_قرمز.ToString() + " ;\n";
            content += "endColor_G = " + this.رنگ_پايان_سبز.ToString() + " ;\n";
            content += "endColor_B = " + this.رنگ_پايان_آبي.ToString() + " ;\n";
            content += "startAlpha = " + this.شفافيت_آغاز_ذرات.ToString() + " ;\n";
            content += "endAlpha = " + this.شفافيت_پايان_ذرات.ToString() + " ;\n";
            content += "minParticleLifeTime = " + this.كمينه_زندگي_ذرات.ToString() + " ;\n";
            content += "maxParticleLifeTime = " + this.بيشينه_زندگي_ذرات.ToString() + " ;\n";
            content += "minStartSpeed = " + this.كمينه_سرعت_شروع.ToString() + " ;\n";
            content += "maxStartSpeed = " + this.بيشينه_سرعت_شروع.ToString() + " ;\n";
            content += "acceleration_x = " + this.شتاب_x.ToString() + " ;\n";
            content += "acceleration_y = " + this.شتاب_y.ToString() + " ;\n";
            content += "gravity_x = " + this.گرانش_x.ToString() + " ;\n";
            content += "gravity_y = " + this.گرانش_y.ToString() + " ;\n";
            content += "minScale = " + this.مقياس_كمينه.ToString() + " ;\n";
            content += "maxScale = " + this.مقياس_بيشينه.ToString() + " ;\n";
            content += "rpt = " + this.تكرار.ToString().ToLower() + " ;\n";
            content += "--end of Particle Attributes...\n\n";

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
            content += "--End of Particle...\n";
            content += "\n--Functions ...\n";
            content += this.script;
            content += "\n--End of Functions ...";
            System.IO.File.WriteAllText(path, content);
        }
    }
}
