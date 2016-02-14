uniform sampler2D atlas;
uniform mat4 transform_VP;

#ifdef _FRAGMENT_

in vec2 fragTexcoord;
in vec3 norm;

layout (location = 0) out vec4 out_color;

void main(void)
{
    vec4 tcol = texture(atlas, fragTexcoord).rgba;
	if(tcol.a < 0.5)
	    discard;

    const vec3 lightvector = normalize(vec3(0.1, 0.4, 0.3));
	float coef = min(1, max(0, dot(norm, lightvector)) + 0.5);
    out_color = tcol * coef;
	out_color.a = 1;
}

#endif

#ifdef _VERTEX_

in vec3 vertex;
in vec2 texture;
in vec3 normal;

out vec2 fragTexcoord;
out vec3 norm;

void main(void)
{
    gl_Position = transform_VP * vec4(vertex, 1);
    fragTexcoord = texture;
    norm = normal;
}

#endif