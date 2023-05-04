//
// Created by Whisky on 2023/4/29.
//

#ifndef HELLOWORLD_LCD_COLOR_H
#define HELLOWORLD_LCD_COLOR_H


#define WHITE           0xFFFF      /* 白色 */
#define BLACK           0x0000      /* 黑色 */
#define RED             0xF800      /* 红色 */
#define GREEN           0x07E0      /* 绿色 */
#define BLUE            0x001F      /* 蓝色 */
#define MAGENTA         0XF81F      /* 品红色/紫红色 = BLUE + RED */
#define YELLOW          0XFFE0      /* 黄色 = GREEN + RED */
#define CYAN            0X07FF      /* 青色 = GREEN + BLUE */
#define BROWN           0XBC40      /* 棕色 */
#define BRRED           0XFC07      /* 棕红色 */
#define GRAY            0X8430      /* 灰色 */
#define DARKBLUE        0X01CF      /* 深蓝色 */
#define LIGHTBLUE       0X7D7C      /* 浅蓝色 */
#define GRAYBLUE        0X5458      /* 灰蓝色 */
#define LIGHTGREEN      0X841F      /* 浅绿色 */
#define LGRAY           0XC618      /* 浅灰色(PANNEL),窗体背景色 */
#define LGRAYBLUE       0XA651      /* 浅灰蓝色(中间层颜色) */
#define LBBLUE          0X2B12      /* 浅棕蓝色(选择条目的反色) */


/* Colors for display */
#define COLOR_565_ALICEBLUE                 0xF7DF
#define COLOR_565_ANTIQUEWHITE              0xFF5A
#define COLOR_565_AQUA                      0x07FF
#define COLOR_565_AQUAMARINE                0x7FFA
#define COLOR_565_AZURE                     0xF7FF
#define COLOR_565_BEIGE                     0xF7BB
#define COLOR_565_BISQUE                    0xFF38
#define COLOR_565_BLACK                     0x0000
#define COLOR_565_BLANCHEDALMOND            0xFF59
#define COLOR_565_BLUE                      0x001F
#define COLOR_565_BLUEVIOLET                0x895C
#define COLOR_565_BROWN                     0xA145
#define COLOR_565_BURLYWOOD                 0xDDD0
#define COLOR_565_CADETBLUE                 0x5CF4
#define COLOR_565_CHARTREUSE                0x7FE0
#define COLOR_565_CHOCOLATE                 0xD343
#define COLOR_565_CORAL                     0xFBEA
#define COLOR_565_CORNFLOWERBLUE            0x64BD
#define COLOR_565_CORNSILK                  0xFFDB
#define COLOR_565_CRIMSON                   0xD8A7
#define COLOR_565_CYAN                      0x07FF
#define COLOR_565_DARKBLUE                  0x0011
#define COLOR_565_DARKCYAN                  0x0451
#define COLOR_565_DARKGOLDENROD             0xBC21
#define COLOR_565_DARKGRAY                  0xAD55
#define COLOR_565_DARKGREEN                 0x0320
#define COLOR_565_DARKKHAKI                 0xBDAD
#define COLOR_565_DARKMAGENTA               0x8811
#define COLOR_565_DARKOLIVEGREEN            0x5345
#define COLOR_565_DARKORANGE                0xFC60
#define COLOR_565_DARKORCHID                0x9999
#define COLOR_565_DARKRED                   0x8800
#define COLOR_565_DARKSALMON                0xECAF
#define COLOR_565_DARKSEAGREEN              0x8DF1
#define COLOR_565_DARKSLATEBLUE             0x49F1
#define COLOR_565_DARKSLATEGRAY             0x2A69
#define COLOR_565_DARKTURQUOISE             0x067A
#define COLOR_565_DARKVIOLET                0x901A
#define COLOR_565_DEEPPINK                  0xF8B2
#define COLOR_565_DEEPSKYBLUE               0x05FF
#define COLOR_565_DIMGRAY                   0x6B4D
#define COLOR_565_DODGERBLUE                0x1C9F
#define COLOR_565_FIREBRICK                 0xB104
#define COLOR_565_FLORALWHITE               0xFFDE
#define COLOR_565_FORESTGREEN               0x2444
#define COLOR_565_FUCHSIA                   0xF81F
#define COLOR_565_GAINSBORO                 0xDEFB
#define COLOR_565_GHOSTWHITE                0xFFDF
#define COLOR_565_GOLD                      0xFEA0
#define COLOR_565_GOLDENROD                 0xDD24
#define COLOR_565_GRAY                      0x8410
#define COLOR_565_GREEN                     0x0400
#define COLOR_565_GREENYELLOW               0xAFE5
#define COLOR_565_HONEYDEW                  0xF7FE
#define COLOR_565_HOTPINK                   0xFB56
#define COLOR_565_INDIANRED                 0xCAEB
#define COLOR_565_INDIGO                    0x4810
#define COLOR_565_IVORY                     0xFFFE
#define COLOR_565_KHAKI                     0xF731
#define COLOR_565_LAVENDER                  0xE73F
#define COLOR_565_LAVENDERBLUSH             0xFF9E
#define COLOR_565_LAWNGREEN                 0x7FE0
#define COLOR_565_LEMONCHIFFON              0xFFD9
#define COLOR_565_LIGHTBLUE                 0xAEDC
#define COLOR_565_LIGHTCORAL                0xF410
#define COLOR_565_LIGHTCYAN                 0xE7FF
#define COLOR_565_LIGHTGREEN                0x9772
#define COLOR_565_LIGHTGREY                 0xD69A
#define COLOR_565_LIGHTPINK                 0xFDB8
#define COLOR_565_LIGHTSALMON               0xFD0F
#define COLOR_565_LIGHTSEAGREEN             0x2595
#define COLOR_565_LIGHTSKYBLUE              0x867F
#define COLOR_565_LIGHTSLATEGRAY            0x7453
#define COLOR_565_LIGHTSTEELBLUE            0xB63B
#define COLOR_565_LIGHTYELLOW               0xFFFC
#define COLOR_565_LIME                      0x07E0
#define COLOR_565_LIMEGREEN                 0x3666
#define COLOR_565_LINEN                     0xFF9C
#define COLOR_565_MAGENTA                   0xF81F
#define COLOR_565_MAROON                    0x8000
#define COLOR_565_MEDIUMAQUAMARINE          0x6675
#define COLOR_565_MEDIUMBLUE                0x0019
#define COLOR_565_MEDIUMORCHID              0xBABA
#define COLOR_565_MEDIUMPURPLE              0x939B
#define COLOR_565_MEDIUMSEAGREEN            0x3D8E
#define COLOR_565_MEDIUMSLATEBLUE           0x7B5D
#define COLOR_565_MEDIUMSPRINGGREEN         0x07D3
#define COLOR_565_MEDIUMTURQUOISE           0x4E99
#define COLOR_565_MEDIUMVIOLETRED           0xC0B0
#define COLOR_565_MIDNIGHTBLUE              0x18CE
#define COLOR_565_MINTCREAM                 0xF7FF
#define COLOR_565_MISTYROSE                 0xFF3C
#define COLOR_565_MOCCASIN                  0xFF36
#define COLOR_565_NAVAJOWHITE               0xFEF5
#define COLOR_565_NAVY                      0x0010
#define COLOR_565_OLDLACE                   0xFFBC
#define COLOR_565_OLIVE                     0x8400
#define COLOR_565_OLIVEDRAB                 0x6C64
#define COLOR_565_ORANGE                    0xFD20
#define COLOR_565_ORANGERED                 0xFA20
#define COLOR_565_ORCHID                    0xDB9A
#define COLOR_565_PALEGOLDENROD             0xEF55
#define COLOR_565_PALEGREEN                 0x9FD3
#define COLOR_565_PALETURQUOISE             0xAF7D
#define COLOR_565_PALEVIOLETRED             0xDB92
#define COLOR_565_PAPAYAWHIP                0xFF7A
#define COLOR_565_PEACHPUFF                 0xFED7
#define COLOR_565_PERU                      0xCC27
#define COLOR_565_PINK                      0xFE19
#define COLOR_565_PLUM                      0xDD1B
#define COLOR_565_POWDERBLUE                0xB71C
#define COLOR_565_PURPLE                    0x8010
#define COLOR_565_RED                       0xF800
#define COLOR_565_ROSYBROWN                 0xBC71
#define COLOR_565_ROYALBLUE                 0x435C
#define COLOR_565_SADDLEBROWN               0x8A22
#define COLOR_565_SALMON                    0xFC0E
#define COLOR_565_SANDYBROWN                0xF52C
#define COLOR_565_SEAGREEN                  0x2C4A
#define COLOR_565_SEASHELL                  0xFFBD
#define COLOR_565_SIENNA                    0xA285
#define COLOR_565_SILVER                    0xC618
#define COLOR_565_SKYBLUE                   0x867D
#define COLOR_565_SLATEBLUE                 0x6AD9
#define COLOR_565_SLATEGRAY                 0x7412
#define COLOR_565_SNOW                      0xFFDF
#define COLOR_565_SPRINGGREEN               0x07EF
#define COLOR_565_STEELBLUE                 0x4416
#define COLOR_565_TAN                       0xD5B1
#define COLOR_565_TEAL                      0x0410
#define COLOR_565_THISTLE                   0xDDFB
#define COLOR_565_TOMATO                    0xFB08
#define COLOR_565_TURQUOISE                 0x471A
#define COLOR_565_VIOLET                    0xEC1D
#define COLOR_565_WHEAT                     0xF6F6
#define COLOR_565_WHITE                     0xFFFF
#define COLOR_565_WHITESMOKE                0xF7BE
#define COLOR_565_YELLOW                    0xFFE0
#define COLOR_565_YELLOWGREEN               0x9E66


#endif //HELLOWORLD_LCD_COLOR_H
