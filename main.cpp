#include <iostream>
#include "Transmitter.h"
#include "Receiver.h"
#include "Wall.h"
#define epsR 4.8
#define sigma 0.018
#define thickness 0.15
//#define R 73

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    Transmitter emetteur;
    Receiver recepteur;
    //Wall parois [3];

    /*parois[0].setPosX(0);
    parois[0].setPosY(0);
    parois[0].setLength(80);
    parois[0].setAngle(90);
    parois[0].setEpsR(epsR);
    parois[0].setSigma(sigma);
    parois[0].setThickness(thickness);

    parois[1].setPosX(0);
    parois[1].setPosY(20);
    parois[1].setLength(80);
    parois[1].setAngle(0);
    parois[1].setEpsR(epsR);
    parois[1].setSigma(sigma);
    parois[1].setThickness(thickness);

    parois[2].setPosX(0);
    parois[2].setPosY(80);
    parois[2].setLength(80);
    parois[2].setAngle(0);
    parois[2].setEpsR(epsR);
    parois[2].setSigma(sigma);
    parois[2].setThickness(thickness);*/

    emetteur.setPosX(32);
    emetteur.setPosY(10);
    recepteur.setPosX(47);
    recepteur.setPosY(65);

    return 0;
}