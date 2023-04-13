// Constants.h
#pragma once
#include <SDL_stdinc.h>

const double epsR = 4.8;
const double sigma = 0.018;
const double thickness = 0.15;
const double Z_0 = 376.7303135;
const double mu_0 = 1.256637061e-6;
const double eps_0 = 8.854187817e-12;
const double c = 299792458;
const double Ra = 73;
const complex<double> j(0.0, 1.0);
//const double freq = 868300000;
const double freq = 60e9;
const double omega = freq * 2 * M_PI; //pulsation de l'onde
const double lambda = c / freq; //longueur d'onde
const double beta = omega / c;

const double epsR_GRP = 8.7;
const double epsR_plast = 2.25;
const double sigma_GRP = 0.868;
const double sigma_plast = 0.003;
const double thickness_plane = 0.1;

const double epsilon = 1.0e-4;