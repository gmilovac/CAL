#version 330 core
in vec4 vert;
in vec3 norm;
in vec3 color;
in vec3 lightDir;

uniform bool wireshade;

//uniform vec3 offset;

out  vec4 fragColor;

void main(void)
{
    //norm = norm + random()
    if (wireshade) {
        fragColor = vec4(color,1);
    } else {
        vec3 objColor = color;
        fragColor = vec4((clamp(dot(norm, lightDir), 0, 1) * 0.7 +  0.3) * objColor, 1.0);
    }
}
