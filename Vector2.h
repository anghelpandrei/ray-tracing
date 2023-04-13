#include <ostream>
#pragma once

class Vector2 {
public:
    Vector2() = default;                  //constructeur par défaut
    inline double Vector2::dotP(Vector2& vec) { return x * vec.x + y * vec.y; }   //produit scalaire
    inline double Vector2::norm() { return sqrt(x * x + y * y); }                 //norme d'un vecteur
    double x;                  //composante x du vecteur
    double y;                  //composante y du vecteur
    Vector2(double x, double y) : x(x), y(y) {};//constructeur permettant de définir un point ou un vecteur avec des coordonnees
    friend inline Vector2 operator+(const Vector2& vec1, const Vector2& vec2) { return Vector2(vec1.x + vec2.x, vec1.y + vec2.y); } //addition vectorielle
    friend inline Vector2 operator-(const Vector2& vec1, const Vector2& vec2) { return Vector2(vec1.x - vec2.x, vec1.y - vec2.y); } //soustraction vectorielle
    friend inline Vector2 operator*(const Vector2& vec, const double& scal) { return Vector2(vec.x * scal, vec.y * scal); }  //multiplication par un scalaire
    friend inline Vector2 operator*(const double& scal, const Vector2& vec) { return Vector2(vec.x * scal, vec.y * scal); }  //multiplication par un scalaire dans l'autre sens
    friend inline Vector2 operator/(const Vector2& vec, const double& scal) { return Vector2(vec.x / scal, vec.y / scal); }  //division par un scalaire
    friend inline std::ostream& operator<<(std::ostream& ostream, const Vector2& vec) { ostream << "(x = " << vec.x << ", y = " << vec.y << ")"; return ostream; }
};