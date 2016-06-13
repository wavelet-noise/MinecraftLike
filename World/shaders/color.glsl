uniform sampler2D atlas;
uniform sampler2D shadowmap;
uniform sampler3D rgbtable;

uniform mat4 transform_VP;

#ifdef _FRAGMENT_

in vec2 tex;
in vec4 co;

layout (location = 0) out vec4 out_color;
                                       

void main(void)
{
    out_color = co;
}

#endif

#ifdef _VERTEX_

layout(location = 1) in vec4 position;
layout(location = 2) in vec2 texture;
layout(location = 3) in vec4 color;

out vec2 tex;
out vec4 co;

void main(void)
{
	gl_Position = transform_VP * vec4(position.xyz, 1);
	tex = texture;
	co = color;
}

#endif