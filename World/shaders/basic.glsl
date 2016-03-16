uniform sampler2D atlas;
uniform sampler2D shadowmap;
uniform sampler3D rgbtable;
uniform mat4 transform_VP;
uniform mat4 shadow_VP;

#ifdef _FRAGMENT_

in vec2 fragTexcoord;
in vec3 norm;
in vec4 ShadowCoord;
in vec4 pos;

layout (location = 0) out vec4 out_color;

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

void main(void)
{
    vec4 tcol = texture(atlas, fragTexcoord).rgba;
	if(tcol.a < 0.5)
	    discard;
	
	float bias = 0.005;
	float shadow = 1.0;
    for (int i=0;i<4;i++){
		if ( texture( shadowmap, ShadowCoord.xy + poissonDisk[i]/600.0 ).x  <  ShadowCoord.z-bias ){
			shadow-=0.2;
		}
    }
	if ( texture( shadowmap, ShadowCoord.xy ).x  <  ShadowCoord.z - bias){
	    shadow = 0.5;
	}

    vec3 lightvector = normalize(pos.xyz + vec3(300));
	float coef = min(1, max(0, dot(norm, lightvector)) + 0.5);
	out_color = shadow * tcol * coef;
	out_color = texture(rgbtable, out_color.xyz);
}

#endif

#ifdef _VERTEX_

in vec3 vertex;
in vec2 texture;
in vec3 normal;

out vec2 fragTexcoord;
out vec3 norm;
out vec4 ShadowCoord;
out vec4 pos;

void main(void)
{
    pos = transform_VP * vec4(vertex, 1);
	gl_Position = pos;
	ShadowCoord = shadow_VP * vec4(vertex, 1);
    fragTexcoord = texture;
    norm = normal;
}

#endif