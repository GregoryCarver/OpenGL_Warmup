#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 colorIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 colorOut;
out vec3 normalOut;
out vec3 posOut;

void main()
{
    //vec3 modelViewPos = vec3(view * model * vec4(aPos, 1.0f));
    //vec3 modelViewNorm = vec3(view * model * vec4(normalIn, 0.0f));
    //float coeff = max(dot(modelViewNorm, normalize(lightPos - modelViewPos)), 0.1);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);;
    posOut = vec3(view * model * vec4(aPos, 1.0f));
    colorOut = colorIn;
    normalOut = normalize(vec3(model * vec4(normalIn, 0.0f)));
}