uniform mat4 modelview;
uniform mat4 projection;
uniform sampler2D atlas;
uniform sampler3D rgbtable;
uniform sampler2D shadowmap;

#ifdef _VERTEX_

layout(location = 1) in vec4 position;
layout(location = 2) in vec2 texture;
layout(location = 3) in vec4 color;

layout(location = 0) in vec2 corner;

out vec2 texCoord;
out vec4 outcol;

void main()
{
    vec4 eyePos = modelview * vec4(position.xyz, 1);        
    eyePos.xy += 1 * (corner - vec2(0.5)); 
    gl_Position = projection * eyePos;  
	
    texCoord = texture;
	outcol = color;
}

#endif

#ifdef _FRAGMENT_

in vec2 texCoord;
in vec4 outcol;

layout (location = 0) out vec4 out_color;

void main()
{
	vec4 tcol = texture(atlas, texCoord).rgba;
	if(tcol.a < 0.5)
	    discard;
		
	out_color = tcol * outcol;
}

#endif