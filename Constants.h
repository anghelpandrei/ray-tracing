// Constants.h
#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

const double epsR = 4.8;
const double sigma = 0.018;
const double thickness = 0.15;
const double Z_0 = 377;
const double mu_0 = 0.000001256637;
const double eps_0 = 0.000000000008854187;
const double c = 299792458;
const double Ra = 73;
const complex<double> j(0.0, 1.0);
const double freq = 868300000;
const double omega = freq * 2 * M_PI; //pulsation de l'onde
const double lambda = c / freq; //longueur d'onde
const double beta = omega / c;
const double ptxgtx = 0.00164;

const double epsR_GRP = 8.7;
const double epsR_plast = 2.25;
const double sigma_GRP = 0.868;
const double sigma_plast = 0.003;
const double thickness_plane = 0.1;
#endif
