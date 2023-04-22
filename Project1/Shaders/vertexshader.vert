#version 430 core

//input var
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uvs;

// Uniform matrices
uniform mat4 camPos;
uniform mat4 model;

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;




out VS_OUT
{
   vec3 Pos;
   vec3 Norms;
   vec3 Color;
   vec2 TexCor;
} vs_out;

void main()
{
    //use a negative rotation gltf files use counterclockwise rotation instead of opengl clockwise rotation.
    vs_out.Pos = vec3(model * translation * -rotation * scale * vec4(pos, 1.0f));

    vs_out.Norms = normal;

    vs_out.Color = color;
    
    vs_out.TexCor = mat2(0.0, -1.0, 1.0, 0.0) * uvs;

    gl_Position = (camPos * vec4(vs_out.Pos, 1.0f));
}
