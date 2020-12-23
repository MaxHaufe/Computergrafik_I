
#version 450 core  // 420, 330 core , compatibility

struct Material {
    vec3 ambient;
    vec3 diffuse;
    float shininess;
};

uniform Material material;

in vec4 Color;
in vec3 Normal;
in vec3 FragPos;

//uniform vec3 Ambient;			//todo
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 EyeDirection; 
//uniform float Shininess;		//TODO

layout(location=0) out vec4 fColor;

void main() {
	vec3 lightDirection=normalize(LightPosition);
	float diffuse=max(0.0,dot(Normal,lightDirection));
	vec3 halfVector=normalize(lightDirection+EyeDirection);
	float specular=max(0.0,dot(Normal,halfVector));
	if (diffuse==0.0) specular=0.0; else specular=pow(specular,material.shininess);
	vec3 scatteredLight=material.ambient+material.diffuse*(LightColor*diffuse);
	vec3 reflectedLight=LightColor*specular;
	vec3 rgb=min(Color.rgb*scatteredLight+reflectedLight,vec3(1.0));
	fColor=vec4(rgb,Color.a);
 }
