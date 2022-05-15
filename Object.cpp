#include "Object.h"
#include <cmath>

double Object::dotP(Object& vec) {
    return x * vec.x + y * vec.y;
}

double Object::norm() {
    return sqrt(x * x + y * y);
}

Object::Object() {
    x = 0;
    y = 0;
}

Object::Object(double x, double y) {
    this->x = x;
    this->y = y;
}

Object operator+(const Object& obj1, const Object& obj2) {
    return Object(obj1.x + obj2.x, obj1.y + obj2.y);
}

Object operator-(const Object& obj1, const Object& obj2) {
    return Object(obj1.x - obj2.x, obj1.y - obj2.y);
}

Object operator*(const Object& obj, const double& scal)
{
    return Object(obj.x * scal, obj.y * scal);
}

Object operator*(const double& scal, const Object& obj)
{
    return Object(obj.x * scal, obj.y * scal);
}

Object operator/(const Object& obj, const double& scal)
{
    return Object(obj.x / scal, obj.y / scal);
}

std::ostream& operator<<(std::ostream& ostream, const Object& obj) {
    ostream << "x = " << obj.x << ", y = " << obj.y;
    return ostream;
}
