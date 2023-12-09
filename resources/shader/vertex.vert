#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inColor;
out vec4 vert;
out vec3 norm;
out vec3 color;
out vec3 lightDir;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;

void main()
{
    vert  = mvMatrix * vec4(vertex, 1.0);
    norm  = transpose(inverse(mat3(mvMatrix))) *  normal;
    color = inColor;
    lightDir = normalize(vec3(mvMatrix * vec4(1, 0, 1, 0)));
    gl_Position = projMatrix * mvMatrix * vec4(vertex, 1.0);

//    posWorld = vec3(model*(vec4(posObject,1.0)));
//    normalWorld = normalize( (inverse(transpose(mat3(model)))) * normalize(normalObject)); //
//    gl_Position = proj*view*model*(vec4(posObject,1.0));
}
