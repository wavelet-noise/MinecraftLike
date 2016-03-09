uniform sampler2D atlas;
uniform sampler2D shadowmap;
uniform mat4 transform_VP;
uniform mat4 shadow_VP;

#ifdef _FRAGMENT_

in vec2 fragTexcoord;
in vec3 norm;
in vec4 ShadowCoord;

layout (location = 0) out vec4 out_color;

void main(void)
{
    vec4 tcol = texture(atlas, fragTexcoord).rgba;
	if(tcol.a < 0.5)
	    discard;
		
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
	
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
	
	
	float distanceFromLight = texture2D(shadowmap, shadowCoordinateWdivide.st).z;
	
	
 	float shadow = 1.0;
 	if (ShadowCoord.w > 0.0)
 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.1 : 1.0 ;

    const vec3 lightvector = normalize(vec3(0.1, 0.4, 0.3));
	float coef = min(1, max(0, dot(norm, lightvector)) + 0.5);
    out_color = shadow * tcol * coef;
	out_color = vec4(distanceFromLight,0,0,0);
	out_color.a = 1;
}

#endif

#ifdef _VERTEX_

in vec3 vertex;
in vec2 texture;
in vec3 normal;

out vec2 fragTexcoord;
out vec3 norm;
out vec4 ShadowCoord;

void main(void)
{
    gl_Position = transform_VP * vec4(vertex, 1);
	ShadowCoord = shadow_VP * vec4(vertex, 1);
    fragTexcoord = texture;
    norm = normal;
}

#endif