#ifdef _FRAGMENT_

uniform sampler2D atlas;
in vec2 fragTexcoord;
in vec3 norm;

layout (location = 0) out vec4 out_color;

void main(void)
{
    out_color = vec4(texture(atlas, fragTexcoord).rgba);
}

#endif

#ifdef _VERTEX_

in vec3 vertex;
in vec2 texture;
in vec3 normal;

uniform mat4 transform_VP;

out vec2 fragTexcoord;
out vec3 norm;

void main(void)
{
    gl_Position = transform_VP * vec4(vertex, 1);
    fragTexcoord = texture;
    norm = normal;
}

#endif