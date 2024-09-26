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
    Node* parent = nullptr;
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
        if (parent != nullptr){
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
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, m_DifBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, m_RefBO);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(texid, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_texcBO);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, m_vertex_coordinates.size());
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(0);
    mvp = projection * CURRENT_CAMERA->getMatrix() * glm::inverse(matrix);
    glUniformMatrix4fv(mvpid, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(vid, 1, GL_FALSE, &CURRENT_CAMERA->getMatrix()[0][0]);
    glUniformMatrix4fv(mid, 1, GL_FALSE, &glm::inverse(matrix)[0][0]);
}

std::vector<Node> loadMeshFromFile(std::string file_name){
    const std::string tempFname = file_name; //Имя файла в const строке
    std::vector<Node> node_set = {};
    std::string material;
    std::ifstream obj(tempFname); //Файл объектов в obj
    Node temp;
    if (obj.is_open()) { //Если открыт
        std::vector<glm::vec3>vertices = {};
        std::vector<glm::vec2>texture_coordinates = {};
        std::vector<glm::vec3>normals = {};
        std::string parsed;
        Material last_material;
        bool new_mesh = 0;
        while (getline(obj, parsed)) {
            try{
                std::vector<std::string> tempstr = splitByStrings(parsed, " ");
                if (tempstr[0] == "o") {
                    if (new_mesh)
                        node_set.push_back(temp);
                    else
                        new_mesh = 1;
                    temp = *new Node;
                    temp.mesh.m_mesh_name = tempstr[1];
                }
                else if (tempstr[0] == "mtllib") {
                    addMaterial(tempstr[1]);
                }
                else if (parsed[0] != '#' && parsed[0] != 'm') {
                    if (tempstr[0] == "f") {
                            for (int i = 1; i < tempstr.size(); i++) {
                                std::vector<std::string>vertex = splitByStrings(tempstr[i], "/");
                                if (vertex[0] != ""){
                                    temp.mesh.addVertex(vertices[stoi(vertex[0]) - 1]);
                                    temp.mesh.addDiffuseColor(last_material.diffuse_color);
                                    temp.mesh.addReflectionCoefficient(last_material.reflection_coefficient);
                                }             
                                if (vertex[1] != "")
                                    temp.mesh.addTexureCoordinate(texture_coordinates[stoi(vertex[1]) - 1]);
                                else
                                    temp.mesh.addTexureCoordinate(glm::vec2(0, 0));
                                if (vertex[2] != "")
                                    temp.mesh.addNormalVector(normals[stoi(vertex[2]) - 1]);
                            }  
                    }
                    else if (tempstr[0] == "usemtl") {
                        last_material = findMaterialByName(tempstr[1]);
                        if (last_material.has_texture)
                            temp.mesh.addTexture_data(last_material);
                    }
                    else if (tempstr[0] != "s") {
                        glm::vec3 coords = getFloatVector(tempstr, 1);
                        if (tempstr[0] == "v") //Если строка это вершины
                            vertices.push_back(coords);
                        else if (tempstr[0] == "vt") //Тот же алгоритм с текстурными координатами
                            texture_coordinates.push_back(glm::vec2(coords[0], coords[1]));
                        else if (tempstr[0] == "vn") //И нормальными
                            normals.push_back(coords);
                    }
                }
            }
            catch(...){
                std::cout << "Got an error in string '" << parsed <<"'\n";
            }
        }
        node_set.push_back(temp);
        for (unsigned int i = 0; i < node_set.size(); i++){
            node_set[i].mesh.translateDataToBuffer();
        }        
    }
    return node_set;
}