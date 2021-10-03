#pragma once

namespace Sage
{

    // Note: Original format with inline moved out to keep goal of C++11 compatibility with the library (for now anyway, since there 
    //       are quite a few reasons to move into at least C++17)
    class PanColor
    {
    public:
        static RgbColor  AliceBlue            ;//=  _ccc(0xFFF0F8FF);
        static RgbColor  AntiqueWhite         ;//=  _ccc(0xFFFAEBD7);
        static RgbColor  Aqua                 ;//=  _ccc(0xFF00FFFF);
        static RgbColor  Aquamarine           ;//=  _ccc(0xFF7FFFD4);
        static RgbColor  Azure                ;//=  _ccc(0xFFF0FFFF);
        static RgbColor  Beige                ;//=  _ccc(0xFFF5F5DC);
        static RgbColor  Bisque               ;//=  _ccc(0xFFFFE4C4);
        static RgbColor  Black                ;//=  _ccc(0xFF000000);
        static RgbColor  BlanchedAlmond       ;//=  _ccc(0xFFFFEBCD);
        static RgbColor  Blue                 ;//=  _ccc(0xFF0000FF);
        static RgbColor  BlueViolet           ;//=  _ccc(0xFF8A2BE2);
        static RgbColor  Brown                ;//=  _ccc(0xFFA52A2A);
        static RgbColor  BurlyWood            ;//=  _ccc(0xFFDEB887);
        static RgbColor  CadetBlue            ;//=  _ccc(0xFF5F9EA0);
        static RgbColor  Chartreuse           ;//=  _ccc(0xFF7FFF00);
        static RgbColor  Chocolate            ;//=  _ccc(0xFFD2691E);
        static RgbColor  Coral                ;//=  _ccc(0xFFFF7F50);
        static RgbColor  CornflowerBlue       ;//=  _ccc(0xFF6495ED);
        static RgbColor  Cornsilk             ;//=  _ccc(0xFFFFF8DC);
        static RgbColor  Crimson              ;//=  _ccc(0xFFDC143C);
        static RgbColor  Cyan                 ;//=  _ccc(0xFF00FFFF);
        static RgbColor  DarkBlue             ;//=  _ccc(0xFF00008B);
        static RgbColor  DarkCyan             ;//=  _ccc(0xFF008B8B);
        static RgbColor  DarkGoldenrod        ;//=  _ccc(0xFFB8860B);
        static RgbColor  DarkGray             ;//=  _ccc(0xFFA9A9A9);
        static RgbColor  DarkGreen            ;//=  _ccc(0xFF006400);
        static RgbColor  DarkKhaki            ;//=  _ccc(0xFFBDB76B);
        static RgbColor  DarkMagenta          ;//=  _ccc(0xFF8B008B);
        static RgbColor  DarkOliveGreen       ;//=  _ccc(0xFF556B2F);
        static RgbColor  DarkOrange           ;//=  _ccc(0xFFFF8C00);
        static RgbColor  DarkOrchid           ;//=  _ccc(0xFF9932CC);
        static RgbColor  DarkRed              ;//=  _ccc(0xFF8B0000);
        static RgbColor  DarkSalmon           ;//=  _ccc(0xFFE9967A);
        static RgbColor  DarkSeaGreen         ;//=  _ccc(0xFF8FBC8B);
        static RgbColor  DarkSlateBlue        ;//=  _ccc(0xFF483D8B);
        static RgbColor  DarkSlateGray        ;//=  _ccc(0xFF2F4F4F);
        static RgbColor  DarkTurquoise        ;//=  _ccc(0xFF00CED1);
        static RgbColor  DarkViolet           ;//=  _ccc(0xFF9400D3);
        static RgbColor  DeepPink             ;//=  _ccc(0xFFFF1493);
        static RgbColor  DeepSkyBlue          ;//=  _ccc(0xFF00BFFF);
        static RgbColor  DimGray              ;//=  _ccc(0xFF696969);
        static RgbColor  DodgerBlue           ;//=  _ccc(0xFF1E90FF);
        static RgbColor  Firebrick            ;//=  _ccc(0xFFB22222);
        static RgbColor  FloralWhite          ;//=  _ccc(0xFFFFFAF0);
        static RgbColor  ForestGreen          ;//=  _ccc(0xFF228B22);
        static RgbColor  Fuchsia              ;//=  _ccc(0xFFFF00FF);
        static RgbColor  Gainsboro            ;//=  _ccc(0xFFDCDCDC);
        static RgbColor  GhostWhite           ;//=  _ccc(0xFFF8F8FF);
        static RgbColor  Gold                 ;//=  _ccc(0xFFFFD700);
        static RgbColor  Goldenrod            ;//=  _ccc(0xFFDAA520);
        static RgbColor  Gray                 ;//=  _ccc(0xFF808080);
        static RgbColor  Green                ;//=  _ccc(0xFF008000);
        static RgbColor  GreenYellow          ;//=  _ccc(0xFFADFF2F);
        static RgbColor  Honeydew             ;//=  _ccc(0xFFF0FFF0);
        static RgbColor  HotPink              ;//=  _ccc(0xFFFF69B4);
        static RgbColor  IndianRed            ;//=  _ccc(0xFFCD5C5C);
        static RgbColor  Indigo               ;//=  _ccc(0xFF4B0082);
        static RgbColor  Ivory                ;//=  _ccc(0xFFFFFFF0);
        static RgbColor  Khaki                ;//=  _ccc(0xFFF0E68C);
        static RgbColor  Lavender             ;//=  _ccc(0xFFE6E6FA);
        static RgbColor  LavenderBlush        ;//=  _ccc(0xFFFFF0F5);
        static RgbColor  LawnGreen            ;//=  _ccc(0xFF7CFC00);
        static RgbColor  LemonChiffon         ;//=  _ccc(0xFFFFFACD);
        static RgbColor  LightBlue            ;//=  _ccc(0xFFADD8E6);
        static RgbColor  LightCoral           ;//=  _ccc(0xFFF08080);
        static RgbColor  LightCyan            ;//=  _ccc(0xFFE0FFFF);
        static RgbColor  LightGoldenrodYellow ;//=  _ccc(0xFFFAFAD2);
        static RgbColor  LightGray            ;//=  _ccc(0xFFD3D3D3);
        static RgbColor  LightGreen           ;//=  _ccc(0xFF90EE90);
        static RgbColor  LightPink            ;//=  _ccc(0xFFFFB6C1);
        static RgbColor  LightSalmon          ;//=  _ccc(0xFFFFA07A);
        static RgbColor  LightSeaGreen        ;//=  _ccc(0xFF20B2AA);
        static RgbColor  LightSkyBlue         ;//=  _ccc(0xFF87CEFA);
        static RgbColor  LightSlateGray       ;//=  _ccc(0xFF778899);
        static RgbColor  LightSteelBlue       ;//=  _ccc(0xFFB0C4DE);
        static RgbColor  LightYellow          ;//=  _ccc(0xFFFFFFE0);
        static RgbColor  Lime                 ;//=  _ccc(0xFF00FF00);
        static RgbColor  LimeGreen            ;//=  _ccc(0xFF32CD32);
        static RgbColor  Linen                ;//=  _ccc(0xFFFAF0E6);
        static RgbColor  Magenta              ;//=  _ccc(0xFFFF00FF);
        static RgbColor  Maroon               ;//=  _ccc(0xFF800000);
        static RgbColor  MediumAquamarine     ;//=  _ccc(0xFF66CDAA);
        static RgbColor  MediumBlue           ;//=  _ccc(0xFF0000CD);
        static RgbColor  MediumOrchid         ;//=  _ccc(0xFFBA55D3);
        static RgbColor  MediumPurple         ;//=  _ccc(0xFF9370DB);
        static RgbColor  MediumSeaGreen       ;//=  _ccc(0xFF3CB371);
        static RgbColor  MediumSlateBlue      ;//=  _ccc(0xFF7B68EE);
        static RgbColor  MediumSpringGreen    ;//=  _ccc(0xFF00FA9A);
        static RgbColor  MediumTurquoise      ;//=  _ccc(0xFF48D1CC);
        static RgbColor  MediumVioletRed      ;//=  _ccc(0xFFC71585);
        static RgbColor  MidnightBlue         ;//=  _ccc(0xFF191970);
        static RgbColor  MintCream            ;//=  _ccc(0xFFF5FFFA);
        static RgbColor  MistyRose            ;//=  _ccc(0xFFFFE4E1);
        static RgbColor  Moccasin             ;//=  _ccc(0xFFFFE4B5);
        static RgbColor  NavajoWhite          ;//=  _ccc(0xFFFFDEAD);
        static RgbColor  Navy                 ;//=  _ccc(0xFF000080);
        static RgbColor  OldLace              ;//=  _ccc(0xFFFDF5E6);
        static RgbColor  Olive                ;//=  _ccc(0xFF808000);
        static RgbColor  OliveDrab            ;//=  _ccc(0xFF6B8E23);
        static RgbColor  Orange               ;//=  _ccc(0xFFFFA500);
        static RgbColor  OrangeRed            ;//=  _ccc(0xFFFF4500);
        static RgbColor  Orchid               ;//=  _ccc(0xFFDA70D6);
        static RgbColor  PaleGoldenrod        ;//=  _ccc(0xFFEEE8AA);
        static RgbColor  PaleGreen            ;//=  _ccc(0xFF98FB98);
        static RgbColor  PaleTurquoise        ;//=  _ccc(0xFFAFEEEE);
        static RgbColor  PaleVioletRed        ;//=  _ccc(0xFFDB7093);
        static RgbColor  PapayaWhip           ;//=  _ccc(0xFFFFEFD5);
        static RgbColor  PeachPuff            ;//=  _ccc(0xFFFFDAB9);
        static RgbColor  Peru                 ;//=  _ccc(0xFFCD853F);
        static RgbColor  Pink                 ;//=  _ccc(0xFFFFC0CB);
        static RgbColor  Plum                 ;//=  _ccc(0xFFDDA0DD);
        static RgbColor  PowderBlue           ;//=  _ccc(0xFFB0E0E6);
        static RgbColor  Purple               ;//=  _ccc(0xFF800080);
        static RgbColor  Red                  ;//=  _ccc(0xFFFF0000);
        static RgbColor  RosyBrown            ;//=  _ccc(0xFFBC8F8F);
        static RgbColor  RoyalBlue            ;//=  _ccc(0xFF4169E1);
        static RgbColor  SaddleBrown          ;//=  _ccc(0xFF8B4513);
        static RgbColor  Salmon               ;//=  _ccc(0xFFFA8072);
        static RgbColor  SandyBrown           ;//=  _ccc(0xFFF4A460);
        static RgbColor  SeaGreen             ;//=  _ccc(0xFF2E8B57);
        static RgbColor  SeaShell             ;//=  _ccc(0xFFFFF5EE);
        static RgbColor  Sienna               ;//=  _ccc(0xFFA0522D);
        static RgbColor  Silver               ;//=  _ccc(0xFFC0C0C0);
        static RgbColor  SkyBlue              ;//=  _ccc(0xFF87CEEB);
        static RgbColor  SlateBlue            ;//=  _ccc(0xFF6A5ACD);
        static RgbColor  SlateGray            ;//=  _ccc(0xFF708090);
        static RgbColor  Snow                 ;//=  _ccc(0xFFFFFAFA);
        static RgbColor  SpringGreen          ;//=  _ccc(0xFF00FF7F);
        static RgbColor  SteelBlue            ;//=  _ccc(0xFF4682B4);
        static RgbColor  Tan                  ;//=  _ccc(0xFFD2B48C);
        static RgbColor  Teal                 ;//=  _ccc(0xFF008080);
        static RgbColor  Thistle              ;//=  _ccc(0xFFD8BFD8);
        static RgbColor  Tomato               ;//=  _ccc(0xFFFF6347);
        static RgbColor  Transparent          ;//=  _ccc(0x00FFFFFF);
        static RgbColor  Turquoise            ;//=  _ccc(0xFF40E0D0);
        static RgbColor  Violet               ;//=  _ccc(0xFFEE82EE);
        static RgbColor  Wheat                ;//=  _ccc(0xFFF5DEB3);
        static RgbColor  White                ;//=  _ccc(0xFFFFFFFF);
        static RgbColor  WhiteSmoke           ;//=  _ccc(0xFFF5F5F5);
        static RgbColor  Yellow               ;//=  _ccc(0xFFFFFF00);
        static RgbColor  YellowGreen          ;//=  _ccc(0xFF9ACD32);
    };

    // Other colors not aligned with Pantone colors, as well as internal colors such as button text colors, etc. 
    // Most colors in this section are "aesthetically pleasing" colors more aligned with their names (i.e. dark blue), whereas
    // the Pantone colors with similar names can look, well, awful. 

    // ** NOTE: Do not change these values without changing them in the text-based initialization (CPasWindow.cpp), as they are still hard-coded there.
    //          $$ To do: change text-based initialization to use these values rather than hard-coded values. 
    //
    // ** NOTE: If values are added here, they also need to be added (as appropriate) to the text-based colors generated in CpasWindow.cpp -- these colors are
    //          used in text strings where a text string is neded and a value name cannot be used; therefore, they must align as needed. 

    class SageColor
    {
    public:
	    static RgbColor  DefaultBgColor                  ; // RGB(20,40,121));
	    static RgbColor  DefaultFgColor                  ; // RGB(255,255,255));
	    static RgbColor  SliderTextColor                 ; // RGB(128,128,128));
	    static RgbColor  Green                           ; // RGB(0,255,0));
	    static RgbColor  DarkGreen                       ; // RGB(0,128,0));
	    static RgbColor  LightGreen                      ; // RGB(128,255,128));
	    static RgbColor  Blue                            ; // RGB(0,0,255));
	    static RgbColor  DarkBlue                        ; // RGB(0,0,92));
	    static RgbColor  MidBlue                         ; // RGB(0,0,128));
	    static RgbColor  LightBlue                       ; // RGB(128,128,255));
	    static RgbColor  SkyBlue                         ; // RGB(40,145,255));
	    static RgbColor  SkyBlueDark                     ; // RGB(0,30,128));
	    static RgbColor  SkyBlueLight                    ; // RGB(75,165,255));
	    static RgbColor  Cyan                            ; // RGB(0,255,255)); 
	    static RgbColor  Red                             ; // RGB(255,0,0));
	    static RgbColor  LightRed                        ; // RGB(255,128,128));
	    static RgbColor  LightYellow                     ; // RGB(255,255,128));
	    static RgbColor  Yellow                          ; // RGB(255,255,0));
	    static RgbColor  Magenta                         ; // RGB(255,0,255));
	    static RgbColor  MediumMagenta                   ; // RGB(255,92,255));
	    static RgbColor  LightMagenta                    ; // RGB(255,128,255));
	    static RgbColor  Purple                          ; // RGB(255,0,255));
	    static RgbColor  LightPurple                     ; // RGB(255,128,255));
	    static RgbColor  MediumPurple                    ; // RGB(255,92,255));
	    static RgbColor  White                           ; // RGB(255,255,255)); 
	    static RgbColor  Gray172                         ; // RGB(172,172,172)); 
	    static RgbColor  Gray192                         ; // RGB(192,192,192)); 
	    static RgbColor  Gray220                         ; // RGB(220,220,220)); 
	    static RgbColor  Gray128                         ; // RGB(128,128,128)); 
	    static RgbColor  Gray32                          ; // RGB(32,32,32)); 
	    static RgbColor  Gray42                          ; // RGB(42,42,42)); 
	    static RgbColor  Gray64                          ; // RGB(64,64,64)); 
	    static RgbColor  Black                           ; // RGB(0,0,0)); 
	    static RgbColor  LightGray                       ; // RGB(200,200,200));
	    static RgbColor  LightGrey                       ; // RGB(200,200,200));
	    static RgbColor  MidGray                         ; // RGB(64,64,64));
	    static RgbColor  MidGrey                         ; // RGB(64,64,64));
	    static RgbColor  DarkGray                        ; // RGB(32,32,32));
	    static RgbColor  DarkGrey                        ; // RGB(32,32,32));
	    static RgbColor  Gray                            ; // RGB(128,128,128));
	    static RgbColor  Grey                            ; // RGB(128,128,128));
	    static RgbColor  NearWhite                       ; // RGB(220,220,220));
	    static RgbColor  ButtonTextColorNormal           ; // RGB(220,220,220));
	    static RgbColor  ButtonTextColorHighlighted      ; // RGB(255,255,255));
	    static RgbColor  ButtonTextColorPressed          ; // RGB(255,255,255));
	    static RgbColor  ButtonTextColorDisabled         ; // RGB(170,170,170));
	    static RgbColor  CheckboxTextColorNormal         ; // RGB(220,220,220));
	    static RgbColor  CheckboxTextColorHighlighted    ; // RGB(255,255,255));
	    static RgbColor  CheckboxTextColorChecked        ; // RGB(220,220,220));
	    static RgbColor  CheckboxTextColorCheckedHigh    ; // RGB(220,220,220));
	    static RgbColor  CheckboxTextColorDisabled       ; // RGB(170,170,170));
    };
}