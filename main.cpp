////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Contains main function to create a window and run engine that
///        repeatedly generates a framebuffer and displays it.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>


// headers
#include "CompileShaders.h"
#include "Render/RayTracing.h"
#include "Render/Rasterization.h"
#include "ObjParser.h"


////////////////////////////////////////////////////////////////////////////////
// Global variables - avoid these

// Window
int g_width{1360};
int g_height{768};
int g_window{0};


// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{
  std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};
bool g_animation{false};

// global data
std::shared_ptr<Render> g_render;


////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void
initialize(char** _argv) {

  std::ifstream i(_argv[1]);
  json g_j;
  i >> g_j;
  if (g_j.at("RenderOption") == "RayTracing")
    g_render = std::make_shared<RayTracing>(g_j, g_width, g_height);
  else {
    g_render = std::make_shared<Rasterization>(g_j);
  }

  g_render->initialize();

}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for resize of window
///
/// Responsible for setting window size (viewport) and projection matrix.
void 
resize(GLFWwindow* window, int _w, int _h) {
  g_width = _w;
  g_height = _h;

  // Viewport
  glfwGetFramebufferSize(window, &g_width, &g_height);
  glViewport(0, 0, g_width, g_height);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief Timer function to fix framerate in a GLUT application
/// @param _v Value (not used here)
///
/// Note, this is rudametary and fragile.
// void
// timer(int _v) {
//   if(g_window != 0) {
//     //glutPostRedisplay();

//     g_delay = std::max(0.f, 1.f/FPS - g_frameRate);
//     //glutTimerFunc((unsigned int)(1000.f*g_delay), timer, 0);
//   }
//   else
//     exit(0);
// }

////////////////////////////////////////////////////////////////////////////////
/// @brief Draw function for single frame
void
draw(GLFWwindow* window) {
  using namespace std::chrono;
  //////////////////////////////////////////////////////////////////////////////
  // Record frame time
  high_resolution_clock::time_point time = high_resolution_clock::now();
  g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
  g_frameTime = time;
  g_framesPerSecond = 1.f/(g_delay + g_frameRate);
  //printf("FPS: %6.2f\n", g_framesPerSecond);

  //////////////////////////////////////////////////////////////////////////////
  // Show
  if(g_animation)
    g_render->update(g_frameRate);
  g_render->draw(window);

  
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    g_animation = true;
  // else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
  //   g_animation = false;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void error_callback(int error, const char* description){
    std::cout << error << ":" << description << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Main

////////////////////////////////////////////////////////////////////////////////
/// @brief main
/// @param _argc Count of command line arguments
/// @param _argv Command line arguments
/// @return Application success status
int 
main(int _argc, char** _argv) {
  //////////////////////////////////////////////////////////////////////////////
  // Verify GLAD initialization for function pointers
  // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  //   std::cout << "Failed to initialize GLAD" << std::endl;
  //   return -1;
  // }    

  //////////////////////////////////////////////////////////////////////////////
  // Instantiate GLFW window
  glfwSetErrorCallback(error_callback);
  std::cout << "Initializing GLFW\n" << std::endl;
  if (!glfwInit()) { 
    std::cout << "GLFW Initialization Failed" << std::endl;  
    return -1;
  } 

  std::ifstream i(_argv[1]);
  json g_j;
  i >> g_j;
  if (g_j.at("RenderOption") == "Rasterization") {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    fprintf(stderr, "OpenGL version recieved: %d.%d.%d", major, minor, rev);
  }
  //////////////////////////////////////////////////////////////////////////////
  // Create GLFW window object
  GLFWwindow* window = glfwCreateWindow(g_width, g_height, "Spiderling: A Rudamentary Game Engine", NULL, NULL);
  if (!window) {
    std::cout << "GLFWWindow Initialization Failed" << std::endl;  
    glfwTerminate(); 
    return -1;
  }
  glfwMakeContextCurrent(window);       // Makes a new context to do work on
  glfwGetFramebufferSize(window, &g_width, &g_height);
  glViewport(0, 0, g_width, g_height);  // Initialize viewport

  // GL
  initialize(_argv);

  //////////////////////////////////////////////////////////////////////////////
  // Assign callback functions
  std::cout << "Assigning Callback functions" << std::endl;
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetFramebufferSizeCallback(window, resize);  
  // glutTimerFunc(1000/FPS, timer, 0);   // https://discourse.glfw.org/t/is-there-any-timer-callback-in-glfw/1155/4

  //////////////////////////////////////////////////////////////////////////////
  // Start application
  std::cout << "Starting Application" << std::endl;
  
  while (!glfwWindowShouldClose(window)) {  //close window
      draw(window);
      glfwSwapBuffers(window); //swap buffer
      glfwPollEvents();
  }

  //////////////////////////////////////////////////////////////////////////////
  // End Application
  glfwDestroyWindow(window); // Destroys window
  glfwTerminate();

  return 0;
}

#if   defined(OSX)
#pragma clang diagnostic pop
#endif
