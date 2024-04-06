class Node{
public:
    Node* parent;
    std::list<Node*>children{};
    std::string script;
    void attachScript(std::string scriptname);
    void addChild();
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
    glm::mat4 view;
    Camera(){
        view = glm::lookAt(
        glm::vec3(0, 0, 0), 
        glm::vec3(0, 0, 1), 
        glm::vec3(0, 1, 0));
    }
    void translate(glm::vec3 new_position){
        //this->position = new_position;
        this->view = glm::translate(this->view, new_position);
        //this->updateView();
    }
    void translate(GLfloat x, GLfloat y, GLfloat z);
    //void rotate(GLfloat x, GLfloat y, GLfloat z);
    // void updateView(){
    //     view = glm::lookAt(position, position + direction, orientation);
    // }
    void makeCurrent();
    void rotateX(GLfloat angle){
        glm::mat4 temp_rotation = glm::mat4(1.f);
        temp_rotation = glm::rotate(temp_rotation, angle, {1, 0, 0});
        this->view = temp_rotation * this->view;
    }
    void rotateY(GLfloat angle){
        glm::mat4 temp_rotation = glm::mat4(1.f);
        temp_rotation = glm::rotate(temp_rotation, angle, {0, 1, 0});
        this->view = temp_rotation * this->view;
    }
};

Camera* CURRENT_CAMERA = new Camera();

void Camera::makeCurrent(){
    CURRENT_CAMERA = this;
}

// void Camera::rotate(GLfloat x, GLfloat y, GLfloat z){
//     this->rotation_x = glm::rotate(this->rotation_x, x, {1, 0, 0});
//     this->rotation_y = glm::rotate(this->rotation_y, y, {0, 1, 0});
//     this->rotation_z = glm::rotate(this->rotation_z, z, {0, 0, 1});
//     this->view = this->rotation_x * this->rotation_y * this->rotation_z * this->view;
//     // this->rotation.y = fmod(this->rotation.y + y, PI2);
//     // this->rotation.x = fmod(this->rotation.x + x, PI2);
//     // this->rotation.z = fmod(this->rotation.z + z, PI2);
//     // float cos_y = abs(cos(this->rotation.y));
//     // float sin_y = abs(sin(this->rotation.y));
//     // this->direction = glm::rotateX(this->direction, cos_y * z);
//     // this->direction = glm::rotateY(this->direction, y);
//     // this->direction = glm::rotateZ(this->direction, sin_y * z);
//     // this->orientation = glm::rotateX(this->orientation, sin_y * x);
//     // this->orientation = glm::rotateZ(this->orientation, cos_y * x);
//     //this->updateView();
// }
