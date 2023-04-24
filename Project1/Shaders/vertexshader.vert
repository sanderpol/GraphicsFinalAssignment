#version 430 core

//input var
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

// Uniform matrices
uniform mat4 camMat;
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
    vec3 currenpos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
    vs_out.Pos = currenpos;

    vs_out.Norms = aNormal;

    vs_out.Color = aColor;
    
    vs_out.TexCor = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

    gl_Position = (camMat * vec4(currenpos, 1.0f));
}
