uniform sampler2D atlas;
uniform sampler2D shadowmap;
uniform sampler3D rgbtable;
uniform mat4 transform_VP;
uniform mat4 shadow_VP;

#ifdef _FRAGMENT_

in vec2 fragTexcoord;
in vec3 norm;
in vec4 sc;
in vec4 pos;

layout (location = 0) out vec4 out_color;

vec2 poissonDisk[16] = vec2[](
	vec2( -0.94201624, -0.39906216 ),
	vec2( 0.94558609, -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2( 0.34495938, 0.29387760 ),
	vec2( -0.91588581, 0.45771432 ),
	vec2( -0.81544232, -0.87912464 ),
	vec2( -0.38277543, 0.27676845 ),
	vec2( 0.97484398, 0.75648379 ),
	vec2( 0.44323325, -0.97511554 ),
	vec2( 0.53742981, -0.47373420 ),
	vec2( -0.26496911, -0.41893023 ),
	vec2( 0.79197514, 0.19090188 ),
	vec2( -0.24188840, 0.99706507 ),
	vec2( -0.81409955, 0.91437590 ),
	vec2( 0.19984126, 0.78641367 ),
	vec2( 0.14383161, -0.14100790 )
);
                                       

void main(void)
{
    vec4 tcol = texture(atlas, fragTexcoord).rgba;
	if(tcol.a < 0.5)
	    discard;
	
	float shadow = 1.0;
    for (int i=0; i<16; i++){
		if ( texture( shadowmap, sc.xy + poissonDisk[i]/1024.0 ).x  <  sc.z ){
			shadow -= 1.0f/16.0f;
		}
    }

    vec3 lightvector = normalize(pos.xyz + vec3(100));
	float coef = min(1, max(0, dot(norm, lightvector) * shadow) + 0.5);
	out_color = coef * tcol;
	out_color = texture(rgbtable, out_color.xyz);
}

#endif

#ifdef _VERTEX_

in vec3 vertex;
in vec2 texture;
in vec3 normal;

out vec2 fragTexcoord;
out vec3 norm;
out vec4 sc;
out vec4 pos;

void main(void)
{
	const float bias = 0.005;

    pos = transform_VP * vec4(vertex, 1);
	gl_Position = pos;
	sc = shadow_VP * vec4(vertex, 1);
	
	sc.x = sc.x/(abs(sc.x)+0.1f);
    sc.y = sc.y/(abs(sc.y)+0.1f);
	
	sc.x  = sc.x * 0.5 + 0.5;
    sc.y  = sc.y * 0.5 + 0.5; 
    sc.z  = sc.z * 0.5 + 0.5 - bias;   
	
    fragTexcoord = texture;
    norm = normal;
}

#endif