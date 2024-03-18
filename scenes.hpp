namespace scn{
    std::vector<Node2D>nodes2D={};
    void scene(){
        for (GLuint i = 0; i < nodes2D.size(); i++){
            nodes2D[i].mesh.draw();
        }
    }
    void sceneSetup(){
        Node2D testRectangle;
        testRectangle.mesh.setRectangleShape(0, 0, 1024, 512);
        nodes2D.push_back(testRectangle);
        Camera camera;
    }
};

