#version 430 core

out vec4 FragColor;

// Input from vertex shader
in VS_OUT
{
   vec3 Pos;
   vec3 Norms;
   vec3 Color;
   vec2 TexCor;
} vs_out;


//
uniform sampler2D diffuse0;
uniform sampler2D specular0;


// Material properties
uniform vec3 lightPos;
uniform vec4 lightColor;
uniform vec3 camPos;

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(vs_out.Norms);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - vs_out.Pos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float finalSpec = specAmount * specularLight;

	return (texture(diffuse0, vs_out.TexCor) * (diffuse + ambient) + texture(specular0, vs_out.TexCor).r * finalSpec) * lightColor;
}

void main()
{   
//    //ambient light
//    float ambient = 0.2f;
//        
//    vec3 norm = normalize(fs_in.Normal);
//    vec3 lightDir = normalize(lightPos);
//    float diffuse = max(dot(norm, lightDir), 0.0f);
//
//    float specLight = 0.5f;
//    vec3 viewDir = normalize(camPos - fs_in.Pos);
//    vec3 reflectionDir = reflect(-lightDir, norm);
//    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 16);
//    float finalSpecular = specAmount * specLight;
//
//    vec4 diffuseTex = texture(diffuse0, fs_in.TexCor);
//    float ambientDiffuse = diffuse + ambient;
//    float specularTex = texture(specular0, fs_in.TexCor).r * finalSpecular;
	FragColor = direcLight();

}
