static GLFWwindow* CURRENT_WINDOW;

class Input{
    double mouse_x = 0, mouse_y = 0;
public:
    
    void getMousePosition(){
        glfwGetCursorPos(CURRENT_WINDOW, &this->mouse_x, &this->mouse_y);
    }
    void setMouseMode(unsigned short mode){
        switch (mode) {
            case 0:
                glfwSetInputMode(CURRENT_WINDOW, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                break;
            case 1:
                glfwSetInputMode(CURRENT_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case 2:
                glfwSetInputMode(CURRENT_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                if (glfwRawMouseMotionSupported())
                    glfwSetInputMode(CURRENT_WINDOW, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
                else
                    printf("WARNING! RawMouseMotion is not supported!");
                break;
            default:
                glfwSetInputMode(CURRENT_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
        }
    }
    float getMouseX(){
        return mouse_x;
    }
    float getMouseY(){
        return mouse_y;
    }
};
Input input;