#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "model.h"
#include "render.h"

struct Camera{
    bool state[4]; // a w d s
    glm::vec3 camera_pos;
    glm::vec3 camera_front;
    glm::vec3 camera_up;
    GLfloat delta_time;
    GLfloat last_time;
    GLfloat yaw;
    GLfloat pitch;
    GLfloat last_x;
    GLfloat last_y;
    bool first_mouse, post_effect;
    float screen_width, screen_height;
}camera;

int funccmp_win( const void * val1, const void * val2 ){
    glm::vec3 a = *(glm::vec3 *)(val1);
    glm::vec3 b = *(glm::vec3 *)(val2);
    float dis1 = glm::length(camera.camera_pos - a);
    float dis2 = glm::length(camera.camera_pos - b);
    if(dis1 > dis2)
        return 0;
    else
        return 1;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_W){
        if(action == GLFW_PRESS){
            camera.state[1] = true;
        }else if(action == GLFW_RELEASE){
            camera.state[1] = false;
        }
    }
    if(key == GLFW_KEY_S){
        if(action == GLFW_PRESS){
            camera.state[3] = true;
        }else if(action == GLFW_RELEASE){
            camera.state[3] = false;
        }
    }
    if(key == GLFW_KEY_A){
        if(action == GLFW_PRESS){
            camera.state[0] = true;
        }else if(action == GLFW_RELEASE){
            camera.state[0] = false;
        }
    }
    if(key == GLFW_KEY_D){
        if(action == GLFW_PRESS){
            camera.state[2] = true;
        }else if(action == GLFW_RELEASE){
            camera.state[2] = false;
        }
    }
    if(key == GLFW_KEY_E){
        if(action == GLFW_PRESS){
            if(camera.post_effect == false){
                camera.post_effect = true;
            }else{
                camera.post_effect = false;
            }
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(camera.first_mouse){
        camera.last_x = xpos;
        camera.last_y = ypos;
        camera.first_mouse = false;
    }
  
    GLfloat xoffset = xpos - camera.last_x;
    GLfloat yoffset = camera.last_y - ypos; 
    camera.last_x = xpos;
    camera.last_y = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    camera.yaw   += xoffset;
    camera.pitch += yoffset;
    if(camera.pitch > 89.0f){
        camera.pitch = 89.0f;
    }else   if(camera.pitch < -89.0f){
        camera.pitch = -89.0f;
    }
}


int camera_step(){
    GLfloat new_time = glfwGetTime();
    camera.delta_time = new_time - camera.last_time;
    camera.last_time = new_time;
    GLfloat speed = 0.65f * camera.delta_time;

    glm::vec3 dirfront;
    dirfront.x = cos(camera.pitch * 0.017453f) * cos(camera.yaw * 0.017453f);
    dirfront.y = sin(camera.pitch * 0.017453f);
    dirfront.z = cos(camera.pitch * 0.017453f) * sin(camera.yaw * 0.017453f);
    camera.camera_front = glm::normalize(dirfront);

    if(camera.state[1] == true)
        camera.camera_pos += speed * camera.camera_front;
    if(camera.state[3] == true)
        camera.camera_pos -= speed * camera.camera_front;
    if(camera.state[0] == true)
        camera.camera_pos -= glm::normalize(glm::cross(camera.camera_front, camera.camera_up)) * speed;
    if(camera.state[2] == true)
        camera.camera_pos += glm::normalize(glm::cross(camera.camera_front, camera.camera_up)) * speed;
    return 0;
}

int main(){
    camera.screen_width = 1450;
    camera.screen_height = 900;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(camera.screen_width, camera.screen_height, "OpenGL_3.3", nullptr, nullptr);
    if (window == nullptr){
	    std::cout << "ERROR create GLFW window" << std::endl;
	    glfwTerminate();
	    return 0;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;  // FOR UNLOCK expansion
    GLenum err = glewInit();
    if (err != GLEW_OK){
        std::cout << "ERROR glewInit()  " << glGetError() << std::endl;
        return 0;
    }
/***
***/
    int win_width, win_height;
    glfwGetFramebufferSize(window, &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


/**** Camera **/
camera.first_mouse = true;
camera.last_time = glfwGetTime();
camera.delta_time = 0;
for(int i = 0; i < 4; i++){
    camera.state[i] = false;
}
camera.yaw   = -90.0f;
camera.pitch = 0.0f;
camera.last_x = 400;
camera.last_y = 300;
camera.camera_pos   = glm::vec3(2.0f, 0.0f,  2.0f);
camera.camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
camera.camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);
/****/
/**** SHADERS *****/
GLuint light_sh = compile_shader("../shaders/cube_light_v.c", "../shaders/cube_light_f.c");
GLuint light_vao = set_light_vao();
GLuint sky_texture, sky_VAO;
GLuint sky_sh = compile_shader("../shaders/sky_map_v.c", "../shaders/sky_map_f.c");
set_sky_box(sky_texture, sky_VAO);
 
GLuint mirror_sh = compile_shader("../shaders/mirror_v.c", "../shaders/mirror_f.c");
GLuint mirror_vao = set_mirror_vao();

GLuint bilb_texture = loaad_bilboard_texture();
GLuint bilb_vao = bilbord_set_vao();
GLuint bilb_sh = compile_shader("../shaders/bilbord_v.c", "../shaders/bilbord_f.c");

camera.post_effect = false;
GLuint post_vao, post_texture, post_buffer;
set_post_vao( camera.screen_width,  camera.screen_height,  post_buffer,  post_texture,  post_vao);
GLuint post_sh = compile_shader("../shaders/post_v.c", "../shaders/post_f.c");

/******/

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        camera_step();
        glClearColor(0.9f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

if(camera.post_effect){
    glBindFramebuffer(GL_FRAMEBUFFER, post_buffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
}

        glm::mat4 model(1.0f);
        glm::mat4 view = glm::lookAt(camera.camera_pos, camera.camera_pos + camera.camera_front, camera.camera_up);
        glm::mat4 projection = glm::perspective(0.78539f, camera.screen_width / camera.screen_height, 0.1f, 100.0f);
        
        drow_light(light_vao, light_sh, model, view, projection);

        drow_mirror_box(mirror_vao,  mirror_sh,  glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 5.0f)), view,  projection, sky_texture,  camera.camera_pos);


        drow_sky_box(sky_VAO, sky_sh, model, view,  projection,  sky_texture);
        drow_bilbords(  bilb_sh ,  bilb_vao, view, projection,  bilb_texture, camera.camera_pos);



if(camera.post_effect){
    drow_post_effect(post_vao, post_sh, post_texture);
}




        glfwSwapBuffers(window);
    }
    glfwTerminate();


    return 0;
}
