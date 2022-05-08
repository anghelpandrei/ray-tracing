#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H


class Object {
public:
    Object();                  //constructeur par défaut (utile pour initialiser des listes)
    double dotP(Object vec);   //produit scalaire
    Object crossP(Object vec); //produit vectoriel
    double norm();             //norme d'un vecteur
    double x;                  //position X du centre de l'objet
    double y;                  //position Y du centre de l'objet
    Object(double x, double y);//constructeur permettant de définir un point ou un vecteur

    friend Object operator+(const Object& obj1, const Object& obj2); //addition vectorielle
    friend Object operator-(const Object& obj1, const Object& obj2); //soustraction vectorielle
    friend Object operator*(const Object& obj, const double& scal);  //multiplication par un scalaire
    friend Object operator*(const double& scal, const Object& obj);  //multiplication par un scalaire dans l'autre sens
    friend Object operator/(const Object& obj, const double& scal);  //division par un scalaire
};


#endif //RAYTRACING_OBJECT_H
