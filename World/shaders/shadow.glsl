uniform sampler2D atlas;
uniform sampler2D shadowmap;
uniform mat4 transform_VP;
uniform mat4 shadow_VP;

#ifdef _FRAGMENT_

in vec2 fragTexcoord;

layout (location = 0) out vec4 out_color;

void main(void)
{
	vec4 tcol = texture(atlas, fragTexcoord).rgba;
	if(tcol.a < 0.5)
	    discard;
}

#endif

#ifdef _VERTEX_

in vec3 vertex;
in vec2 texture;

out vec2 fragTexcoord;

void main(void)
{
    vec4 sc = transform_VP * vec4(vertex, 1);
	//sc.x = sc.x/(abs(sc.x)+0.05f);
    //sc.y = sc.y/(abs(sc.y)+0.05f);
	gl_Position = sc;
	fragTexcoord = texture;
}

#endif