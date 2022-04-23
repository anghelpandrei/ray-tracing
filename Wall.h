#ifndef RAYTRACING_WALL_H
#define RAYTRACING_WALL_H


#include "Object.h"

class Wall : public Object {
public:
    Wall(double posX, double posY, double length, double angle, double thickness, double epsR, double sigma);

public:
    double getLength() const;

    void setLength(double length);

    double getAngle() const;

    void setAngle(double angle);

    double getThickness() const;

    void setThickness(double thickness);

    double getEpsR() const;

    void setEpsR(double epsR);

    double getSigma() const;

    void setSigma(double sigma);

private:
    double length;    //longueur du mur
    double angle;     //angle du mur par rapport à l'axe des abscisses
    double thickness; //épaisseur du mur/paroi
    double epsR;      //permittivité relative
    double sigma;     //conductivité électrique

    double posX;
    double posY;
};


#endif //RAYTRACING_WALL_H
