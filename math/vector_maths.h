#pragma once
#include <vector>

class Vector2f{
public:
    float x, y;
    Vector2f(float x, float y){
        this->x = x;
        this->y = y;
    }
    inline void add(float x_to_add, float y_to_add){
        x += x_to_add;
        y += y_to_add;
    }
};

