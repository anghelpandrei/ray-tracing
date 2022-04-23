#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H


class Object {
public:

    double getPosY() const;
    double getPosX() const;
    void setPosX(double posX);
    void setPosY(double posY);

    double posX; //position X du centre de l'objet
    double posY; //position Y du centre de l'objet
};


#endif //RAYTRACING_OBJECT_H
