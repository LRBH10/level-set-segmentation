#include "levelset.h"
#include <math.h>

/*#define landa1	1
#define landa2	1
#define dt		1
#define mu		0.01*(255*255)
#define nu		0*/
#define ep		1

double max(double x,double y)
{
	if(x>y) return x;
	else return y;
}
double min(double x,double y)
{
	if(x>y) return y;
	else return x;
}

/////////////LA CONVERSTION VERS LE NIVEAU DE GRIS
void levelset::tab_intensite(QImage *image,int **intensite)
{
	for(int w=0;w<image->width();w++)
		for(int h=0;h<image->height();h++)
		{
			QColor coleur(image->pixel(w,h));
			intensite[w][h]=coleur.red();
		}
}


//////////////////////LE LEVEL  ZERO
void levelset::level_zero(int x,int y,int ray,double **phi,int width,int height)
{
	for(int w=0;w<width;w++)
		for(int h=0;h<height;h++)
		{
			float result=sqrt((float)((w-x)*(w-x)+(h-y)*(h-y)));
			if((int)(result)==ray)  phi[w][h]=0;
			else phi[w][h]=-(result-ray);			
		}
}


/////////////fonction HEAVSIDE
int levelset::H(double x)
{
	return 0.5*(1+(2/3.14 )*atan( x/ ep ) ); 
}

///////////////fonction DIRAC
double levelset::Dirac(int H)
{	
	return  ( 1/3.14 )*( ep/( ep*ep + H*H)); 
}

///////////////fonction DELTA  PHI
double levelset::dphi(double **phi,int x,int y)
{
	double inter;
	double part1=phi[x+1][y]-phi[x-1][y];
	double part2=phi[x][y+1]-phi[x][y-1];
	inter=0,5*0,5*(part1*part1+part2*part2);
	return sqrt(inter);
}

//////////////////////LENGHT
double levelset::lenght(double **phi, int width,int height)
{
	double len=0;
	for(int w=1;w<width-1;w++)
		for(int h=1;h<height-1;h++)
		{
			double max_phi = max(max(phi[w][h], phi[w + 1][h]), max(phi[w][h + 1], phi[w + 1][h + 1]));
			double min_phi = min(min(phi[w][h], phi[w + 1][h]), min(phi[w][h + 1], phi[w + 1][h + 1]));
			if(max_phi > 0.0 && min_phi < 0.0||phi[w][h]==0)  len+=Dirac(H(phi[w][h]))*dphi(phi,w,h);
		}
	return len;
}



////////////////SURFACE
double levelset::area(double **phi, int width,int height)
{
	double area=0;
	for(int w=1;w<width-1;w++)
		for(int h=1;h<height-1;h++)
		if(phi[w][h]>=0)  area+=H(phi[w][h]);
	return area;
}

///////////////fonction de CALCULE DE C1 et C2
int levelset::C(double **phi,int **intensite,int width,int height,int test)
{
	int nbr=0;
	int c=0;
	for(int w=0;w<width;w++)
		for(int h=0;h<height;h++)
		{
			if(phi[w][h]<=0 && test==1)			
			{
				c+=intensite[w][h];
				nbr++;
			}
			else if(phi[w][h]>0 && test==2)
			{
				c+=intensite[w][h];
				nbr++;
			}
		}
	return c/nbr;
}



///////////////////LA FONCTION DIV
double levelset::div(double **phi,int i,int j)
{
	double phi_x=(phi[i][j+1]-phi[i][j-1])/2.0;
	double phi_y=(phi[i+1][j]-phi[i-1][j])/2.0 ;
	double phi_xx=(phi[i][j+1]+phi[i][j-1]-2*phi[i][j])/1.0;
	double phi_yy=(phi[i+1][j]+phi[i-1][j]-2*phi[i][j])/1.0;
	double phi_xy =(phi[i+1][j+1]+phi[i-1][j-1]-phi[i-1][j+1]-phi[i+1][j-1])/4.0 ;
	
	double k=0;
	if(phi_x==0 && phi_y==0) k=0;
	else k=(phi_xx*phi_y*phi_y-2*phi_x*phi_y*phi_xy+phi_yy*phi_x*phi_x)/ pow((phi_x*phi_x+phi_y*phi_y),3.0/2.0);

	return k;
}

////////////////////LES TERME DES VITESSE

///////////////1
double levelset::F1(int u0,double lenght,double area,int c1,int c2,double landa1,double landa2,double mu,double nu)
{
	int part1=abs (u0-c1);
	int part2=abs (u0-c2);
	double inter= mu*lenght + nu*area + landa1*part1*part1 - landa2*part2*part2;
	return inter;
}


///////////////2
double levelset::F2(double **phi,int u0,int x,int y,int c1,int c2,double landa1,double landa2,double mu,double nu)
{
	double dirac = Dirac ( H(phi[x][y]) );
	double k = div(phi,x,y);
	int part1 = abs (u0-c1);
	int part2 = abs (u0-c2);
	double part3 = mu *  k  - nu + landa1 * part1*part1 - landa2 * part2*part2;
	double inter = dirac*part3;
	return inter;
}

/////////////3
double levelset::F3(double **phi,int x,int y,int u0)
{
	double k = div ( phi ,x ,y);
	double result = ( 1 + ( ep * k ) ) / ( 1 + u0 );
	return result;
}

/////////////////////LA FONCTION LEVEL SET 
void levelset::evoluer_phi(double **phinew,double **phiold,int **intensite, int width,int height,int nbr,double landa1,double landa2,double mu,double nu,double dt)
{
	int c1 = C(phiold, intensite, width, height, 1);
	int c2 = C(phiold, intensite, width, height, 2);
	double ar = area( phiold, width, height);
	double la = lenght( phiold, width, height);
		
	for(int w=1;w<width-1;w++)
		for(int h=1;h<height-1;h++)
		{
			if(nbr==1)		phinew[w][h]=phiold[w][h]+F1(intensite[w][h],la,ar,c1,c2,landa1,landa2,mu,nu)*dt;
			else if(nbr==2)	phinew[w][h]=phiold[w][h]+F2(phiold,intensite[w][h],w,h,c1,c2,landa1,landa2,mu,nu)*dt;
			//phinew[w][h] = phiold[w][h] + F3(phiold, w, h, intensite[w][h]) * dt * dphi(phiold,w,h);
		}
}

//////////////////ESSAI
void levelset::essai(QImage *image,int x,int y,int ray,int iter,int nbr,double landa1,double landa2,double mu,double nu,double dt)
{
	double cour[150];

	int wid=image->width();
	int hei=image->height();
	
	double **phiold=new double *[wid];
	for(int i=0;i<wid;i++)		phiold[i]=new double [hei];

	double **phinew=new double *[wid];
	for(int i=0;i<wid;i++)		phinew[i]=new double [hei];

	int **intensite=new int *[wid];
	for(int i=0;i<wid;i++)		intensite[i]=new int [hei];

	levelset::tab_intensite(image, intensite);
	level_zero(x, y, ray, phiold, wid, hei);

	int k=0;
	for(int i=0;i<iter;i++)
	{
		evoluer_phi(phinew,phiold,intensite,wid,hei,nbr,landa1,landa2,mu,nu,dt);
		for(int w=0;w<wid;w++)
			for(int h=0;h<hei;h++)
				phiold[w][h]=phinew[w][h];

		double somme=0;
		for(int w=0;w<wid;w++)
			for(int h=0;h<hei;h++)
				somme+=div(phiold,x,h);
		cour[k]=somme/wid*hei;
		k++;
	}
	
	QImage *ima=new QImage("inter.bmp");
	/*for(int i=0;i<k;i++)
	{
		//image->setPixel(50+j,150+cour[j],qRgb(255,0,0));
		/*FILE *out;
		fopen_s(&out,"rabah2.txt","w");
		fprintf(out,"%lf\n",cour[i]);
		fclose(out);
	}

	ima->save("inter.bmp");*/

	for(int w=1;w<wid-1;w++)
		for(int h=1;h<hei-1;h++)
		{
			double max_phi=max(max(phiold[w][h],phiold[w+1][h]),max(phiold[w][h+1],phiold[w+1][h+1]));
			double min_phi=min(min(phiold[w][h],phiold[w+1][h]),min(phiold[w][h+1],phiold[w+1][h+1]));

			if(max_phi>0.0&&min_phi<0.0||phiold[w][h]==0)image->setPixel(w,h,qRgb(255,0,0));
		}
}
