#include "Object.h"

double Object::getX() const {
    return x;
}

double Object::getY() const {
    return y;
}

void Object::setX(double x) {
    this->x = x;
}

void Object::setY(double y) {
    this->y = y;
}
Object::Object(double x, double y) {
    this->x = x;
    this->y = y;
}
