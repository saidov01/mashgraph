#version 330 core
out vec4 FragColor;

in vec3 o_normal;
in vec3 o_position;

uniform vec3 camera_pos;
uniform samplerCube skybox;

void main()
{             
    vec3 i = normalize(o_position - camera_pos);
    vec3 r = reflect(i, normalize(o_normal));
    FragColor = vec4( mix(vec3(0.7529, 0.7529, 0.7529),  texture(skybox, r).rgb, 0.7), 1.0);
}
