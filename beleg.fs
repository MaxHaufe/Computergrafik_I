
#version 450 core  // 420, 330 core , compatibility

struct Material {
    vec3 ambient;
    vec3 diffuse;
	vec3 specular;
    float shininess;
};

uniform Material material;

in vec4 Color;
in vec3 Normal;
in vec4 Position;
in vec2 fTexCoord;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 EyeDirection; 
uniform sampler2D tex;
uniform float textureEnabled;

//vec3 EyeDirection =  vec3(0.0f, 3.5f, 10.0f) * vec3(Position);		//hardcoded temporary EyeDirection

layout(location=0) out vec4 fColor;

void main() {
	//diffuse lightning | works
	
	float ConstantAttenuation = 1;
	float LinearAttenuation = 0;
	float QuadraticAttenuation = 0;

	vec3 lightDirection=normalize(LightPosition-vec3(Position));	//point
	//vec3 lightDirection=normalize(LightPosition);	//diffuse
	float lightDistance=length(lightDirection);

	float attenuation=1.0/(ConstantAttenuation+LinearAttenuation*lightDistance+QuadraticAttenuation*lightDistance*lightDistance);

	float diffuse=max(0.0,dot(Normal,lightDirection));
	vec3 halfVector=normalize(lightDirection+normalize(EyeDirection));
	float specular=max(0.0,dot(Normal,halfVector));
	if (diffuse==0.0) specular=0.0; else specular=pow(specular,material.shininess);
	vec3 scatteredLight=material.ambient+material.diffuse*(LightColor*diffuse)*attenuation;
	vec3 reflectedLight=LightColor*material.specular*specular*attenuation;
	vec3 rgb=min(Color.rgb*scatteredLight+reflectedLight,vec3(1.0));

	if (textureEnabled == 1.0f){
		//fColor = texture2D(tex,fTexCoord);
		vec3 text = texture2D(tex,fTexCoord).rgb;
		vec3 rgb=min(text.rgb*scatteredLight+reflectedLight,vec3(1.0,1.0,1.0));
		fColor=vec4(rgb,texture2D(tex,fTexCoord).a);
		//fColor  = vec4(fTexCoord, 1,0);
	}
	else{
		//fColor = texture2D(tex,fTexCoord);
		fColor=vec4(rgb,Color.a);
	}
}