Imports System.Drawing
Imports System.Reflection
Imports System.Runtime.CompilerServices
Imports System.Runtime.InteropServices

Namespace Sagebox


    Module Sagebox

        Public Structure RgbStruct
            Public red As Integer
            Public green As Integer
            Public blue As Integer
            Public alpha As Integer
        End Structure


        Public Class CColor

            Public Structure ColorStruct
                Public color1 As RgbStruct
                Public color2 As RgbStruct
            End Structure

            Public colors As ColorStruct

            Public Sub New(color As Color)
                colors.color1.red = color.R
                colors.color1.green = color.G
                colors.color1.blue = color.B
                colors.color1.alpha = color.A
                colors.color2.alpha = -1 ' i.e. no second color
            End Sub

            Public Sub New(_color As Color, _color2 As Color)
                colors.color1.red = _color.R
                colors.color1.green = _color.G
                colors.color1.blue = _color.B
                colors.color1.alpha = _color.A
                colors.color2.red = _color2.R
                colors.color2.green = _color2.G
                colors.color2.blue = _color2.B
                colors.color2.alpha = _color2.A
            End Sub

            Public Shared Function Grad(color As Color, color2 As Color) As CColor
                Return New CColor(color, color2)
            End Function

            Public Sub New(color As String)
                SDLL._SageVB_ColorFromString(color, colors.color1, colors.color2)

            End Sub
            '     public shared implicit operator New(color as Color) => new CColor(color)
            '     public shared implicit operator New(color as string color) => new CColor(color)
        End Class

        Public Class CPoint
            Public x As Single
            Public y As Single
            Public Sub New(_x As Single, _y As Single)
                x = _x
                y = _y
            End Sub

            'public static implicit operator CPoint(Point p) => new CPoint(p.X, p.Y);
            'public static implicit operator CPoint(PointF p) => new CPoint(p.X, p.Y);
            'public static implicit operator CPoint(Size s) => new CPoint(s.Width, s.Height);
            'public static implicit operator CPoint(SizeF s) => new CPoint(s.Width, s.Height);

            Public Shared Operator +(_class As CPoint, _input As CPoint) As CPoint
                Return New CPoint(_class.x + _input.x, _class.y + _input.y)
            End Operator

            Public Shared Operator -(_class As CPoint, _input As CPoint) As CPoint
                Return New CPoint(_class.x - _input.x, _class.y - _input.y)
            End Operator

            Public Shared Operator /(_class As CPoint, _input As CPoint) As CPoint
                Return New CPoint(_class.x / _input.x, _class.y / _input.y)
            End Operator

            Public Shared Operator /(_class As CPoint, _input As Single) As CPoint
                Return New CPoint(_class.x / _input, _class.y / _input)
            End Operator

            Public Shared Operator *(_class As CPoint, _input As CPoint) As CPoint
                Return New CPoint(_class.x * _input.x, _class.y * _input.y)
            End Operator

            Public Function SelfRotateXY(fAngle As Single) As CPoint
                fAngle = 3.14159F * fAngle / 180.0F
                Dim fTemp = x * CSng(Math.Cos(fAngle)) - y * CSng(Math.Sin(fAngle))
                y = x * CSng(Math.Sin(fAngle)) + y * CSng(Math.Cos(fAngle))
                x = fTemp
                Return Me
            End Function

            ''' <summary>
            ''' Returns a PointF() structure containing the x and y values of the CPoint structure.
            ''' <para></para>
            ''' <para></para>
            ''' note: Just as with PointF structures CPoint structure values are floating point values
            ''' </summary>
            ''' <returns></returns>
            Public Function pointF() As PointF
                Return New PointF(x, y)
            End Function

            ''' <summary>
            ''' Returns a Point() structure containing the x and y values of the CPoint structure.
            ''' <para></para>
            ''' <para></para>
            ''' note: When converting CPoint to a Point structure, the floating-point values in the CPoint structure are converted to
            ''' integers since Point() contains integer values.
            ''' </summary>
            ''' <returns></returns>
            Public Function point() As Point
                Return New Point(CInt(x), CInt(y))
            End Function


            Public Function toF() As PointF
                Return New PointF(x, y)
            End Function


        End Class

        Public Structure point_struct
            Public x As Single
            Public y As Single
        End Structure

        Public Structure ResourceReturn
            Public type As Integer
            Public pointer As Long
            Public resource As Long
        End Structure


        Public Class SDLL

            ' --> Sagebox Global Functions
            '     *** Set direct or relative path in the first function _SageVB_Init, then "SageboxVB.dll" can be used after that.

            Public Declare Function _SageVB_Init Lib "SageboxVB.dll" (value As Integer, local_dir As String) As Integer
            Public Declare Function _SageVB_SetIniFilePath Lib "SageboxVB.dll" (local_dir As String) As Boolean
            Public Declare Function _SageVB_GetEvent Lib "SageboxVB.dll" () As Boolean
            Public Declare Function _SageVB_Write Lib "SageboxVB.dll" (x As String, win_pointer As Long, keywords As Long) As Boolean
            Public Declare Function _SageVB_ExitButton Lib "SageboxVB.dll" (message As String) As Integer
            Public Declare Function _SageVB_GetkwObj Lib "SageboxVB.dll" () As Long        ' $$ Deprecated

            Public Declare Function _SageVB_NewWindow Lib "SageboxVB.dll" (keywords As Long) As Long
            Public Declare Function _SageVB_DevSlider Lib "SageboxVB.dll" (title As String, keywords As Long, isFloat As Boolean) As Long
            Public Declare Function _SageVB_DevText Lib "SageboxVB.dll" (height As Integer, title As String, keywords As Long) As Long
            Public Declare Function _SageVB_DevInputBox Lib "SageboxVB.dll" (title As String, keywords As Long) As Long
            Public Declare Sub _SageVB_LogVInfo Lib "SageboxVB.dll" (_module As String, message As String)
            Public Declare Function _SageVB_SetLogLevelStr Lib "SageboxVB.dll" (level As String) As Boolean
            Public Declare Function _SageVB_DebugWrite Lib "SageboxVB.dll" (message As String, bAddNewLine As Boolean) As Boolean
            Public Declare Sub _SageVB_DisableDefaults Lib "SageboxVB.dll" ()
            Public Declare Sub _SageVB_DisplayDefaultPaths Lib "SageboxVB.dll" ()

            ' kw Class Functions

            Public Declare Function _SageVB_kw_PenSize Lib "SageboxVB.dll" (x As Single) As Long
            Public Declare Function _SageVB_kw_AddObjects Lib "SageboxVB.dll" (p1 As Long, p2 As Long) As Long
            Public Declare Function _SageVB_kw_TestReturn Lib "SageboxVB.dll" () As ResourceReturn   ' $$ deprecated 
            Public Declare Function _SageVB_kw_FreeResource Lib "SageboxVB.dll" (pResource As Long, type As Integer) As Long             ' Always returns 0
            Public Declare Sub _SageVB_kw_EvalObject Lib "SageboxVB.dll" (p1 As Long)
            Public Declare Function _SageVB_kw_GenericColor Lib "SageboxVB.dll" (index As Integer, r As Integer, g As Integer, b As Integer) As Long
            Public Declare Function _SageVB_kw_GenericBiColor Lib "SageboxVB.dll" (index As Integer, colors As CColor.ColorStruct) As Long
            Public Declare Function _SageVB_kw_GenericString Lib "SageboxVB.dll" (index As Integer, value As String) As ResourceReturn
            Public Declare Function _SageVB_kw_GenericFloat Lib "SageboxVB.dll" (index As Integer, value As Single) As Long
            Public Declare Function _SageVB_kw_GenericInteger Lib "SageboxVB.dll" (index As Integer, value As Integer) As Long
            Public Declare Function _SageVB_kw_GenericBoolean Lib "SageboxVB.dll" (index As Integer, value As Boolean) As Long
            Public Declare Function _SageVB_kw_RangeInt Lib "SageboxVB.dll" (min As Integer, max As Integer) As Long
            Public Declare Function _SageVB_kw_RangeFloat Lib "SageboxVB.dll" (min As Single, Max As Single) As Long
            Public Declare Function _SageVB_kw_SizePairInt Lib "SageboxVB.dll" (s1 As Integer, s2 As Integer) As Long
            Public Declare Function _SageVB_kw_SizePairFloat Lib "SageboxVB.dll" (s1 As Single, s2 As Single) As Long
            Public Declare Function _SageVB_kw_PosPairInt Lib "SageboxVB.dll" (s1 As Integer, s2 As Integer) As Long
            Public Declare Function _SageVB_kw_PosPairFloat Lib "SageboxVB.dll" (s1 As Single, s2 As Single) As Long
            Public Declare Function _SageVB_kw_SetJustType Lib "SageboxVB.dll" (justType As Integer, value As Boolean) As Long

            ' --> Slider Functions

            Public Declare Function _SageVB_Slider_GetPos Lib "SageboxVB.dll" (pointer As Long) As Integer
            Public Declare Function _SageVB_Slider_GetPosf Lib "SageboxVB.dll" (pointer As Long) As Double
            Public Declare Function _SageVB_Slider_GetPosff Lib "SageboxVB.dll" (pointer As Long) As Single
            Public Declare Function _SageVB_Slider_Moved Lib "SageboxVB.dll" (pointer As Long) As Boolean

            ' Input Box Functions


            Public Declare Function _SageVB_InputBox_ReturnPressed Lib "SageboxVB.dll" (pointer As Long) As Boolean
            Public Declare Function _SageVB_InputBox_GetInteger Lib "SageboxVB.dll" (pointer As Long) As Integer
            Public Declare Function _SageVB_InputBox_GetText Lib "SageboxVB.dll" (pointer As Long) As IntPtr
            Public Declare Function _SageVB_InputBox_ClearText Lib "SageboxVB.dll" (pointer As Long) As Boolean

            ' --> Window Functions

            Public Declare Function _SageVB_Window_DrawCircle_Rgb Lib "SageboxVB.dll" (p1 As Long, x As Single, y As Single, radius As Single, ByRef colors As CColor.ColorStruct, filled As Boolean, keywords As Long) As Boolean
            Public Declare Function _SageVB_Window_DrawEllipse_Rgb Lib "SageboxVB.dll" (p1 As Long, x As Single, y As Single, xwidth As Single, ywidth As Single, ByRef colors As CColor.ColorStruct, filled As Boolean, keywords As Long) As Boolean
            Public Declare Function _SageVB_Window_DrawRectangle_Rgb Lib "SageboxVB.dll" (p1 As Long, x As Single, y As Single, xwidth As Single, ywidth As Single, ByRef colors As CColor.ColorStruct, filled As Boolean, keywords As Long) As Boolean
            Public Declare Function _SageVB_Window_DrawTriangle_Rgb Lib "SageboxVB.dll" (p1 As Long, x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, ByRef colors As CColor.ColorStruct, filled As Boolean, fast As Boolean, keywords As Long) As Boolean
            Public Declare Function _SageVB_Window_DrawPolygon_Rgb Lib "SageboxVB.dll" (p1 As Long, iSides As Integer, ByRef p As point_struct, ByRef colors As CColor.ColorStruct, filled As Boolean, keywords As Long) As Boolean
            Public Declare Function _SageVB_Window_SetPixel_Rgb Lib "SageboxVB.dll" (p1 As Long, x As Integer, y As Integer, red As Integer, green As Integer, blue As Integer) As Boolean

            Public Declare Function _SageVB_Window_Cls Lib "SageboxVB.dll" (p1 As Long, red1 As Integer, green1 As Integer, blue1 As Integer,
                                                                         red2 As Integer, green2 As Integer, blue2 As Integer, radial As Boolean) As Boolean

            Public Declare Function _SageVB_Window_ClsStr Lib "SageboxVB.dll" (p1 As Long, color As String, radial As Boolean) As Boolean
            Public Declare Function _SageVB_ColorFromString Lib "SageboxVB.dll" (color As String, ByRef color1 As RgbStruct, ByRef color2 As RgbStruct) As Boolean
            '            public Declare Sub     _SageVB_ObjectTest Lib "SageboxVB.dll"(ByRef p1 As CIPoint);  '' $$ Deprecated and should be removed from C# also
            Public Declare Sub _SageVB_PointStructTest Lib "SageboxVB.dll" (iSize As Integer, ByRef p As point_struct)
            Public Declare Function _SageVB_Window_GetWindowSize Lib "SageboxVB.dll" (p1 As Long, ByRef x As Single, ByRef y As Single) As Boolean
            Public Declare Function _SageVB_Window_GetWindowCenter Lib "SageboxVB.dll" (p1 As Long, ByRef x As Single, ByRef y As Single) As Boolean
            Public Declare Function _SageVB_Window_SetAutoUpdateStr Lib "SageboxVB.dll" (p1 As Long, sUpdateType As String) As Boolean
            Public Declare Function _SageVB_Window_SetAutoUpdateInt Lib "SageboxVB.dll" (p1 As Long, iUpdateType As Integer) As Boolean

        End Class


        Public Class RgbColor
            Public Red As Integer
            Public Green As Integer
            Public Blue As Integer

            <MethodImplAttribute(MethodImplOptions.AggressiveInlining)>
            Sub New(_Red As Integer, _Green As Integer, _Blue As Integer)
                Red = _Red
                Green = _Green
                Blue = _Blue
            End Sub

            Sub New()
                ' Placeholder
            End Sub

        End Class


        ' Temporary conversion function for Sagebox prototype. 
        ' Input values will be converted to RgbColor() values later on

        <MethodImplAttribute(MethodImplOptions.AggressiveInlining)>
        Private Function _ccc(x As Integer) As Color
            '            return  New RgbColor ((x >> 16) And 255,(x >> 8) And 255,x And 255)
            Return Color.FromArgb((x >> 16) And 255, (x >> 8) And 255, x And 255)

        End Function

        Public Structure PanColor
            Public Shared AliceBlue = _ccc(&HFFF0F8FF)
            Public Shared AntiqueWhite = _ccc(&HFFFAEBD7)
            Public Shared Aqua = _ccc(&HFF00FFFF)
            Public Shared Aquamarine = _ccc(&HFF7FFFD4)
            Public Shared Azure = _ccc(&HFFF0FFFF)
            Public Shared Beige = _ccc(&HFFF5F5DC)
            Public Shared Bisque = _ccc(&HFFFFE4C4)
            Public Shared Black = _ccc(&HFF000000)
            Public Shared BlanchedAlmond = _ccc(&HFFFFEBCD)
            Public Shared Blue = _ccc(&HFF0000FF)
            Public Shared BlueViolet = _ccc(&HFF8A2BE2)
            Public Shared Brown = _ccc(&HFFA52A2A)
            Public Shared BurlyWood = _ccc(&HFFDEB887)
            Public Shared CadetBlue = _ccc(&HFF5F9EA0)
            Public Shared Chartreuse = _ccc(&HFF7FFF00)
            Public Shared Chocolate = _ccc(&HFFD2691E)
            Public Shared Coral = _ccc(&HFFFF7F50)
            Public Shared CornflowerBlue = _ccc(&HFF6495ED)
            Public Shared Cornsilk = _ccc(&HFFFFF8DC)
            Public Shared Crimson = _ccc(&HFFDC143C)
            Public Shared Cyan = _ccc(&HFF00FFFF)
            Public Shared DarkBlue = _ccc(&HFF00008B)
            Public Shared DarkCyan = _ccc(&HFF008B8B)
            Public Shared DarkGoldenrod = _ccc(&HFFB8860B)
            Public Shared DarkGray = _ccc(&HFFA9A9A9)
            Public Shared DarkGreen = _ccc(&HFF006400)
            Public Shared DarkKhaki = _ccc(&HFFBDB76B)
            Public Shared DarkMagenta = _ccc(&HFF8B008B)
            Public Shared DarkOliveGreen = _ccc(&HFF556B2F)
            Public Shared DarkOrange = _ccc(&HFFFF8C00)
            Public Shared DarkOrchid = _ccc(&HFF9932CC)
            Public Shared DarkRed = _ccc(&HFF8B0000)
            Public Shared DarkSalmon = _ccc(&HFFE9967A)
            Public Shared DarkSeaGreen = _ccc(&HFF8FBC8B)
            Public Shared DarkSlateBlue = _ccc(&HFF483D8B)
            Public Shared DarkSlateGray = _ccc(&HFF2F4F4F)
            Public Shared DarkTurquoise = _ccc(&HFF00CED1)
            Public Shared DarkViolet = _ccc(&HFF9400D3)
            Public Shared DeepPink = _ccc(&HFFFF1493)
            Public Shared DeepSkyBlue = _ccc(&HFF00BFFF)
            Public Shared DimGray = _ccc(&HFF696969)
            Public Shared DodgerBlue = _ccc(&HFF1E90FF)
            Public Shared Firebrick = _ccc(&HFFB22222)
            Public Shared FloralWhite = _ccc(&HFFFFFAF0)
            Public Shared ForestGreen = _ccc(&HFF228B22)
            Public Shared Fuchsia = _ccc(&HFFFF00FF)
            Public Shared Gainsboro = _ccc(&HFFDCDCDC)
            Public Shared GhostWhite = _ccc(&HFFF8F8FF)
            Public Shared Gold = _ccc(&HFFFFD700)
            Public Shared Goldenrod = _ccc(&HFFDAA520)
            Public Shared Gray = _ccc(&HFF808080)
            Public Shared Green = _ccc(&HFF008000)
            Public Shared GreenYellow = _ccc(&HFFADFF2F)
            Public Shared Honeydew = _ccc(&HFFF0FFF0)
            Public Shared HotPink = _ccc(&HFFFF69B4)
            Public Shared IndianRed = _ccc(&HFFCD5C5C)
            Public Shared Indigo = _ccc(&HFF4B0082)
            Public Shared Ivory = _ccc(&HFFFFFFF0)
            Public Shared Khaki = _ccc(&HFFF0E68C)
            Public Shared Lavender = _ccc(&HFFE6E6FA)
            Public Shared LavenderBlush = _ccc(&HFFFFF0F5)
            Public Shared LawnGreen = _ccc(&HFF7CFC00)
            Public Shared LemonChiffon = _ccc(&HFFFFFACD)
            Public Shared LightBlue = _ccc(&HFFADD8E6)
            Public Shared LightCoral = _ccc(&HFFF08080)
            Public Shared LightCyan = _ccc(&HFFE0FFFF)
            Public Shared LightGoldenrodYellow = _ccc(&HFFFAFAD2)
            Public Shared LightGray = _ccc(&HFFD3D3D3)
            Public Shared LightGreen = _ccc(&HFF90EE90)
            Public Shared LightPink = _ccc(&HFFFFB6C1)
            Public Shared LightSalmon = _ccc(&HFFFFA07A)
            Public Shared LightSeaGreen = _ccc(&HFF20B2AA)
            Public Shared LightSkyBlue = _ccc(&HFF87CEFA)
            Public Shared LightSlateGray = _ccc(&HFF778899)
            Public Shared LightSteelBlue = _ccc(&HFFB0C4DE)
            Public Shared LightYellow = _ccc(&HFFFFFFE0)
            Public Shared Lime = _ccc(&HFF00FF00)
            Public Shared LimeGreen = _ccc(&HFF32CD32)
            Public Shared Linen = _ccc(&HFFFAF0E6)
            Public Shared Magenta = _ccc(&HFFFF00FF)
            Public Shared Maroon = _ccc(&HFF800000)
            Public Shared MediumAquamarine = _ccc(&HFF66CDAA)
            Public Shared MediumBlue = _ccc(&HFF0000CD)
            Public Shared MediumOrchid = _ccc(&HFFBA55D3)
            Public Shared MediumPurple = _ccc(&HFF9370DB)
            Public Shared MediumSeaGreen = _ccc(&HFF3CB371)
            Public Shared MediumSlateBlue = _ccc(&HFF7B68EE)
            Public Shared MediumSpringGreen = _ccc(&HFF00FA9A)
            Public Shared MediumTurquoise = _ccc(&HFF48D1CC)
            Public Shared MediumVioletRed = _ccc(&HFFC71585)
            Public Shared MidnightBlue = _ccc(&HFF191970)
            Public Shared MintCream = _ccc(&HFFF5FFFA)
            Public Shared MistyRose = _ccc(&HFFFFE4E1)
            Public Shared Moccasin = _ccc(&HFFFFE4B5)
            Public Shared NavajoWhite = _ccc(&HFFFFDEAD)
            Public Shared Navy = _ccc(&HFF000080)
            Public Shared OldLace = _ccc(&HFFFDF5E6)
            Public Shared Olive = _ccc(&HFF808000)
            Public Shared OliveDrab = _ccc(&HFF6B8E23)
            Public Shared Orange = _ccc(&HFFFFA500)
            Public Shared OrangeRed = _ccc(&HFFFF4500)
            Public Shared Orchid = _ccc(&HFFDA70D6)
            Public Shared PaleGoldenrod = _ccc(&HFFEEE8AA)
            Public Shared PaleGreen = _ccc(&HFF98FB98)
            Public Shared PaleTurquoise = _ccc(&HFFAFEEEE)
            Public Shared PaleVioletRed = _ccc(&HFFDB7093)
            Public Shared PapayaWhip = _ccc(&HFFFFEFD5)
            Public Shared PeachPuff = _ccc(&HFFFFDAB9)
            Public Shared Peru = _ccc(&HFFCD853F)
            Public Shared Pink = _ccc(&HFFFFC0CB)
            Public Shared Plum = _ccc(&HFFDDA0DD)
            Public Shared PowderBlue = _ccc(&HFFB0E0E6)
            Public Shared Purple = _ccc(&HFF800080)
            Public Shared Red = _ccc(&HFFFF0000)
            Public Shared RosyBrown = _ccc(&HFFBC8F8F)
            Public Shared RoyalBlue = _ccc(&HFF4169E1)
            Public Shared SaddleBrown = _ccc(&HFF8B4513)
            Public Shared Salmon = _ccc(&HFFFA8072)
            Public Shared SandyBrown = _ccc(&HFFF4A460)
            Public Shared SeaGreen = _ccc(&HFF2E8B57)
            Public Shared SeaShell = _ccc(&HFFFFF5EE)
            Public Shared Sienna = _ccc(&HFFA0522D)
            Public Shared Silver = _ccc(&HFFC0C0C0)
            Public Shared SkyBlue = _ccc(&HFF87CEEB)
            Public Shared SlateBlue = _ccc(&HFF6A5ACD)
            Public Shared SlateGray = _ccc(&HFF708090)
            Public Shared Snow = _ccc(&HFFFFFAFA)
            Public Shared SpringGreen = _ccc(&HFF00FF7F)
            Public Shared SteelBlue = _ccc(&HFF4682B4)
            Public Shared Tan = _ccc(&HFFD2B48C)
            Public Shared Teal = _ccc(&HFF008080)
            Public Shared Thistle = _ccc(&HFFD8BFD8)
            Public Shared Tomato = _ccc(&HFFFF6347)
            Public Shared Transparent = _ccc(&HFFFFFF)
            Public Shared Turquoise = _ccc(&HFF40E0D0)
            Public Shared Violet = _ccc(&HFFEE82EE)
            Public Shared Wheat = _ccc(&HFFF5DEB3)
            Public Shared White = _ccc(&HFFFFFFFF)
            Public Shared WhiteSmoke = _ccc(&HFFF5F5F5)
            Public Shared Yellow = _ccc(&HFFFFFF00)
            Public Shared YellowGreen = _ccc(&HFF9ACD32)
        End Structure


        Public Structure SageColor
            Public Shared DefaultBgColor As Color = Color.FromArgb(20, 40, 121)
            Public Shared DefaultFgColor As Color = Color.FromArgb(255, 255, 255)
            Public Shared SliderTextColor As Color = Color.FromArgb(128, 128, 128)
            Public Shared Green As Color = Color.FromArgb(0, 255, 0)
            Public Shared DarkGreen As Color = Color.FromArgb(0, 128, 0)
            Public Shared LightGreen As Color = Color.FromArgb(128, 255, 128)
            Public Shared Blue As Color = Color.FromArgb(0, 0, 255)
            Public Shared Blue32 As Color = Color.FromArgb(0, 0, 32)
            Public Shared Blue48 As Color = Color.FromArgb(0, 0, 48)
            Public Shared Blue64 As Color = Color.FromArgb(0, 0, 64)
            Public Shared DarkBlue As Color = Color.FromArgb(0, 0, 92)
            Public Shared LightBlue As Color = Color.FromArgb(40, 100, 255)
            Public Shared SkyBlue As Color = Color.FromArgb(40, 145, 255)
            Public Shared SkyBlueDark As Color = Color.FromArgb(0, 30, 128)
            Public Shared SkyBlueLight As Color = Color.FromArgb(75, 165, 255)
            Public Shared PaleBlueDark As Color = Color.FromArgb(40, 100, 140)
            Public Shared PaleBlue As Color = Color.FromArgb(103, 179, 217)
            Public Shared PaleBlueLight As Color = Color.FromArgb(145, 190, 215)
            Public Shared LightCyan As Color = Color.FromArgb(128, 255, 255)
            Public Shared Cyan As Color = Color.FromArgb(0, 255, 255)
            Public Shared MidCyan As Color = Color.FromArgb(15, 200, 200)
            Public Shared DarkCyan As Color = Color.FromArgb(30, 130, 130)
            Public Shared Red As Color = Color.FromArgb(255, 0, 0)
            Public Shared LightRed As Color = Color.FromArgb(255, 128, 128)
            Public Shared LightYellow As Color = Color.FromArgb(255, 255, 128)
            Public Shared Yellow As Color = Color.FromArgb(255, 255, 0)
            Public Shared Magenta As Color = Color.FromArgb(255, 0, 255)
            Public Shared MediumMagenta As Color = Color.FromArgb(255, 92, 255)
            Public Shared LightMagenta As Color = Color.FromArgb(255, 128, 255)
            Public Shared Purple As Color = Color.FromArgb(255, 0, 255)
            Public Shared LightPurple As Color = Color.FromArgb(255, 128, 255)
            Public Shared MediumPurple As Color = Color.FromArgb(255, 92, 255)
            Public Shared White As Color = Color.FromArgb(255, 255, 255)
            Public Shared Gray172 As Color = Color.FromArgb(172, 172, 172)
            Public Shared Gray192 As Color = Color.FromArgb(192, 192, 192)
            Public Shared Gray220 As Color = Color.FromArgb(220, 220, 220)
            Public Shared Gray128 As Color = Color.FromArgb(128, 128, 128)
            Public Shared Gray32 As Color = Color.FromArgb(32, 32, 32)
            Public Shared Gray42 As Color = Color.FromArgb(42, 42, 42)
            Public Shared Gray64 As Color = Color.FromArgb(64, 64, 64)
            Public Shared Gray72 As Color = Color.FromArgb(72, 72, 72)
            Public Shared Gray92 As Color = Color.FromArgb(92, 92, 92)
            Public Shared Black As Color = Color.FromArgb(0, 0, 0)
            Public Shared LightGray As Color = Color.FromArgb(200, 200, 200)
            Public Shared LightGrey As Color = Color.FromArgb(200, 200, 200)
            Public Shared MidGray As Color = Color.FromArgb(64, 64, 64)
            Public Shared MidGrey As Color = Color.FromArgb(64, 64, 64)
            Public Shared DarkGray As Color = Color.FromArgb(32, 32, 32)
            Public Shared DarkGrey As Color = Color.FromArgb(32, 32, 32)
            Public Shared Gray As Color = Color.FromArgb(128, 128, 128)
            Public Shared Grey As Color = Color.FromArgb(128, 128, 128)
            Public Shared NearWhite As Color = Color.FromArgb(220, 220, 220)
            Public Shared ButtonTextColorNormal As Color = Color.FromArgb(220, 220, 220)
            Public Shared ButtonTextColorHighlighted As Color = Color.FromArgb(255, 255, 255)
            Public Shared ButtonTextColorPressed As Color = Color.FromArgb(255, 255, 255)
            Public Shared ButtonTextColorDisabled As Color = Color.FromArgb(170, 170, 170)
            Public Shared CheckboxTextColorNormal As Color = Color.FromArgb(220, 220, 220)
            Public Shared CheckboxTextColorHighlighted As Color = Color.FromArgb(255, 255, 255)
            Public Shared CheckboxTextColorChecked As Color = Color.FromArgb(220, 220, 220)
            Public Shared CheckboxTextColorCheckedHigh As Color = Color.FromArgb(220, 220, 220)
            Public Shared CheckboxTextColorDisabled As Color = Color.FromArgb(170, 170, 170)
            Public Shared Orange As Color = Color.FromArgb(255, 115, 0)
            Public Shared LightOrange As Color = Color.FromArgb(255, 130, 0)
            Public Shared DarkOrange As Color = Color.FromArgb(255, 85, 0)
            Public Shared _Undefined As RgbColor = New RgbColor(-1, -1, -1)
        End Structure

        ' $$$ Figure out where to put this

        Private Sub LogVInfo(_Module As String, Message As String)
            SDLL._SageVB_LogVInfo(_Module, Message)
        End Sub

        '#Disable Warning VBS8981      ' 'kw' being lowercase triggers a warning
        '        '  but we want to keep kw as lowercase, otherwise
        '        '  it creates noisy code.

        Public Class kw
            Public pointer As Long = 0
            Public resource As Long = 0
            Public resourceType As Integer = 0

            Sub New()
                'Dim x = _SageVB_GetkwObj()
                LogVInfo("kw::New", "In Constructor")
            End Sub

            Protected Overrides Sub Finalize() ' destructor
                LogVInfo("kw::Finalize", "Executed Sagebox kw class Finalize.")
            End Sub

            Sub Dispose()
                resource = SDLL._SageVB_kw_FreeResource(resource, resourceType)
                LogVInfo("kw::Dispose", "Executed Sagebox kw class Dispose.")
            End Sub
            Sub Close()
                LogVInfo("kw::Close", "Executed Sagebox kw class Close.")
            End Sub

            Public Shared Operator +(ByVal _class As kw, ByVal _input As kw) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_AddObjects(_class.pointer, _input.pointer)
                SDLL._SageVB_kw_EvalObject(kwx.pointer)
                Return kwx
            End Operator
            Private Function UpdateResources(ByRef newKey As kw, ByRef r As ResourceReturn) As kw
                pointer = r.pointer
                resource = r.resource ' $$ Should free resource if it already exists
                resourceType = r.type
                Return newKey
            End Function

            Public Shared Function Color(ByVal Input As String) As kw
                Dim kwx = New kw()
                Return kwx.UpdateResources(kwx, SDLL._SageVB_kw_GenericString(1, Input))
            End Function


            Public Shared Function Color(ByVal Input As Color) As kw
                Dim kwx = New kw()

                kwx.pointer = SDLL._SageVB_kw_GenericColor(13, Input.R, Input.G, Input.B)
                Return kwx
            End Function

            Public Shared Function fgColor(ByVal color As Color) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericBiColor(8, New CColor(color).colors)
                Return kwx
            End Function

            Public Shared Function fgColor(color As String) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericBiColor(8, New CColor(color).colors)
                Return kwx
            End Function

            Public Shared Function bgColor(ByVal color As Color, Optional color2 As Color = Nothing) As kw
                Dim kwx = New kw()

                Dim outColor As New CColor(color)

                If Not color2.IsEmpty() Then
                    outColor.colors.color2.red = color2.R
                    outColor.colors.color2.green = color2.G
                    outColor.colors.color2.blue = color2.B
                    outColor.colors.color2.alpha = color2.A
                End If
                kwx.pointer = SDLL._SageVB_kw_GenericBiColor(9, outColor.colors)
                Return kwx
            End Function

            Public Shared Function bgColor(color As String) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericBiColor(9, New CColor(color).colors)
                Return kwx
            End Function

            Public Shared Function PenColor(ByVal Input As String) As kw
                Dim kwx = New kw()
                Return kwx.UpdateResources(kwx, SDLL._SageVB_kw_GenericString(3, Input))
            End Function

            Public Shared Function PenColor(ByVal Input As Color) As kw
                Dim kwx = New kw()

                kwx.pointer = SDLL._SageVB_kw_GenericColor(10, Input.R, Input.G, Input.B)
                Return kwx
            End Function



            ''' <summary>
            ''' <b>Sets the size of the pen used for a drawing function.</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            ''' <item><param name="1">The default pen size Is 1 (i.e. for drawing functions when <b>kw.PenSize()</b> Is Not used)</param></item>.  
            ''' <item><param name="2">When <b>kw.PenSize()</b> Is omitted (i.e. Not used for a drawing function), the global pen size setting Is used.
            ''' The global pen size setting defaults to 1, but can be change (see below)</param></item>.  
            ''' <item><param name="3">When using <b>kw.PenSize()</b>, the pen size Is only for the current drawing operation.</param></item>
            ''' <item><param name="4">Use <b>win.SetPenSize()</b> to set a global pen size for all drawing functions (in which case <b>kw.PenSize()</b>
            ''' can be used to divert from the global pen size for specific drawing operations.
            ''' </param></item>
            ''' <item><param name="5">Pen Sizes can be floating point values, allowing for anti-aliased lines And shapes.</param></item>
            '''</list>
            ''' </summary>
            ''' <param name="input"></param>
            ''' <returns></returns>
            Public Shared Function PenSize(ByVal Input As Single) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(4, Input) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            ''' <summary>
            ''' Sets the angle of an object such as a circle, square, ellipse, bitmap, etc. in degrees.
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="Angle"><b>Angle</b> - Angle Is in Degrees (i.e. 0-360).  Use <em>AngleRad()</em> to set angle in radians</param></item>
            '''</list>
            ''' <para></para>
            ''' ** note: <b> Angle()</b> And <b>AngleDeg()</b> are the same Function
            ''' </summary>
            ''' <param name="Angle"> - Integer, float Or double angle value in radians (see Angle() to use radians instead of degrees)</param>
            ''' <returns></returns>
            Public Shared Function Angle(ByVal Input As Single) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(5, Input) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            Public Shared Function PadX(ByVal Input As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(6, Input) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            Public Shared Function PadY(ByVal Input As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(7, Input) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            Public Shared Function SetDefault(ByVal Input As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(11, Input) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            Public Shared Function SetDefault(ByVal Input As Single) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(17, Input) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            Public Shared Function SetDefault(ByVal Input As Double) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericFloat(17, CSng(Input)) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            Public Shared Function _Default(ByVal Input As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(11, Input) '_SageVB_kw_Font(Input)
                Return kwx
            End Function

            ''' <summary>
            ''' Set the Text Font for the current Control, Widget, Or Window.
            ''' <para></para>
            '''<list type="bullet">
            ''' <item><param name="plainInt">
            ''' When a plain integer Is used, i.e. <em><b>Font(15)</b></em>, it Is set to the default font with a size of the value,
            ''' i.e. <em>"Arial,15"</em>
            ''' </param></item>
            ''' </list>
            ''' <para></para>
            ''' <b><ul>Examples:</ul></b>
            ''' <para></para>
            ''' <para></para>
            '''<list type="bullet">
            '''<item><param name="example1"><b>Font(20)</b> - Set the font to "Arial,20".</param></item>
            ''' <item><param name="example1"><b>Font("Arial,20")</b> - Set the font to "Arial,20".
            ''' <br />- You can also add italic And bold, e.g. <em>Font("Arial,20,bold,italic")</em></param></item>
            ''' <item><param name="example1"><b>Font("MyFont")</b> - Set the font by a previously created font named <em>"MyFont"</em>
            ''' </param></item>
            ''' </list>
            ''' <para></para>
            ''' *** note: <b> Font()</b> And <b>TextFont()</b> are the same Function.
            ''' </summary>
            ''' <returns></returns>
            Public Shared Function Font(ByVal Input As String) As kw
                Dim kwx = New kw()
                Dim resourceRet = SDLL._SageVB_kw_GenericString(0, Input) '_SageVB_kw_Font(Input)
                kwx.pointer = resourceRet.pointer
                kwx.resource = resourceRet.resource ' $$ Should free resource if it already exists
                kwx.resourceType = resourceRet.type

                Return kwx
            End Function

            Public Shared Function Font(ByVal fontSize As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericInteger(12, fontSize)
                Return kwx
            End Function

            Public Shared Function NumbersOnly(Optional bNumbersOnly As Boolean = False) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericBoolean(14, bNumbersOnly)
                Return kwx
            End Function

            Public Shared Function Range(min As Integer, max As Integer)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_RangeInt(min, max)
                Return kwx
            End Function

            Public Shared Function Range(min As Single, max As Single)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_RangeFloat(min, max)
                Return kwx
            End Function

            Public Shared Function Range(min As Double, max As Double)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_RangeFloat(CSng(min), CSng(max))
                Return kwx
            End Function

            ''' <summary>
            ''' Sets the justification of an object relative to the window client area, such as text output, a text widget, 
            ''' Or other controls And some shapes.
            ''' <para></para>
            ''' <para></para>
            ''' Typically, an object Is placed at a specific Or default X,Y position.  
            ''' However, setting the justification of an object can place the object in anchored points in the window.
            ''' <para></para>
            ''' <para></para>
            ''' For example, specifying <b>kw.Just("center")</b> will center a text widget in the window, 
            ''' where <b>Just("BottomRight")</b> will place the text widget in the bottom-right area of the window.
            ''' <para></para>
            ''' <para></para>
            ''' This Is also useful for Text Widget Controls, Text output to Windows, as well as other controls And certain graphics shapes.
            ''' See TextJust() to specifically control text when using functions that can set the justificatin for an object And text
            ''' at the same time (when only text Is used, Just() And TextJust() are the same function)
            ''' <para></para>
            ''' <para></para>
            ''' note: Justification Is usually relative to the window, but can also be related to a region (if active) Or shape boundaries.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Use <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Use <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Justification Types using Text Just Type (e.g. Just("center"))</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            ''' <item><param name="Center"><b>"Center"</b> - Sets text Or object in the center of the window Or region</param></item>
            ''' <item><param name="TopLeft"><b>"TopLeft"</b> - Sets text Or object in the upper-left</param></item>
            ''' <item><param name="TopCenter"><b>"TopCenter"</b> - Sets text Or object to the top-center</param></item>
            ''' <item><param name="TopRight"><b>"TopRight"</b> - Sets text Or object to the top-right</param></item>
            ''' <item><param name="RightCenter"><b>"RightCenter"</b> - Sets text Or object to the right in the center of the window/region</param></item>
            ''' <item><param name="BottomRight"><b>"BottomRight"</b> - Sets text Or object to the bottom right</param></item>
            ''' <item><param name="BottomCenter"><b>"BottomCenter"</b> - Sets the text Or object to the bottom center of the window/region</param></item>
            ''' <item><param name="BottomLeft"><b>"BottomLeft"</b> - Sets the text Or object to the bottom-left</param></item>
            ''' <item><param name="LeftCenter"><b>"LeftCenter"</b> - Sets the text Or object to the left (x) And center (y) of the window/region</param></item>
            ''' <item><param name="CenterX"><b>"CenterX"</b> - Sets the text Or object to the horizonal center of the default Or otherwise specified vertical setting</param></item>
            ''' <item><param name="CenterY"><b>"CenterY"</b> - Sets the text Or object to the vertical center of the default Or otherwise specified horizontal setting.</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' ** note:  <b> Just()</b> And <b>TextJust()</b> are the same Function.
            ''' </summary>
            ''' <param name = "justType" > Type of justification, i.e.JustType:Center, JustType:TopRight, etc.</param>
            ''' <returns></returns>
            Public Shared Function Just(ByVal Input As String) As kw
                Dim kwx = New kw()
                Dim resourceRet = SDLL._SageVB_kw_GenericString(15, Input) '_SageVB_kw_Font(Input)
                kwx.pointer = resourceRet.pointer
                kwx.resource = resourceRet.resource ' $$ Should free resource if it already exists
                kwx.resourceType = resourceRet.type

                Return kwx
            End Function


            Public Shared Function JustBottom(Optional bJustBottom As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(0, bJustBottom)
                Return kwx   ' JustType:BottomCenter
            End Function

            Public Shared Function JustTop(Optional bJustTop As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(1, bJustTop)
                Return kwx ' JustType : TopCenter
            End Function

            Public Shared Function JustRight(Optional bJustRight As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(2, bJustRight)
                Return kwx ' JustType : RightCenter
            End Function

            Public Shared Function JustCenter(Optional bJustCenter As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(3, bJustCenter)
                Return kwx ' JustType : Center()
            End Function

            Public Shared Function JustLeft(Optional bJustLeft As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(4, bJustLeft)
                Return kwx ' JustType : LeftCenter
            End Function

            Public Shared Function JustTopLeft(Optional bJustTopLeft As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(5, bJustTopLeft)
                Return kwx ' JustType : TopLeft
            End Function

            Public Shared Function JustTopRight(Optional bJustTopRight As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(6, bJustTopRight)
                Return kwx ' JustType : TopRight
            End Function

            Public Shared Function JustTopCenter(Optional bJustTopCenter As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(7, bJustTopCenter)
                Return kwx ' JustType : TopCenter
            End Function

            Public Shared Function JustBottomLeft(Optional bJustBottomLeft As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(8, bJustBottomLeft)
                Return kwx ' JustType : BottomLeft
            End Function

            Public Shared Function JustBottomRight(Optional bJustBottomRight As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(9, bJustBottomRight)
                Return kwx ' JustType : BottomRight
            End Function

            Public Shared Function JustBottomCenter(Optional bJustBottomCenter As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(10, bJustBottomCenter)
                Return kwx ' JustType : BottomCenter
            End Function

            ''' <summary>
            ''' Sets the Control, Window, Widget Or other object to the center of the window (Or region).
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">
            ''' Use <b>PadY()</b> to set a vertical offset from this position.</param></item>
            ''' <item><param name="PadX">
            ''' Use <b>PadX()</b> to set a horizontal offset from this position.</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note : <b> Center()</b> And <b>JustCenter()</b> the same Function
            ''' </summary>
            ''' <returns></returns>
            Public Shared Function Center(Optional bCenter As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(11, bCenter)
                Return kwx ' JustType : Center()
            End Function

            ''' <summary>
            ''' Centers the text Or object in the horizontal center of the current vertical placement.
            ''' <para></para>
            ''' <para></para>
            ''' For example, with Write(), the text will be written at the current vertical (Y) value from the last output, but 
            ''' will center the text in the horizontal center of the window
            ''' </summary>
            ''' <param name="bCenterX"></param>
            ''' <returns></returns>
            Public Shared Function CenterX(Optional bCenterX As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(12, bCenterX)
                Return kwx ' JustType : CenterX()
            End Function

            Public Shared Function CenterY(Optional bCenterY As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(13, bCenterY)
                Return kwx ' JustType : CenterY()
            End Function

            Public Shared Function TextCenter(Optional bTextCenter As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(14, bTextCenter)
                Return kwx ' JustType : Center()
            End Function

            Public Shared Function TextCenterX(Optional bTextCenterX As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(15, bTextCenterX)
                Return kwx ' JustType : CenterX()
            End Function

            Public Shared Function TextCenterY(Optional bTextCenterY As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(16, bTextCenterY)
                Return kwx ' JustType : CenterY()
            End Function

            Public Shared Function JustCenterX(Optional bJustCenterX As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(15, bJustCenterX)
                Return kwx ' JustType : CenterX()
            End Function

            Public Shared Function JustCenterY(Optional bJustCenterY As Boolean = True)
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SetJustType(16, bJustCenterY)
                Return kwx   ' JustType:CenterY()
            End Function

            ' SetPos() And Variants -- Same as Pos().

            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function SetPos(x As Integer, y As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairInt(x, y)
                Return kwx
            End Function


            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function SetPos(x As Single, y As Single) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(x, y)
                Return kwx
            End Function

            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function SetPos(x As Double, y As Double) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(CSng(x), CSng(y))
                Return kwx
            End Function

            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function SetPos(position As CPoint) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(CSng(position.x), CSng(position.y))
                Return kwx
            End Function

            ' Pos() And Variants -- Same as SetPos().

            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function Pos(x As Integer, y As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairInt(x, y)
                Return kwx
            End Function


            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function Pos(x As Single, y As Single) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(x, y)
                Return kwx
            End Function

            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function Pos(x As Double, y As Double) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(CSng(x), CSng(y))
                Return kwx
            End Function

            ''' <summary>
            ''' Pos will set the x And y position of the output of an object Or text, overriding the default position.
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="PadY">Optionally add <b>PadY</b> to adjust the vertical placement from the set justification type (e.g. PadY(20) adds 20 pixels to the Y placement)</param></item>
            '''<item><param name="PadX">Optionally add <b>PadX</b> to adjust the horizontal placement from the set justification type (e.g. PadY(40) adds 40 pixels to the X placement)</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <b>Separate values Or paired Point values may be used:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            '''<item><param name="separate">Separate integer Or float values: e.g. <b>Pos(400,200)</b></param></item>
            '''<item><param name="point">Point, PointF, And Sagebox CPoint type: e.g. <b>Pos(New Point(400,200))</b> Or <b>Pos(MyPoint)</b></param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' **note: <b> kw.Pos()</b> And <b>kw.SetPos()</b> are the same Function
            ''' </summary>
            ''' <param name="x">x - x position</param>
            ''' <param name="y">y - y position</param>
            ''' <returns></returns>
            Public Shared Function Pos(position As CPoint) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_PosPairFloat(CSng(position.x), CSng(position.y))
                Return kwx
            End Function

            ' Size() And Variants -- Same as SetSize().

            Public Shared Function Size(w As Integer, h As Integer) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SizePairInt(w, h)
                Return kwx
            End Function

            Public Shared Function Size(w As Single, h As Single) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat(w, h)
                Return kwx
            End Function

            Public Shared Function Size(w As Double, h As Double) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat(CSng(w), CSng(h))
                Return kwx
            End Function

            Public Shared Function Size(in_size As CPoint) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_SizePairFloat(CSng(in_size.x), CSng(in_size.y))
                Return kwx
            End Function

            Public Shared Function UpdateNow(Optional update_now As Boolean = True) As kw
                Dim kwx = New kw()
                kwx.pointer = SDLL._SageVB_kw_GenericBoolean(16, update_now)
                Return kwx
            End Function

            ' Just for testing
            Sub New(ByRef prev As kw)
                Console.WriteLine("in copy constructor for kw")
            End Sub


        End Class


        Public Class Sage
            Public Enum AutoUpdateType
                Off = 0
                _On = 1 ' 'On' was a reserved word, so '_On' is used instead.
                Timer = 2
                Immediate = 3

            End Enum


            ' Create a Gradient color (2 colors) from Color types
            Public Shared Function GradColor(color1 As Color, color2 As Color) As CColor
                Return New CColor(color1, color2)
            End Function
            Public Shared Function GradColor(color As String) As CColor
                Return New CColor(color)
            End Function

            Public Class Slider
                Public pointer As Long
                Sub New(ByVal _pointer As Long)
                    pointer = _pointer
                End Sub

                Public Function GetPos() As Integer
                    Return SDLL._SageVB_Slider_GetPos(pointer)
                End Function
                Public Function GetPosf() As Double
                    Return SDLL._SageVB_Slider_GetPosf(pointer)
                End Function
                Public Function GetPosff() As Single
                    Return SDLL._SageVB_Slider_GetPosff(pointer)
                End Function
                Public Function Moved() As Integer
                    Return SDLL._SageVB_Slider_Moved(pointer)
                End Function
            End Class

            Public Class InputBox
                Public pointer As Long
                Sub New(ByVal _pointer As Long)
                    pointer = _pointer
                End Sub

                ''' <summary>
                ''' Returns True if a return was entered in the input Box.
                ''' <para></para>
                ''' <para></para>
                ''' Once a return Is pressed, functions such as <b>GetText()</b>, <b>GetInteger()</b>, etc. can be used to retrieve
                ''' the text Or convert the text into a value.
                ''' </summary>
                ''' <returns></returns>
                Public Function ReturnPressed() As Boolean
                    Return SDLL._SageVB_InputBox_ReturnPressed(pointer)
                End Function

                ''' <summary>
                ''' Clears any text Or values displaying in the Input Box.
                ''' </summary>
                ''' <returns></returns>
                Public Function ClearText() As Boolean
                    Return SDLL._SageVB_InputBox_ClearText(pointer)
                End Function

                ''' <summary>
                ''' Returns the Integer value of the text displaying in the Input Box.
                ''' <para></para>
                ''' <para></para>
                ''' If there Is no text Or the text starts with non-numeric values (e.g. "abcd"), 0 Is returned.
                ''' </summary>
                ''' <returns></returns>
                Public Function GetInteger() As Integer
                    Return SDLL._SageVB_InputBox_GetInteger(pointer)
                End Function

                ''' <summary>
                ''' Returns the text displaying in the Input Box as a string.
                ''' <para></para>
                ''' <para></para>
                ''' If there Is no text displaying in the input Box a string with empty contents Is returned, e.g. ""
                ''' </summary>
                ''' <returns></returns>
                Public Function GetText() As String
                    Dim g As String = Marshal.PtrToStringAnsi(SDLL._SageVB_InputBox_GetText(pointer))
                    Return SDLL._SageVB_InputBox_GetInteger(pointer)
                End Function

            End Class


            Shared kMaxPointStructValue As Integer = 250

            Public Class Window
                ''private point_transfer as point_struct[] = new point_struct[kMaxPointStructValue] 'Used to transfer raw point arrays to graphics functions, e.g. DrawPolygon()
                Private point_transfer(kMaxPointStructValue) As point_struct

                Public pointer As Long
                Sub New(ByVal _pointer As Long)
                    pointer = _pointer
                End Sub

                '''<summary>
                '''Write Text out to the window. 
                '''<para></para>
                '''<para></para>
                '''The write() function will write text to the window and can be used with many options.
                '''<para></para>
                '''<para></para>
                '''A basic example is: <b>>window.write("Hello World")</b>, <b>window.write("Hello World")</b>,
                '''or <b>window.write(<em>$"My Variable is: {MyVariable}"</em></b>) 
                '''<para></para>
                '''<para></para>
                '''Sagebox keywords (e.g. <em><b>kw.Font(100)</b></em> options can be included.  Some various options are as follows:
                '''<para></para>
                '''<para></para>
                ''' <list type="bullet">
                '''<item><param name="Font"><b>Font</b> - Set the font to be used for the text</param></item>
                '''<item><param name="Center,CenterX,CenterY"><b>Center,CenterX,CenterY</b> - Center the text in various ways 
                ''' (i.e. CenterX centers in the X-axis, etc.)</param></item>
                '''<item><param name="TextColor,fgColor"><b>fgColor or TextColor</b> - Set the text color for the text.</param></item>
                '''<item><param name="bgColor"><b>bgColor</b> - Set the background color for the text</param></item>
                '''<item><param name="Angle"><b>Angle</b> - Set the rotational angle of the text</param></item>
                '''<item><param name="Opacity"><b>Opacity</b> - Set the opacity of the text</param></item>
                '''<item><param name="Pos"><b>Pos</b> - Set the position in the window of the text, 
                '''e.g. Write("Hello World",kw.pos(x,y))</param></item>
                '''</list>
                '''<para></para>
                '''<para></para>
                '''Controls can be embedded in the text line to change colors and font sizes:
                '''<para></para>
                '''<para></para>
                '''For example:
                '''<list type="bullet">
                '''<item><param name="Font"><b>window.write("This <em>{r}word{}</em> is in the color red").</b>
                ''' <br />   
                '''    - Note the {} to close.  
                ''' <br />   
                '''    - With C# formatted strings, an extra "{}" is needed, such as <em>"MyValue {{r}}{myvalue}{{}} is in red".</em>
                ''' </param></item>
                '''</list>
                '''<para></para>
                '''<para></para>
                '''More than one control can be used such as: <b>win.write("This is in <em>{r}Red{}</em> and this is in 
                '''<em>{b}Blue</em>)".</b> 
                '''<br />
                '''You do not need the closing {} if it is as the end of the line.
                '''<para></para>
                '''<para></para>
                '''<b><ul>Some Curly-Brace Embedded Controls</ul></b>
                '''<list type="bullet">
                '''<item><param name="Color"><b>{&lt;color&gt;}</b> - Where the color is a defined color such as {red} (or {r}), 
                '''{blue}, {skyblue}, etc. e.g. <em>"this {blue}word{} is in blue"</em> ({} closes the color or font)
                '''<br />
                '''You can use abbreviations for most primary colors, such as <em>{y} = {yellow}</em>, etc.
                '''</param></item>                       
                '''<item><param name="FontSize"><b>{font size}</b> - e.g. <em>"This is in the normal font, and 
                '''                      {30}this is in a 30-point font"</em></param></item>
                '''<item><param name="FontName"><b>{font name}</b> - e.g. <em>"This is in the normal font, and 
                '''                             {Courier New,20}This is in a 20-point Courier New font"</em></param></item>
                '''<item><param name="SetXPos"><b>{x = &lt;value&gt;}</b> - Sets the X position on the current line,
                ''' e.g. <em>"This value{x=100}starts at position 100 pixels from the left."</em>
                ''' <br />This is useful in aligning data values with titling to the left, since typical fonts have varied sizes for each printed letter/numeral vs. static sizes.</param></item>
                '''</list>
                '''<para></para>
                '''<para></para>
                '''note: <em>when using <b>kw.Angle()</b> and <b>kw.Opacity()</b>, embedded options no longer work, 
                '''such as <b>{font}</b>, <b>{color}</b> etc.
                '''<br />
                ''' - e.g. rather than "{50}This is a big font", when using Angle() and/or Opacity() options, use write("This is a big font",kw.Font(50)) instead.</em>
                '''<para></para>
                '''<para></para>
                '''<b>Examples:</b>
                '''<para></para>
                '''<para></para>
                '''<list type="bullet">
                '''<item><param name="Example1"><b>MyWindow.write("Hello World",kw.Font(40),kw.Center())</b>
                '''<br />- Writes a big "Hello World" in the center of the screen<br /></param></item>
                ''' <item><param name="Example1"><b>MyWindow.write("Hello World",kw.fgColor("red")</b>
                '''<br />- Writes "Hello World" in red<br /></param></item>
                '''     
                '''<item><param name="Example1"><b>MyWindow.write("{r}Hello World")</b>
                '''<br />- Also writes "Hello World" in red<br /></param></item>
                '''     
                '''<item><param name="Example1"><b>MyWindow.write("Hello World",kw::Font(50))</b>
                '''<br />- Writes "Hello World" in a 50-point font size.<br /></param></item>
                '''     
                '''<item><param name="Example1"><b>MyWindow.write("{50}Hello World")</b>
                '''<br />- Also writes "Hello World" in a 50-point font size.<br /></param></item>
                '''</list>
                ''' </summary>
                ''' <param name="message"></param>
                ''' <param name="keywords"></param>
                ''' <returns></returns>
                Public Function Write(Message As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Dim p As Long = 0
                    If (keywords IsNot Nothing) Then p = keywords.pointer
                    If (p) Then SDLL._SageVB_kw_EvalObject(p)
                    Return SDLL._SageVB_Write(Message, pointer, p)
                End Function

                ' ====> Circle Functions  (x,y & point  & pointF -> Color, String, CColor : 9 prototypes for each function)

                ' -> x,y versions (String,Color, CColor)

                Public Function DrawCircle(x As Single, y As Single, Radius As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, Radius, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawCircle(x As Single, y As Single, Radius As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, Radius, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawCircle(x As Single, y As Single, Radius As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, Radius, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function


                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function DrawCircle(p As Point, Radius As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function DrawCircle(p As Point, Radius As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function DrawCircle(p As Point, Radius As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawCircle(p As PointF, Radius As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function DrawCircle(p As PointF, Radius As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function DrawCircle(p As PointF, Radius As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function



                ' Fill Circle


                ' -> x,y versions (String,Color, CColor)

                Public Function FillCircle(x As Single, y As Single, Radius As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, Radius, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillCircle(x As Single, y As Single, Radius As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, Radius, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillCircle(x As Single, y As Single, Radius As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, x, y, Radius, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function FillCircle(p As Point, Radius As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function FillCircle(p As Point, Radius As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function FillCircle(p As Point, Radius As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillCircle(p As PointF, Radius As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function FillCircle(p As PointF, Radius As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function
                Public Function FillCircle(p As PointF, Radius As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawCircle_Rgb(pointer, p.X, p.Y, Radius, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' ====> Ellipse Functions  (x,y & point  & pointF -> Color, String, CColor : 9 prototypes for each function)

                ' -> x,y versions (String,Color, CColor)

                Public Function DrawEllipse(x As Single, y As Single, xwidth As Single, yheight As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth, yheight, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawEllipse(x As Single, y As Single, xwidth As Single, yheight As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth, yheight, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawEllipse(x As Single, y As Single, xwidth As Single, yheight As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth, yheight, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function DrawEllipse(p As Point, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawEllipse(p As Point, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawEllipse(p As Point, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawEllipse(p As PointF, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawEllipse(p As PointF, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawEllipse(p As PointF, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -- Fill Ellipse

                Public Function FillEllipse(x As Single, y As Single, xwidth As Single, ywidth As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth, ywidth, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillEllipse(x As Single, y As Single, xwidth As Single, ywidth As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth, ywidth, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillEllipse(x As Single, y As Single, xwidth As Single, ywidth As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, x, y, xwidth, ywidth, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function FillEllipse(p As Point, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillEllipse(p As Point, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillEllipse(p As Point, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillEllipse(p As PointF, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillEllipse(p As PointF, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillEllipse(p As PointF, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawEllipse_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function


                ' ====> Rectangle Functions  (x,y & point  & pointF -> Color, String, CColor : 9 prototypes for each function)

                ' -> x,y versions (String,Color, CColor)

                Public Function DrawRectangle(x As Single, y As Single, width As Single, height As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, width, height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawRectangle(x As Single, y As Single, width As Single, height As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, width, height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawRectangle(x As Single, y As Single, width As Single, height As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, width, height, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function DrawRectangle(p As Point, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawRectangle(p As Point, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawRectangle(p As Point, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawRectangle(p As PointF, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawRectangle(p As PointF, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawRectangle(p As PointF, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -- Fill Rectangle

                Public Function FillRectangle(x As Single, y As Single, width As Single, height As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, width, height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillRectangle(x As Single, y As Single, width As Single, height As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, width, height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillRectangle(x As Single, y As Single, width As Single, height As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, x, y, width, height, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function FillRectangle(p As Point, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillRectangle(p As Point, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillRectangle(p As Point, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillRectangle(p As PointF, size As Size, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillRectangle(p As PointF, size As Size, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, New CColor(color).colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillRectangle(p As PointF, size As Size, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawRectangle_Rgb(pointer, p.X, p.Y, size.Width, size.Height, color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' ====> Triangle Functions  (x,y & point  & pointF -> Color, String, CColor : 9 prototypes for each function)

                ' -> x,y versions (String,Color, CColor)

                Public Function DrawTriangle(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawTriangle(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, color.colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function DrawTriangle(p1 As Point, p2 As Point, p3 As Point, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As Point, p2 As Point, p3 As Point, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As Point, p2 As Point, p3 As Point, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As PointF, p2 As PointF, p3 As PointF, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As PointF, p2 As PointF, p3 As PointF, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As PointF, p2 As PointF, p3 As PointF, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As CPoint, p2 As CPoint, p3 As CPoint, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As CPoint, p2 As CPoint, p3 As CPoint, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, New CColor(color).colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangle(p1 As CPoint, p2 As CPoint, p3 As CPoint, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.colors, False, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' --- Fill Triangle

                ' -> x,y versions (String,Color, CColor)

                Public Function FillTriangle(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillTriangle(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, color.colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function FillTriangle(p1 As Point, p2 As Point, p3 As Point, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As Point, p2 As Point, p3 As Point, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As Point, p2 As Point, p3 As Point, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As PointF, p2 As PointF, p3 As PointF, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As PointF, p2 As PointF, p3 As PointF, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As PointF, p2 As PointF, p3 As PointF, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As CPoint, p2 As CPoint, p3 As CPoint, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As CPoint, p2 As CPoint, p3 As CPoint, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, New CColor(color).colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangle(p1 As CPoint, p2 As CPoint, p3 As CPoint, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.colors, True, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' ====> Fast Triangle Functions  (x,y & point  & pointF -> Color, String, CColor : 9 prototypes for each function)

                ' -> x,y versions (String,Color, CColor)

                Public Function DrawTriangleFast(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangleFast(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function DrawTriangleFast(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, color.colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function DrawTriangleFast(p1 As Point, p2 As Point, p3 As Point, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangleFast(p1 As Point, p2 As Point, p3 As Point, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangleFast(p1 As Point, p2 As Point, p3 As Point, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangleFast(p1 As PointF, p2 As PointF, p3 As PointF, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangleFast(p1 As PointF, p2 As PointF, p3 As PointF, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function DrawTriangleFast(p1 As PointF, p2 As PointF, p3 As PointF, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, False, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' --- Fill Triangle Fast

                ' -> x,y versions (String,Color, CColor)

                Public Function FillTriangleFast(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangleFast(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, New CColor(color).colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' CColor version for use of symbolic gradients, e.g. new CColor(Color.Red,Color.Blue)
                Public Function FillTriangleFast(x1 As Single, y1 As Single, x2 As Single, y2 As Single, x3 As Single, y3 As Single, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, x1, y1, x2, y2, x3, y3, color.colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' -> Point & PointF versions ($$ for prototype purposes, until something like implicit constructors can be used)

                Public Function FillTriangleFast(p1 As Point, p2 As Point, p3 As Point, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangleFast(p1 As Point, p2 As Point, p3 As Point, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangleFast(p1 As Point, p2 As Point, p3 As Point, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangleFast(p1 As PointF, p2 As PointF, p3 As PointF, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangleFast(p1 As PointF, p2 As PointF, p3 As PointF, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, New CColor(color).colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                Public Function FillTriangleFast(p1 As PointF, p2 As PointF, p3 As PointF, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return SDLL._SageVB_Window_DrawTriangle_Rgb(pointer, p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, color.colors, True, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' ====> Polygon Functions (Normal,Filled -> Color, String, and CColor values -> Point and PointF : 6 total prototypes)

                Public Function DrawPolygon(ByRef points() As Point, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Dim length As Integer = points.Length

                    ' Use static point buffer so we don't have to reallocate.  If it's too small, then allocate a new one and is it as a temporary

                    Dim pout = point_transfer
                    Dim ptemp() As point_struct

                    If length > kMaxPointStructValue Then
                        ReDim ptemp(length)
                        pout = ptemp
                    End If

                    For i As Integer = 0 To length - 1
                        pout(i).x = points(i).X
                        pout(i).y = points(i).Y
                    Next
                    Return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer, length, pout(0), color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' For prototype purposes, DrawPolygon with other types call the main Polygon function indirectly (to shorten code)

                Public Function DrawPolygon(ByRef points() As Point, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return DrawPolygon(points, New CColor(color), keywords)
                End Function

                Public Function DrawPolygon(ByRef points() As Point, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return DrawPolygon(points, New CColor(color), keywords)
                End Function

                ' PointF versions

                Public Function DrawPolygon(ByRef points() As PointF, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Dim length As Integer = points.Length

                    ' Use static point buffer so we don't have to reallocate.  If it's too small, then allocate a new one and is it as a temporary

                    Dim pout = point_transfer
                    Dim ptemp() As point_struct

                    If length > kMaxPointStructValue Then
                        ReDim ptemp(length)
                        pout = ptemp
                    End If

                    For i As Integer = 0 To length - 1
                        pout(i).x = points(i).X
                        pout(i).y = points(i).Y
                    Next
                    Return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer, length, pout(0), color.colors, False, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' For prototype purposes, DrawPolygon with other types call the main Polygon function indirectly (to shorten code)

                Public Function DrawPolygon(ByRef points() As PointF, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return DrawPolygon(points, New CColor(color), keywords)
                End Function

                Public Function DrawPolygon(ByRef points() As PointF, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return DrawPolygon(points, New CColor(color), keywords)
                End Function

                '--- FillPolygon


                Public Function FillPolygon(ByRef points() As Point, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Dim length As Integer = points.Length

                    ' Use static point buffer so we don't have to reallocate.  If it's too small, then allocate a new one and is it as a temporary

                    Dim pout = point_transfer
                    Dim ptemp() As point_struct

                    If length > kMaxPointStructValue Then
                        ReDim ptemp(length)
                        pout = ptemp
                    End If

                    For i As Integer = 0 To length - 1
                        pout(i).x = points(i).X
                        pout(i).y = points(i).Y
                    Next
                    Return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer, length, pout(0), color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' For prototype purposes, DrawPolygon with other types call the main Polygon function indirectly (to shorten code)

                Public Function FillPolygon(ByRef points() As Point, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return FillPolygon(points, New CColor(color), keywords)
                End Function

                Public Function FillPolygon(ByRef points() As Point, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return FillPolygon(points, New CColor(color), keywords)
                End Function

                ' PointF versions

                Public Function FillPolygon(ByRef points() As PointF, color As CColor, Optional ByRef keywords As kw = Nothing) As Boolean
                    Dim length As Integer = points.Length

                    ' Use static point buffer so we don't have to reallocate.  If it's too small, then allocate a new one and is it as a temporary

                    Dim pout = point_transfer
                    Dim ptemp() As point_struct

                    If length > kMaxPointStructValue Then
                        ReDim ptemp(length)
                        pout = ptemp
                    End If

                    For i As Integer = 0 To length - 1
                        pout(i).x = points(i).X
                        pout(i).y = points(i).Y
                    Next
                    Return SDLL._SageVB_Window_DrawPolygon_Rgb(pointer, length, pout(0), color.colors, True, If(keywords Is Nothing, 0, keywords.pointer))
                End Function

                ' For prototype purposes, DrawPolygon with other types call the main Polygon function indirectly (to shorten code)

                Public Function FillPolygon(ByRef points() As PointF, color As Color, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return FillPolygon(points, New CColor(color), keywords)
                End Function

                Public Function FillPolygon(ByRef points() As PointF, color As String, Optional ByRef keywords As kw = Nothing) As Boolean
                    Return FillPolygon(points, New CColor(color), keywords)
                End Function

                Public Function DrawPixel(x As Integer, y As Integer, ByRef color As Color) As Boolean
                    Return SDLL._SageVB_Window_SetPixel_Rgb(pointer, x, y, color.R, color.G, color.B)
                End Function

                Public Function SetPixel(x As Integer, y As Integer, ByRef color As Color) As Boolean
                    Return SDLL._SageVB_Window_SetPixel_Rgb(pointer, x, y, color.R, color.G, color.B)
                End Function



                Public Function Cls(Optional Color1 As Color = Nothing, Optional Color2 As Color = Nothing) As Boolean
                    Dim red1 As Integer = -1
                    Dim green1 As Integer = -1
                    Dim blue1 As Integer = -1

                    Dim red2 As Integer = -1
                    Dim green2 As Integer = -1
                    Dim blue2 As Integer = -1

                    '                    If Color2 IIs Nothing Then Color2 = SageColor._Undefined
                    '                    If Color1 Is Nothing Then Color1 = SageColor._Undefined

                    If Not Color2.IsEmpty Then
                        red2 = Color2.R
                        green2 = Color2.G
                        blue2 = Color2.B
                    End If
                    If Not Color1.IsEmpty Then
                        red2 = Color1.R
                        green2 = Color1.G
                        blue2 = Color1.B
                    End If
                    Return SDLL._SageVB_Window_Cls(pointer, red1, green1, blue1, red2, green2, blue2, False)

                End Function

                Public Function Cls(Color As String) As Boolean
                    Return SDLL._SageVB_Window_ClsStr(pointer, Color, False)
                End Function


                Public Function ClsRadial(Optional Color1 As Color = Nothing, Optional Color2 As Color = Nothing) As Boolean
                    Dim red1 As Integer = -1
                    Dim green1 As Integer = -1
                    Dim blue1 As Integer = -1

                    Dim red2 As Integer = -1
                    Dim green2 As Integer = -1
                    Dim blue2 As Integer = -1

                    '                    If Color2 IIs Nothing Then Color2 = SageColor._Undefined
                    '                    If Color1 Is Nothing Then Color1 = SageColor._Undefined

                    If Not Color2.IsEmpty Then
                        red2 = Color2.R
                        green2 = Color2.G
                        blue2 = Color2.B
                    End If
                    If Not Color1.IsEmpty Then
                        red2 = Color1.R
                        green2 = Color1.G
                        blue2 = Color1.B
                    End If
                    Return SDLL._SageVB_Window_Cls(pointer, red1, green1, blue1, red2, green2, blue2, True)

                End Function

                Public Function ClsRadial(Color As String) As Boolean
                    Return SDLL._SageVB_Window_ClsStr(pointer, Color, True)
                End Function

                Public Function GetWindowSize() As CPoint
                    Dim x As Single = 0
                    Dim y As Single = 0
                    SDLL._SageVB_Window_GetWindowSize(pointer, x, y)
                    Return New CPoint(x, y)
                End Function

                Public Function GetWindowCenter() As CPoint
                    Dim x As Single = 0
                    Dim y As Single = 0
                    SDLL._SageVB_Window_GetWindowCenter(pointer, x, y)
                    Return New CPoint(x, y)
                End Function

                Public Function SetAutoUpdate(eUpdateType As AutoUpdateType) As Boolean
                    Return SDLL._SageVB_Window_SetAutoUpdateInt(pointer, CInt(eUpdateType))
                End Function

                Public Function SetAutoUpdate(sUpdateType As String) As Boolean
                    Return SDLL._SageVB_Window_SetAutoUpdateStr(pointer, sUpdateType)
                End Function

            End Class


            ' ------------------------
            ' Sagebox Global Functions
            ' ------------------------

            ''' <summary>
            ''' Brings up an input box that informs the user the program has ended.
            ''' <para></para>
            ''' An Ok button Is displayed that will continue program execution once pressed.
            ''' <para></para>
            ''' <para></para>
            ''' The ExitButton() function Is useful to let the user know the program has ended, And also to
            ''' <para></para>
            ''' keep the program from returning to the operating system, which will close down all Sagebox windows.
            ''' <para></para>
            ''' <para></para>
            ''' <b>Parameters:</b>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            ''' <item><param name="message"><b>Message</b> - <em>[optional]</em> text to display in the input box.
            ''' <br />
            ''' - Just using <b>ExitButton()</b> brings up a default message informing the user the program has completed.
            ''' <br />
            ''' - Using a specific message will change the displayed message, such as <b>ExitButton("Personalized Message Goes Here");</b>
            ''' </param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            ''' <item><param name="return">ExitButton() returns 0, allowing using statements such as 
            ''' <em><b>return ExitButton();</b></em></param></item>
            ''' </list>
            ''' </summary>
            ''' <param name="message"></param>
            ''' <returns></returns>            
            Public Shared Sub ExitButton(Optional Message As String = Nothing, Optional ByRef keywords As kw = Nothing)
                SDLL._SageVB_ExitButton(If(Message Is Nothing, "", Message))
            End Sub

            <MethodImplAttribute(MethodImplOptions.AggressiveInlining)>
            Public Shared Function GetEvent() As Boolean
                Return SDLL._SageVB_GetEvent()
            End Function

            ''' <summary>
            ''' NewWindow -- Create a New popup window.
            ''' <para></para>
            ''' <para></para>
            ''' Creates a New window on the desktop And returns a window object where you can perform window-based functions on that window.
            ''' <para></para>
            ''' <para></para>
            ''' Parameters:
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            ''' <item><param name="title"><b>title</b> - [optional] Title of window (in title bar)</param></item>
            ''' <item><param name="keywords"><b>keywords</b> - [optional] optional keywords, e.g. Size(600,500), bgColor(&quot;black&quot;), etc.</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' Keywords And Pybox options can be included.  Some various options are as follows:
            ''' <para></para>
            ''' <list type="bullet">
            ''' <item><param name="title"><b>Font</b> - Sets the default font for the window, e.g. &quot;Font(40)&quot; Or Font(&quot;Arial,40&quot;)</param></item>
            ''' <item><param name="title"><b>bgColor</b> - Sets the background color of the window (e.g. bgColor(&quot;Red&quot;))</param></item>
            ''' <item><param name="title"><b>bgGradient</b> - Sets a gradient background color of the window. 
            '''                   Such as bgGradient("black,blue").  
            '''                   You can also use Cls() to clear the window.</param></item>
            ''' <item><param name="title"><b>TextColor</b> - Sets the text/foreground color of the window</param></item>
            ''' <item><param name="title"><b>NoAutoUpdate</b> - Tells the window to Not update automatically when
            '''                   graphics are placed in the window.  
            '''                   The program must call Update() itself to update the window contents.  
            '''                   This can prevent flashing in real-time drawing.</param></item>
            ''' <item><param name="title"><b>ResizeOk</b> - Allows the window to be resized by the user. </param></item>
            ''' <item><param name="title"><b>Size</b> - Sets the size of the window.  
            '''                   Usually the default Is 1200x800, e.g. Size(1200,800)</param></item>
            ''' <item><param name="title"><b>Pos</b> - Sets the position of the window.  
            '''                   e.g. Pos(200,400). The Window Is otherwise automatically placed</param></item>
            ''' </list>
            ''' <para></para>
            ''' <para></para>
            ''' Examples:
            ''' <para></para>
            ''' <para></para>
            ''' <list type="bullet">
            ''' <item><param name="example1"><b>myWindow = new_window()
            ''' <br />mWindow.write(&quot;Hello World\n&quot;)</b>
            ''' <br />- Creates a New window And writes "Hello World" to it.</param></item>
            ''' <item><param name="example3"><b>myWindow = new_window("This is the Title",kw.Size(500,200),kw.Pos(50,100),kw.bgColor("Blue"))</b>
            ''' <br> - This sets the upper title of the window, the size to (500,20), the Position to (50,100) on the desktop, And sets the background color to blue</br></param></item>
            ''' <item><param name="example4"><b>myWindow = new_window(kw.bgColor(&quot;Blue&quot;))</b>
            ''' <br /> - This just sets the background color to blue. The Position, Size, And Window Title are set to defaults</param></item>
            ''' </list>
            ''' </summary>
            ''' <param name="title"> - [optional] title of the window.  kw.Title() can be used as a keyword instead.</param>
            ''' <param name="keywords"> - [optional] keyword controls the the window, e.g. kw.bgColor(&quot;black&quot;)</param>
            ''' <returns></returns>            
            Public Shared Function NewWindow(Optional ByRef keywords As kw = Nothing) As Window
                Return New Window(SDLL._SageVB_NewWindow(If(keywords Is Nothing, 0, keywords.pointer)))
            End Function

            '---> Dev Window Functions

            Public Shared Function DevSlider(title As String, Optional ByRef keywords As kw = Nothing) As Slider
                Dim pKeys As Long = 0
                If (keywords IsNot Nothing) Then pKeys = keywords.pointer
                Return New Slider(SDLL._SageVB_DevSlider(title, pKeys, False))
            End Function
            Public Shared Function DevSliderf(title As String, Optional ByRef keywords As kw = Nothing) As Slider
                Dim pKeys As Long = 0
                If (keywords IsNot Nothing) Then pKeys = keywords.pointer
                Return New Slider(SDLL._SageVB_DevSlider(title, pKeys, True))
            End Function

            Public Shared Function DevText(Optional text As String = "", Optional ByRef keywords As kw = Nothing) As Window
                Dim pKeys As Long = 0
                If (keywords IsNot Nothing) Then pKeys = keywords.pointer
                Return New Window(SDLL._SageVB_DevText(0, text, pKeys))
            End Function

            Public Shared Function DevText(height As Integer, Optional ByRef keywords As kw = Nothing) As Window
                Dim pKeys As Long = 0
                If (keywords IsNot Nothing) Then pKeys = keywords.pointer
                Return New Window(SDLL._SageVB_DevText(height, "", pKeys))
            End Function

            Public Shared Function DevText(height As Integer, text As String, Optional ByRef keywords As kw = Nothing) As Window
                Dim pKeys As Long = 0
                If (keywords IsNot Nothing) Then pKeys = keywords.pointer
                Return New Window(SDLL._SageVB_DevText(height, text, pKeys))
            End Function

            Public Shared Function DevInputBox(title As String, Optional ByRef keywords As kw = Nothing) As InputBox
                Dim pKeys As Long = 0
                If (keywords IsNot Nothing) Then pKeys = keywords.pointer
                Return New InputBox(SDLL._SageVB_DevInputBox(title, pKeys))
            End Function


            Public Shared Function SetIniPath(iniPath As String) As Boolean
                Return SDLL._SageVB_SetIniFilePath(iniPath)
            End Function

            Public Shared Function SetDebugLevel(level As String)
                Return SDLL._SageVB_SetLogLevelStr(level)
            End Function

            Public Shared Function DebugWrite(message As String) As Boolean
                Return SDLL._SageVB_DebugWrite(message, False)
            End Function

            Public Shared Function DebugWriteln(message As String) As Boolean
                Return SDLL._SageVB_DebugWrite(message, True)
            End Function

            Public Shared Sub DisableDefaults()
                SDLL._SageVB_DisableDefaults()
            End Sub

            Public Shared Sub DisplayDefaultPaths()
                SDLL._SageVB_DisplayDefaultPaths()
            End Sub


            ' Set here so we can specify a long-DLL name just once for the _SageVB_Init() Lib function and use just "sagebox.dll" afterward.
            Shared Sub New()
                SDLL._SageVB_Init(123, System.IO.Path.GetFullPath("..\..\..\"))
                LogVInfo("kw::New", "Executed Sage Class Constructor.")
            End Sub
        End Class

        ' Set here so we can specify a long-DLL name just once for the _SageVB_Init() Lib function and use just "sagebox.dll" afterward.
        Sub New()
            '   LogVInfo("Sagebox::New", "in Sagebox.New()")
            SDLL._SageVB_Init(123, System.IO.Path.GetFullPath("..\..\..\"))
            LogVInfo("Sagebox::New", "Executed Sagebox Module Constructor.")
        End Sub
        Sub Finalize() ' destructor
            LogVInfo("Sagebox::Finalize", "Executed Sagebox Module Finalize.")
        End Sub

    End Module

End Namespace
