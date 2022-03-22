#version 330 core

in vec3 colorOut;
in vec3 normalOut;
in vec3 posOut;

out vec4 FragColor;

uniform vec3 lightPos;

void main()
{
   vec3 lightDir = normalize(lightPos - posOut);
   float coeff = max(dot(lightDir, normalOut), 0.1f);
   vec3 color = coeff * colorOut;
   FragColor = vec4(color, 1.0f);
}