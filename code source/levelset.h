#ifndef head_levelset
#define head_levelset

#include <QtGui/QImage>
#include <QtGui/QColor>

namespace levelset
{
	int H(double x);
	double Dirac(int H);
	double dphi(double **phi,int x,int y);
	int C(double **phi,int **intensite,int width,int height,int test);
	double lenght(double **phi, int width,int height);
	double area(double **phi, int width,int height);
	double div(double **phi,int x,int y);
	double F2(double **phi,int intensite,int x,int y,int c1,int c2,double landa1,double landa2,double mu,double nu);
	double F1(int intensite,double lenght,double area,int c1,int c2,double landa1,double landa2,double mu,double nu);
	double F3(double **phi,int x,int y,int u0);
	void tab_intensite(QImage *image,int **intensite);
	void level_zero(int x,int y,int ray,double **phi,int width,int height);
	void essai(QImage *image,int x,int y,int ray,int iter,int nbr,double landa1,double landa2,double mu,double nu,double dt);
	void evoluer_phi(double **phinew,double **phiold,int **intensite, int width,int height,int nbr,double landa1,double landa2,double mu,double nu,double dt);   
}
#endif
