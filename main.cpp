#define SET_CURSOR_VISIBLE 1
#define SET_CURSOR_HIDDEN 0
#define SET_CURSOR_DISABLED 2
//#include <signal.h>
//#include <thread>
#include <iostream>
//#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include "utility/error_callback.hpp"
#include "input/input.hpp"
#include "python/pythonscript.hpp"
#include "graphics/shader.hpp"
#include "graphics/draw.hpp"
#include "nodes/nodes.hpp"
#include "ui/ui.hpp"
#include "scenes.hpp"
#include "graphics/window.hpp"
#include "math/vector_maths.h"

int main() {
    glfwSetErrorCallback(&Error);
    Window new_window(1920, 1080, "Window");
    new_window.render();
    return 0;
}
