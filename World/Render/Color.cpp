



#include "Color.h"

Color::Color()
  : raw(0x00, 0x00, 0x00, 0xFF)
{

}

Color::Color(const glm::u8vec4 &color)
  : raw(color)
{

}

Color::Color(glm::u8 r, glm::u8 g, glm::u8 b, glm::u8 a)
  : raw(r, g, b, a)
{

}

Color::Color(float r, float g, float b, float a)
	: raw(r*255, g*255, b*255, a*255)
{
}

const Color Color::AliceBlue            (240 / 255.f, 248 / 255.f, 255 / 255.f, 1.f);
const Color Color::AntiqueWhite         (250 / 255.f, 235 / 255.f, 215 / 255.f, 1.f);
const Color Color::Aqua                 (0.f, 255 / 255.f, 255 / 255.f, 1.f);
const Color Color::Aquamarine           (127 / 255.f, 255 / 255.f, 212 / 255.f, 1.f);
const Color Color::Azure                (240 / 255.f, 255 / 255.f, 255 / 255.f, 1.f);
const Color Color::Beige                (245 / 255.f, 245 / 255.f, 220 / 255.f, 1.f);
const Color Color::Bisque               (255 / 255.f, 228 / 255.f, 196 / 255.f, 1.f);
const Color Color::Black                (0.f, 0.f, 0.f, 1.f);
const Color Color::BlanchedAlmond       (255 / 255.f, 235 / 255.f, 205 / 255.f, 1.f);
const Color Color::Blue                 (0.f, 0.f, 255 / 255.f, 1.f);
const Color Color::BlueViolet           (138 / 255.f, 43 / 255.f, 226 / 255.f, 1.f);
const Color Color::Brown                (165 / 255.f, 42 / 255.f, 42 / 255.f, 1.f);
const Color Color::BurlyWood            (222 / 255.f, 184 / 255.f, 135 / 255.f, 1.f);
const Color Color::CadetBlue            (95 / 255.f, 158 / 255.f, 160 / 255.f, 1.f);
const Color Color::Chartreuse           (127 / 255.f, 255 / 255.f, 0.f, 1.f);
const Color Color::Chocolate            (210 / 255.f, 105 / 255.f, 30 / 255.f, 1.f);
const Color Color::Clear                (0.f, 0.f, 0.f, 0.f);
const Color Color::Coral                (255 / 255.f, 127 / 255.f, 80 / 255.f, 1.f);
const Color Color::CornflowerBlue       (100 / 255.f, 149 / 255.f, 237 / 255.f, 1.f);
const Color Color::Cornsilk             (255 / 255.f, 248 / 255.f, 220 / 255.f, 1.f);
const Color Color::Crimson              (220 / 255.f, 20 / 255.f, 60 / 255.f, 1.f);
const Color Color::Cyan                 (0.f, 255 / 255.f, 255 / 255.f, 1.f);
const Color Color::DarkBlue             (0.f, 0.f, 139 / 255.f, 1.f);
const Color Color::DarkCyan             (0.f, 139 / 255.f, 139 / 255.f, 1.f);
const Color Color::DarkGoldenrod        (184 / 255.f, 134 / 255.f, 11 / 255.f, 1.f);
const Color Color::DarkGray             (169 / 255.f, 169 / 255.f, 169 / 255.f, 1.f);
const Color Color::DarkGreen            (0.f, 100 / 255.f, 0.f, 1.f);
const Color Color::DarkKhaki            (189 / 255.f, 183 / 255.f, 107 / 255.f, 1.f);
const Color Color::DarkMagenta          (139 / 255.f, 0.f, 139 / 255.f, 1.f);
const Color Color::DarkOliveGreen       (85 / 255.f, 107 / 255.f, 47 / 255.f, 1.f);
const Color Color::DarkOrange           (255 / 255.f, 140 / 255.f, 0.f, 1.f);
const Color Color::DarkOrchid           (153 / 255.f, 50 / 255.f, 204 / 255.f, 1.f);
const Color Color::DarkRed              (139 / 255.f, 0.f, 0.f, 1.f);
const Color Color::DarkSalmon           (233 / 255.f, 150 / 255.f, 122 / 255.f, 1.f);
const Color Color::DarkSeaGreen         (143 / 255.f, 188 / 255.f, 143 / 255.f, 1.f);
const Color Color::DarkSlateBlue        (72 / 255.f, 61 / 255.f, 139 / 255.f, 1.f);
const Color Color::DarkSlateGray        (47 / 255.f, 79 / 255.f, 79 / 255.f, 1.f);
const Color Color::DarkTurquoise        (0.f, 206 / 255.f, 209 / 255.f, 1.f);
const Color Color::DarkViolet           (148 / 255.f, 0.f, 211 / 255.f, 1.f);
const Color Color::DeepPink             (255 / 255.f, 20 / 255.f, 147 / 255.f, 1.f);
const Color Color::DeepSkyBlue          (0.f, 191 / 255.f, 255 / 255.f, 1.f);
const Color Color::DimGray              (105 / 255.f, 105 / 255.f, 105 / 255.f, 1.f);
const Color Color::DodgerBlue           (30 / 255.f, 144 / 255.f, 255 / 255.f, 1.f);
const Color Color::FireBrick            (178 / 255.f, 34 / 255.f, 34 / 255.f, 1.f);
const Color Color::FloralWhite          (255 / 255.f, 250 / 255.f, 240 / 255.f, 1.f);
const Color Color::ForestGreen          (34 / 255.f, 139 / 255.f, 34 / 255.f, 1.f);
const Color Color::Fuchsia              (255 / 255.f, 0.f, 255 / 255.f, 1.f);
const Color Color::Gainsboro            (220 / 255.f, 220 / 255.f, 220 / 255.f, 1.f);
const Color Color::GhostWhite           (248 / 255.f, 248 / 255.f, 255 / 255.f, 1.f);
const Color Color::Gold                 (255 / 255.f, 215 / 255.f, 0.f, 1.f);
const Color Color::Goldenrod            (218 / 255.f, 165 / 255.f, 32 / 255.f, 1.f);
const Color Color::Gray                 (128 / 255.f, 128 / 255.f, 128 / 255.f, 1.f);
const Color Color::Green                (0.f, 128 / 255.f, 0.f, 1.f);
const Color Color::GreenYellow          (173 / 255.f, 255 / 255.f, 47 / 255.f, 1.f);
const Color Color::Honeydew             (240 / 255.f, 255 / 255.f, 240 / 255.f, 1.f);
const Color Color::HotPink              (255 / 255.f, 105 / 255.f, 180 / 255.f, 1.f);
const Color Color::IndianRed            (205 / 255.f, 92 / 255.f, 92 / 255.f, 1.f);
const Color Color::Indigo               (75 / 255.f, 0.f, 130 / 255.f, 1.f);
const Color Color::Ivory                (255 / 255.f, 255 / 255.f, 240 / 255.f, 1.f);
const Color Color::Khaki                (240 / 255.f, 230 / 255.f, 140 / 255.f, 1.f);
const Color Color::Lavender             (230 / 255.f, 230 / 255.f, 250 / 255.f, 1.f);
const Color Color::LavenderBlush        (255 / 255.f, 240 / 255.f, 245 / 255.f, 1.f);
const Color Color::LawnGreen            (124 / 255.f, 252 / 255.f, 0.f, 1.f);
const Color Color::LemonChiffon         (255 / 255.f, 250 / 255.f, 205 / 255.f, 1.f);
const Color Color::LightBlue            (173 / 255.f, 216 / 255.f, 230 / 255.f, 1.f);
const Color Color::LightCoral           (240 / 255.f, 128 / 255.f, 128 / 255.f, 1.f);
const Color Color::LightCyan            (224 / 255.f, 255 / 255.f, 255 / 255.f, 1.f);
const Color Color::LightGoldenrodYellow (250 / 255.f, 250 / 255.f, 210 / 255.f, 1.f);
const Color Color::LightGreen           (144 / 255.f, 238 / 255.f, 144 / 255.f, 1.f);
const Color Color::LightGrey            (211 / 255.f, 211 / 255.f, 211 / 255.f, 1.f);
const Color Color::LightPink            (255 / 255.f, 182 / 255.f, 193 / 255.f, 1.f);
const Color Color::LightSalmon          (255 / 255.f, 160 / 255.f, 122 / 255.f, 1.f);
const Color Color::LightSeaGreen        (32 / 255.f, 178 / 255.f, 170 / 255.f, 1.f);
const Color Color::LightSkyBlue         (135 / 255.f, 206 / 255.f, 250 / 255.f, 1.f);
const Color Color::LightSlateGray       (119 / 255.f, 136 / 255.f, 153 / 255.f, 1.f);
const Color Color::LightSteelBlue       (176 / 255.f, 196 / 255.f, 222 / 255.f, 1.f);
const Color Color::LightYellow          (255 / 255.f, 255 / 255.f, 224 / 255.f, 1.f);
const Color Color::Lime                 (0.f, 255 / 255.f, 0.f, 1.f);
const Color Color::LimeGreen            (50 / 255.f, 205 / 255.f, 50 / 255.f, 1.f);
const Color Color::Linen                (250 / 255.f, 240 / 255.f, 230 / 255.f, 1.f);
const Color Color::Magenta              (255 / 255.f, 0.f, 255 / 255.f, 1.f);
const Color Color::Maroon               (128 / 255.f, 0.f, 0.f, 1.f);
const Color Color::MediumAquamarine     (102 / 255.f, 205 / 255.f, 170 / 255.f, 1.f);
const Color Color::MediumBlue           (0.f, 0.f, 205 / 255.f, 1.f);
const Color Color::MediumOrchid         (186 / 255.f, 85 / 255.f, 211 / 255.f, 1.f);
const Color Color::MediumPurple         (147 / 255.f, 112 / 255.f, 219 / 255.f, 1.f);
const Color Color::MediumSeaGreen       (60 / 255.f, 179 / 255.f, 113 / 255.f, 1.f);
const Color Color::MediumSlateBlue      (123 / 255.f, 104 / 255.f, 238 / 255.f, 1.f);
const Color Color::MediumSpringGreen    (0.f, 250 / 255.f, 154 / 255.f, 1.f);
const Color Color::MediumTurquoise      (72 / 255.f, 209 / 255.f, 204 / 255.f, 1.f);
const Color Color::MediumVioletRed      (199 / 255.f, 21 / 255.f, 133 / 255.f, 1.f);
const Color Color::MidnightBlue         (25 / 255.f, 25 / 255.f, 112 / 255.f, 1.f);
const Color Color::MintCream            (245 / 255.f, 255 / 255.f, 250 / 255.f, 1.f);
const Color Color::MistyRose            (255 / 255.f, 228 / 255.f, 225 / 255.f, 1.f);
const Color Color::Moccasin             (255 / 255.f, 228 / 255.f, 181 / 255.f, 1.f);
const Color Color::NavajoWhite          (255 / 255.f, 222 / 255.f, 173 / 255.f, 1.f);
const Color Color::Navy                 (0.f, 0.f, 128 / 255.f, 1.f);
const Color Color::OldLace                 (253 / 255.f, 245 / 255.f, 230 / 255.f, 1.f);
const Color Color::Olive                 (128 / 255.f, 128 / 255.f, 0.f, 1.f);
const Color Color::OliveDrab                 (107 / 255.f, 142 / 255.f, 35 / 255.f, 1.f);
const Color Color::Orange                 (255 / 255.f, 165 / 255.f, 0.f, 1.f);
const Color Color::OrangeRed                 (255 / 255.f, 69 / 255.f, 0.f, 1.f);
const Color Color::Orchid                 (218 / 255.f, 112 / 255.f, 214 / 255.f, 1.f);
const Color Color::PaleGoldenrod                 (238 / 255.f, 232 / 255.f, 170 / 255.f, 1.f);
const Color Color::PaleGreen                 (152 / 255.f, 251 / 255.f, 152 / 255.f, 1.f);
const Color Color::PaleTurquoise                 (175 / 255.f, 238 / 255.f, 238 / 255.f, 1.f);
const Color Color::PaleVioletRed                 (219 / 255.f, 112 / 255.f, 147 / 255.f, 1.f);
const Color Color::PapayaWhip                 (255 / 255.f, 239 / 255.f, 213 / 255.f, 1.f);
const Color Color::PeachPuff                 (255 / 255.f, 218 / 255.f, 185 / 255.f, 1.f);
const Color Color::Peru                 (205 / 255.f, 133 / 255.f, 63 / 255.f, 1.f);
const Color Color::Plum                 (221 / 255.f, 160 / 255.f, 221 / 255.f, 1.f);
const Color Color::PowderBlue                 (176 / 255.f, 224 / 255.f, 230 / 255.f, 1.f);
const Color Color::Purple                 (128 / 255.f, 0.f, 128 / 255.f, 1.f);
const Color Color::RebeccaPurple       (102 / 255.f, 51 / 255.f, 153 / 255.f, 1.f);
const Color Color::Red              (255 / 255.f, 0.f, 0.f, 1.f);
const Color Color::RosyBrown              (188 / 255.f, 143 / 255.f, 143 / 255.f, 1.f);
const Color Color::RoyalBlue              (65 / 255.f, 105 / 255.f, 225 / 255.f, 1.f);
const Color Color::SaddleBrown              (139 / 255.f, 69 / 255.f, 19 / 255.f, 1.f);
const Color Color::Salmon              (250 / 255.f, 128 / 255.f, 114 / 255.f, 1.f);
const Color Color::SandyBrown              (244 / 255.f, 164 / 255.f, 96 / 255.f, 1.f);
const Color Color::SeaGreen         (46 / 255.f, 139 / 255.f, 87 / 255.f, 1.f);
const Color Color::Seashell         (255 / 255.f, 245 / 255.f, 238 / 255.f, 1.f);
const Color Color::SeaBlue         (26 / 255.f, 71 / 255.f, 128 / 255.f, 1.f);
const Color Color::Sienna         (160 / 255.f, 82 / 255.f, 45 / 255.f, 1.f);
const Color Color::Silver         (192 / 255.f, 192 / 255.f, 192 / 255.f, 1.f);
const Color Color::SkyBlue         (135 / 255.f, 206 / 255.f, 235 / 255.f, 1.f);
const Color Color::SlateBlue         (106 / 255.f, 90 / 255.f, 205 / 255.f, 1.f);
const Color Color::SlateGray              (112 / 255.f, 128 / 255.f, 144 / 255.f, 1.f);
const Color Color::Snow              (255 / 255.f, 250 / 255.f, 250 / 255.f, 1.f);
const Color Color::SpringGreen              (0.f, 255 / 255.f, 127 / 255.f, 1.f);
const Color Color::SteelBlue              (70 / 255.f, 130 / 255.f, 180 / 255.f, 1.f);
const Color Color::Tan              (210 / 255.f, 180 / 255.f, 140 / 255.f, 1.f);
const Color Color::Teal              (0.f, 128 / 255.f, 128 / 255.f, 1.f);
const Color Color::Thistle              (216 / 255.f, 191 / 255.f, 216 / 255.f, 1.f);
const Color Color::Tomato      (255 / 255.f, 99 / 255.f, 71 / 255.f, 1.f);
const Color Color::Turquoise   (64 / 255.f, 224 / 255.f, 208 / 255.f, 1.f);
const Color Color::Violet      (238 / 255.f, 130 / 255.f, 238 / 255.f, 1.f);
const Color Color::Wheat       (245 / 255.f, 222 / 255.f, 179 / 255.f, 1.f);
const Color Color::White       (255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f);
const Color Color::WhiteSmoke  (245 / 255.f, 245 / 255.f, 245 / 255.f, 1.f);
const Color Color::Yellow      (255 / 255.f, 255 / 255.f, 0.f, 1.f);
const Color Color::YellowGreen (154 / 255.f, 205 / 255.f, 50 / 255.f, 1.f);
const Color Color::Pink        (255 / 255.f, 192 / 255.f, 203 / 255.f, 1.f);

