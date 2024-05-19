#include "example_scripts/camera_control.hpp"
namespace scn{
    void cameraScript(Node* camera);
    Node character;
    Node small_cube, cube;
    std::unordered_map <int, bool>keymap = {
        {GLFW_KEY_W, false},
        {GLFW_KEY_S, false},
        {GLFW_KEY_A, false},
        {GLFW_KEY_D, false},
        {GLFW_KEY_SPACE, false},
        {GLFW_KEY_LEFT_CONTROL, false}
    };
    std::vector<Node>ak = {};
    float SPEED = 0.1;
    Node* test_camera = new Camera();
    void getKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
            keymap[GLFW_KEY_W] = true;
        else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
            keymap[GLFW_KEY_W] = false;
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
            keymap[GLFW_KEY_S] = true;
        else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
            keymap[GLFW_KEY_S] = false;
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            keymap[GLFW_KEY_A] = true;
        else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
            keymap[GLFW_KEY_A] = false;
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
            keymap[GLFW_KEY_D] = true;
        else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
            keymap[GLFW_KEY_D] = false;
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            keymap[GLFW_KEY_SPACE] = true;
        else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
            keymap[GLFW_KEY_SPACE] = false;
        if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
            keymap[GLFW_KEY_LEFT_CONTROL] = true;
        else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
            keymap[GLFW_KEY_LEFT_CONTROL] = false;
        // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //     character.translate({0, 0, SPEED});
        // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        //     character.translate({0, 0, -SPEED});
        // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //     character.translate({-SPEED, 0, 0});
        // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        //     character.translate({SPEED, 0, 0});
        // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        //     character.translate({0, SPEED, 0});
        // if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        //     character.translate({0, -SPEED, 0});
    }
    void keyHandler(){
        if (keymap[GLFW_KEY_W] == true)
            character.translate({0, 0, SPEED});
        if (keymap[GLFW_KEY_S] == true)
            character.translate({0, 0, -SPEED});
        if (keymap[GLFW_KEY_A] == true)
            character.translate({-SPEED, 0, 0});
        if (keymap[GLFW_KEY_D] == true)
            character.translate({SPEED, 0, 0});
        if (keymap[GLFW_KEY_SPACE] == true)
            character.translate({0, SPEED, 0});
        if (keymap[GLFW_KEY_LEFT_CONTROL] == true)
            character.translate({0, -SPEED, 0});
    }
    void scene(){
        small_cube.rotateY(glm::radians(1.f));
        for (int i = 0; i < ak.size(); i++){
            ak[i].draw();
        }
        cube.draw();
        keyHandler();
        small_cube.draw();
        cameraScript(*&test_camera);
    }
    void sceneSetup(){
        setPerspectiveProjection();
        ak = loadMeshFromFile("AGAT.obj");
        for (int i = 0; i < ak.size(); i++){
            ak[i].mesh.setShaders("shaders/cube1.vertexshader", "shaders/cube1.fragmentshader");
        }
        small_cube.mesh.setShaders("shaders/cube2.vertexshader", "shaders/cube2.fragmentshader");
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

