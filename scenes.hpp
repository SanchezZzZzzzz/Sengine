#include "example_scripts/camera_control.hpp"
namespace scn{
    void cameraScript(Camera* camera);
    std::vector<Node2D>nodes2D={};
    Camera test_camera;
    void scene(){
        for (GLuint i = 0; i < nodes2D.size(); i++){
            nodes2D[i].mesh.draw();
        }
        cameraScript(&test_camera);
    }
    void sceneSetup(){
        Node2D testRectangle;
        testRectangle.mesh.setCubeShape(0, 0, 0, 20);
        nodes2D.push_back(testRectangle);
        test_camera.translate({0, 0, 30});
        //test_camera.rotate(0, glm::radians(180.f), 0);
        test_camera.makeCurrent();
        input.setMouseMode(SET_CURSOR_DISABLED);
    }
    void cameraScript(Camera* camera, Node char_body){
         camera->rotateY(glm::radians(input.mouse_x));
         camera->rotateX(glm::radians(input.mouse_y));
    }
};

