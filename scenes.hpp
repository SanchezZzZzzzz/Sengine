#include "example_scripts/camera_control.hpp"
namespace scn{
    void cameraScript(Node* camera);
    Node character;
    Node small_cube, cube;
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
        if (key == GLFW_KEY_LEFT_CONTROL)
            character.translate({0, -SPEED, 0});
    }
    void scene(){
        small_cube.rotateY(glm::radians(1.f));
        cube.draw();
        small_cube.draw();
        cameraScript(*&test_camera);
    }
    void sceneSetup(){
        setPerspectiveProjection();
        small_cube.mesh.setShaders("/home/sanchez/game_engines/Cengine/Cengine/shaders/cube2.vertexshader", "/home/sanchez/game_engines/Cengine/Cengine/shaders/cube2.fragmentshader");
        cube.mesh.setShaders("/home/sanchez/game_engines/Cengine/Cengine/shaders/cube1.vertexshader", "/home/sanchez/game_engines/Cengine/Cengine/shaders/cube1.fragmentshader");
        small_cube.mesh.setCubeShape(0, 0, 0, 3);
        small_cube.rotateX(glm::radians(180.f));
        small_cube.translate({0, -20, 0});
        cube.mesh.setCubeShape(0, 0, 0, 10);    
        cube.translate({0, 0, 0});
        character.addChild(*&test_camera);
        character.translate({0, 0, 10});
        test_camera->makeCurrent();
        input.setMouseMode(SET_CURSOR_DISABLED);
        glfwSetKeyCallback(CURRENT_WINDOW, getKeyInput);
    }
    void cameraScript(Node* camera){
         camera->rotateX(glm::radians(input.getMouseY()) * 0.3);
         character.rotateY(glm::radians(input.getMouseX()) * -0.3);
    }
};

