#ifndef RENDER_H
#define RENDER_H

#include "my_stb.h"
#include "model.h"

GLuint set_light_vao(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_ver), cube_ver, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);

    return VAO;
}


int drow_light(GLuint vao, GLuint sh, glm::mat4 model,  glm::mat4 view, glm::mat4 projection){
    glUseProgram(sh);

    glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    return 0;
}


int set_sky_box(GLuint &sky_texture, GLuint &sky_VAO){
const char *  textured_cube[] = {
        "./texture/skybox/posx.jpg",
        "./texture/skybox/negx.jpg",
        "./texture/skybox/posy.jpg",
        "./texture/skybox/negy.jpg",
        "./texture/skybox/posz.jpg",
        "./texture/skybox/negz.jpg"     
        };
float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,    1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,     1.0f, -1.0f,  1.0f,     1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,     1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,     1.0f, -1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,     1.0f,  1.0f, -1.0f,    1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,     1.0f, -1.0f,  1.0f
};
    glGenTextures(1, &sky_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture);
    unsigned char *data;
    int tex_width , tex_height;
    for(unsigned int i = 0; i < 6; i++){
        //data = SOIL_load_image(textured_cube[i], &tex_width, &tex_height, 0, SOIL_LOAD_RGB);
        int n;
        data = stbi_load(textured_cube[i], &tex_width, &tex_height, &n, 0);
        if(data == NULL)
            std::cout << "ERROR SKY TEXTURE" << std::endl;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
         stbi_image_free(data);
        //SOIL_free_image_data(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &sky_VAO);
    glBindVertexArray(sky_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return 0;
}

int drow_sky_box(GLuint sky_VAO, GLuint sh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint sky_texture){
glDepthFunc(GL_LEQUAL); 
        glUseProgram(sh);  
        glBindVertexArray(sky_VAO);
        view = glm::mat4(glm::mat3(view));  
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0); 
glDepthFunc(GL_LESS); 
    return 0;
}

GLuint set_mirror_vao(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_for_cubemap), vertices_for_cubemap, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return VAO;
}

int drow_mirror_box(GLuint mirror_VAO, GLuint sh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint sky_texture, glm::vec3 cam_pos){
        glUseProgram(sh);  
        glBindVertexArray(mirror_VAO);
        glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(sh, "camera_pos"), 1, glm::value_ptr(cam_pos));   
        glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0); 

    return 0;
}


GLuint bilbord_set_vao(){
    GLuint VAO;
    float transparent_vertices[] = {
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparent_vertices), transparent_vertices, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return VAO;
}

GLuint loaad_bilboard_texture(){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    int  tex_width, tex_height, n;
    unsigned char * image = stbi_load("./texture/glass.png", &tex_width, &tex_height, &n, 0);
    //image = SOIL_load_image("./texture/glass.png", &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}


int funccmp_win( const void * val1, const void * val2 );
int drow_bilbords( GLuint sh , GLuint vao, glm::mat4 view, glm::mat4 projection, GLuint texture, glm::vec3 camera_pos){
    glm::vec3 win_pos[] = {
        glm::vec3(3.0f,  0.5f,  1.0f - 5.0f),
        glm::vec3(3.5f,  0.5f,  2.0f - 5.0f),
        glm::vec3(3.0f,  0.5f,  3.0f - 5.0f),
        glm::vec3(3.5f,  0.5f,  4.0f - 5.0f),
        glm::vec3(3.0f,  0.5f,  5.0f - 5.0f)
    };
    qsort(&win_pos, 5, sizeof(glm::vec3), funccmp_win);
    glUseProgram(sh);  
    for(int i = 0; i < 5; i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, win_pos[i]);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));      
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(sh, "Texture"), 0);
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vao); 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0); 
    }
    return 0;
}




int set_post_vao(int w, int h, GLuint &framebuffer, GLuint &tex_colorbuffer, GLuint &postVAO){
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  
    glGenTextures(1, &tex_colorbuffer);
    glBindTexture(GL_TEXTURE_2D, tex_colorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_colorbuffer, 0); 
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    float quad_vertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &postVAO);
    glBindVertexArray(postVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return 0;
}

int drow_post_effect(GLuint vao, GLuint sh, GLuint texture){

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(sh);  
    glBindVertexArray(vao);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(sh, "screen_texture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);  
    glEnable(GL_DEPTH_TEST);

    return 0;
}

struct Material{
    GLuint diffuse;
    GLuint specular;
    GLuint normal;
    GLuint shadow;
    bool outline;
    GLuint height;
    bool paralax;
};

//GLfloat myplane_vertices[] = {
//         15.0f, -0.0f,  15.0f, 0.0f, 0.1f, 0.0f, 6.0f, 0.0f,
//        -15.0f, -0.0f,  15.0f, 0.0f, 0.1f, 0.0f,  0.0f, 0.0f,
//        -15.0f, -0.0f, -15.0f, 0.0f, 0.1f, 0.0f,  0.0f, 6.0f,
//         15.0f, -0.0f,  15.0f, 0.0f, 0.1f, 0.0f,  6.0f, 0.0f,
//        -15.0f, -0.0f, -15.0f, 0.0f, 0.1f, 0.0f,  0.0f, 6.0f,
//         15.0f, -0.0f, -15.0f, 0.0f, 0.1f, 0.0f,  6.0f, 6.0f
//    };

GLfloat myplane_vertices[] = { 
     15,  0.0 ,  15 , 0 , 1 , 0 , 6 , 0 , -1 , 0 , 0 , 

    -15 , 0.0 , -15 , 0 , 1 , 0 , 0 , 6 , -1 , 0 , 0 , 
    -15 , 0.0 ,  15 , 0 , 1 , 0 , 0 , 0 , -1 , 0 , 0 , 
     15,  0.0 ,  15 , 0 , 1 , 0 , 6 , 0 , -1 , 0 , 0 , 

     15,  0.0 , -15 , 0 , 1 , 0 , 6 , 6 , -1 , 0 , 0 , 
    -15 , 0.0 , -15 , 0 , 1 , 0 , 0 , 6 , -1 , 0 , 0  } ;

GLfloat myplane_vertices_[] = { 
     15,  0.0 ,  15 , 0 , 1 , 0 , 6 , 0 , 1 , 0 , 0 , 
    -15 , 0.0 ,  15 , 0 , 1 , 0 , 0 , 0 , 1 , 0 , 0 , 
    -15 , 0.0 , -15 , 0 , 1 , 0 , 0 , 6 , 1 , 0 , 0 , 
     15,  0.0 ,  15 , 0 , 1 , 0 , 6 , 0 , 1 , 0 , 0 , 
    -15 , 0.0 , -15 , 0 , 1 , 0 , 0 , 6 , 1 , 0 , 0,
     15,  0.0 , -15 , 0 , 1 , 0 , 6 , 6 , 1 , 0 , 0  
  } ;

GLuint set_plane_vao(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(myplane_vertices), myplane_vertices, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));

    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}



GLuint set_cubes_vao(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_for_cube_n), vertices_for_cube_n, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}


GLuint load_diffuse_texture(){
    GLuint diffuse_texture;
    glGenTextures(1, &diffuse_texture);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int  tex_width, tex_height, n;
    unsigned char * image = stbi_load("./texture/Tiles_02/Tiles_02_basecolor.jpg", &tex_width, &tex_height, &n, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return diffuse_texture;
}

GLuint load_specular_texture(){
    GLuint specular_texture;
    glGenTextures(1, &specular_texture);
    glBindTexture(GL_TEXTURE_2D, specular_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int  tex_width, tex_height, n;
    unsigned char * image = stbi_load("./texture/Tiles_02/Tiles_02_glossiness.jpg", &tex_width, &tex_height, &n, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return specular_texture;
}

GLuint load_normal_texture(){
    GLuint normal_texture;
    glGenTextures(1, &normal_texture);
    glBindTexture(GL_TEXTURE_2D, normal_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int  tex_width, tex_height, n;
    unsigned char * image = stbi_load("./texture/Tiles_02/Tiles_02_normalOgl.jpg", &tex_width, &tex_height, &n, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return normal_texture;
}



GLuint load_height_texture(){
    GLuint height;
    glGenTextures(1, &height);
    glBindTexture(GL_TEXTURE_2D, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int  tex_width, tex_height, n;
    unsigned char * image = stbi_load("./texture/Tiles_02/Tiles_02_height.jpg", &tex_width, &tex_height, &n, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return height;
}




int my_drow_plane_or_cube(GLuint sh, GLuint VAO, glm::vec3 camera_pos, glm::vec3 light_pos, glm::mat4 model,  glm::mat4 view, glm::mat4 projection, Material material, 
glm::mat4 shadow_matrix, int num){
        glUseProgram(sh);

        glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));     
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(sh, "shadow_matrix"), 1, GL_FALSE, glm::value_ptr(shadow_matrix));

        glUniform3fv(glGetUniformLocation(sh, "light_param.view_pos"), 1, glm::value_ptr(camera_pos));        
        glUniform3fv(glGetUniformLocation(sh, "light_param.light_pos"), 1, glm::value_ptr(light_pos));   
        glUniform1f(glGetUniformLocation(sh, "light_param.constant"), 1.00f); 
        glUniform1f(glGetUniformLocation(sh, "light_param.linear"),  0.030f); 
        glUniform1f(glGetUniformLocation(sh, "light_param.quadratic"), 0.0050f );      
        glUniform1i(glGetUniformLocation(sh, "material.outline"),  material.outline);      
        glUniform1i(glGetUniformLocation(sh, "material.paralax"),  material.paralax);      
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.diffuse);
        glUniform1i(glGetUniformLocation(sh, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material.specular);
        glUniform1i(glGetUniformLocation(sh, "material.specular"), 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material.normal);
        glUniform1i(glGetUniformLocation(sh, "material.normal_map"), 2);
        glUniform1f(glGetUniformLocation(sh,"material.shininess"),  	0.4f * 128.0f);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, material.shadow);
        glUniform1i(glGetUniformLocation(sh, "material.shadow_map"), 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, material.height);
        glUniform1i(glGetUniformLocation(sh, "material.height_map"), 4);

        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, num);
        glBindVertexArray(0); 
        return 0;
}



int set_shadow_buff_and_texture(GLuint &buff, GLuint &texture, int w, int h){
    glGenFramebuffers(1, &buff);

glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,  w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
glm::vec4 bor_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(bor_color));

glBindFramebuffer(GL_FRAMEBUFFER, buff);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
glDrawBuffer(GL_NONE);
glReadBuffer(GL_NONE);
glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return 0;
}


#endif
