#include "thread.h"
#include "image.h"
#include "levelset.h"

double max1(double x,double y)
{
	if(x>y) return x;
	else return y;
}
double min1(double x,double y)
{
	if(x>y) return y;
	else return x;
}

#include <QMessageBox>

Thread::Thread(QObject* parent) : QThread(parent)
{
	test=true;
}
 
Thread::~Thread()
{
}
    
void Thread::run()
{
	QImage *inter;
	
		
	int wid=image->width();
	int hei=image->height();
	
	double **phiold=new double *[wid];
	for(int i=0;i<wid;i++)		phiold[i]=new double [hei];

	double **phinew=new double *[wid];
	for(int i=0;i<wid;i++)		phinew[i]=new double [hei];

	int **intensite=new int *[wid];
	for(int i=0;i<wid;i++)		intensite[i]=new int [hei];

	levelset::tab_intensite(image, intensite);
	levelset::level_zero(x, y, ray, phiold, wid, hei);
		
	int i=0;
	while(test)
	{	
		levelset::evoluer_phi(phinew,phiold,intensite,wid,hei,nbr,landa1,landa2,mu,nu,dt);
		for(int w=0;w<wid;w++)
			for(int h=0;h<hei;h++)
				phiold[w][h]=phinew[w][h];
		
		inter=new QImage(*image);

		for(int w=1;w<wid-1;w++)
			for(int h=1;h<hei-1;h++)
			{
				double max_phi = max1( max1( phiold[w][h], phiold[w + 1][h] ), max1( phiold[w][h + 1] , phiold[w + 1][h + 1] ) );
				double min_phi = min1( min1( phiold[w][h], phiold[w + 1][h] ), min1(phiold[w][h + 1], phiold[w + 1][h + 1]));
	
				if ( max_phi > 0.0 && min_phi < 0.0 || phiold[w][h]==0 ) inter->setPixel(w , h, qRgb(255,0,0) );
			}
		emit signalIMG(*inter);

		msleep(30);
		i++;
	}
}
    
void Thread::setparame(QImage imag,int X,int Y,int Ray,int Nbr,double Landa1,double Landa2,double Mu,double Nu,double Dt)
{
	image=new QImage(imag);
	x=X;
	y=Y;
	ray=Ray;
	nbr=Nbr;
	landa1=Landa1;
	landa2=Landa2;
	mu=Mu;
	nu=Nu;
	dt=Dt;
}
void Thread::setTest(bool Test)
{
	test=Test;
}