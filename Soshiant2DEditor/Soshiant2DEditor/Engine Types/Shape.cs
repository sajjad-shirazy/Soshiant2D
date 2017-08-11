using System;
using System.Collections.Generic;
using System.Text;

namespace Soshiant2DEditor.Engine_Types
{
    public enum ShapeTypes
    {
        CIRCLE,
        LINE,
        POLYGON
    }
    [Serializable]
    public class Shape
    {
        public ShapeTypes type;
        public int id;
    }
    [Serializable]
    public class LineShape : Shape
    {
        public float aX, aY;
        public float bX, bY;
        public float thickness;
    }
    [Serializable]
    public class CircleShape : Shape
    {
        public float x, y, r;
    }
}
