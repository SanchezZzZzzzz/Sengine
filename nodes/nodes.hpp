glm::mat4 identity = glm::mat4(1.f);
glm::mat4 projection = glm::mat4(1.f);
glm::mat4 mvp;
void setPerspectiveProjection(){
    projection = glm::perspective(glm::radians(window_properties.fov), window_properties.getWindowX() / window_properties.getWindowY(), 0.1f, 1000.f);
}
 
class Node{
protected:
    glm::mat4 matrix = glm::mat4(1.f);
    glm::vec3 rotation = {0.f, 0.f, 0.f};
    glm::vec3 position = {0.f, 0.f, 0.f};
public:
    Mesh mesh;
    Node* parent;
    std::list<Node*>children{};
    std::string script;
    void attachScript(std::string scriptname);
    void addChild(Node* new_child){
        this->children.push_back(new_child);
        new_child->parent = this;
    }
    void rotateX(GLfloat angle){  
        this->rotation.x = fmod(rotation.x + angle, PI2);
        this->matrix = glm::rotate(identity, angle, {1, 0, 0}) * this->matrix;
    }
    void rotateY(GLfloat angle){
        this->matrix = glm::rotate(identity, angle, {0, 1, 0}) * this->matrix;
        this->rotation.y = fmod(rotation.y + angle, PI2);
    }
    void rotateZ(GLfloat angle){
        this->matrix = glm::rotate(identity, angle, {0, 0, 1}) * this->matrix;
        this->rotation.z = fmod(rotation.z + angle, PI2);
    }
    void translateGlobal(glm::vec3 new_position){
        this->matrix = this->matrix * glm::translate(identity, new_position);
    }
    void translate(glm::vec3 new_position){
        this->matrix = glm::translate(identity, new_position) * this->matrix;
    }
    void makeCurrent();
    glm::mat4 getMatrix(){
        if (this->parent != nullptr){
            return matrix * parent->getMatrix();
        }
        return matrix;
    }
    void draw(){
        mesh.draw(this->getMatrix());
    }
};
void Node::attachScript(std::string scriptname){
    const std::string scriptfile = scriptname;
}

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
Node* CURRENT_CAMERA;
void Node::makeCurrent(){
    CURRENT_CAMERA = this;
}
//draw() is here due to CURRENT_CAMERA is here (this shit fucked me up)
void Mesh::draw(glm::mat4 matrix){
    glUseProgram(programID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, m_vertex_coordinates.size());
    glDisableVertexAttribArray(0);
    mvp = projection * CURRENT_CAMERA->getMatrix() * glm::inverse(matrix);
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
}