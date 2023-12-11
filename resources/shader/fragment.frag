#version 330 core
in vec4 vert;
in vec3 norm;
in vec3 color;
in vec3 lightDir;

uniform bool wireshade;
uniform bool cell;

uniform sampler2D textureSampler;

out  vec4 fragColor;

void main(void)
{
    if (wireshade) {
        fragColor = vec4(0, 0, 0 ,1);
    } else {
        if (cell){
            float lambert = dot(norm, lightDir);
            float toon = 0.5 * smoothstep(0.66, 0.67, lambert) + 0.5;
            float outline = smoothstep(0.2, 0.21, norm.z);
            vec3 finalColor = color * outline * toon * vec3(0.8, 0.8, 0.8);  // RGB values here, (0.7, 0.3, 0.5);
            fragColor = vec4(finalColor, 1.0);
        }
        else{
            // BUMP MAPPING ATTEMPT
            // fragColor = texture(textureSampler, fragUV);
            vec3 objColor = color;
            fragColor = vec4((clamp(dot(norm, lightDir), 0, 1) * 0.7 +  0.3) * objColor, 1.0);
        }
    }
}
