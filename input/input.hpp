static GLFWwindow* CURRENT_WINDOW;

class Input{
private:
    float m_x, m_y;
public:
    double mouse_x = 0, mouse_y = 0;
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
    double getWindowSizeX(){
        return this->m_x;
    }
    float getWindowSizeY(){
        return m_y;
    }
    void setWindowSize(float x, float y){
        this->m_x = x;
        this->m_y = y;
    }
};
Input input;