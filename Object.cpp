//
// Created by Andrei on 23/04/2022.
//

#include "Object.h"

double Object::getPosX() const {
    return posX;
}

double Object::getPosY() const {
    return posY;
}

void Object::setPosX(double posX) {
    Object::posX = posX;
}

void Object::setPosY(double posY) {
    Object::posY = posY;
}
