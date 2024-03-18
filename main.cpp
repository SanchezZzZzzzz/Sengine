#define PY_SSIZE_T_CLEAN
#include <thread>
#include <iostream>
#include <string>
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
#include <Python.h>
#include "input/input.hpp"
#include "python/pythonscript.hpp"
#include "graphics/shader.hpp"
#include "graphics/draw.hpp"
#include "nodes/nodes.hpp"
#include "ui/ui.hpp"
#include "scenes.hpp"
#include "graphics/window.hpp"
#include "math/vector_maths.h"

static wchar_t* program;
static Window* current_window;
int main(int argc, char*argv[]) {
    program = Py_DecodeLocale(argv[0], NULL);
    Window new_window(1024, 768, "Window", input);
    current_window = &new_window;
    new_window.render();
    return 0;
}
