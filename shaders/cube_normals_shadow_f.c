#version 330 core

out vec4 result_color;

/*
in Ver_out {
    vec3 frag_pos;
    vec2 tex_coords;
    mat3 tbn;
    //vec3 normal;
}ver_in;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal_map;
    float shininess;
};
 
struct Light{
    float constant;
    float linear;
    float quadratic;
    vec3 light_pos;
    vec3 view_pos;
};

uniform Material material;
uniform Light light_param;
*/
void main()
{

    //vec3 normal = t;
    //normal = normalize(normal * 2.0f - 1.0f);
    //vec3 normal = ver_in.normal;
/*
    vec3 normal = texture(material.normal_map, ver_in.tex_coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(ver_in.tbn * normal);  

    vec3 color = texture(material.diffuse, ver_in.tex_coords).rgb;
    vec3 ambient = 0.40f * color;


    vec3 light_dir = normalize(light_param.light_pos - ver_in.frag_pos);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse = diff * color;


    vec3 view_dir = normalize(light_param.view_pos - ver_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
    vec3 specular = texture(material.specular, ver_in.tex_coords).rgb * spec;



    float distance = length(light_param.light_pos -  ver_in.frag_pos);
    float attenuation = 1.0 / (light_param.constant + light_param.linear * distance + light_param.quadratic * (distance * distance));

    vec3 result = ambient + diffuse + specular;

    result = result * attenuation;
    result_color = vec4(result, 1.0f);
*/
}







