// Most of the System using statements were automatically added

using System;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Resources;

namespace Sagebox
{
    namespace Main
    {
        public struct RgbStruct
        {
            public int red;
            public int green;
            public int blue;
            public int alpha;
        }
        public class CColor
        {
            public struct Colors
            {
                public RgbStruct color1;
                public RgbStruct color2;
            }

            public Colors colors;

            public CColor(Color color)
            {
                colors.color1.red = color.R;
                colors.color1.green = color.G;
                colors.color1.blue = color.B;
                colors.color1.alpha = color.A;
                colors.color2.alpha = -1; // i.e. no second color

            }
            public CColor(Color _color, Color _color2)
            {
                colors.color1.red = _color.R;
                colors.color1.green = _color.G;
                colors.color1.blue = _color.B;
                colors.color1.alpha = _color.A;
                colors.color2.red = _color2.R;
                colors.color2.green = _color2.G;
                colors.color2.blue = _color2.B;
                colors.color2.alpha = _color2.A;

            }

            public static CColor Grad(Color color, Color color2) { return new CColor(color, color2); }
            public CColor(string color)
            {
                SDLL._SageVB_ColorFromString(color, ref colors.color1, ref colors.color2);

                //LogVInfo("TestColor", string.Format("color1 = {0},{1},{2},{3}", color1.red, color1.green, color1.blue, color1.alpha));
                //LogVInfo("TestColor", string.Format("color2 = {0},{1},{2},{3}", color2.red, color2.green, color2.blue, color2.alpha));
            }
            public static implicit operator CColor(Color color) => new CColor(color);
            public static implicit operator CColor(string color) => new CColor(color);

        }

        public class CPoint
        {
            public float x;
            public float y;
            public CPoint(float x, float y) { this.x = x; this.y = y; }

            public static implicit operator CPoint(Point p) => new CPoint(p.X, p.Y);
            public static implicit operator CPoint(PointF p) => new CPoint(p.X, p.Y);
            public static implicit operator CPoint(Size s) => new CPoint(s.Width, s.Height);
            public static implicit operator CPoint(SizeF s) => new CPoint(s.Width, s.Height);

            public static CPoint operator +(CPoint _class, CPoint _input)
            {
                return new CPoint(_class.x + _input.x, _class.y + _input.y);
            }
            public static CPoint operator -(CPoint _class, CPoint _input)
            {
                return new CPoint(_class.x - _input.x, _class.y - _input.y);

            }
            public static CPoint operator /(CPoint _class, float _input)
            {
               return new CPoint(_class.x/_input,_class.y/_input);
            }
            public static CPoint operator /(CPoint _class, CPoint _input)
            {
               return new CPoint(_class.x/_input.x,_class.y/_input.y);
            }
            public static CPoint operator *(CPoint _class, CPoint _input)
            {
               return new CPoint(_class.x*_input.x,_class.y*_input.y);
            }
 
            public CPoint SelfRotateXY(float fAngle)
            {
                fAngle = 3.14159F * fAngle / 180.0F;
                var fTemp = x * (float) Math.Cos(fAngle) - y * (float) Math.Sin(fAngle);
                y = x * (float) Math.Sin(fAngle) + y * (float) Math.Cos(fAngle);
                x = fTemp;
                return this;
            }

            public PointF toPointF()
            {
                return new PointF(x, y);
            }

            public PointF toF()
            {
                return new PointF(x, y);
            }

        }

        // $$ Delete CIPoint
        public class CIPoint
        {
            public int x;
            public int y;
            public CIPoint(int x, int y) { this.x = x; this.y = y; }

            public static implicit operator CIPoint(Point p) => new CIPoint(p.X, p.Y);
            public static implicit operator CIPoint(Size s) => new CIPoint(s.Width, s.Height);
            
            public static CIPoint operator +(CIPoint _class, CIPoint _input)
            {
                return new CIPoint(_class.x + _input.x, _class.y + _input.y);
            }
            public static CIPoint operator -(CIPoint _class, CIPoint _input)
            {
                return new CIPoint(_class.x - _input.x, _class.y - _input.y);
            }
        }

        struct point_struct
        {
            public float x;
            public float y;
        }
        public struct ResourceReturn
        {
            public int type;
            public long pointer;
            public long resource;
        }

        class SDLL
        {
            // Set this path to relative or absolute path, e.g. c:\\mydir\\myproject\\SageboxCS.dll"

            // ** note ** -- "..\\..\\..\\..\\SageboxCS.dll" is used since SageboxCS is one directory above
            //               the executable file, which is in source_directory\bin\debug

            private const string kDLL = "..\\..\\..\\..\\SageboxCS.dll";

            // --> Sagebox Global Functions

            [DllImport(kDLL)] public static extern bool _SageVB_GetEvent();
            [DllImport(kDLL)] public static extern bool _SageVB_Write(string x, long win_pointer, long keywords);
            [DllImport(kDLL)] public static extern int _SageVB_ExitButton(string message);
            [DllImport(kDLL)] public static extern int _SageVB_Init(int value,string local_dir);
            [DllImport(kDLL)] public static extern bool _SageVB_SetIniFilePath(string local_dir);
            [DllImport(kDLL)] public static extern long _SageVB_GetkwObj();        // $$ Deprecated


            [DllImport(kDLL)] public static extern long _SageVB_NewWindow(long keywords,string title);
            [DllImport(kDLL)] public static extern long _SageVB_DevSlider(string title, long keywords, bool bisFloat);
            [DllImport(kDLL)] public static extern long _SageVB_DevText(int height,string title, long keywords);
            [DllImport(kDLL)] public static extern long _SageVB_DevInputBox(string title, long keywords);
            [DllImport(kDLL)] public static extern void _SageVB_LogVInfo(string _module_, string message);
            [DllImport(kDLL)] public static extern bool _SageVB_SetLogLevelStr(string level);
            [DllImport(kDLL)] public static extern bool _SageVB_DebugWrite(string message,bool bAddNewline);
            [DllImport(kDLL)] public static extern void _SageVB_DisableDefaults();

            // kw Class Functions

            [DllImport(kDLL)] public static extern long _SageVB_kw_PenSize(float x);
            [DllImport(kDLL)] public static extern long _SageVB_kw_AddObjects(long p1, long p2);
            [DllImport(kDLL)] public static extern ResourceReturn _SageVB_kw_TestReturn(); // $$ deprecated
            [DllImport(kDLL)] public static extern long _SageVB_kw_FreeResource(long pResource, int type);     // Always returns 0
            [DllImport(kDLL)] public static extern void VB_kw_EvalObject(long p1);
            [DllImport(kDLL)] public static extern long _SageVB_kw_GenericColor(int index, int r, int g, int b);
            [DllImport(kDLL)] public static extern long _SageVB_kw_GenericBiColor(int index, ref CColor.Colors colors);
            [DllImport(kDLL)] public static extern ResourceReturn _SageVB_kw_GenericString(int index, string value);
            [DllImport(kDLL)] public static extern long _SageVB_kw_GenericFloat(int index, float value);
            [DllImport(kDLL)] public static extern long _SageVB_kw_GenericInteger(int index, int value);
            [DllImport(kDLL)] public static extern long _SageVB_kw_GenericBoolean(int index, bool value);
            [DllImport(kDLL)] public static extern long _SageVB_kw_RangeInt(int iMin,int iMax);
            [DllImport(kDLL)] public static extern long _SageVB_kw_RangeFloat(float fMin,float fMax);
            [DllImport(kDLL)] public static extern long _SageVB_kw_SizePairInt(int s1,int s2);
            [DllImport(kDLL)] public static extern long _SageVB_kw_SizePairFloat(float s1,float s2);
            [DllImport(kDLL)] public static extern long _SageVB_kw_PosPairInt(int s1,int s2);
            [DllImport(kDLL)] public static extern long _SageVB_kw_PosPairFloat(float s1,float s2);
            [DllImport(kDLL)] public static extern long _SageVB_kw_SetJustType(int justType,bool value);


            // --> Slider Functions

            [DllImport(kDLL)] public static extern int _SageVB_Slider_GetPos(long pointer);
            [DllImport(kDLL)] public static extern double _SageVB_Slider_GetPosf(long pointer);
            [DllImport(kDLL)] public static extern float _SageVB_Slider_GetPosff(long pointer);
            [DllImport(kDLL)] public static extern bool _SageVB_Slider_Moved(long pointer);

            // --> Input Box Functions

            [DllImport(kDLL)] public static extern bool _SageVB_InputBox_ReturnPressed(long pointer);
            [DllImport(kDLL)] public static extern int _SageVB_InputBox_GetInteger(long pointer);
            [DllImport(kDLL)] public static extern IntPtr _SageVB_InputBox_GetText(long pointer);
            [DllImport(kDLL)] public static extern bool _SageVB_InputBox_ClearText(long pointer);

            // --> Window Functions
            [DllImport(kDLL)] public static extern bool _SageVB_Window_DrawCircle_Rgb(long p1, float x, float y, float radius, ref CColor.Colors colors, bool filled, long keywords);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_DrawEllipse_Rgb(long p1, float x, float y, float xwidth,float ywidth, ref CColor.Colors colors, bool filled, long keywords);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_DrawRectangle_Rgb(long p1, float x, float y, float xwidth,float ywidth, ref CColor.Colors colors, bool filled, long keywords);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_DrawTriangle_Rgb(long p1, float x1,float y1,float x2,float y2,float x3,float y3, ref CColor.Colors colors, bool filled, bool fast, long keywords);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_DrawPolygon_Rgb(long p1, int iSides,ref point_struct p, ref CColor.Colors colors, bool filled, long keywords);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_SetPixel_Rgb(long p1, int x,int y,int red,int green,int blue);

            [DllImport(kDLL)] public static extern bool _SageVB_Window_Cls(long p1, int red1, int green1, int blue1, int red2, int green2, int blue2, bool radial);

            [DllImport(kDLL)] public static extern bool _SageVB_Window_ClsStr(long p1, string color,bool radial);
            [DllImport(kDLL)] public static extern bool _SageVB_ColorFromString(string color,ref RgbStruct color1,ref RgbStruct color2);
            [DllImport(kDLL)] public static extern void _SageVB_ObjectTest(ref CIPoint p1);
            [DllImport(kDLL)] public static extern void _SageVB_PointStructTest(int iSize,ref point_struct p);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_GetWindowSize(long p1,ref float x,ref float y);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_GetWindowCenter(long p1,ref float x,ref float y);
            [DllImport(kDLL)] public static extern void _SageVB_DisplayDefaultPaths();
            [DllImport(kDLL)] public static extern bool _SageVB_Window_SetAutoUpdateStr(long p1,string sUpdateType);
            [DllImport(kDLL)] public static extern bool _SageVB_Window_SetAutoUpdateInt(long p1,int iUpdateType);

        }

        public class RgbColor
        {
            public int Red;
            public int Green;
            public int Blue;

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            public RgbColor(int _red, int _green, int _blue)
            {
                Red = _red;
                Green = _green;
                Blue = _blue;
            }

            public RgbColor()
            {
                // Placeholder
            }
        }


        public struct PanColor
        {
            private static Color _ccc(int x)
            {

                //  return new RgbColor((x >> 16) & 255, (x >> 8) & 255, x & 255);
                return Color.FromArgb((x >> 16) & 255, (x >> 8) & 255, x & 255);
            }
            public static Color AliceBlue = _ccc(0x00F0F8FF);
            public static Color AntiqueWhite = _ccc(0x00FAEBD7);
            public static Color Aqua = _ccc(0x0000FFFF);
            public static Color Aquamarine = _ccc(0x007FFFD4);
            public static Color Azure = _ccc(0x00F0FFFF);
            public static Color Beige = _ccc(0x00F5F5DC);
            public static Color Bisque = _ccc(0x00FFE4C4);
            public static Color Black = _ccc(0x00000000);
            public static Color BlanchedAlmond = _ccc(0x00FFEBCD);
            public static Color Blue = _ccc(0x000000FF);
            public static Color BlueViolet = _ccc(0x008A2BE2);
            public static Color Brown = _ccc(0x00A52A2A);
            public static Color BurlyWood = _ccc(0x00DEB887);
            public static Color CadetBlue = _ccc(0x005F9EA0);
            public static Color Chartreuse = _ccc(0x007FFF00);
            public static Color Chocolate = _ccc(0x00D2691E);
            public static Color Coral = _ccc(0x00FF7F50);
            public static Color CornflowerBlue = _ccc(0x006495ED);
            public static Color Cornsilk = _ccc(0x00FFF8DC);
            public static Color Crimson = _ccc(0x00DC143C);
            public static Color Cyan = _ccc(0x0000FFFF);
            public static Color DarkBlue = _ccc(0x0000008B);
            public static Color DarkCyan = _ccc(0x00008B8B);
            public static Color DarkGoldenrod = _ccc(0x00B8860B);
            public static Color DarkGray = _ccc(0x00A9A9A9);
            public static Color DarkGreen = _ccc(0x00006400);
            public static Color DarkKhaki = _ccc(0x00BDB76B);
            public static Color DarkMagenta = _ccc(0x008B008B);
            public static Color DarkOliveGreen = _ccc(0x00556B2F);
            public static Color DarkOrange = _ccc(0x00FF8C00);
            public static Color DarkOrchid = _ccc(0x009932CC);
            public static Color DarkRed = _ccc(0x008B0000);
            public static Color DarkSalmon = _ccc(0x00E9967A);
            public static Color DarkSeaGreen = _ccc(0x008FBC8B);
            public static Color DarkSlateBlue = _ccc(0x00483D8B);
            public static Color DarkSlateGray = _ccc(0x002F4F4F);
            public static Color DarkTurquoise = _ccc(0x0000CED1);
            public static Color DarkViolet = _ccc(0x009400D3);
            public static Color DeepPink = _ccc(0x00FF1493);
            public static Color DeepSkyBlue = _ccc(0x0000BFFF);
            public static Color DimGray = _ccc(0x00696969);
            public static Color DodgerBlue = _ccc(0x001E90FF);
            public static Color Firebrick = _ccc(0x00B22222);
            public static Color FloralWhite = _ccc(0x00FFFAF0);
            public static Color ForestGreen = _ccc(0x00228B22);
            public static Color Fuchsia = _ccc(0x00FF00FF);
            public static Color Gainsboro = _ccc(0x00DCDCDC);
            public static Color GhostWhite = _ccc(0x00F8F8FF);
            public static Color Gold = _ccc(0x00FFD700);
            public static Color Goldenrod = _ccc(0x00DAA520);
            public static Color Gray = _ccc(0x00808080);
            public static Color Green = _ccc(0x00008000);
            public static Color GreenYellow = _ccc(0x00ADFF2F);
            public static Color Honeydew = _ccc(0x00F0FFF0);
            public static Color HotPink = _ccc(0x00FF69B4);
            public static Color IndianRed = _ccc(0x00CD5C5C);
            public static Color Indigo = _ccc(0x004B0082);
            public static Color Ivory = _ccc(0x00FFFFF0);
            public static Color Khaki = _ccc(0x00F0E68C);
            public static Color Lavender = _ccc(0x00E6E6FA);
            public static Color LavenderBlush = _ccc(0x00FFF0F5);
            public static Color LawnGreen = _ccc(0x007CFC00);
            public static Color LemonChiffon = _ccc(0x00FFFACD);
            public static Color LightBlue = _ccc(0x00ADD8E6);
            public static Color LightCoral = _ccc(0x00F08080);
            public static Color LightCyan = _ccc(0x00E0FFFF);
            public static Color LightGoldenrodYellow = _ccc(0x00FAFAD2);
            public static Color LightGray = _ccc(0x00D3D3D3);
            public static Color LightGreen = _ccc(0x0090EE90);
            public static Color LightPink = _ccc(0x00FFB6C1);
            public static Color LightSalmon = _ccc(0x00FFA07A);
            public static Color LightSeaGreen = _ccc(0x0020B2AA);
            public static Color LightSkyBlue = _ccc(0x0087CEFA);
            public static Color LightSlateGray = _ccc(0x00778899);
            public static Color LightSteelBlue = _ccc(0x00B0C4DE);
            public static Color LightYellow = _ccc(0x00FFFFE0);
            public static Color Lime = _ccc(0x0000FF00);
            public static Color LimeGreen = _ccc(0x0032CD32);
            public static Color Linen = _ccc(0x00FAF0E6);
            public static Color Magenta = _ccc(0x00FF00FF);
            public static Color Maroon = _ccc(0x00800000);
            public static Color MediumAquamarine = _ccc(0x0066CDAA);
            public static Color MediumBlue = _ccc(0x000000CD);
            public static Color MediumOrchid = _ccc(0x00BA55D3);
            public static Color MediumPurple = _ccc(0x009370DB);
            public static Color MediumSeaGreen = _ccc(0x003CB371);
            public static Color MediumSlateBlue = _ccc(0x007B68EE);
            public static Color MediumSpringGreen = _ccc(0x0000FA9A);
            public static Color MediumTurquoise = _ccc(0x0048D1CC);
            public static Color MediumVioletRed = _ccc(0x00C71585);
            public static Color MidnightBlue = _ccc(0x00191970);
            public static Color MintCream = _ccc(0x00F5FFFA);
            public static Color MistyRose = _ccc(0x00FFE4E1);
            public static Color Moccasin = _ccc(0x00FFE4B5);
            public static Color NavajoWhite = _ccc(0x00FFDEAD);
            public static Color Navy = _ccc(0x00000080);
            public static Color OldLace = _ccc(0x00FDF5E6);
            public static Color Olive = _ccc(0x00808000);
            public static Color OliveDrab = _ccc(0x006B8E23);
            public static Color Orange = _ccc(0x00FFA500);
            public static Color OrangeRed = _ccc(0x00FF4500);
            public static Color Orchid = _ccc(0x00DA70D6);
            public static Color PaleGoldenrod = _ccc(0x00EEE8AA);
            public static Color PaleGreen = _ccc(0x0098FB98);
            public static Color PaleTurquoise = _ccc(0x00AFEEEE);
            public static Color PaleVioletRed = _ccc(0x00DB7093);
            public static Color PapayaWhip = _ccc(0x00FFEFD5);
            public static Color PeachPuff = _ccc(0x00FFDAB9);
            public static Color Peru = _ccc(0x00CD853F);
            public static Color Pink = _ccc(0x00FFC0CB);
            public static Color Plum = _ccc(0x00DDA0DD);
            public static Color PowderBlue = _ccc(0x00B0E0E6);
            public static Color Purple = _ccc(0x00800080);
            public static Color Red = _ccc(0x00FF0000);
            public static Color RosyBrown = _ccc(0x00BC8F8F);
            public static Color RoyalBlue = _ccc(0x004169E1);
            public static Color SaddleBrown = _ccc(0x008B4513);
            public static Color Salmon = _ccc(0x00FA8072);
            public static Color SandyBrown = _ccc(0x00F4A460);
            public static Color SeaGreen = _ccc(0x002E8B57);
            public static Color SeaShell = _ccc(0x00FFF5EE);
            public static Color Sienna = _ccc(0x00A0522D);
            public static Color Silver = _ccc(0x00C0C0C0);
            public static Color SkyBlue = _ccc(0x0087CEEB);
            public static Color SlateBlue = _ccc(0x006A5ACD);
            public static Color SlateGray = _ccc(0x00708090);
            public static Color Snow = _ccc(0x00FFFAFA);
            public static Color SpringGreen = _ccc(0x0000FF7F);
            public static Color SteelBlue = _ccc(0x004682B4);
            public static Color Tan = _ccc(0x00D2B48C);
            public static Color Teal = _ccc(0x00008080);
            public static Color Thistle = _ccc(0x00D8BFD8);
            public static Color Tomato = _ccc(0x00FF6347);
            public static Color Transparent = _ccc(0x00FFFFFF);
            public static Color Turquoise = _ccc(0x0040E0D0);
            public static Color Violet = _ccc(0x00EE82EE);
            public static Color Wheat = _ccc(0x00F5DEB3);
            public static Color White = _ccc(0x00FFFFFF);
            public static Color WhiteSmoke = _ccc(0x00F5F5F5);
            public static Color Yellow = _ccc(0x00FFFF00);
            public static Color YellowGreen = _ccc(0x009ACD32);
        }
        public struct SageColor
        {
  
            public static Color DefaultBgColor                  = Color.FromArgb(20, 40, 121);
            public static Color DefaultFgColor                  = Color.FromArgb(255, 255, 255);
            public static Color SliderTextColor                 = Color.FromArgb(128, 128, 128);
            public static Color Green                           = Color.FromArgb(0, 255, 0);
            public static Color DarkGreen                       = Color.FromArgb(0, 128, 0);
            public static Color LightGreen                      = Color.FromArgb(128, 255, 128);
            public static Color Blue                            = Color.FromArgb(0, 0, 255);
            public static Color Blue32                          = Color.FromArgb(0, 0, 32);
            public static Color Blue48                          = Color.FromArgb(0, 0, 48);
            public static Color Blue64                          = Color.FromArgb(0, 0, 64);
            public static Color DarkBlue                        = Color.FromArgb(0, 0, 92);
            public static Color LightBlue                       = Color.FromArgb(40, 100, 255); // new RgbColor(40, 100, 255);
            public static Color SkyBlue                         = Color.FromArgb(40, 145, 255);
            public static Color SkyBlueDark                     = Color.FromArgb(0, 30, 128);
            public static Color SkyBlueLight                    = Color.FromArgb(75, 165, 255);
            public static Color PaleBlueDark                    = Color.FromArgb(40, 100, 140);
            public static Color PaleBlue                        = Color.FromArgb(103, 179, 217);
            public static Color PaleBlueLight                   = Color.FromArgb(145, 190, 215);
            public static Color LightCyan                       = Color.FromArgb(128, 255, 255);
            public static Color Cyan                            = Color.FromArgb(0, 255, 255);
            public static Color MidCyan                         = Color.FromArgb(15, 200, 200);
            public static Color DarkCyan                        = Color.FromArgb(30, 130, 130);
            public static Color Red                             = Color.FromArgb(255, 0, 0);
            public static Color LightRed                        = Color.FromArgb(255, 128, 128);
            public static Color LightYellow                     = Color.FromArgb(255, 255, 128);
            public static Color Yellow                          = Color.FromArgb(255, 255, 0);
            public static Color Magenta                         = Color.FromArgb(255, 0, 255);
            public static Color MediumMagenta                   = Color.FromArgb(255, 92, 255);
            public static Color LightMagenta                    = Color.FromArgb(255, 128, 255);
            public static Color Purple                          = Color.FromArgb(255, 0, 255);
            public static Color LightPurple                     = Color.FromArgb(255, 128, 255);
            public static Color MediumPurple                    = Color.FromArgb(255, 92, 255);
            public static Color White                           = Color.FromArgb(255, 255, 255);
            public static Color Gray172                         = Color.FromArgb(172, 172, 172);
            public static Color Gray192                         = Color.FromArgb(192, 192, 192);
            public static Color Gray220                         = Color.FromArgb(220, 220, 220);
            public static Color Gray128                         = Color.FromArgb(128, 128, 128);
            public static Color Gray32                          = Color.FromArgb(32, 32, 32);
            public static Color Gray42                          = Color.FromArgb(42, 42, 42);
            public static Color Gray64                          = Color.FromArgb(64, 64, 64);
            public static Color Gray72                          = Color.FromArgb(72, 72, 72);
            public static Color Gray92                          = Color.FromArgb(92, 92, 92);
            public static Color Black                           = Color.FromArgb(0, 0, 0);
            public static Color LightGray                       = Color.FromArgb(200, 200, 200);
            public static Color LightGrey                       = Color.FromArgb(200, 200, 200);
            public static Color MidGray                         = Color.FromArgb(64, 64, 64);
            public static Color MidGrey                         = Color.FromArgb(64, 64, 64);
            public static Color DarkGray                        = Color.FromArgb(32, 32, 32);
            public static Color DarkGrey                        = Color.FromArgb(32, 32, 32);
            public static Color Gray                            = Color.FromArgb(128, 128, 128);
            public static Color Grey                            = Color.FromArgb(128, 128, 128);
            public static Color NearWhite                       = Color.FromArgb(220, 220, 220);
            public static Color ButtonTextColorNormal           = Color.FromArgb(220, 220, 220);
            public static Color ButtonTextColorHighlighted      = Color.FromArgb(255, 255, 255);
            public static Color ButtonTextColorPressed          = Color.FromArgb(255, 255, 255);
            public static Color ButtonTextColorDisabled         = Color.FromArgb(170, 170, 170);
            public static Color CheckboxTextColorNormal         = Color.FromArgb(220, 220, 220);
            public static Color CheckboxTextColorHighlighted    = Color.FromArgb(255, 255, 255);
            public static Color CheckboxTextColorChecked        = Color.FromArgb(220, 220, 220);
            public static Color CheckboxTextColorCheckedHigh    = Color.FromArgb(220, 220, 220);
            public static Color CheckboxTextColorDisabled       = Color.FromArgb(170, 170, 170);
            public static Color Orange                          = Color.FromArgb(255, 115, 0);
            public static Color LightOrange                     = Color.FromArgb(255, 130, 0);
            public static Color DarkOrange                      = Color.FromArgb(255, 85, 0);
            public static RgbColor _Undefined                   = new RgbColor(-1, -1, -1);
        };
        public struct SageColorA
        {
            public static Color DefaultBgColor(int alpha) { return Color.FromArgb(alpha, 20, 40, 121); }
            public static Color DefaultFgColor(int alpha) { return Color.FromArgb(alpha, 255, 255, 255); }
            public static Color SliderTextColor(int alpha) { return Color.FromArgb(alpha, 128, 128, 128); }
            public static Color Green(int alpha) { return Color.FromArgb(alpha, 0, 255, 0); }
            public static Color DarkGreen(int alpha) { return Color.FromArgb(alpha, 0, 128, 0); }
            public static Color LightGreen(int alpha) { return Color.FromArgb(alpha, 128, 255, 128); }
            public static Color Blue(int alpha) { return Color.FromArgb(alpha, 0, 0, 255); }
            public static Color Blue32(int alpha) { return Color.FromArgb(alpha, 0, 0, 32); }
            public static Color Blue48(int alpha) { return Color.FromArgb(alpha, 0, 0, 48); }
            public static Color Blue64(int alpha) { return Color.FromArgb(alpha, 0, 0, 64); }
            public static Color DarkBlue(int alpha) { return Color.FromArgb(alpha, 0, 0, 92); }
            public static Color LightBlue(int alpha) { return Color.FromArgb(alpha, 40, 100, 255); }
            public static Color SkyBlue(int alpha) { return Color.FromArgb(alpha, 40, 145, 255); }
            public static Color SkyBlueDark(int alpha) { return Color.FromArgb(alpha, 0, 30, 128); }
            public static Color SkyBlueLight(int alpha) { return Color.FromArgb(alpha, 75, 165, 255); }
            public static Color PaleBlueDark(int alpha) { return Color.FromArgb(alpha, 40, 100, 140); }
            public static Color PaleBlue(int alpha) { return Color.FromArgb(alpha, 103, 179, 217); }
            public static Color PaleBlueLight(int alpha) { return Color.FromArgb(alpha, 145, 190, 215); }
            public static Color LightCyan(int alpha) { return Color.FromArgb(alpha, 128, 255, 255); }
            public static Color Cyan(int alpha) { return Color.FromArgb(alpha, 0, 255, 255); }
            public static Color MidCyan(int alpha) { return Color.FromArgb(alpha, 15, 200, 200); }
            public static Color DarkCyan(int alpha) { return Color.FromArgb(alpha, 30, 130, 130); }
            public static Color Red(int alpha) { return Color.FromArgb(alpha, 255, 0, 0); }
            public static Color LightRed(int alpha) { return Color.FromArgb(alpha, 255, 128, 128); }
            public static Color LightYellow(int alpha) { return Color.FromArgb(alpha, 255, 255, 128); }
            public static Color Yellow(int alpha)  { return Color.FromArgb(alpha, 255, 255, 0); }
            public static Color Magenta(int alpha)  { return Color.FromArgb(alpha, 255, 0, 255); }
            public static Color MediumMagenta(int alpha)  { return Color.FromArgb(alpha, 255, 92, 255); }
            public static Color LightMagenta(int alpha)  { return Color.FromArgb(alpha, 255, 128, 255); }
            public static Color Purple(int alpha)  { return Color.FromArgb(alpha, 255, 0, 255); }
            public static Color LightPurple(int alpha)  { return Color.FromArgb(alpha, 255, 128, 255); }
            public static Color MediumPurple(int alpha)  { return Color.FromArgb(alpha, 255, 92, 255); }
            public static Color White(int alpha)  { return Color.FromArgb(alpha, 255, 255, 255); }
            public static Color Gray172(int alpha)  { return Color.FromArgb(alpha, 172, 172, 172); }
            public static Color Gray192(int alpha)  { return Color.FromArgb(alpha, 192, 192, 192); }
            public static Color Gray220(int alpha)  { return Color.FromArgb(alpha, 220, 220, 220); }
            public static Color Gray128(int alpha)  { return Color.FromArgb(alpha, 128, 128, 128); }
            public static Color Gray32(int alpha)  { return Color.FromArgb(alpha, 32, 32, 32); }
            public static Color Gray42(int alpha)  { return Color.FromArgb(alpha, 42, 42, 42); }
            public static Color Gray64(int alpha)  { return Color.FromArgb(alpha, 64, 64, 64); }
            public static Color Gray72(int alpha)  { return Color.FromArgb(alpha, 72, 72, 72); }
            public static Color Gray92(int alpha)  { return Color.FromArgb(alpha, 92, 92, 92); }
            public static Color Black(int alpha)  { return Color.FromArgb(alpha, 0, 0, 0); }
            public static Color LightGray(int alpha)  { return Color.FromArgb(alpha, 200, 200, 200); }
            public static Color LightGrey(int alpha)  { return Color.FromArgb(alpha, 200, 200, 200); }
            public static Color MidGray(int alpha)  { return Color.FromArgb(alpha, 64, 64, 64); }
            public static Color MidGrey(int alpha)  { return Color.FromArgb(alpha, 64, 64, 64); }
            public static Color DarkGray(int alpha)  { return Color.FromArgb(alpha, 32, 32, 32); }
            public static Color DarkGrey(int alpha)  { return Color.FromArgb(alpha, 32, 32, 32); }
            public static Color Gray(int alpha)  { return Color.FromArgb(alpha, 128, 128, 128); }
            public static Color Grey(int alpha)  { return Color.FromArgb(alpha, 128, 128, 128); }
            public static Color NearWhite(int alpha)  { return Color.FromArgb(alpha, 220, 220, 220); }
            public static Color ButtonTextColorNormal(int alpha)  { return Color.FromArgb(alpha, 220, 220, 220); }
            public static Color ButtonTextColorHighlighted(int alpha)  { return Color.FromArgb(alpha, 255, 255, 255); }
            public static Color ButtonTextColorPressed(int alpha)  { return Color.FromArgb(alpha, 255, 255, 255); }
            public static Color ButtonTextColorDisabled(int alpha)  { return Color.FromArgb(alpha, 170, 170, 170); }
            public static Color CheckboxTextColorNormal(int alpha)  { return Color.FromArgb(alpha, 220, 220, 220); }
            public static Color CheckboxTextColorHighlighted(int alpha)  { return Color.FromArgb(alpha, 255, 255, 255); }
            public static Color CheckboxTextColorChecked(int alpha)  { return Color.FromArgb(alpha, 220, 220, 220); }
            public static Color CheckboxTextColorCheckedHigh(int alpha)  { return Color.FromArgb(alpha, 220, 220, 220); }
            public static Color CheckboxTextColorDisabled(int alpha)  { return Color.FromArgb(alpha, 170, 170, 170); }
            public static Color Orange(int alpha) { return Color.FromArgb(alpha, 255, 115, 0); }
            public static Color LightOrange(int alpha) { return Color.FromArgb(alpha, 255, 130, 0); }
            public static Color DarkOrange(int alpha) { return Color.FromArgb(alpha, 255, 85, 0); }
        };


        #pragma warning disable CS8981      // 'kw' being lowercase triggers a warning
                                            // but we want to keep kw as lowercase, otherwise
                                            // it creates noisy code.

        public class kw
        {
            public long pointer = 0;
            public long resource = 0;
            public int resourceType = 0;


            kw()
            {
            }
            public kw UpdateResources(ref kw newKey, ref ResourceReturn r)
            {
                pointer = r.pointer;
                resource = r.resource; //$$ Should free resource if it already exists
                resourceType = r.type;
                return newKey;
            }

            public static kw operator +(kw _class, kw _input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_AddObjects(_class.pointer, _input.pointer);
                return kwx;
            }

            ~kw()
            {
                Console.WriteLine("in kw class destructor.\n");
            }
        
            // $$ add optional color2 here
            public static kw Color(CColor color)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericColor(13, color.colors.color1.red, color.colors.color1.green, color.colors.color1.blue);

                return kwx;
            }

            // $$ Test fgColor with alpha value in C++; fix accordingly here and in VB
            public static kw fgColor(CColor color)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericColor(8, color.colors.color1.red, color.colors.color1.green, color.colors.color1.blue);

                return kwx;
            }

            public static kw bgColor(CColor color)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericBiColor(9, ref color.colors);

                return kwx;
            }

            public static kw PenColor(CColor color)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericColor(10, color.colors.color1.red, color.colors.color1.green, color.colors.color1.blue);

                return kwx;
            }

            /// <summary>
            /// <b>Sets the size of the pen used for a drawing function.</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            /// <item><param name="1">The default pen size is 1 (i.e. for drawing functions when <b>kw.PenSize()</b> is not used)</param></item>.  
            /// <item><param name="2">When <b>kw.PenSize()</b> is omitted (i.e. not used for a drawing function), the global pen size setting is used.
            /// The global pen size setting defaults to 1, but can be change (see below)</param></item>.  
            /// <item><param name="3">When using <b>kw.PenSize()</b>, the pen size is only for the current drawing operation.</param></item>
            /// <item><param name="4">Use <b>win.SetPenSize()</b> to set a global pen size for all drawing functions (in which case <b>kw.PenSize()</b>
            /// can be used to divert from the global pen size for specific drawing operations.
            /// </param></item>
            /// <item><param name="5">Pen Sizes can be floating point values, allowing for anti-aliased lines and shapes.</param></item>
            ///</list>
            /// </summary>
            /// <param name="input"></param>
            /// <returns></returns>
            public static kw PenSize(float input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(4, input);

                return kwx;
            }

            /// <summary>
            /// Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="Angle"><b>Angle</b> - Angle is in Degrees (i.e. 0-360).  Use <em>AngleRad()</em> to set angle in radians</param></item>
            ///</list>
            /// <para></para>
            /// ** note: <b>Angle()</b> and <b>AngleDeg()</b> are the same function
            /// </summary>
            /// <param name="Angle"> - Integer, float or double angle value in radians (see Angle() to use radians instead of degrees)</param>
            /// <returns></returns>
            public static kw Angle(float input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(5, input);

                return kwx;
            }

            public static kw PadX(int input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(6, input);

                return kwx;
            }

            public static kw PadY(int input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(7, input);

                return kwx;
            }

            public static kw Default(int input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(11, input);

                return kwx;
            }

            public static kw Default(float input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(17, input);

                return kwx;
            }

            public static kw Default(double input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(17, (float) input);

                return kwx;
            }

            public static kw SetDefault(int input)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(11, input);

                return kwx;
            }

            /// <summary>
            /// Set the Text Font for the current Control, Widget, or Window.
            /// <para></para>
            ///<list type="bullet">
            /// <item><param name="plainInt">
            /// When a plain integer is used, i.e. <em><b>Font(15)</b></em>, it is set to the default font with a size of the value,
            /// i.e. <em>"Arial,15"</em>
            /// </param></item>
            /// </list>
            /// <para></para>
            /// <b><ul>Examples:</ul></b>
            /// <para></para>
            /// <para></para>
            ///<list type="bullet">
            ///<item><param name="example1"><b>Font(20)</b> - Set the font to "Arial,20".</param></item>
            /// <item><param name="example1"><b>Font("Arial,20")</b> - Set the font to "Arial,20".
            /// <br />- You can also add italic and bold, e.g. <em>Font("Arial,20,bold,italic")</em></param></item>
            /// <item><param name="example1"><b>Font("MyFont")</b> - Set the font by a previously created font named <em>"MyFont"</em>
            /// </param></item>
            /// </list>
            /// <para></para>
            /// *** note: <b>Font()</b> and <b>TextFont()</b> are the same function.
            /// </summary>
            /// <returns></returns>
            public static kw Font(string input)
            {
                var kwx = new kw();
                var rr = SDLL._SageVB_kw_GenericString(0, input);
                kwx.pointer = rr.pointer;
                kwx.resource = rr.resource; // $$ Should free resource if it already exists
                kwx.resourceType = rr.type;

                return kwx;

            }
            /// <summary>
            /// Set the Text Font for the current Control, Widget, or Window.
            /// <para></para>
            ///<list type="bullet">
            /// <item><param name="plainInt">
            /// When a plain integer is used, i.e. <em><b>Font(15)</b></em>, it is set to the default font with a size of the value,
            /// i.e. <em>"Arial,15"</em>
            /// </param></item>
            /// </list>
            /// <para></para>
            /// <b><ul>Examples:</ul></b>
            /// <para></para>
            /// <para></para>
            ///<list type="bullet">
            ///<item><param name="example1"><b>Font(20)</b> - Set the font to "Arial,20".</param></item>
            /// <item><param name="example1"><b>Font("Arial,20")</b> - Set the font to "Arial,20".
            /// <br />- You can also add italic and bold, e.g. <em>Font("Arial,20,bold,italic")</em></param></item>
            /// <item><param name="example1"><b>Font("MyFont")</b> - Set the font by a previously created font named <em>"MyFont"</em>
            /// </param></item>
            /// </list>
            /// <para></para>
            /// *** note: <b>Font()</b> and <b>TextFont()</b> are the same function.
            /// </summary>
            /// <returns></returns>
            public static kw Font(int fontSize)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(12, fontSize);
                return kwx;

            }

            public static kw NumbersOnly(bool bNumbersOnly = true)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericBoolean(14, bNumbersOnly);
                return kwx;
            }

            public static kw Range(int iMin,int iMax)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_RangeInt(iMin,iMax);
                return kwx;
            }

            public static kw Rangef(float fMin, float fMax)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_RangeFloat(fMin,fMax);
                return kwx;
            }

            public static kw Range(float fMin, float fMax)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_RangeFloat(fMin,fMax);
                return kwx;
            }

            public static kw Range(double fMin, double fMax)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_RangeFloat((float) fMin,(float) fMax);
                return kwx;
            }

            /// <summary>
            /// Sets the justification of an object relative to the window client area, such as text output, a text widget, 
            /// or other controls and some shapes.
            /// <para></para>
            /// <para></para>
            /// Typically, an object is placed at a specific or default X,Y position.  
            /// However, setting the justification of an object can place the object in anchored points in the window.
            /// <para></para>
            /// <para></para>
            /// For example, specifying <b>kw.Just("center")</b> will center a text widget in the window, 
            /// where <b>Just("BottomRight")</b> will place the text widget in the bottom-right area of the window.
            /// <para></para>
            /// <para></para>
            /// This is also useful for Text Widget Controls, Text output to Windows, as well as other controls and certain graphics shapes.
            /// See TextJust() to specifically control text when using functions that can set the justificatin for an object and text
            /// at the same time (when only text is used, Just() and TextJust() are the same function)
            /// <para></para>
            /// <para></para>
            /// note: Justification is usually relative to the window, but can also be related to a region (if active) or shape boundaries.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Use <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Use <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Justification Types using Text Just Type (e.g. Just("center"))</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            /// <item><param name="Center"><b>"Center"</b> - Sets text or object in the center of the window or region</param></item>
            /// <item><param name="TopLeft"><b>"TopLeft"</b> - Sets text or object in the upper-left</param></item>
            /// <item><param name="TopCenter"><b>"TopCenter"</b> - Sets text or object to the top-center</param></item>
            /// <item><param name="TopRight"><b>"TopRight"</b> - Sets text or object to the top-right</param></item>
            /// <item><param name="RightCenter"><b>"RightCenter"</b> - Sets text or object to the right in the center of the window/region</param></item>
            /// <item><param name="BottomRight"><b>"BottomRight"</b> - Sets text or object to the bottom right</param></item>
            /// <item><param name="BottomCenter"><b>"BottomCenter"</b> - Sets the text or object to the bottom center of the window/region</param></item>
            /// <item><param name="BottomLeft"><b>"BottomLeft"</b> - Sets the text or object to the bottom-left</param></item>
            /// <item><param name="LeftCenter"><b>"LeftCenter"</b> - Sets the text or object to the left (x) and center (y) of the window/region</param></item>
            /// <item><param name="CenterX"><b>"CenterX"</b> - Sets the text or object to the horizonal center of the default or otherwise specified vertical setting</param></item>
            /// <item><param name="CenterY"><b>"CenterY"</b> - Sets the text or object to the vertical center of the default or otherwise specified horizontal setting.</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// ** note:  <b>Just()</b> and <b>TextJust()</b> are the same function.
            /// </summary>
            /// <param name = "justType" > Type of justification, i.e.JustType::Center, JustType::TopRight, etc.</param>
            /// <returns></returns>
            public static kw Just(string justType)
            {
                var kwx = new kw();
                var rr = SDLL._SageVB_kw_GenericString(15, justType);
                kwx.pointer = rr.pointer;
                kwx.resource = rr.resource; // $$ Should free resource if it already exists
                kwx.resourceType = rr.type;

                return kwx;

            }

            public static kw JustBottom(bool bJustBottom = true) 
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(0, bJustBottom); return kwx;   // JustType::BottomCenter
            }
            public static kw JustTop(bool bJustTop = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(1, bJustTop); return kwx;   // JustType::TopCenter
            }
            public static kw JustRight(bool bJustRight = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(2, bJustRight); return kwx;   // JustType::RightCenter
            }
            public static kw JustCenter(bool bJustCenter = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(3, bJustCenter); return kwx;   // JustType::Center
            }
            public static kw JustLeft(bool bJustLeft = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(4, bJustLeft); return kwx;   // JustType::LeftCenter
            }
            public static kw JustTopLeft(bool bJustTopLeft = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(5, bJustTopLeft); return kwx;   // JustType::TopLeft
            }
            public static kw JustTopRight(bool bJustTopRight = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(6, bJustTopRight); return kwx;   // JustType::TopRight
            }
            public static kw JustTopCenter(bool bJustTopCenter = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(7, bJustTopCenter); return kwx;   // JustType::TopCenter
            }
            public static kw JustBottomLeft(bool bJustBottomLeft = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(8, bJustBottomLeft); return kwx;   // JustType::BottomLeft
            }
            public static kw JustBottomRight(bool bJustBottomRight = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(9, bJustBottomRight); return kwx;   // JustType::BottomRight
            }
            public static kw JustBottomCenter(bool bJustBottomCenter = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(10, bJustBottomCenter); return kwx;   // JustType::BottomCenter
            }

            /// <summary>
            /// Sets the Control, Window, Widget or other object to the center of the window (or region).
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">
            /// Use <b>PadY()</b> to set a vertical offset from this position.</param></item>
            /// <item><param name="PadX">
            /// Use <b>PadX()</b> to set a horizontal offset from this position.</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note : <b>Center()</b> and <b>JustCenter()</b> the same function
            /// </summary>
            /// <returns></returns>
            public static kw Center(bool bCenter = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(11, bCenter); return kwx;   // JustType::Center
            }

            /// <summary>
            /// Centers the text or object in the horizontal center of the current vertical placement.
            /// <para></para>
            /// <para></para>
            /// For example, with Write(), the text will be written at the current vertical (Y) value from the last output, but 
            /// will center the text in the horizontal center of the window
            /// </summary>
            /// <param name="bCenterX"></param>
            /// <returns></returns>
            public static kw CenterX(bool bCenterX = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(12, bCenterX); return kwx;   // JustType::CenterX
            }
            public static kw CenterY(bool bCenterY = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(13, bCenterY); return kwx;   // JustType::CenterY
            }
            public static kw TextCenter(bool bTextCenter = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(14, bTextCenter); return kwx;   // JustType::Center
            }
            public static kw TextCenterX(bool bTextCenterX = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(15, bTextCenterX); return kwx;   // JustType::CenterX
            }
            public static kw TextCenterY(bool bTextCenterY = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(16, bTextCenterY); return kwx;   // JustType::CenterY
            }
            public static kw JustCenterX(bool bJustCenterX = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(15, bJustCenterX); return kwx;   // JustType::CenterX
            }
            public static kw JustCenterY(bool bJustCenterY = true)
            {
                var kwx = new kw(); kwx.pointer = SDLL._SageVB_kw_SetJustType(16, bJustCenterY); return kwx;   // JustType::CenterY
            }

            // SetPos() and Variants -- Same as Pos()

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw SetPos(int x, int y)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairInt(x, y);
                return kwx;

            }

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw SetPos(float w, float h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(w, h);
                return kwx;
            }

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw SetPos(double w, double h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat((float)w, (float)h);
                return kwx;
            }

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw SetPos(CPoint size)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat((float)size.x, (float)size.y);
                return kwx;
            }

            // Pos() and Variants -- Same as SetPos()

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw Pos(int x, int y)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairInt(x, y);
                return kwx;

            }

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw Pos(float w, float h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(w, h);
                return kwx;
            }

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw Pos(double w, double h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat((float)w, (float)h);
                return kwx;
            }

            /// <summary>
            /// Pos will set the x and y position of the output of an object or text, overriding the default position.
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            ///<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <b>Separate values or paired Point values may be used:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            ///<item><param name="separate">Separate integer or float values: e.g. <b>Pos(400,200)</b></param></item>
            ///<item><param name="point">Point, PointF, and Sagebox CPoint type: e.g. <b>Pos(new Point(400,200))</b> or <b>Pos(MyPoint)</b></param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// **note: <b>kw.Pos()</b> and <b>kw.SetPos()</b> are the same function
            /// </summary>
            /// <param name="x">x - x position</param>
            /// <param name="y">y - y position</param>
            /// <returns></returns>
            public static kw Pos(CPoint size)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat((float)size.x, (float)size.y);
                return kwx;
            }


            // SetSize() and Variants -- Same as Size().

            public static kw SetSize(int w,int h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairInt(w, h);
                return kwx;
            }

            public static kw SetSize(float w,float h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat(w, h);
                return kwx;
            }

            public static kw SetSize(double w, double h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat((float) w, (float) h);
                return kwx;
            }

            public static kw SetSize(CPoint size)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat((float) size.x, (float) size.y);
                return kwx;
            }

            // Size() and Variants -- Same as SetSize()

            public static kw Size(int w, int h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairInt(w, h);
                return kwx;
            }

            public static kw Size(float w, float h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat(w, h);
                return kwx;
            }

            public static kw Size(double w, double h)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat((float)w, (float)h);
                return kwx;
            }

            public static kw Size(CPoint size)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat((float)size.x, (float)size.y);
                return kwx;
            }

            public static kw UpdateNow(bool bUpdateNow = true)
            {
                var kwx = new kw();
                kwx.pointer = SDLL._SageVB_kw_GenericBoolean(16,bUpdateNow);
                return kwx;
            }


            // Just for testing
            public kw(ref kw prev)
            {
                Console.WriteLine("in copy constructor for kw");
            }
        }

        #pragma warning restore CS8981      // 'kw' being lowercase triggers a warning

        class Sage
        {
            public enum AutoUpdateType
            {
                Off         = 0,
                On          = 1,
                Timer       = 2,
                Immediate   = 3,
            }

            static Sage()
            {
                SDLL._SageVB_Init(0, System.IO.Path.GetFullPath(@"..\..\..\"));
            }

            // Create a Gradient color (2 colors) from Color types
            static public CColor GradColor(Color color1, Color color2) { return new CColor(color1, color2); }


            // $$ Can this go up top without a class? 
            // $$ Move this up top (within a class if necessary, e.g. utils.LogVInfo(), etc.)

            static void LogVInfo(string _module, string message)
            {
                SDLL._SageVB_LogVInfo(_module, message);
            }


            public class Slider
            {
                public long pointer;
                public Slider(long _pointer)
                {
                    pointer = _pointer;
                }
                public int GetPos()
                {
                    return SDLL._SageVB_Slider_GetPos(pointer);
                }
                public double GetPosf()
                {
                    return SDLL._SageVB_Slider_GetPosf(pointer);
                }
               public float GetPosff()
                {
                    return SDLL._SageVB_Slider_GetPosff(pointer);
                }
                public bool Moved()
                {
                    return SDLL._SageVB_Slider_Moved(pointer);
                }
            }


            public class InputBox
            {
                public long pointer;
                public InputBox(long _pointer)
                {
                    pointer = _pointer;
                }

                /// <summary>
                /// Returns True if a return was entered in the input Box.
                /// <para></para>
                /// <para></para>
                /// Once a return is pressed, functions such as <b>GetText()</b>, <b>GetInteger()</b>, etc. can be used to retrieve
                /// the text or convert the text into a value.
                /// </summary>
                /// <returns></returns>
                public bool ReturnPressed()
                {
                    return SDLL._SageVB_InputBox_ReturnPressed(pointer);
                }

                /// <summary>
                /// Clears any text or values displaying in the Input Box.
                /// </summary>
                /// <returns></returns>
               public bool ClearText()
                {
                    return SDLL._SageVB_InputBox_ClearText(pointer);
                }

                /// <summary>
                /// Returns the Integer value of the text displaying in the Input Box.
                /// <para></para>
                /// <para></para>
                /// If there is no text or the text starts with non-numeric values (e.g. "abcd"), 0 is returned.
                /// </summary>
                /// <returns></returns>
                public int GetInteger()
                {
                    return SDLL._SageVB_InputBox_GetInteger(pointer);
                }

                /// <summary>
                /// Returns the text displaying in the Input Box as a string.
                /// <para></para>
                /// <para></para>
                /// If there is no text displaying in the input Box a string with empty contents is returned, e.g. ""
                /// </summary>
                /// <returns></returns>
                public string GetText()
                {
                    string g = Marshal.PtrToStringAnsi(SDLL._SageVB_InputBox_GetText(pointer));
                    return g;
                }
            }

            const int kMaxPointStructValue = 250;

            public class Window
            {
 
                private point_struct[] point_transfer = new point_struct[kMaxPointStructValue]; // Used to transfer raw point arrays to graphics functions, e.g. DrawPolygon()
                public long pointer;
                public Window(long _pointer)
                {
                    pointer = _pointer;
                }

                ///<summary>
                ///Write Text out to the window. 
                ///<para></para>
                ///<para></para>
                ///The write() function will write text to the window and can be used with many options.
                ///<para></para>
                ///<para></para>
                ///A basic example is: <b>>window.write("Hello World")</b>, <b>window.write("Hello World")</b>,
                ///or <b>window.write(<em>$"My Variable is: {MyVariable}"</em></b>) 
                ///<para></para>
                ///<para></para>
                ///Sagebox keywords (e.g. <em><b>kw.Font(100)</b></em> options can be included.  Some various options are as follows:
                ///<para></para>
                ///<para></para>
                /// <list type="bullet">
                ///<item><param name="Font"><b>Font</b> - Set the font to be used for the text</param></item>
                ///<item><param name="Center,CenterX,CenterY"><b>Center,CenterX,CenterY</b> - Center the text in various ways 
                /// (i.e. CenterX centers in the X-axis, etc.)</param></item>
                ///<item><param name="TextColor,fgColor"><b>fgColor or TextColor</b> - Set the text color for the text.</param></item>
                ///<item><param name="bgColor"><b>bgColor</b> - Set the background color for the text</param></item>
                ///<item><param name="Angle"><b>Angle</b> - Set the rotational angle of the text</param></item>
                ///<item><param name="Opacity"><b>Opacity</b> - Set the opacity of the text</param></item>
                ///<item><param name="Pos"><b>Pos</b> - Set the position in the window of the text, 
                ///e.g. Write("Hello World",kw.pos(x,y))</param></item>
                ///</list>
                ///<para></para>
                ///<para></para>
                ///Controls can be embedded in the text line to change colors and font sizes:
                ///<para></para>
                ///<para></para>
                ///For example:
                ///<list type="bullet">
                ///<item><param name="Font"><b>window.write("This <em>{r}word{}</em> is in the color red").</b>
                /// <br />   
                ///    - Note the {} to close.  
                /// <br />   
                ///    - With C# formatted strings, an extra "{}" is needed, such as <em>"MyValue {{r}}{myvalue}{{}} is in red".</em>
                /// </param></item>
                ///</list>
                ///<para></para>
                ///<para></para>
                ///More than one control can be used such as: <b>win.write("This is in <em>{r}Red{}</em> and this is in 
                ///<em>{b}Blue</em>)".</b> 
                ///<br />
                ///You do not need the closing {} if it is as the end of the line.
                ///<para></para>
                ///<para></para>
                ///<b><ul>Some Curly-Brace Embedded Controls</ul></b>
                ///<list type="bullet">
                ///<item><param name="Color"><b>{&lt;color&gt;}</b> - Where the color is a defined color such as {red} (or {r}), 
                ///{blue}, {skyblue}, etc. e.g. <em>"this {blue}word{} is in blue"</em> ({} closes the color or font)
                ///<br />
                ///You can use abbreviations for most primary colors, such as <em>{y} = {yellow}</em>, etc.
                ///</param></item>                       
                ///<item><param name="FontSize"><b>{font size}</b> - e.g. <em>"This is in the normal font, and 
                ///                      {30}this is in a 30-point font"</em></param></item>
                ///<item><param name="FontName"><b>{font name}</b> - e.g. <em>"This is in the normal font, and 
                ///                             {Courier New,20}This is in a 20-point Courier New font"</em></param></item>
                ///<item><param name="SetXPos"><b>{x = &lt;value&gt;}</b> - Sets the X position on the current line,
                /// e.g. <em>"This value{x=100}starts at position 100 pixels from the left."</em>
                /// <br />This is useful in aligning data values with titling to the left, since typical fonts have varied sizes for each printed letter/numeral vs. static sizes.</param></item>
                ///</list>
                ///<para></para>
                ///<para></para>
                ///note: <em>when using <b>kw.Angle()</b> and <b>kw.Opacity()</b>, embedded options no longer work, 
                ///such as <b>{font}</b>, <b>{color}</b> etc.
                ///<br />
                /// - e.g. rather than "{50}This is a big font", when using Angle() and/or Opacity() options, use write("This is a big font",kw.Font(50)) instead.</em>
                ///<para></para>
                ///<para></para>
                ///<b>Examples:</b>
                ///<para></para>
                ///<para></para>
                ///<list type="bullet">
                ///<item><param name="Example1"><b>MyWindow.write("Hello World",kw.Font(40),kw.Center())</b>
                ///<br />- Writes a big "Hello World" in the center of the screen<br /></param></item>
                /// <item><param name="Example1"><b>MyWindow.write("Hello World",kw.fgColor("red")</b>
                ///<br />- Writes "Hello World" in red<br /></param></item>
                ///     
                ///<item><param name="Example1"><b>MyWindow.write("{r}Hello World")</b>
                ///<br />- Also writes "Hello World" in red<br /></param></item>
                ///     
                ///<item><param name="Example1"><b>MyWindow.write("Hello World",kw::Font(50))</b>
                ///<br />- Writes "Hello World" in a 50-point font size.<br /></param></item>
                ///     
                ///<item><param name="Example1"><b>MyWindow.write("{50}Hello World")</b>
                ///<br />- Also writes "Hello World" in a 50-point font size.<br /></param></item>
                ///</list>
                /// </summary>
                /// <param name="message"></param>
                /// <param name="keywords"></param>
                /// <returns></returns>
                public bool Write(string message, kw? keywords = null)
                {
                    return SDLL._SageVB_Write(message, pointer, keywords == null ? 0 : keywords.pointer);
                }

                // Circle Functions

                public bool DrawCircle(float x, float y, float radius, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, radius, ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }


                public bool DrawCircle(CPoint p, float radius, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.x,p.y, radius, ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }
 
                public bool FillCircle(float x, float y, float radius, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, radius, ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

                 public bool FillCircle(CPoint p, float radius, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.x, p.y, radius, ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

 
                public bool FillCircle(Point p, float radius, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X,p.Y, radius, ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }
 
                // Ellipse Functions

                public bool DrawEllipse(float x, float y, float xwidth, float ywidth, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth,ywidth, ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }
     

                public bool DrawEllipse(CPoint p, float xwidth, float ywidth, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.x, p.y, xwidth,ywidth, ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }
     

                public bool FillEllipse(float x, float y, float xwidth, float ywidth, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth,ywidth, ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }
                
                public bool FillEllipse(CPoint p, CPoint size, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.x, p.y, size.x,size.y, ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

                // Rectangle Functions

                public bool DrawRectangle(float x, float y, float xwidth, float ywidth, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, xwidth, ywidth, ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }

                public bool DrawRectangle(CPoint p, float xwidth, float ywidth, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.x, p.y, xwidth, ywidth, ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }


                public bool FillRectangle(float x, float y, float xwidth, float ywidth, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, xwidth, ywidth, ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillRectangle(CPoint p, CPoint size, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.x, p.y, size.x, size.y, ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

                // Triangle Functions


                public bool DrawTriangle(float x1,float y1,float x2,float y2,float x3,float y3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1,y1,x2,y2,x3,y3, ref color.colors, false, false, keywords == null ? 0 : keywords.pointer);
                }

                public bool DrawTriangle(CPoint p1, CPoint p2,CPoint p3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x,p1.y,p2.x,p2.y,p3.x,p3.y, ref color.colors, false, false, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, ref color.colors, true, false, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillTriangle(CPoint p1, CPoint p2, CPoint p3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, ref color.colors, true, false, keywords == null ? 0 : keywords.pointer);
                }

                // Fast Triangle Functions

                public bool DrawTriangleFast(float x1, float y1, float x2, float y2, float x3, float y3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, ref color.colors, false, true, keywords == null ? 0 : keywords.pointer);
                }

                public bool DrawTriangleFast(CPoint p1, CPoint p2, CPoint p3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, ref color.colors, false, true, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillTriangleFast(float x1, float y1, float x2, float y2, float x3, float y3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, ref color.colors, true, true, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillTriangleFast(CPoint p1, CPoint p2, CPoint p3, CColor color, kw? keywords = null)
                {
                    return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, ref color.colors, true, true, keywords == null ? 0 : keywords.pointer);
                }

                // Polygon Functions

                public bool DrawPolygon(ref Point[] points, CColor color, kw? keywords = null)
                {
                    int length = points.Length;


                    ref point_struct[] px = ref point_transfer;  // $$ Does this cause a realocation or is it just a pointer?

                    point_struct[] p2;
                    // Place points into pure points array.  If the array is too small, create a new one and use it instead.
                    if (length > kMaxPointStructValue)
                    {
                        p2 = new point_struct[length];
                        px = p2;
                    }
                    for (int i = 0; i < length; i++) { px[i].x = points[i].X; px[i].y = points[i].Y; }

                    return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer,length,ref px[0], ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }

                public bool DrawPolygon(ref PointF[] points, CColor color, kw? keywords = null)
                {
                    int length = points.Length;


                    ref point_struct[] px = ref point_transfer;  // $$ Does this cause a realocation or is it just a pointer?

                    point_struct[] p2;
                    // Place points into pure points array.  If the array is too small, create a new one and use it instead.
                    if (length > kMaxPointStructValue)
                    {
                        p2 = new point_struct[length];
                        px = p2;
                    }
                    for (int i = 0; i < length; i++) { px[i].x = points[i].X; px[i].y = points[i].Y; }

                    return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer,length,ref px[0], ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }

                public bool DrawPolygon(ref CPoint[] points, CColor color, kw? keywords = null)
                {
                    int length = points.Length;


                    ref point_struct[] px = ref point_transfer;  // $$ Does this cause a realocation or is it just a pointer?

                    point_struct[] p2;
                    // Place points into pure points array.  If the array is too small, create a new one and use it instead.
                    if (length > kMaxPointStructValue)
                    {
                        p2 = new point_struct[length];
                        px = p2;
                    }
                    for (int i = 0; i < length; i++) { px[i].x = points[i].x; px[i].y = points[i].y; }

                    return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer,length,ref px[0], ref color.colors, false, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillPolygon(ref Point[] points, CColor color, kw? keywords = null)
                {
                    int length = points.Length;


                    ref point_struct[] px = ref point_transfer;  // $$ Does this cause a realocation or is it just a pointer?

                    point_struct[] p2;
                    // Place points into pure points array.  If the array is too small, create a new one and use it instead.
                    if (length > kMaxPointStructValue)
                    {
                        p2 = new point_struct[length];
                        px = p2;
                    }
                    for (int i = 0; i < length; i++) { px[i].x = points[i].X; px[i].y = points[i].Y; }

                    return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer,length,ref px[0], ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillPolygon(ref PointF[] points, CColor color, kw? keywords = null)
                {
                    int length = points.Length;


                    ref point_struct[] px = ref point_transfer;  // $$ Does this cause a realocation or is it just a pointer?

                    point_struct[] p2;
                    // Place points into pure points array.  If the array is too small, create a new one and use it instead.
                    if (length > kMaxPointStructValue)
                    {
                        p2 = new point_struct[length];
                        px = p2;
                    }
                    for (int i = 0; i < length; i++) { px[i].x = points[i].X; px[i].y = points[i].Y; }

                    return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer,length,ref px[0], ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

                public bool FillPolygon(ref CPoint[] points, CColor color, kw? keywords = null)
                {
                    int length = points.Length;


                    ref point_struct[] px = ref point_transfer;  // $$ Does this cause a realocation or is it just a pointer?

                    point_struct[] p2;
                    // Place points into pure points array.  If the array is too small, create a new one and use it instead.
                    if (length > kMaxPointStructValue)
                    {
                        p2 = new point_struct[length];
                        px = p2;
                    }
                    for (int i = 0; i < length; i++) { px[i].x = points[i].x; px[i].y = points[i].y; }

                    return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer,length,ref px[0], ref color.colors, true, keywords == null ? 0 : keywords.pointer);
                }

                // Other graphics functions


                // Rectangle Functions (specific prototypes (CColor, Color, etc.) used here to speed up SetPixel()
                public bool DrawPixel(int x, int y, ref Color color)
                {
                    return SDLL._SageVB_Window_SetPixel_Rgb(pointer,x,y,color.R,color.G,color.B);
                }

                public bool SetPixel(int x, int y, ref Color color)
                {
                    return SDLL._SageVB_Window_SetPixel_Rgb(pointer,x,y,color.R,color.G,color.B);
                }

                //$$ Should probably changce to CColor
                public bool Cls(Color? Color1 = null, Color? Color2 = null)
                {
                    int red1 = -1;
                    int green1 = -1;
                    int blue1 = -1;

                    int red2 = -1;
                    int green2 = -1;
                    int blue2 = -1;

                    if (Color2 != null) { red2 = Color2.Value.R; green2 = Color2.Value.G; blue2 = Color2.Value.B; }
                    if (Color1 != null) { red1 = Color1.Value.R; green1 = Color1.Value.G; blue1 = Color1.Value.B; }
                    return SDLL._SageVB_Window_Cls(pointer, red1, green1, blue1, red2, green2, blue2, false);
                }
                public bool ClsRadial(Color? Color1 = null, Color? Color2 = null)
                {
                    int red1 = -1;
                    int green1 = -1;
                    int blue1 = -1;

                    int red2 = -1;
                    int green2 = -1;
                    int blue2 = -1;

                    if (Color2 != null) { red2 = Color2.Value.R; green2 = Color2.Value.G; blue2 = Color2.Value.B; }
                    if (Color1 != null) { red1 = Color1.Value.R; green1 = Color1.Value.G; blue1 = Color1.Value.B; }
                    return SDLL._SageVB_Window_Cls(pointer, red1, green1, blue1, red2, green2, blue2, true);
                }

                // $$ Can remove if Cls() is changed to CColor above
                public bool Cls(string color)
                {
                    return SDLL._SageVB_Window_ClsStr(pointer, color,false);
                }
                public bool ClsRadial(string color)
                {
                    return SDLL._SageVB_Window_ClsStr(pointer, color,true);
                }

                public CPoint GetWindowSize()
                {
                    float x = 0, y = 0;
                    SDLL._SageVB_Window_GetWindowSize(pointer,ref x, ref y);
                    return new CPoint(x, y);
                }
                public CPoint GetWindowCenter()
                {
                    float x = 0, y = 0;
                    SDLL._SageVB_Window_GetWindowCenter(pointer,ref x, ref y);
                    return new CPoint(x, y);
                }
                public bool SetAutoUpdate(AutoUpdateType eUpdateType)
                {
                    return SDLL._SageVB_Window_SetAutoUpdateInt(pointer,(int) eUpdateType);
                }

                public bool SetAutoUpdate(string sUpdateType)
                {
                    return SDLL._SageVB_Window_SetAutoUpdateStr(pointer,sUpdateType);
                }
            }

            // ------------------------
            // Sagebox Global Functions
            // ------------------------

            public static void ObjectTest(CIPoint p1)
            {
                SDLL._SageVB_ObjectTest(ref p1);
            }

            /// <summary>
            /// Brings up an input box that informs the user the program has ended.
            /// <para></para>
            /// An Ok button is displayed that will continue program execution once pressed.
            /// <para></para>
            /// <para></para>
            /// The ExitButton() function is useful to let the user know the program has ended, and also to
            /// <para></para>
            /// keep the program from returning to the operating system, which will close down all Sagebox windows.
            /// <para></para>
            /// <para></para>
            /// <b>Parameters:</b>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            /// <item><param name="message"><b>message</b> - <em>[optional]</em> text to display in the input box.
            /// <br />
            /// - Just using <b>ExitButton()</b> brings up a default message informing the user the program has completed.
            /// <br />
            /// - Using a specific message will change the displayed message, such as <b>ExitButton("Personalized Message Goes Here");</b>
            /// </param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            /// <item><param name="return">ExitButton() returns 0, allowing using statements such as 
            /// <em><b>return ExitButton();</b></em></param></item>
            /// </list>
            /// </summary>
            /// <param name="message"></param>
            /// <returns></returns>
            public static int ExitButton(string message = "")
            {
                return SDLL._SageVB_ExitButton(message);
            }

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            public static bool GetEvent()
            {
                return SDLL._SageVB_GetEvent();
            }

            /// <summary>
            /// NewWindow -- Create a new popup window.
            /// <para></para>
            /// <para></para>
            /// Creates a new window on the desktop and returns a window object where you can perform window-based functions on that window.
            /// <para></para>
            /// <para></para>
            /// Parameters:
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            /// <item><param name="title"><b>title</b> - [optional] Title of window (in title bar)</param></item>
            /// <item><param name="keywords"><b>keywords</b> - [optional] optional keywords, e.g. Size(600,500), bgColor(&quot;black&quot;), etc.</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// Keywords and Pybox options can be included.  Some various options are as follows:
            /// <para></para>
            /// <list type="bullet">
            /// <item><param name="title"><b>Font</b> - Sets the default font for the window, e.g. &quot;Font(40)&quot; or Font(&quot;Arial,40&quot;)</param></item>
            /// <item><param name="title"><b>bgColor</b> - Sets the background color of the window (e.g. bgColor(&quot;Red&quot;))</param></item>
            /// <item><param name="title"><b>bgGradient</b> - Sets a gradient background color of the window. 
            ///                   Such as bgGradient("black,blue").  
            ///                   You can also use Cls() to clear the window.</param></item>
            /// <item><param name="title"><b>TextColor</b> - Sets the text/foreground color of the window</param></item>
            /// <item><param name="title"><b>NoAutoUpdate</b> - Tells the window to not update automatically when
            ///                   graphics are placed in the window.  
            ///                   The program must call Update() itself to update the window contents.  
            ///                   This can prevent flashing in real-time drawing.</param></item>
            /// <item><param name="title"><b>ResizeOk</b> - Allows the window to be resized by the user. </param></item>
            /// <item><param name="title"><b>Size</b> - Sets the size of the window.  
            ///                   Usually the default is 1200x800, e.g. Size(1200,800)</param></item>
            /// <item><param name="title"><b>Pos</b> - Sets the position of the window.  
            ///                   e.g. Pos(200,400). The Window is otherwise automatically placed</param></item>
            /// </list>
            /// <para></para>
            /// <para></para>
            /// Examples:
            /// <para></para>
            /// <para></para>
            /// <list type="bullet">
            /// <item><param name="example1"><b>myWindow = new_window()
            /// <br />mWindow.write(&quot;Hello World\n&quot;)</b>
            /// <br />- Creates a new window and writes "Hello World" to it.</param></item>
            /// <item><param name="example3"><b>myWindow = new_window("This is the Title",kw.Size(500,200),kw.Pos(50,100),kw.bgColor("Blue"))</b>
            /// <br> - This sets the upper title of the window, the size to (500,20), the Position to (50,100) on the desktop, and sets the background color to blue</br></param></item>
            /// <item><param name="example4"><b>myWindow = new_window(kw.bgColor(&quot;Blue&quot;))</b>
            /// <br /> - This just sets the background color to blue. The Position, Size, and Window Title are set to defaults</param></item>
            /// </list>
            /// </summary>
            /// <param name="title"> - [optional] title of the window.  kw.Title() can be used as a keyword instead.</param>
            /// <param name="keywords"> - [optional] keyword controls the the window, e.g. kw.bgColor(&quot;black&quot;)</param>
            /// <returns></returns>
            public static Window NewWindow(kw? keywords = null)
            {
                return new Window(SDLL._SageVB_NewWindow(keywords == null ? 0 : keywords.pointer,""));
            }

            public static Window NewWindow(string title,kw? keywords = null)
            {
                return new Window(SDLL._SageVB_NewWindow(keywords == null ? 0 : keywords.pointer,title));
            }

            // ---> Dev Window Functions

            public static Slider DevSlider(string title, kw? keywords = null)
            {
                return new Slider(SDLL._SageVB_DevSlider(title, keywords == null ? 0 : keywords.pointer,false));
            }
            public static Slider DevSliderf(string title, kw? keywords = null)
            {
                return new Slider(SDLL._SageVB_DevSlider(title, keywords == null ? 0 : keywords.pointer,true));
            }
            public static Window DevText(string? text, kw? keywords = null)
            {
                return new Window(SDLL._SageVB_DevText(0, text == null ? "" : text, keywords == null ? 0 : keywords.pointer));
            }
            public static Window DevText(int height,kw? keywords = null)
            {
                return new Window(SDLL._SageVB_DevText(height,"", keywords == null ? 0 : keywords.pointer));
            }
            public static Window DevText(int height,string text, kw? keywords = null)
            {
                return new Window(SDLL._SageVB_DevText(height, text, keywords == null ? 0 : keywords.pointer));
            }
            public static InputBox DevInputBox(string title = "", kw? keywords = null)
            {
                return new InputBox(SDLL._SageVB_DevInputBox(title, keywords == null ? 0 : keywords.pointer));
            }

            public static bool SetIniPath(string iniPath)
            {
                return SDLL._SageVB_SetIniFilePath(iniPath);
            }

            public static bool SetDebugLevel(string level)
            {
                return SDLL._SageVB_SetLogLevelStr(level);
            }
            public static bool DebugWrite(string sMessage)
            {
                return SDLL._SageVB_DebugWrite(sMessage,false);
            }
            public static bool DebugWriteln(string sMessage)
            {
                return SDLL._SageVB_DebugWrite(sMessage,true);
            }

            public static void DisableDefaults()
            {
                SDLL._SageVB_DisableDefaults();
            }
            public static void DisplayDefaultPaths()
            {
                SDLL._SageVB_DisplayDefaultPaths();
            }
        }
    }

}
