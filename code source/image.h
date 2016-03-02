#ifndef rimage
#define rimage

#include <QtGui/QImage>
#include <QtGui/QColor>
#include <math.h>

struct pixeltype
{
	int x;
	int y;
	QRgb color;
	int r,g,b;
};
struct floatpixel
{
	float r,b,g;
};
		

namespace RImage
{
				//les filtres
	void median(QImage *);
	void mediansans(QImage *);
	void gaussien(QImage *);
	void gaussiensans(QImage *);
	void passehaut(QImage *);

			//gardien d'un pixel
	pixeltype  soblepixel(QImage *,int,int);
	pixeltype  gradienpixel(QImage *,int,int);
	pixeltype  perwitpixel(QImage *,int,int);
	floatpixel NormeGrad(QImage *,int,int);
			//gradien d'une image
	void soble(QImage *);
	void gradien(QImage *);
	void perwit(QImage *);
	void gradienami(QImage *,float);
			//optionnelle
	void Niveau_Gris(QImage *);
	void detecte_objet(QImage *image,int x,int y, QRgb colour);
	void aide_detecte_objet(QImage *image,int x,int y,QRgb src);
	double distance(QImage *image,int x1,int y1,int x2,int y2);
	void surface(QImage *image,int x,int y,QRgb src);
	void dupliquer(QImage *image,QRgb src);
	double surfaceMM(QImage *image,QRgb src);

	///////
	
	
};


#endif
