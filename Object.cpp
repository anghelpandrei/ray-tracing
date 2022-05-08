#include "Object.h"
#include <cmath>

double Object::dotP(Object vec) {
    return x * vec.x + y * vec.y;
}

Object Object::crossP(Object vec) {
    return Object(vec.y - y, x - vec.x);
}

double Object::norm() {
    return sqrt(x * x + y * y);
}

Object::Object() {

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