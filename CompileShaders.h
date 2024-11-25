////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Utilities for compiling shaders
////////////////////////////////////////////////////////////////////////////////
#ifndef _COMPILE_SHADERS_H_
#define _COMPILE_SHADERS_H_

// STL
#include <string>

// GL
#define GL_GLEXT_PROTOTYPES
#if   defined(OSX)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#elif defined(LINUX)
#include <GL/glut.h>
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief Compile a vertex shader and fragment shader together
/// @param _vertexShader Filename of vertex shader
/// @param _fragmentShader Filename of fragment shader
/// @return GL program identifier
GLuint compileProgram(const std::string& _vertexShader,
                      const std::string& _fragmentShader);

#if   defined(OSX)
#pragma clang diagnostic pop
#endif

#endif
