#version 330 core

out vec4 result_color;

in Ver_out {
    vec3 frag_pos;
    vec2 tex_coords;
    mat3 tbn;
    vec4 shadow_position;
    //vec3 normal;
    mat3 inv_tbn;
}ver_in;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal_map;
    float shininess;
    sampler2D shadow_map;
    bool outline;
    sampler2D height_map;
    bool paralax;
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

float calc_shadow(vec3 normal, vec3 light_dir){
    vec3 proj = ver_in.shadow_position.xyz / ver_in.shadow_position.w;
    proj = proj * 0.5f + 0.5f;
    if(proj.z > 1.0f){
        return 0.0f;
    }
    float real_depth = proj.z;
    float shadow = 0.0f;
    float bias = max(0.12f * (1.0f - dot(normal, light_dir)), 0.006f);
    vec2 texelSize = 1.0f / textureSize(material.shadow_map, 0);
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            float tmp = texture(material.shadow_map, proj.xy + vec2(x, y) * texelSize).r;
            if(real_depth - bias > tmp)
                shadow += 1.0f;
        }
    }
    shadow /= 9.0f;
//    if(real_depth - bias > depth)
//        shadow = 1.0;
//    else
//        shadow = 0.0f;
    //float depth = texture(material.shadow_map, proj.xy).r; 
    return shadow;
}


vec2 ParallaxMapping(vec2 tex_coords, vec3 view_dir){
    float height_scale = 0.018f; //0.01
    const float num_layers = 10.0f; //30
    float layer_depth = 1.0f / num_layers;
    float current_l_depth = 0.0f;
    vec2 vec_shift = (view_dir.xy / 1.0f) * height_scale; 
    vec2 delta_coords = vec_shift / num_layers;
    vec2  current_coords  = tex_coords;
    //float current_depth_value = texture(material.height_map, current_coords).r;      
    float current_depth_value = 1.0f - texture(material.height_map, current_coords).r;  
    while(current_l_depth < current_depth_value){
        current_coords -= delta_coords;
        //current_depth_value = texture(material.height_map, current_coords).r;  
        current_depth_value = 1.0f - texture(material.height_map, current_coords).r;  
        current_l_depth += layer_depth;  
    }
    delta_coords *= 0.5f;
    layer_depth *= 0.5f;
    current_coords += delta_coords;
    current_l_depth -= layer_depth;  
    //int iter = 5;
    for(int iter = 5; iter > 0; iter--){
        //current_depth_value = texture(material.height_map, current_coords).r;  
        current_depth_value = 1.0f - texture(material.height_map, current_coords).r;  
        delta_coords *= 0.5f;
        layer_depth *= 0.5f;
        if(current_depth_value > current_l_depth){
            current_coords -= delta_coords;
            current_l_depth += layer_depth;
        }else{
            current_coords += delta_coords;
            current_l_depth -= layer_depth;
        }
    }
    return current_coords; 
}

void main()
{
    vec2 new_tex_coords;
    if(material.paralax){
        vec3 view_dir_tbn = normalize(ver_in.inv_tbn * normalize(light_param.view_pos - ver_in.frag_pos));
        //view_dir_tbn.x *= -1.0f;
        new_tex_coords = ParallaxMapping(ver_in.tex_coords,  view_dir_tbn);
    }else{
        new_tex_coords = ver_in.tex_coords;
    }

    vec3 normal = texture(material.normal_map, new_tex_coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(ver_in.tbn * normal);  

    vec3 color = texture(material.diffuse, new_tex_coords).rgb;
    vec3 ambient = 0.30f * color;


    vec3 light_dir = normalize(light_param.light_pos - ver_in.frag_pos);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse = diff * color;


    vec3 view_dir = normalize(light_param.view_pos - ver_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
    vec3 specular = texture(material.specular, new_tex_coords).rgb * spec;



    float distance = length(light_param.light_pos -  ver_in.frag_pos);
    float attenuation = 1.0 / (light_param.constant + light_param.linear * distance + light_param.quadratic * (distance * distance));

    float shadow = calc_shadow(normal, light_dir);

    vec3 result = ambient + (diffuse + specular)*(1.0f - shadow);

    result = result * attenuation;
    if(material.outline)
        result_color = vec4(result.r, 0.0f, 0.0f, 1.0f);
    else
        result_color = vec4(result, 1.0f);
}



/*
void main()
{
        vec3 view_dir_tbn  = normalize( ver_in.inv_tbn*(light_param.view_pos - ver_in.frag_pos));
        vec2 new_tex_coords = ParallaxMapping(ver_in.tex_coords,  view_dir_tbn);


    vec3 normal = texture(material.normal_map, ver_in.tex_coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(ver_in.tbn * normal);  

    vec3 color = texture(material.diffuse, ver_in.tex_coords).rgb;
    vec3 ambient = 0.30f * color;


    vec3 light_dir = normalize(light_param.light_pos - ver_in.frag_pos);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse = diff * color;


    vec3 view_dir = normalize(light_param.view_pos - ver_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
    vec3 specular = texture(material.specular, ver_in.tex_coords).rgb * spec;



    float distance = length(light_param.light_pos -  ver_in.frag_pos);
    float attenuation = 1.0 / (light_param.constant + light_param.linear * distance + light_param.quadratic * (distance * distance));

    float shadow = calc_shadow(normal, light_dir);

    vec3 result = ambient + (diffuse + specular)*(1.0f - shadow);

    result = result * attenuation;
    if(material.outline)
        result_color = vec4(result.r, 0.0f, 0.0f, 1.0f);
    else
        result_color = vec4(result, 1.0f);
}
*/


