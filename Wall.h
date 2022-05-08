#ifndef RAYTRACING_WALL_H
#define RAYTRACING_WALL_H


#include "Object.h"

class Wall : public Object {
public:

    Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma);
    
    bool sameSide(Object point1, Object point2);
    Object image(Object point);
    Object symmetryP(Object point);
    Object reflexP(Object point1, Object point2);
    double cos_i(Object p1, Object p2);
    double sin_i(double cos_i);
    double sin_t(double sin_i);
    double cos_t(double sin_t);
    complex<double> we(double cos_i);
    complex<double> transmission(complex<double> ref_perp, double cos_i);
    


    double length;    //longueur du mur
    double angle;     //angle du mur par rapport à l'axe des abscisses
    double thickness; //épaisseur du mur/paroi
    double epsR;      //permittivité relative
    double sigma;     //conductivité électrique

    double x2;
    double y2;

    double a;
    double b;

    double nx;
    double ny;

    double ux;
    double uy;

    Object n;
    Object u;

    double beta;
    complex<double> j;
    double alpha_m;
    double beta_m;
    complex<double> gamma_m;
    complex<double> Z_m;
};


#endif //RAYTRACING_WALL_H
