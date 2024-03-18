class Node{
public:
    Node *parent;
    std::list<Node>children{};
    std::string script;
    void attachScript(std::string scriptname);
};
void Node::attachScript(std::string scriptname){
    const std::string scriptfile = scriptname;
}

class Camera: public Node {
    glm::vec3 position {0, 0, 0};
    glm::vec3 rotation {1, 0, 0};
    glm::vec3 orientation {0, 1, 0};
    glm::mat4 view;
public:
    Camera(){
        view = glm::lookAt(position, rotation, orientation);
    }
    void makeCurrent();
};
void Camera::makeCurrent(){
    current_window.camera = this;
}
class Node2D: public Node{
public:
    GLfloat x, y;
    Mesh mesh;    
};
