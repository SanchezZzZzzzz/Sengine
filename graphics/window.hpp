
class Window{
private:
    float m_x, m_y;
    double m_mouse_x, m_mouse_y;
    bool m_closed = 0;
    Camera* current_camera;
    GLFWwindow* m_window;
public:
    Window(unsigned short x, unsigned short y, const char window_name[], Input input) {
        glewExperimental = GL_TRUE;
        if (GL_TRUE != glfwInit())
            fprintf(stderr, "Error!");
        this->m_x = x;
        this->m_y = y;
        this->m_window = glfwCreateWindow(this->m_x, this->m_y, window_name, NULL, NULL);
        glfwMakeContextCurrent(m_window);
        glewExperimental = GL_TRUE;
        glewInit();
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }
    void render();
    void close() {
        m_closed = 1;
    }
    void set_orthogonal(GLfloat x_start, GLfloat x_end, GLfloat y_start, GLfloat y_end, GLfloat z_near, GLfloat z_far, GLuint &programID){
        glm::mat4 projection = glm::ortho(x_start, x_end, y_start, y_end, z_near, z_far);
        glm::mat4 model = glm::mat4(1.f);
        glm::mat4 mvp = projection;
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    }
};
void Window::render(){
        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
        glClearColor(0.0, 0.0, 0.2, 0.0f);
        scn::sceneSetup();
        GLuint ProgramID = LoadShaders("/home/sanchez/game_engines/Cengine/Cengine/shaders/SimpleVertexShader.vertexshader", "/home/sanchez/game_engines/Cengine/Cengine/shaders/fragments.fragmentshader" );
        do{
            glfwGetCursorPos(this->m_window, &input.mouse_x, &input.mouse_y);
            glClear( GL_COLOR_BUFFER_BIT );
            set_orthogonal(0.f, 1024.f, 0.f, 768.f, -1.0f, 1.0f, ProgramID);
            scn::scene();
            glUseProgram(ProgramID);
            glFlush();
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
        while(glfwWindowShouldClose(m_window) == 0 && m_closed == 0);
    }

