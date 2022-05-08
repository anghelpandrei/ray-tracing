#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H


class Object {
public:
    Object();
    double dotP(Object vec);
    double norm();

    double x; //position X du centre de l'objet
    double y; //position Y du centre de l'objet
    Object(double x, double y);

    friend Object operator+(const Object& obj1, const Object& obj2);
    friend Object operator-(const Object& obj1, const Object& obj2);
    friend Object operator*(const Object& obj, const double& scal);
    friend Object operator*(const double& scal, const Object& obj);
    friend Object operator/(const Object& obj, const double& scal);
};


#endif //RAYTRACING_OBJECT_H
