#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H


class Object {
public:

    double getY() const;
    double getX() const;
    void setX(double x);
    void setY(double y);

    double x; //position X du centre de l'objet
    double y; //position Y du centre de l'objet
    Object(double x, double y);
};


#endif //RAYTRACING_OBJECT_H
