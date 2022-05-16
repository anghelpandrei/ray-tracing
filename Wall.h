#ifndef RAYTRACING_WALL_H
#define RAYTRACING_WALL_H
#include "Vector2.h"
#include <complex>

using namespace std;
class Wall : public Vector2 {
public:

    Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma);
    
    Vector2 Wall::image(const Vector2& p);
    bool inter(const Vector2& p1, const Vector2& p2, Vector2& interP);  //renvoie le point d'intersection entre le mur et la droite passant par p1 et p2
    bool inter(const Vector2& p1, const Vector2& p2);                   //renvoie le point d'intersection entre le mur et la droite passant par p1 et p2
    complex<double> transmission(const Vector2& p1, const Vector2& p2); //renvoie le coefficient de transmission à travers le mur
    complex<double> reflection(const Vector2& p1, const Vector2& p2);   //renvoie le coefficient de réflexion sur le mur
    
    double length;    //longueur du mur
    double angle;     //angle du mur par rapport à l'axe des abscisses
    double thickness; //épaisseur du mur
    double epsR;      //permittivité relative
    double sigma;     //conductivité électrique

    double x2;        //abscisse de l'autre bout du mur
    double y2;        //ordonnée de l'autre bout du mur

    Vector2 X;        //vecteur d'un bout du mur
    Vector2 X2;       //vecteur de l'autre bout du mur
    
    double nx;        //composantes du vecteur normal du mur
    double ny;
   
    double ux;        //composantes du vecteur directeur du mur
    double uy;

    double alpha_m;          //alpha_m correspondant au mur
    double beta_m;           //beta_m correspondant au mur
    complex<double> gamma_m; //gamma_m correspondant au mur
    complex<double> Z_m;     //Z_m correspondant au mur
};


#endif //RAYTRACING_WALL_H
