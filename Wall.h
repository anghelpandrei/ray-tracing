#ifndef RAYTRACING_WALL_H
#define RAYTRACING_WALL_H
#include "Object.h"
#include <complex>

using namespace std;
class Wall : public Object {
public:

    Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma);
    
    Object image(const Object& p);                 //renvoie l'image d'un point p par symétrie axiale avec le mur
    bool inter(const Object& p1, const Object& p2, Object& interP);    //renvoie le point d'intersection entre le mur et la droite passant par p1 et p2
    bool inter2(const Object& p1, const Object& p2);    //renvoie le point d'intersection entre le mur et la droite passant par p1 et p2
    complex<double> transmission(const Object& p1, const Object& p2); //renvoie le coefficient de transmission à travers le mur
    complex<double> reflection(const Object& p1, const Object& p2);   //renvoie le coefficient de réflexion sur le mur
    
    double length;    //longueur du mur
    double angle;     //angle du mur par rapport à l'axe des abscisses
    double thickness; //épaisseur du mur
    double epsR;      //permittivité relative
    double sigma;     //conductivité électrique

    double x2;        //abscisse de l'autre bout du mur
    double y2;        //ordonnée de l'autre bout du mur

    Object X;         //vecteur d'un bout du mur
    Object X2;        //vecteur de l'autre bout du mur
    Object n;         //vecteur normal normalisé
    Object u;         //vecteur directeur normalisé

    double alpha_m;          //alpha_m correspondant au mur
    double beta_m;           //beta_m correspondant au mur
    complex<double> gamma_m; //gamma_m correspondant au mur
    complex<double> Z_m;     //Z_m correspondant au mur
};


#endif //RAYTRACING_WALL_H
