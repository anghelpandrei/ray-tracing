#include "Constants.h"
Vector2 emetteurTP8(32, 10);    //emetteur du TP8
Vector2 recepteurTP8(47, 65);   //recepteur du TP8

Vector2 emetteur1(7, 17);    //emetteurs de l'avion
Vector2 emetteur2(29, 11);

Wall walls[] = { 
    Wall(0, 0, 80, 90, thickness, epsR, sigma),
    Wall(0, 20, 80, 0, thickness, epsR, sigma),
    Wall(0, 80, 80, 0, thickness, epsR, sigma),
};

const int n_walls = 55;
Wall wallsPlane[] = {
    //PAROIS EXTERIEURES DE L'AVION
    Wall(0, 10, 31, 0, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(31, 10, sqrt(2), 45, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(32, 11, 6, 90, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(32, 17, sqrt(2), 135, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(31, 18, 31, 180, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(0, 18, 4.008582683, 183.75,thickness_plane,epsR_GRP,sigma_GRP),//13
    Wall(-4, 17.73782615, 3.025886882,187.5,thickness_plane,epsR_GRP,sigma_GRP),//12
    Wall(-7,17.286866, 2.070552361, 195, thickness_plane,epsR_GRP,sigma_GRP),//11
    Wall(-9, 16.80697027, 1.732050808, 210, thickness_plane,epsR_GRP,sigma_GRP),//10
    Wall(-10.5, 15.94094487, 1.414213562, 225,thickness_plane,epsR_GRP,sigma_GRP),//9
    Wall(- 11.5, 14.94094487, 1.0, 240,thickness_plane,epsR_GRP,sigma_GRP),//8

    Wall(-12,14.07491947, 0.15, 270, thickness_plane,epsR_GRP,sigma_GRP),//7

    Wall(-12, 13.925080536454264, 1.0, 300, thickness_plane, epsR_GRP, sigma_GRP),//6

    Wall(-11.5, 13.059055132669824, 1.414213562, 315, thickness_plane, epsR_GRP, sigma_GRP),//5

    Wall(-10.5, 12.059055132669824, 1.732050808, 330, thickness_plane, epsR_GRP, sigma_GRP),//4

    Wall(-9, 11.193029728885385,  2.070552361, 345, thickness_plane, epsR_GRP, sigma_GRP),//3

    Wall(-7, 10.65713134402314, 3.025886882, 352.5, thickness_plane, epsR_GRP, sigma_GRP),//2

    Wall(-4, 10.262173851260952, 4.008582683, 356.25, thickness_plane, epsR_GRP, sigma_GRP),//1
    //18

    //PAROIS INTERIEURES DE L'AVION
    Wall(-4, 17.73782615, 7.4768, 270, thickness_plane, epsR_GRP, sigma_GRP),//12
    Wall(0, 10, 3, 90, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(0, 18, 3, 270, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(8, 10, 3, 90, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(8, 18, 3, 270, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(26, 10, 3, 90, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(26, 18, 3, 270, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(28, 10, 3, 90, thickness_plane, epsR_GRP, sigma_GRP),
    Wall(28, 18, 3, 270, thickness_plane, epsR_GRP, sigma_GRP),
    //27
    //SIEGES 1ERE CLASSE HAUT
    Wall(2, 10 , 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(4, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(6, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    //30
    //SIEGES 1ERE CLASSE BAS
    Wall(2, 18 , 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(4, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(6, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    //33
    //SIEGES 2EME CLASSE HAUT
    Wall(10, 10 , 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(11.5, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(13, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(14.5, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(16, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(17.5, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(19, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(20.5, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(22, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(23.5, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    Wall(25, 10, 3, 90, thickness_plane, epsR_plast, sigma_plast),
    //44
    //SIEGES 2EME CLASSE BAS
    Wall(10, 18 , 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(11.5, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(13, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(14.5, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(16, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(17.5, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(19, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(20.5, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(22, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(23.5, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    Wall(25, 18, 3, 270, thickness_plane, epsR_plast, sigma_plast),
    //55
};