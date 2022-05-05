#ifndef RAYTRACING_WALL_H
#define RAYTRACING_WALL_H


#include "Object.h"

class Wall : public Object {
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

    double getX2() const;
    void setX2(double x2);

    double getY2() const;
    void setY2(double y2);

    double getNx() const;
    void setNx(double nx);

    double getNy() const;
    void setNy(double ny);

    Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma);
    

    Object image(Object point);
    Object symmetryP(Object point);
    Object reflexP(Object point1, Object point2);

    double nx;
    double ny;

private:
    double length;    //longueur du mur
    double angle;     //angle du mur par rapport à l'axe des abscisses
    double thickness; //épaisseur du mur/paroi
    double epsR;      //permittivité relative
    double sigma;     //conductivité électrique

    double x2;
    double y2;

    double a;
    double b;

    double ux;
    double uy;
};


#endif //RAYTRACING_WALL_H
