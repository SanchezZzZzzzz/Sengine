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
        input.setWindowSize(x, y);
    }

    void render();
    void close() {
        m_closed = 1;
    }
    void setPerspective(GLfloat angle, GLfloat width, GLfloat height, GLfloat close, GLfloat far, GLuint &programID){
        glm::mat4 projection = glm::perspective(glm::radians(angle), (float) width/height, close, far);
        glm::mat4 mvp = projection * CURRENT_CAMERA->getMatrix() * glm::mat4(1.f);
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
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
        glClearColor(0.0, 0.0, 0.2, 0.0f);
        glfwSetCursorPos(CURRENT_WINDOW, 0, 0);
        scn::sceneSetup();
        GLuint ProgramID = LoadShaders("/home/sanchez/game_engines/Cengine/Cengine/shaders/SimpleVertexShader.vertexshader", "/home/sanchez/game_engines/Cengine/Cengine/shaders/fragments.fragmentshader" );
        do{
            input.getMousePosition();
            glClear( GL_COLOR_BUFFER_BIT );
            setPerspective(45.f, input.getWindowSizeX(), input.getWindowSizeY(), 0.1, 10000.0f, ProgramID);
            glUseProgram(ProgramID);
            scn::scene();
            glfwSetCursorPos(CURRENT_WINDOW, 0, 0);     
            glFlush();
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
        while(glfwWindowShouldClose(m_window) == 0 && m_closed == 0);
    }

