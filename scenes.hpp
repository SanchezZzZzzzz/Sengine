#include "example_scripts/camera_control.hpp"
namespace scn{
    void cameraScript(Node* camera);
    std::vector<Node2D>nodes2D={};
    std::vector<Node>nodes = {};
    Node character;
    float SPEED = 1;
    Node* test_camera = new Camera();
    void getKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_W)
            character.translate({0, 0, SPEED});
        if (key == GLFW_KEY_S)
            character.translate({0, 0, -SPEED});
        if (key == GLFW_KEY_A)
            character.translate({-SPEED, 0, 0});
        if (key == GLFW_KEY_D)
            character.translate({SPEED, 0, 0});
        if (key == GLFW_KEY_SPACE)
            character.translate({0, SPEED, 0});
    }
    void scene(){
        nodes2D[0].rotateY(glm::radians(1.f));
        for (GLuint i = 0; i < nodes2D.size(); i++){
            nodes2D[i].draw();
        }
        cameraScript(*&test_camera);
    }
    void sceneSetup(){
        Node2D testRectangle;
        character.addChild(*&test_camera);
        testRectangle.mesh.setCubeShape(0, 0, 0, 20);
        nodes2D.push_back(testRectangle);
        character.translate({0, 0, 40});
        test_camera->translate({0, 10, 0});
        test_camera->makeCurrent();
        input.setMouseMode(SET_CURSOR_DISABLED);
        glfwSetKeyCallback(CURRENT_WINDOW, getKeyInput);
    }
    void cameraScript(Node* camera){
         camera->rotateX(glm::radians(input.getMouseY()) * 0.3);
         character.rotateY(glm::radians(input.getMouseX()) * -0.3);
    }
};

