/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
uniform sampler2D atlas;

#ifdef _FRAGMENT_

in vec4 fragColor;
in vec2 uv;

out vec4 out_color;

void main(void)
{
    vec4 tcol = texture(atlas, uv).rgba;
	if(tcol.a < 0.5)
	    discard;
		
	out_color = tcol * fragColor;
}

#endif

#ifdef _VERTEX_

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 3) in vec4 color;

uniform mat4 transform_VP;

out vec4 fragColor;
out vec2 uv;


void main(void)
{
    gl_Position = transform_VP * vec4(position, 1);
    fragColor = color;
    uv = texcoord;
}

#endif

