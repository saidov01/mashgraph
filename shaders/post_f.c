#version 330 core

in vec2 tex_coord;

out vec4 color;

uniform sampler2D screen_texture;

void main(){   
    vec4 tmp_color =  texture(screen_texture, tex_coord);
    color = vec4(tmp_color.x, 0.0, 0.0, 1.0f);
}

/*  float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);   
float kernel[9] = float[](
    1.0 ,  1.0, 1.0,
     1.0, -7.0 , 1.0,
    1.0 ,  1.0, 1.0  
);       */
