#pragma once
#include<vector>
#include"glew.h"
#include"glut.h"
#include<fstream>
#include<string>
#include<iostream>
#include<cmath>
using namespace std;
namespace ml {
vector<string> splitbystrings(string str, string splitter) {
    vector<string> result = {};
    while (str.find(splitter) != string::npos) {
        result.push_back(str.substr(0, str.find(splitter)));
        str.erase(0, str.find(splitter) + 1);
    }
    result.push_back(str);
    return result;
}
vector<float> Get_float_vector(vector<string>& values, int begin) {
    vector<float> result = {};
    for (int i = begin; i < values.size(); i++)
        result.push_back(stof(values[i]));
    return result;
}
    struct Material { //Структура, которая представляет материал
    public:
        string name; //Имя материала
        GLfloat Reflection_coefficient; //Коэффициент отражения
        vector<GLfloat>AmbientLightColor; //Цвет окружающего освещения
        vector<GLfloat>DiffuseColor; //Цвет материала так сказатб
        vector<GLfloat>ReflectionColor; //Цвет отражения
        float Transparency; //Прозрачность

    };
    struct face { //Структура, представляющая грань
    public:
        vector<vector<GLfloat>>v = {}; //Индексы вершин
        vector<vector<GLfloat>>vt = {}; //индексы координатных текстур
        vector<vector<GLfloat>>n = {}; //Индексы нормали
        GLfloat NormalVector[3] = {0, 0, 0}; //Вектор нормали
        Material* FaceMaterial; //Материал грани
    };
    struct Object { //Структура, которая содержит данные о конкретном объекте
    public:
        string name; //Имя объекта
        vector<face>faces; //Все грани объекта
    };
    class Model {
    public:
        vector<Object>objects = {};
        vector<Material>matProps; //Все материалы модели
        string materials = ""; //Файл с материалами
        GLfloat ModelCoords[3] = { 0, 0, 0 };
        GLfloat ModelRot[3] = {0, 0, 0};
        void Model_import(string Fname) { //Метод импорта модели, Fname - obj файл для импортп
            const string tempFname = Fname; //Имя файла в const строке
            ifstream obj(tempFname); //Файл объектов в obj
            if (obj.is_open()) { //Если открыт
                vector<vector<GLfloat>>vertexes = {};
                vector<vector<GLfloat>>texcoords = {};
                vector<vector<GLfloat>>normals = {};
                string parsed; //здесь будет храниться каждая строка
                Object temp; //Новый временный объект
                Material* LastMaterial = &*new Material; //Последний материал будет присваиваться к новой грани
                bool ObjectFound = 0;//Равен 0 до первого объекта
                while (getline(obj, parsed)) { //Парсинг obj
                    vector<string> tempstr = splitbystrings(parsed, " ");
                    if (tempstr[0] == "o") {//Новый объект найден
                        if (ObjectFound) //Если это не первый объект
                            objects.push_back(temp); //Добавляем его ко всем объектам
                        temp = *new Object; //temp теперь новый
                        ObjectFound = 1; //Теперь первых объектов не будэ
                        temp.name = tempstr[1]; //Имя файла - всё кроме "o "
                        cout << "initializing object: " << temp.name << "\n"; //Типа круто когда всё выводится в консоль (ЛУЧШЕ НЕ СМОТРЕТЬ ЕСЛИ В ИМЕНИ ЕСТЬ КИРИЛЛИЦА)
                    }
                    else if (tempstr[0] == "mtllib") { //Если в строке файл материала
                        materials = tempstr[1]; //Имя файла материалов тут
                        cout << "Yay, :D material file found. Name is: " << materials << "\n"; //Говорим что всё найдено (Это ещё не значит что всё будет работать)
                        Add_material(); //Добавляем материалы к модели

                    }
                    else if (parsed[0] != '#' && parsed[0] != 'm') { //если не ненужная строка
                        if (tempstr[0] == "f") { //Андрюха, у нас грань, возможно криминал
                            face Tface = {}; //Временная грань
                            Tface.FaceMaterial = &*LastMaterial; //Материал равен последнему найденному
                            for (int i = 1; i < tempstr.size(); i++) {
                                vector<string>subface = splitbystrings(tempstr[i], "/");
                                if (subface[0] != "")
                                    Tface.v.push_back(vertexes[stoi(subface[0]) - 1]);
                                if (subface[1] != "")
                                    Tface.vt.push_back(texcoords[stoi(subface[1]) - 1]);
                                if (subface[2] != "")
                                    Tface.n.push_back(normals[stoi(subface[2]) - 1]);
                            }
                            GetNormalVector(Tface.NormalVector, Tface.n); //Получаем нормальный вектор
                            temp.faces.push_back(Tface); //Теперь всё хорошо, добавляем грань ко временному объекту
                        }
                        else if (parsed.substr(0, parsed.find(" ")) == "usemtl") { //Если используется другой материал
                            LastMaterial = FindMaterialByName(matProps, parsed.substr(parsed.find(" ") + 1, parsed.size())); //Это у нас теперь последний
                        }
                        else if (tempstr[0] != "s") {
                            vector<float>coords = Get_float_vector(tempstr, 1);
                            if (tempstr[0] == "v") //Если строка это вершины
                                vertexes.push_back(coords);
                            else if (tempstr[0] == "vt") //Тот же алгоритм с текстурными координатами
                                texcoords.push_back(coords);
                            else if (tempstr[0] == "vn") //И нормальными
                                normals.push_back(coords);
                        }
                    }
                }
                objects.push_back(temp); //Добавляем к объектам временный объект
            }
            else //А нет файла у нас!
                cout << "Error! Couldn't open the file!\n";
        }
        void Model_draw(bool is_smooth, bool Enable_dark, bool Enable_material, bool Enable_texture) { //Давайте рисовать!
            glPushMatrix();
            glTranslatef(ModelCoords[0], ModelCoords[1], ModelCoords[2]);
            for (__int8 i = 0; i < objects.size(); i++) { //Перебор по объектам
                Object& objref = objects[i]; //Чтобы не усугублять ситуацию избавимся хотя бы от одного индекса
                for (int j = 0; j < objref.faces.size(); j++) { //Перебор по граням
                    glBegin(GL_POLYGON); //Начинаем рисовать полигон
                    if (Enable_material)
                        glColor3f(objref.faces[j].FaceMaterial->DiffuseColor[0], objref.faces[j].FaceMaterial->DiffuseColor[1], objref.faces[j].FaceMaterial->DiffuseColor[2]);
                    if (!is_smooth)
                        glNormal3fv(objref.faces[j].NormalVector);
                    for (int k = 0; k < objref.faces[j].v.size(); k++) {
                        if (is_smooth)
                        glNormal3f(objref.faces[j].n[k][0], objref.faces[j].n[k][1], objref.faces[j].n[k][2]);
                        if (Enable_texture)
                            glTexCoord2f(objref.faces[j].vt[k][0], objref.faces[j].vt[k][1]);
                        glVertex3f(objref.faces[j].v[k][0], objref.faces[j].v[k][1], objref.faces[j].v[k][2]);
                    }
                    glEnd(); //Дорисовались
                }
            }
            glPopMatrix();
        }
        void Translate(GLfloat x, GLfloat y, GLfloat z) {
            ModelCoords[0] = x;
            ModelCoords[1] = y;
            ModelCoords[2] = z;
        }
    private:
        void Add_material() { //Добавления материала
            const string Mfile = materials; //Строка для имени материала
            ifstream MatFile(Mfile); //Читаем из файла
            Material temp; //Временный материал
            string parsed; //Строка для парсинга
            bool MaterialFound = false;
            if (MatFile.is_open()) {
                while (getline(MatFile, parsed)) {
                    try {
                        if (parsed.substr(0, parsed.find(" ")) == "newmtl") {
                            if (MaterialFound)
                                matProps.push_back(temp);
                            else
                                MaterialFound = 1;
                            temp = *new Material;
                            temp.name = parsed.substr(parsed.find(" ") + 1, parsed.size());
                        }
                        else if (parsed.substr(0, parsed.find(" ")) == "Ns")
                            temp.Reflection_coefficient = stof(parsed.substr(parsed.find(" ") + 1, parsed.size()));
                        else if (parsed.substr(0, parsed.find(" ")) == "Ka")
                            AddValsToArray(temp.AmbientLightColor, parsed);
                        else if (parsed.substr(0, parsed.find(" ")) == "Kd")
                            AddValsToArray(temp.DiffuseColor, parsed);
                        else if (parsed.substr(0, parsed.find(" ")) == "Ks")
                            AddValsToArray(temp.ReflectionColor, parsed);
                        else if (parsed.substr(0, parsed.find(" ")) == "d")
                            temp.Transparency = stof(parsed.substr(parsed.find(" ") + 1, parsed.size()));
                    }
                    catch (...) {
                        cout << "Error occured while reading: " << parsed << "\n";
                    }
                }
                matProps.push_back(temp);
            }
            else {
                cout << "Couldn't find Material file :(\n";
            }
        }
        void AddValsToArray(vector<float>& Property, string read_from) {
            read_from.erase(0, read_from.find(" ") + 1);
            Property.push_back(stof(read_from.substr(0, read_from.find(" "))));
            read_from.erase(0, read_from.find(" ") + 1);
            Property.push_back(stof(read_from.substr(0, read_from.find(" "))));
            read_from.erase(0, read_from.find(" ") + 1);
            Property.push_back(stof(read_from));
        }
        Material* FindMaterialByName(vector<Material>& Matlist, string Matname) {
            for (int i = 0; i < Matlist.size(); i++) {
                if (Matname == Matlist[i].name)
                    return &Matlist[i];
            }
            return &*new Material;
        }
        void GetNormalVector(GLfloat (&NormalVec)[3],vector<vector<GLfloat>>& Normals) {
            for (int i = 0; i < Normals.size(); i++) {
                NormalVec[0] += Normals[i][0];
                NormalVec[1] += Normals[i][1];
                NormalVec[2] += Normals[i][2];
            }
        }
    };
}
