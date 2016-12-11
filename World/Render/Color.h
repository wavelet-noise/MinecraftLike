



#pragma once
#ifndef Color_h__
#define Color_h__

#include <glm/glm.hpp>
#include "imgui/imgui.h"

struct Color
{
public:
	Color();

	Color(const glm::u8vec4 &color);

	Color(glm::u8 r, glm::u8 g, glm::u8 b, glm::u8 a);
	Color(float r, float g, float b, float a);

	operator glm::vec4() const
	{
		return glm::vec4(static_cast<glm::f32>(raw.r) / 255.0f,
			static_cast<glm::f32>(raw.g) / 255.0f,
			static_cast<glm::f32>(raw.b) / 255.0f,
			static_cast<glm::f32>(raw.a) / 255.0f
		);
	}

	operator ImVec4() const
	{
		return glm::vec4(static_cast<glm::f32>(raw.r) / 255.0f,
			static_cast<glm::f32>(raw.g) / 255.0f,
			static_cast<glm::f32>(raw.b) / 255.0f,
			static_cast<glm::f32>(raw.a) / 255.0f
		);
	}

	operator glm::u8vec4() const
	{
		return raw;
	}

	operator unsigned int() const
	{
		return (raw.r << 24) + (raw.g << 16) + (raw.b << 8) + raw.a;
	}

public:
	glm::u8vec4 raw;

public:
	static const Color AliceBlue;
	static const Color AntiqueWhite;
	static const Color Aqua;
	static const Color Aquamarine;
	static const Color Azure;
	static const Color Beige;
	static const Color Bisque;
	static const Color Black;
	static const Color BlanchedAlmond;
	static const Color Blue;
	static const Color BlueViolet;
	static const Color Brown;
	static const Color BurlyWood;
	static const Color CadetBlue;
	static const Color Chartreuse;
	static const Color Chocolate;
	static const Color Clear;
	static const Color Coral;
	static const Color CornflowerBlue;
	static const Color Cornsilk;
	static const Color Crimson;
	static const Color Cyan;
	static const Color DarkBlue;
	static const Color DarkCyan;
	static const Color DarkGoldenrod;
	static const Color DarkGray;
	static const Color DarkGreen;
	static const Color DarkKhaki;
	static const Color DarkMagenta;
	static const Color DarkOliveGreen;
	static const Color DarkOrange;
	static const Color DarkOrchid;
	static const Color DarkRed;
	static const Color DarkSalmon;
	static const Color DarkSeaGreen;
	static const Color DarkSlateBlue;
	static const Color DarkSlateGray;
	static const Color DarkTurquoise;
	static const Color DarkViolet;
	static const Color DeepPink;
	static const Color DeepSkyBlue;
	static const Color DimGray;
	static const Color DodgerBlue;
	static const Color FireBrick;
	static const Color FloralWhite;
	static const Color ForestGreen;
	static const Color Fuchsia;
	static const Color Gainsboro;
	static const Color GhostWhite;
	static const Color Gold;
	static const Color Goldenrod;
	static const Color Gray;
	static const Color Green;
	static const Color GreenYellow;
	static const Color Honeydew;
	static const Color HotPink;
	static const Color IndianRed;
	static const Color Indigo;
	static const Color Ivory;
	static const Color Khaki;
	static const Color Lavender;
	static const Color LavenderBlush;
	static const Color LawnGreen;
	static const Color LemonChiffon;
	static const Color LightBlue;
	static const Color LightCoral;
	static const Color LightCyan;
	static const Color LightGoldenrodYellow;
	static const Color LightGreen;
	static const Color LightGrey;
	static const Color LightPink;
	static const Color LightSalmon;
	static const Color LightSeaGreen;
	static const Color LightSkyBlue;
	static const Color LightSlateGray;
	static const Color LightSteelBlue;
	static const Color LightYellow;
	static const Color Lime;
	static const Color LimeGreen;
	static const Color Linen;
	static const Color Magenta;
	static const Color Maroon;
	static const Color MediumAquamarine;
	static const Color MediumBlue;
	static const Color MediumOrchid;
	static const Color MediumPurple;
	static const Color MediumSeaGreen;
	static const Color MediumSlateBlue;
	static const Color MediumSpringGreen;
	static const Color MediumTurquoise;
	static const Color MediumVioletRed;
	static const Color MidnightBlue;
	static const Color MintCream;
	static const Color MistyRose;
	static const Color Moccasin;
	static const Color NavajoWhite;
	static const Color Navy;
	static const Color OldLace;
	static const Color Olive;
	static const Color OliveDrab;
	static const Color Orange;
	static const Color OrangeRed;
	static const Color Orchid;
	static const Color PaleGoldenrod;
	static const Color PaleGreen;
	static const Color PaleTurquoise;
	static const Color PaleVioletRed;
	static const Color PapayaWhip;
	static const Color PeachPuff;
	static const Color Peru;
	static const Color Plum;
	static const Color PowderBlue;
	static const Color Purple;
	static const Color RebeccaPurple;
	static const Color Red;
	static const Color RosyBrown;
	static const Color RoyalBlue;
	static const Color SaddleBrown;
	static const Color Salmon;
	static const Color SandyBrown;
	static const Color SeaGreen;
	static const Color Seashell;
	static const Color SeaBlue;
	static const Color Sienna;
	static const Color Silver;
	static const Color SkyBlue;
	static const Color SlateBlue;
	static const Color SlateGray;
	static const Color Snow;
	static const Color SpringGreen;
	static const Color SteelBlue;
	static const Color Tan;
	static const Color Teal;
	static const Color Thistle;
	static const Color Tomato;
	static const Color Turquoise;
	static const Color Violet;
	static const Color Wheat;
	static const Color White;
	static const Color WhiteSmoke;
	static const Color Yellow;
	static const Color YellowGreen;
	static const Color Pink;
};



#endif // Color_h__
