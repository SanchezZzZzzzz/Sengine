

GLfloat PI = glm::pi<float>();
GLfloat PI2 = PI*2;
struct Material{
    bool has_texture = 0;
    unsigned char* texture_data;
    int width, height, cnt;
    GLuint texture;
    std::string name;
    GLfloat reflection_coefficient;
    glm::vec3 diffuse_color;
    glm::vec3 ambient_light_color;
    glm::vec3 reflection_color;
    GLfloat transparency;
};
std::vector<Material>materials_list = {};
std::vector<std::string> splitByStrings(std::string string, std::string splitter) {
    std::vector<std::string> result = {};
    while (string.find(splitter) != std::string::npos) {
        result.push_back(string.substr(0, string.find(splitter)));
        string.erase(0, string.find(splitter) + 1);
    }
    result.push_back(string);
    return result;
}
glm::vec3 getFloatVector(std::vector<std::string>& values, unsigned short begin) {
    glm::vec3 result = glm::vec3(stof(values[begin]), stof(values[begin + 1]), stof(values[begin + 2]));
    return result;
}
Material findMaterialByName( std::string Matname) {
    for (int i = 0; i < materials_list.size(); i++) {
        if (Matname == materials_list[i].name)
            return materials_list[i];
    }
    return materials_list[0];
}
bool materialLoaded(std::string material_name){
    for (int i = 0; i < materials_list.size(); i++) {
        if (material_name == materials_list[i].name)
            return true;
    }
    return false;
}
void addMaterial(std::string materials) {
    const std::string material_file = materials;
    std::ifstream material_file_stream(material_file);
    Material temp_material;
    std::string parsed;
    bool MaterialFound = false;
    if (material_file_stream.is_open()) {
        while (getline(material_file_stream, parsed)) {
            try {
                std::vector<std::string>current_string = splitByStrings(parsed, " ");
                if (current_string[0] == "newmtl") {
                    if (MaterialFound)
                        materials_list.push_back(temp_material);
                    else
                        MaterialFound = 1;
                    temp_material = *new Material;
                    temp_material.name = current_string[1];
                }
                else if (current_string[0] == "Ns")
                    temp_material.reflection_coefficient = stof(current_string[1]);
                else if (current_string[0] == "Ka")
                    temp_material.ambient_light_color = getFloatVector(current_string, 1);
                else if (current_string[0] == "Kd")
                    temp_material.diffuse_color = getFloatVector(current_string, 1);
                else if (current_string[0] == "Ks")
                    temp_material.reflection_color = getFloatVector(current_string, 1);
                else if (current_string[0] == "d")
                    temp_material.transparency = stof(current_string[1]);
                else if (current_string[0] == "map_Kd"){
                    const char *const_file_name = current_string[1].data();
                    temp_material.texture_data = stbi_load(const_file_name, &temp_material.width, &temp_material.height, &temp_material.cnt, 0);
                    glGenTextures(1, &temp_material.texture);
                    glBindTexture(GL_TEXTURE_2D, temp_material.texture);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_material.width, temp_material.height, 0, temp_material.cnt==4 ?
                    GL_RGBA: GL_RGB , GL_UNSIGNED_BYTE, temp_material.texture_data);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    stbi_image_free(temp_material.texture_data);
                    temp_material.has_texture = 1;
                }
            }
            catch (...) {
                std::cout << "Error occured while reading: " << parsed << "\n";
            }
        }
        materials_list.push_back(temp_material);
    }
    else {
        std::cout << "Couldn't find Material file :(\n";
    }
}

class Mesh{
protected:
    GLuint m_VBO, m_NBO, m_DifBO, m_RefBO, m_texcBO;
    std::vector<glm::vec3>m_vertex_coordinates = {};
    std::vector<glm::vec3>m_normals = {};
    std::vector<glm::vec2>m_texture_coordinates = {};
    std::vector<glm::vec3>m_diffuse_color = {};
    std::vector<GLfloat>m_reflection_coefficient = {};
    unsigned char* m_texture_data;
    int m_width, m_height, m_cnt;
    GLuint programID = 0;
    GLuint mvpid, mid, vid, texid, texture;
public:
    std::string m_mesh_name = "None";

    void translateDataToBuffer(){
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->m_vertex_coordinates.size(), &m_vertex_coordinates[0], GL_STATIC_DRAW);
        glGenBuffers(1, &m_NBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->m_normals.size(), &m_normals[0], GL_STATIC_DRAW);
        glGenBuffers(1, &m_DifBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_DifBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->m_diffuse_color.size(), &m_diffuse_color[0], GL_STATIC_DRAW);
        glGenBuffers(1, &m_RefBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_RefBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->m_reflection_coefficient.size(), &m_reflection_coefficient, GL_STATIC_DRAW);
        glGenBuffers(1, &m_texcBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_texcBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->m_texture_coordinates.size(), &m_texture_coordinates[0], GL_STATIC_DRAW);
    }
    void addVertex(GLfloat x, GLfloat y, GLfloat z){
        m_vertex_coordinates.push_back({x, y, z});
    }
    void addVertex(glm::vec3 new_vertex){
        m_vertex_coordinates.push_back(new_vertex);
    }
    void addNormalVector(GLfloat x, GLfloat y, GLfloat z){
        m_normals.push_back({x, y, z});
    }
    void addNormalVector(glm::vec3 new_normal){
        m_normals.push_back(new_normal);
    }
    void addTexureCoordinate(GLfloat u, GLfloat v){
        m_texture_coordinates.push_back({u, v});
    }
    void addTexureCoordinate(glm::vec2 new_texture_coordinates){
        m_texture_coordinates.push_back(new_texture_coordinates);
    }
    void addDiffuseColor(glm::vec3 diffuse_color){
        m_diffuse_color.push_back(diffuse_color);
    }
    void addReflectionCoefficient(GLfloat value){
        m_reflection_coefficient.push_back(value);
    }
    void addTexture_data(Material temp_material){
        texture = temp_material.texture;
        m_texture_data = temp_material.texture_data;
        m_width = temp_material.width;
        m_height = temp_material.height;
        m_cnt = temp_material.cnt;
    }
    void setShaders(const char* vertex_shader, const char* fragments_shader){
        programID = LoadShaders(vertex_shader, fragments_shader);
        mvpid = glGetUniformLocation(this->programID, "MVP");
        mid = glGetUniformLocation(this->programID, "M");
        vid = glGetUniformLocation(this->programID, "V");
        texid = glGetUniformLocation(this->programID, "texture_sampler");
    }
    void setRectangleShape(GLfloat x_pos, GLfloat y_pos, GLfloat x_size, GLfloat y_size);
    void setCubeShape(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    void draw(glm::mat4 matrix);
    void loadFromFile(std::string file_name);
};
void Mesh::setRectangleShape(GLfloat x_pos, GLfloat y_pos, GLfloat x_size, GLfloat y_size){
    m_vertex_coordinates.clear();
    m_mesh_name = "Rectangle";
    addVertex(x_pos, y_pos, 0);
    addVertex(x_pos + x_size, y_pos, 0);
    addVertex(x_pos + x_size, y_pos + y_size, 0);
    addVertex(x_pos + x_size, y_pos + y_size, 0);
    addVertex(x_pos, y_pos + y_size, 0);
    addVertex(x_pos, y_pos, 0);
    translateDataToBuffer();
}
void Mesh::setCubeShape(GLfloat x, GLfloat y, GLfloat z, GLfloat size){
    m_vertex_coordinates.clear();
    m_normals.clear();
    m_mesh_name = "Cube";
    size /= 2;
    //rear triangle 1
    addVertex(x - size, y - size, z - size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y + size, z - size);
    addNormalVector(0, 0, -1);
    addNormalVector(0, 0, -1);
    addNormalVector(0, 0, -1);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //rear triangle 2
    addVertex(x + size, y + size, z - size);
    addVertex(x - size, y + size, z - size);
    addVertex(x - size, y - size, z - size);
    addNormalVector(0, 0, -1);
    addNormalVector(0, 0, -1);
    addNormalVector(0, 0, -1);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //left triangle 1
    addVertex(x - size, y - size, z - size);
    addVertex(x - size, y - size, z + size);
    addVertex(x - size, y + size, z + size);
    addNormalVector(-1, 0, 0);
    addNormalVector(-1, 0, 0);
    addNormalVector(-1, 0, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //left triangle 2
    addVertex(x - size, y - size, z - size);
    addVertex(x - size, y + size, z + size);
    addVertex(x - size, y + size, z - size);
    addNormalVector(-1, 0, 0);
    addNormalVector(-1, 0, 0);
    addNormalVector(-1, 0, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //front triangle 1
    addVertex(x + size, y + size, z + size);
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y - size, z + size);
    addNormalVector(0, 0, 1);
    addNormalVector(0, 0, 1);
    addNormalVector(0, 0, 1);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //front triangle 2
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y + size, z + size);
    addVertex(x - size, y + size, z + size);
    addNormalVector(0, 0, 1);
    addNormalVector(0, 0, 1);
    addNormalVector(0, 0, 1);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //right triangle 1
    addVertex(x + size, y + size, z + size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y - size, z + size);
    addNormalVector(1, 0, 0);
    addNormalVector(1, 0, 0);
    addNormalVector(1, 0, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //right triangle 2
    addVertex(x + size, y + size, z - size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y + size, z + size);
    addNormalVector(1, 0, 0);
    addNormalVector(1, 0, 0);
    addNormalVector(1, 0, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //bottom tirangle 1
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y - size, z + size);
    addVertex(x + size, y - size, z - size);
    addNormalVector(0, -1, 0);
    addNormalVector(0, -1, 0);
    addNormalVector(0, -1, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //bottom tirangle 2
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y - size, z - size);
    addVertex(x - size, y - size, z - size);
    addNormalVector(0, -1, 0);
    addNormalVector(0, -1, 0);
    addNormalVector(0, -1, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //top tirangle 1
    addVertex(x - size, y + size, z + size);
    addVertex(x + size, y + size, z + size);
    addVertex(x + size, y + size, z - size);
    addNormalVector(0, 1, 0);
    addNormalVector(0, 1, 0);
    addNormalVector(0, 1, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    //top tirangle 2
    addVertex(x - size, y + size, z + size);
    addVertex(x + size, y + size, z - size);
    addVertex(x - size, y + size, z - size);
    addNormalVector(0, 1, 0);
    addNormalVector(0, 1, 0);
    addNormalVector(0, 1, 0);
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addTexureCoordinate(glm::vec2(0.f, 0.f));
    addReflectionCoefficient(1.f);
    translateDataToBuffer();
}

