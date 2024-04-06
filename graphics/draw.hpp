
GLfloat PI = glm::pi<float>();
GLfloat PI2 = PI*2;

class Mesh{
protected:
    GLuint VBO;
    glm::mat4 m_mesh_transform = glm::mat4(1.0f);
    std::vector<GLfloat>m_vertex_coordinates = {};
    GLfloat *vertices;
    GLuint array_size = 0;
    void addVertex(GLfloat x, GLfloat y, GLfloat z){
        m_vertex_coordinates.push_back(x);
        m_vertex_coordinates.push_back(y);
        m_vertex_coordinates.push_back(z);
    }
    void translateDataToBuffer(std::vector<GLfloat>&vertex_coordinates){
        array_size = vertex_coordinates.size();
        vertices = new GLfloat[vertex_coordinates.size()];
        for (int i = 0; i < vertex_coordinates.size(); i++){
            vertices[i] = vertex_coordinates[i];
        }
        m_vertex_coordinates.clear();
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->array_size, this->vertices, GL_STATIC_DRAW);
    }
public:
    void setRectangleShape(GLfloat x_pos, GLfloat y_pos, GLfloat x_size, GLfloat y_size);
    void setCubeShape(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    void draw(){
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, array_size);
        glDisableVertexAttribArray(0);
    }
};

void Mesh::setRectangleShape(GLfloat x_pos, GLfloat y_pos, GLfloat x_size, GLfloat y_size){
    addVertex(x_pos, y_pos, 0);
    addVertex(x_pos + x_size, y_pos, 0);
    addVertex(x_pos + x_size, y_pos + y_size, 0);
    addVertex(x_pos + x_size, y_pos + y_size, 0);
    addVertex(x_pos, y_pos + y_size, 0);
    addVertex(x_pos, y_pos, 0);
    translateDataToBuffer(this->m_vertex_coordinates);
}

void Mesh::setCubeShape(GLfloat x, GLfloat y, GLfloat z, GLfloat size){
    size /= 2;
    //rear triangle 1
    addVertex(x - size, y - size, z - size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y + size, z - size);
    //rear triangle 2
    addVertex(x + size, y + size, z - size);
    addVertex(x - size, y + size, z - size);
    addVertex(x - size, y - size, z - size);
    //left triangle 1
    addVertex(x - size, y - size, z - size);
    addVertex(x - size, y - size, z + size);
    addVertex(x - size, y + size, z + size);
    //left triangle 2
    addVertex(x - size, y - size, z - size);
    addVertex(x - size, y + size, z + size);
    addVertex(x - size, y + size, z - size);
    //front triangle 1
    addVertex(x + size, y + size, z + size);
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y - size, z + size);
    //front triangle 2
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y + size, z + size);
    addVertex(x - size, y + size, z + size);
    //right triangle 1
    addVertex(x + size, y + size, z + size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y - size, z + size);
    //right triangle 2
    addVertex(x + size, y + size, z - size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y + size, z + size);
    translateDataToBuffer(this->m_vertex_coordinates);
}
//legacy
class Rectangle2D: public Mesh{
public:
    void setShape(GLfloat x_pos, GLfloat y_pos, GLfloat x_size, GLfloat y_size){
        addVertex(x_pos, y_pos, 0);
        addVertex(x_pos + x_size, y_pos, 0);
        addVertex(x_pos + x_size, y_pos + y_size, 0);
        addVertex(x_pos, y_pos + y_size, 0);
        addVertex(x_pos, y_pos, 0);
        translateDataToBuffer(this->m_vertex_coordinates);
    }
    void draw(){
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
    }
};


