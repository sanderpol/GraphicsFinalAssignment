#version 430 core

// Input from vertex shader
in VS_OUT
{
    vec3 Pos;
    vec3 Normal;
    vec3 Color;
    vec2 TexCor;
} fs_in;

// Material properties
uniform vec3 lightPos;
uniform vec4 lightColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform float ambient;
uniform vec3 camPos;
uniform float power;

void main()
{    
    vec3 lightVec = lightPos - fs_in.Pos;
    float lightDist = length(lightVec);
    float intensity = 1.0f / (3.0f * lightDist * lightDist * 0.7f * lightDist + 1.0f);


    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightVec);
    float diffuse = max(dot(norm, lightDir),0.0f) * intensity + ambient;

    float specLight = 0.5f;
    vec3 viewDir = normalize(camPos - fs_in.Pos);
    vec3 reflectionDir = reflect(-lightDir, fs_in.Normal);
    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0), power);
    float specular = specAmount * specLight;

    vec4 diffuseTex = texture(diffuse0, fs_in.TexCor);
    float ambientDiffuse = diffuse * intensity + ambient;
    float specularTex = texture(specular0, fs_in.TexCor).r * specular * intensity;

    gl_FragColor = (diffuseTex * ambientDiffuse * specularTex) * lightColor;

}
