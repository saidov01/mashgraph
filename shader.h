#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

std::string read_code(const char * code_path){
    if(code_path == NULL){
        std::cout << "ERROR no path" << std::endl;
        return "ERROR_NULL_PATH";
    }
    std::string strg;
    std::string code = "";
    std::ifstream file;
    file.open(code_path);
    if(file.is_open() == 0){
        std::cout << "ERROR_OPEN_FILE " << code_path << std::endl;
        return "ERROR_OPEN_FILE";
    }
    while(file.eof() != true){
        getline(file, strg);
        code += strg;
        code += " \n";
    }
    file.close();
    return code;
}

GLuint compile_shader(const char * vertex_path, const char * fragment_path){
    GLuint program;
    std::string vertex_code;
    std::string fragment_code;

    vertex_code = read_code(vertex_path);
    fragment_code = read_code(fragment_path);
    const GLchar * glchar_vertex_code = vertex_code.c_str();
    const GLchar * glchar_fragment_code = fragment_code.c_str();

    GLuint v_sh, f_sh;
    GLint GLsuccess;
    GLchar infoLog[512];
    v_sh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_sh, 1, &glchar_vertex_code, NULL);
    glCompileShader(v_sh);
    glGetShaderiv(v_sh, GL_COMPILE_STATUS, &GLsuccess);
    if(!GLsuccess){
        glGetShaderInfoLog(v_sh, 512, NULL, infoLog);
        std::cout << "ERROR_SHADER_VERTEX_COMPILATION_FAILED\n " << vertex_path << ' ' << fragment_path << '\n' << infoLog << std::endl;
    }
    f_sh = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(f_sh, 1, &glchar_fragment_code, NULL);
    glCompileShader(f_sh);
    glGetShaderiv(f_sh, GL_COMPILE_STATUS, &GLsuccess);
    if(!GLsuccess){
        glGetShaderInfoLog(f_sh, 512, NULL, infoLog);
        std::cout << "ERROR_SHADER_FRAGMENT_COMPILATION_FAILED\n " << vertex_path << ' ' << fragment_path << '\n' << infoLog << std::endl;
    }
    program = glCreateProgram();
    glAttachShader(program, v_sh);
    glAttachShader(program, f_sh);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &GLsuccess);
    if(!GLsuccess){
    glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR_LINK_SHADER_PROGRAM\n " << vertex_path << ' ' << fragment_path << '\n' << infoLog << std::endl;
    }
glDeleteShader(v_sh);
glDeleteShader(f_sh);
    return program;
}






#endif
