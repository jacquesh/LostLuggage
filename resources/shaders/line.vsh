#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewingMatrix;

void main()
{
    vec4 transformedPosition = projectionMatrix*viewingMatrix*vec4(position,1.0f);
    gl_Position = transformedPosition;
}
