#ifdef _FRAGMENT_

uniform sampler2D material_texture;
in vec2 fragTexcoord;
in vec4 col;

layout (location = 0) out vec4 out_color;

void main(void)
{
    out_color = col * vec4(texture(material_texture, fragTexcoord).rgba);
}

#endif

#ifdef _VERTEX_


in vec3 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

uniform mat4 transform_VP;

out vec2 fragTexcoord;
out vec4 col;

void main(void)
{
    gl_Position = transform_VP * vec4(position, 1);
    fragTexcoord = texcoord;
    col = color;
}

#endif