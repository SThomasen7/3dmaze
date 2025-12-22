#include "shader_loader.h"
#include "opengl_headers.h"
#include "consts.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// Util function declaration
std::string read_shader_file(std::string filename);
GLuint get_shader(GLenum type, std::string filename);
GLuint create_program(std::string vertex, std::string fragment);
bool compile_shader(GLuint& shader, std::string filename, std::string &shader_code);

// Shader Loader class functions
ShaderComponent ShaderLoader::load(
    std::string vertex_name, std::string fragment_name){

  ShaderComponent shader;
  shader.program = create_program(vertex_name, fragment_name);

  return shader;
}

// Util functions
std::string read_shader_file(std::string filename){
  std::ifstream t(asset_path+std::string("shaders/")+filename);
  if (!t.is_open()) {
    LOG(LL::Error, "File: ", filename, " could not be opened!");
    return "";
  }
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

GLuint get_shader(GLenum type, std::string filename){
  GLuint shader;
  shader = glCreateShader(type);
  std::string shader_code = read_shader_file(filename);
  if(!compile_shader(shader, filename, shader_code)){
    LOG(LL::Error, "Failed to compile shader: ", filename);
    return 0;
  }
  return shader;
}

bool compile_shader(GLuint& shader, std::string filename, std::string &shader_code) {
  const char *source_str = shader_code.c_str();
  glShaderSource(shader, 1, &source_str, NULL);
  CHECK_OGL_ERROR();
  glCompileShader(shader);
  CHECK_OGL_ERROR();

  // Get compile state
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  CHECK_OGL_ERROR();

  // Log if error
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    CHECK_OGL_ERROR();
    LOG(LL::Error, "ERROR::SHADER::COMPILATION_FAILED: (",
        filename, "): \n", infoLog);
    return false;
  }
  return true;
}

GLuint create_program(std::string vertex, std::string frag) {
  // Create program
  GLuint shaderProgram;
  shaderProgram = glCreateProgram();

  // compile shaders
  GLuint vertex_shader = get_shader(GL_VERTEX_SHADER, vertex);
  GLuint fragment_shader = get_shader(GL_FRAGMENT_SHADER, frag);
  glAttachShader(shaderProgram, vertex_shader);
  CHECK_OGL_ERROR();
  glAttachShader(shaderProgram, fragment_shader);
  CHECK_OGL_ERROR();

  // Link program
  glLinkProgram(shaderProgram);
  CHECK_OGL_ERROR();

  // Clean up shaders
  glDeleteShader(vertex_shader);
  CHECK_OGL_ERROR();
  glDeleteShader(fragment_shader);
  CHECK_OGL_ERROR();

  // Error check
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  CHECK_OGL_ERROR();

  // Log if error
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    CHECK_OGL_ERROR();
    LOG(LL::Error, "ERROR::SHADER::LINK: (", vertex,
        ", ", frag, ") :", infoLog);
    return 0;
  }
  return shaderProgram;
}


