#ifndef _HEADERS_H_
#define _HEADERS_H_ 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stdio.h"
#include <string>
#include "logger.h"

using std::string;

#define CHECK_OGL_ERROR() CHECK_ERROR_R(__FILE__, __LINE__)
inline bool CHECK_ERROR_R(const char* file, int line) {
//#ifdef DEBUG
  GLenum err = glGetError();
  if(err != GL_NO_ERROR){
    string out = file+string("-")+std::to_string(line)+string(": ErrorCode ");

    if(err == GL_INVALID_ENUM){
      out += "GL_INVALID_ENUM";
    }
    else if(err == GL_INVALID_VALUE){
      out += "GL_INVALID_VALUE";
    }
    else if(err == GL_INVALID_OPERATION){
      out += "GL_INVALID_OPERATION";
    }
    else if(err == GL_STACK_OVERFLOW){
      out += "GL_STACK_OVERFLOW";
    }
    else if(err == GL_STACK_UNDERFLOW){
      out += "GL_STACK_UNDERFLOW";
    }
    else if(err == GL_OUT_OF_MEMORY){
      out += "GL_OUT_OF_MEMORY";
    }
    else{
      out += "unknown";
    }
    LOG(LL::Error, out);
    return true;
  }
//#endif
  return false;
}

inline void GLAPIENTRY OpenGLErrorMessageCallback(GLenum source, GLenum type,
    GLuint id, GLenum severity, GLsizei length, const GLchar* message, 
    const void* userParam){
  string source_str = "Unknown";
  string type_str = "Unknown";
  string sev_str = "Unknown";

  switch(source){
    case GL_DEBUG_SOURCE_API:
      source_str = "GL_DEBUG_SOURCE_API";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      source_str = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      source_str = "GL_DEBUG_SOURCE_SHADER_COMPILER";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      source_str = "GL_DEBUG_SOURCE_THIRD_PARTY";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      source_str = "GL_DEBUG_SOURCE_APPLICATION";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      source_str = "GL_DEBUG_SOURCE_OTHER";
      break;
    default:
      break;
  }

  switch(type){
    case GL_DEBUG_TYPE_ERROR:
      type_str = "GL_DEBUG_TYPE_ERROR";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      type_str = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      type_str = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      type_str = "GL_DEBUG_TYPE_PORTABILITY";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      type_str = "GL_DEBUG_TYPE_PERFORMANCE";
      break;
    case GL_DEBUG_TYPE_MARKER:
      type_str = "GL_DEBUG_TYPE_MARKER";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      type_str = "GL_DEBUG_TYPE_PUSH_GROUP";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      type_str = "GL_DEBUG_TYPE_POP_GROUP";
      break;
    case GL_DEBUG_TYPE_OTHER:
      type_str = "GL_DEBUG_TYPE_OTHER";
      break;
    default:
      break;
  }

  switch(severity){
    case GL_DEBUG_SEVERITY_HIGH:
      sev_str = "GL_DEBUG_SEVERITY_HIGH";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      sev_str = "GL_DEBUG_SEVERITY_MEDIUM";
      break;
    case GL_DEBUG_SEVERITY_LOW:
      sev_str = "GL_DEBUG_SEVERITY_LOW";
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      sev_str = "GL_DEBUG_SEVERITY_NOTIFICATION";
      break;
    default:
      break;
  }

  if(severity == GL_DEBUG_SEVERITY_NOTIFICATION){
    return;
  }

  LOG(LL::Info, "DEBUG ", source_str, " ", type_str, " ",
      sev_str, message);
}

#endif
