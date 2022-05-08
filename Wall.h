#ifndef RAYTRACING_WALL_H
#define RAYTRACING_WALL_H
#include "Object.h"
#include <complex>

using namespace std;
class Wall : public Object {
public:

    Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma);
    
    bool isReflected(Object p1, Object p2); //renvoie true si la réflexion est possible (2 points du même côté du mur + point de réflexion sur le mur)
    Object image(Object p);                 //renvoie l'image d'un point p par symétrie axiale avec le mur
    Object interP(Object p1, Object p2);    //renvoie le point d'intersection entre le mur et la droite passant par p1 et p2
    Object refP(Object p1, Object p2);      //renvoie le point de réflexion sur le mur des points p1 et p2
    double cos_i(Object p1, Object p2);     //renvoie cos_theta_i
    double sin_i(double cos_i);             //renvoie sin_theta_i
    double sin_t(double sin_i);             //renvoie sin_theta_t
    double cos_t(double sin_t);             //renvoie cos_theta_t
    complex<double> we(double cos_i);
    complex<double> transmission(complex<double> ref_perp, double cos_i); //renvoie la valeur du coefficient de transmission
    complex<double> reflection(complex<double> ref_perp, double cos_i);   //renvoie la valeur du coefficient de réflexion
    


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

    double omega;            //pulsation de l'onde
    double beta;             //nombre d'onde dans le vide
    complex<double> j;       //unité imaginaire
    double alpha_m;          //alpha_m correspondant au mur
    double beta_m;           //beta_m correspondant au mur
    complex<double> gamma_m; //gamma_m correspondant au mur
    complex<double> Z_m;     //Z_m correspondant au mur
};


#endif //RAYTRACING_WALL_H
