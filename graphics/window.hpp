GLuint vertex_array_object;
class Window{
private:
    float m_x, m_y;
    double m_mouse_x, m_mouse_y;
    bool m_closed = 0;
    GLFWwindow* m_window;
public:
    Window(unsigned short x, unsigned short y, const char window_name[]) {
        glewExperimental = GL_TRUE;
        if (GL_TRUE != glfwInit())
            fprintf(stderr, "Error!");
        this->m_window = glfwCreateWindow(x, y, window_name, NULL, NULL);
        if (this->m_window == NULL){
            fprintf(stderr, "Failed to open GLFW window\n");
        }
        glfwMakeContextCurrent(m_window);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK){
            fprintf(stderr, "glewInit is not OK\n");
        }
        else{
            glGenVertexArrays(1, &vertex_array_object);
            glBindVertexArray(vertex_array_object);
        }
        CURRENT_WINDOW = this->m_window;
        window_properties.setWindowSize(x, y);
    }

    void render();
    void close() {
        m_closed = 1;
    }
    void set_orthogonal(GLfloat x_start, GLfloat x_end, GLfloat y_start, GLfloat y_end, GLfloat z_near, GLfloat z_far, GLuint &programID){
        glm::mat4 projection = glm::ortho(x_start, x_end, y_start, y_end, z_near, z_far);
        glm::mat4 mvp = projection * CURRENT_CAMERA->getMatrix();
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    }
};
void Window::render(){
        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
        glClearColor(0.0f, 0.05, 0.1f, 0.0f);
        glfwSetCursorPos(CURRENT_WINDOW, 0, 0);
        glfwWindowHint(GLFW_SAMPLES, 8);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_FRONT);
        scn::sceneSetup();    
        do{
            input.getMousePosition();
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glfwSetInputMode(CURRENT_WINDOW, GLFW_STICKY_KEYS, GLFW_TRUE);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            scn::scene();
            glfwSetCursorPos(CURRENT_WINDOW, 0, 0);     
            glFlush();
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
        while(glfwWindowShouldClose(m_window) == 0 && m_closed == 0);
    }

