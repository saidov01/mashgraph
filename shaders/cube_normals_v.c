#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 tangens;

out Ver_out {
    vec3 frag_pos;
    vec2 tex_coords;
    mat3 tbn;
    vec4 shadow_position;
    mat3 inv_tbn;
    //vec3 normal;
} ver_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 shadow_matrix;

void main()
{
    ver_out.frag_pos = vec3(model * vec4(position, 1.0));   
    ver_out.tex_coords = tex_coord;
    
    mat3 normal_matrix = transpose(inverse(mat3(model)));

    vec3 N = normalize(normal_matrix * normal);
    vec3 T = normalize(normal_matrix * tangens);
    vec3 B = normalize(cross(N, T));
    ver_out.tbn  = mat3(T, B, N);
    ver_out.inv_tbn = transpose(ver_out.tbn);

    ver_out.shadow_position = shadow_matrix * vec4(ver_out.frag_pos, 1.0);

//ver_out.normal = normalize(normal_matrix * normal);

    gl_Position = projection * view * model * vec4(position, 1.0);
} 


