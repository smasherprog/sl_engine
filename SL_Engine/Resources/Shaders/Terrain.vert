#version 330

layout (location = 0) in vec3 position;

out vec4 fragmentColour;

uniform mat4 perspectiveMatrix;

void main()
{
    gl_Position = perspectiveMatrix * vec4(position, 1.0f);

    fragmentColour = vec4(1, 0, 1, 1);
}