class Node{
protected:
    glm::mat4 matrix = glm::mat4(1.f);
    glm::mat4 transform_matrix = glm::mat4(1.f);
public:
    Node* parent;
    std::list<Node*>children{};
    std::string script;
    void attachScript(std::string scriptname);
    void addChild(Node* new_child){
        children.push_back(new_child);
        new_child->parent = this;
    }
    void rotateX(GLfloat angle){  
        transform_matrix = glm::rotate(transform_matrix, angle, {1, 0, 0});
        this->matrix = transform_matrix * this->matrix;
        this->transform_matrix = glm::mat4(1.f);
        //transformChildren();
    }
    void rotateY(GLfloat angle){
        transform_matrix = glm::rotate(transform_matrix, angle, {0, 1, 0});
        this->matrix = transform_matrix * this->matrix;
        this->transform_matrix = glm::mat4(1.f);
        //transformChildren();
    }
    void translate(glm::vec3 new_position){
        this->transform_matrix = glm::translate(this->transform_matrix, new_position);
        this->matrix = this->matrix * this->transform_matrix;
        this->transform_matrix = glm::mat4(1.f);
        //transformChildren();
    }
    void transformChildren(){
        for (Node* child: children){
            child->matrix = child->matrix * this->transform_matrix;
            child->transformChildren();
        }
        this->transform_matrix = glm::mat4(1.f);
    }
    void makeCurrent();
    glm::mat4 getMatrix(){
        return matrix * parent->matrix;
    }
};
void Node::attachScript(std::string scriptname){
    const std::string scriptfile = scriptname;
}

class Node2D: public Node{
public:
    GLfloat x, y;
    Mesh mesh;    
};
class Camera: public Node{
public:
    Camera(){
        matrix = glm::lookAt(
        glm::vec3(0, 0, 0), 
        glm::vec3(0, 0, 1), 
        glm::vec3(0, 1, 0));
    }
    void makeCurrent();
};
Node* CURRENT_CAMERA = new Camera();
void Node::makeCurrent(){
    CURRENT_CAMERA = this;
}