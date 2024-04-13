#include "example_scripts/camera_control.hpp"
namespace scn{
    void cameraScript(Node* camera);
    std::vector<Node2D>nodes2D={};
    std::vector<Node>nodes = {};
    Node character;
    Node* test_camera = new Camera();
    void getKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods){
        
    }
    void scene(){
        for (GLuint i = 0; i < nodes2D.size(); i++){
            nodes2D[i].mesh.draw();
        }
        cameraScript(*&test_camera);
        //character.rotateY(glm::radians(1.f));
    }
    void sceneSetup(){
        Node2D testRectangle;
        character.addChild(*&test_camera);
        testRectangle.mesh.setCubeShape(0, 0, 0, 20);
        nodes2D.push_back(testRectangle);
        character.translate({0, 20, 40});
        //test_camera.rotate(0, glm::radians(180.f), 0);
        test_camera->makeCurrent();
        input.setMouseMode(SET_CURSOR_DISABLED);
        glfwSetKeyCallback(CURRENT_WINDOW, getKeyInput);
    }
    void cameraScript(Node* camera){
         camera->rotateX(glm::radians(input.mouse_y) * 0.3);
         //camera->rotateY(glm::radians(input.mouse_x));
         character.rotateY(glm::radians(input.mouse_x) * -0.3);
    }
};

