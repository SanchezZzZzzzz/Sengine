GLfloat PI = glm::pi<float>();
GLfloat PI2 = PI*2;
GLuint matrixID;
class Mesh{
private:
    void Add_material();
protected:
    GLuint m_VBO;
    std::vector<glm::vec3>m_vertex_coordinates = {};
    std::vector<glm::vec3>m_normals = {};
    std::string m_mesh_type = "None";
    GLuint programID = 0;
    void addVertex(GLfloat x, GLfloat y, GLfloat z){
        m_vertex_coordinates.push_back({x, y, z});
    }
    void addNormalVector(GLfloat x, GLfloat y, GLfloat z){
        m_normals.push_back({x, y, z});
    }
    void translateDataToBuffer(){
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->m_vertex_coordinates.size(), &m_vertex_coordinates[0], GL_STATIC_DRAW);
    }
public:
    void setShaders(const char* vertex_shader, const char* fragments_shader){
        programID = LoadShaders(vertex_shader, fragments_shader);
        matrixID = glGetUniformLocation(this->programID, "MVP");
    }
    void setRectangleShape(GLfloat x_pos, GLfloat y_pos, GLfloat x_size, GLfloat y_size);
    void setCubeShape(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    void draw(glm::mat4 matrix);
    void loadFromFile(std::string file_name);
};

void Mesh::setRectangleShape(GLfloat x_pos, GLfloat y_pos, GLfloat x_size, GLfloat y_size){
    m_vertex_coordinates.clear();
    m_mesh_type = "Rectangle";
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
    m_mesh_type = "Cube";
    size /= 2;
    //rear triangle 1
    addVertex(x - size, y - size, z - size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y + size, z - size);
    addNormalVector(0, 0, -1);
    //rear triangle 2
    addVertex(x + size, y + size, z - size);
    addVertex(x - size, y + size, z - size);
    addVertex(x - size, y - size, z - size);
    addNormalVector(0, 0, -1);
    //left triangle 1
    addVertex(x - size, y - size, z - size);
    addVertex(x - size, y - size, z + size);
    addVertex(x - size, y + size, z + size);
    addNormalVector(-1, 0, 0);
    //left triangle 2
    addVertex(x - size, y - size, z - size);
    addVertex(x - size, y + size, z + size);
    addVertex(x - size, y + size, z - size);
    addNormalVector(-1, 0, 0);
    //front triangle 1
    addVertex(x + size, y + size, z + size);
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y - size, z + size);
    addNormalVector(0, 0, 1);
    //front triangle 2
    addVertex(x - size, y - size, z + size);
    addVertex(x + size, y + size, z + size);
    addVertex(x - size, y + size, z + size);
    addNormalVector(0, 0, 1);
    //right triangle 1
    addVertex(x + size, y + size, z + size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y - size, z + size);
    addNormalVector(1, 0, 0);
    //right triangle 2
    addVertex(x + size, y + size, z - size);
    addVertex(x + size, y - size, z - size);
    addVertex(x + size, y + size, z + size);
    addNormalVector(1, 0, 0);
    //top tirangle 1
    addVertex(x - size, y + size, z + size);
    addVertex(x + size, y + size, z + size);
    addVertex(x + size, y + size, z - size);
    //top tirangle 2
    addVertex(x - size, y + size, z + size);
    addVertex(x + size, y + size, z - size);
    addVertex(x - size, y + size, z - size);
    translateDataToBuffer();
}
// void Mesh::loadFromFile(std::string file_name){
//     const std::string tempFname = file_name; //Имя файла в const строке
//     std::ifstream obj(tempFname); //Файл объектов в obj
//     if (obj.is_open()) { //Если открыт
//         std::vector<glm::vec3>vertices = {};
//         std::vector<glm::vec3>texture_coordinates = {};
//         std::vector<glm::vec3>normals = {};
//         std::string parsed; //здесь будет храниться каждая строка
//         Object temp; //Новый временный объект
//         Material* last_material = &*new Material; //Последний материал будет присваиваться к новой грани
//         bool objectFound = 0;//Равен 0 до первого объекта
//         while (getline(obj, parsed)) { //Парсинг obj
//             std::vector<std::string> tempstr = splitbystrings(parsed, " ");
//             if (tempstr[0] == "o") {//Новый объект найден
//                 if (objectFound) //Если это не первый объект
//                     objects.push_back(temp); //Добавляем его ко всем объектам
//                 temp = *new Object; //temp теперь новый
//                 ObjectFound = 1; //Теперь первых объектов не будэ
//                 temp.name = tempstr[1]; //Имя файла - всё кроме "o "
//                 cout << "initializing object: " << temp.name << "\n"; //Типа круто когда всё выводится в консоль (ЛУЧШЕ НЕ СМОТРЕТЬ ЕСЛИ В ИМЕНИ ЕСТЬ КИРИЛЛИЦА)
//             }
//             else if (tempstr[0] == "mtllib") { //Если в строке файл материала
//                 materials = tempstr[1]; //Имя файла материалов тут
//                 cout << "Yay, :D material file found. Name is: " << materials << "\n"; //Говорим что всё найдено (Это ещё не значит что всё будет работать)
//                 Add_material(); //Добавляем материалы к модели

//             }
//             else if (parsed[0] != '#' && parsed[0] != 'm') { //если не ненужная строка
//                 if (tempstr[0] == "f") { //Андрюха, у нас грань, возможно криминал
//                     face Tface = {}; //Временная грань
//                     Tface.FaceMaterial = &*LastMaterial; //Материал равен последнему найденному
//                     for (int i = 1; i < tempstr.size(); i++) {
//                         vector<string>subface = splitbystrings(tempstr[i], "/");
//                         if (subface[0] != "")
//                             Tface.v.push_back(vertexes[stoi(subface[0]) - 1]);
//                         if (subface[1] != "")
//                             Tface.vt.push_back(texcoords[stoi(subface[1]) - 1]);
//                         if (subface[2] != "")
//                             Tface.n.push_back(normals[stoi(subface[2]) - 1]);
//                     }
//                     GetNormalVector(Tface.NormalVector, Tface.n); //Получаем нормальный вектор
//                     temp.faces.push_back(Tface); //Теперь всё хорошо, добавляем грань ко временному объекту
//                 }
//                 else if (parsed.substr(0, parsed.find(" ")) == "usemtl") { //Если используется другой материал
//                     LastMaterial = FindMaterialByName(matProps, parsed.substr(parsed.find(" ") + 1, parsed.size())); //Это у нас теперь последний
//                 }
//                 else if (tempstr[0] != "s") {
//                     vector<float>coords = Get_float_vector(tempstr, 1);
//                     if (tempstr[0] == "v") //Если строка это вершины
//                         vertexes.push_back(coords);
//                     else if (tempstr[0] == "vt") //Тот же алгоритм с текстурными координатами
//                         texcoords.push_back(coords);
//                     else if (tempstr[0] == "vn") //И нормальными
//                         normals.push_back(coords);
//                 }
//             }
//         }
//         objects.push_back(temp); //Добавляем к объектам временный объект
//     }
// }
// void Mesh::addMaterial() { //Добавления материала
//             const std::string Mfile = materials; //Строка для имени материала
//             std::ifstream MatFile(Mfile); //Читаем из файла
//             Material temp; //Временный материал
//             std::string parsed; //Строка для парсинга
//             bool MaterialFound = false;
//             if (MatFile.is_open()) {
//                 while (getline(MatFile, parsed)) {
//                     try {
//                         if (parsed.substr(0, parsed.find(" ")) == "newmtl") {
//                             if (MaterialFound)
//                                 matProps.push_back(temp);
//                             else
//                                 MaterialFound = 1;
//                             temp = *new Material;
//                             temp.name = parsed.substr(parsed.find(" ") + 1, parsed.size());
//                         }
//                         else if (parsed.substr(0, parsed.find(" ")) == "Ns")
//                             temp.Reflection_coefficient = stof(parsed.substr(parsed.find(" ") + 1, parsed.size()));
//                         else if (parsed.substr(0, parsed.find(" ")) == "Ka")
//                             AddValsToArray(temp.AmbientLightColor, parsed);
//                         else if (parsed.substr(0, parsed.find(" ")) == "Kd")
//                             AddValsToArray(temp.DiffuseColor, parsed);
//                         else if (parsed.substr(0, parsed.find(" ")) == "Ks")
//                             AddValsToArray(temp.ReflectionColor, parsed);
//                         else if (parsed.substr(0, parsed.find(" ")) == "d")
//                             temp.Transparency = stof(parsed.substr(parsed.find(" ") + 1, parsed.size()));
//                     }
//                     catch (...) {
//                         cout << "Error occured while reading: " << parsed << "\n";
//                     }
//                 }
//                 matProps.push_back(temp);
//             }
//             else {
//                 cout << "Couldn't find Material file :(\n";
//             }
//         }