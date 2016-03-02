#include "image.h"
#include <stdio.h>
void remplir(pixeltype &pixel,int type=1)
{
	if(type==1)
	{
		int r,g,b;
		QColor inter(pixel.color);
		inter.getRgb(&r,&g,&b);
		pixel.r=r;
		pixel.g=g;
		pixel.b=b;
	}
	else if(type==2)
	{
		pixel.color=QColor(pixel.r,pixel.g,pixel.b).rgb();
	}
}

int valid(int color)
{
	if(color>255)color=255;
	else if(color<0)color=0;
	return color;
}

		//************les filters*/
/////////////////////gaussien (moyenne)
	
QRgb getgaussien(QRgb *tab)
{
	int r,g,b;
	int resultr=0,resultg=0,resultb=0;
	for(int i=0;i<9;i++)
	{
		QColor *inter=new QColor(tab[i]);
		inter->getRgb(&r,&g,&b);
		resultr+=r;
		resultg+=g;
		resultb+=b;
	}
		resultr/=9;
		resultb/=9;
		resultg/=9;
	return QColor(valid(resultr),valid(resultg),valid(resultb)).rgb();
}

			//avec modification
void RImage::gaussien(QImage *image)
{
	QRgb tab[9];
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			tab[0]=image->pixel(w-1,h);
			tab[1]=image->pixel(w+1,h);
			tab[2]=image->pixel(w,h-1);
			tab[3]=image->pixel(w,h+1);
			tab[4]=image->pixel(w-1,h-1);
			tab[5]=image->pixel(w+1,h-1);
			tab[6]=image->pixel(w-1,h+1);
			tab[7]=image->pixel(w+1,h+1);
			tab[8]=image->pixel(w,h);
			image->setPixel(w,h,getgaussien(tab));
		}
}
				//sans modification
void RImage::gaussiensans(QImage *image)
{
	pixeltype *gaussien=new pixeltype[image->width()*image->height()];
	QRgb tab[9];
	int f=0;
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			tab[0]=image->pixel(w-1,h);
			tab[1]=image->pixel(w+1,h);
			tab[2]=image->pixel(w,h-1);
			tab[3]=image->pixel(w,h+1);
			tab[4]=image->pixel(w-1,h-1);
			tab[5]=image->pixel(w+1,h-1);
			tab[6]=image->pixel(w-1,h+1);
			tab[7]=image->pixel(w+1,h+1);
			tab[8]=image->pixel(w,h);
			gaussien[f].color=getgaussien(tab);
			gaussien[f].x=w;
			gaussien[f].y=h;
			f++;
		}
	for(int i=0;i<f;i++)
		image->setPixel(gaussien[i].x,gaussien[i].y,gaussien[i].color);
}
//////////////////////median
QRgb getmedian(QRgb *tab)
{
	for(int i=0;i<9;i++)
		for(int j=i+1;j<9;j++)
			if(tab[i]>tab[j])
			{
				QRgb inter=tab[i];
				tab[i]=tab[j];
				tab[j]=inter;
			}
			return tab[4];
}
		//avec modification
void RImage::median(QImage *image)
{
	QRgb tab[9];
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			tab[0]=image->pixel(w-1,h);
			tab[1]=image->pixel(w+1,h);
			tab[2]=image->pixel(w,h-1);
			tab[3]=image->pixel(w,h+1);
			tab[4]=image->pixel(w-1,h-1);
			tab[5]=image->pixel(w+1,h-1);
			tab[6]=image->pixel(w-1,h+1);
			tab[7]=image->pixel(w+1,h+1);
			tab[8]=image->pixel(w,h);
			image->setPixel(w,h,getmedian(tab));
		}
}
		//sans modification
void RImage::mediansans(QImage *image)
{
	pixeltype *median=new pixeltype[image->width()*image->height()];
	QRgb tab[9];
	int f=0;
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			tab[0]=image->pixel(w-1,h);
			tab[1]=image->pixel(w+1,h);
			tab[2]=image->pixel(w,h-1);
			tab[3]=image->pixel(w,h+1);
			tab[4]=image->pixel(w-1,h-1);
			tab[5]=image->pixel(w+1,h-1);
			tab[6]=image->pixel(w-1,h+1);
			tab[7]=image->pixel(w+1,h+1);
			tab[8]=image->pixel(w,h);
			median[f].color=getmedian(tab);
			median[f].x=w;
			median[f].y=h;
			f++;
		}
	for(int i=0;i<f;i++)
		image->setPixel(median[i].x,median[i].y,median[i].color);
}
/////////////////passe haut
QRgb getpassehaut(pixeltype *tab)
{
	int resultr=0,resultg=0,resultb=0;
	for(int i=0;i<9;i++)	remplir(tab[i],1);
	for(int i=0;i<8;i++)
	{
		resultr+=tab[i].r;
		resultg+=tab[i].g;
		resultb+=tab[i].b;
	}
	resultr=9*tab[8].r-resultr;
	resultg=9*tab[8].g-resultg;
	resultb=9*tab[8].b-resultb;
	return QColor(valid(resultr),valid(resultg),valid(resultb)).rgb();	
}

void RImage::passehaut(QImage *image)
{
	pixeltype *passehaut=new pixeltype[image->width()*image->height()];
	int f=0;
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			pixeltype tab[9];
			tab[0].color=image->pixel(w-1,h);
			tab[1].color=image->pixel(w+1,h);
			tab[8].color=image->pixel(w  ,h);
			tab[3].color=image->pixel(w  ,h-1);
			tab[4].color=image->pixel(w  ,h+1);
			tab[5].color=image->pixel(w-1,h+1);
			tab[6].color=image->pixel(w+1,h+1);
			tab[7].color=image->pixel(w+1,h-1);
			tab[2].color=image->pixel(w-1,h-1);	
			passehaut[f].color=getpassehaut(tab);
			passehaut[f].x=w;
			passehaut[f].y=h;
			f++;
		}
	for(int i=0;i<f;i++)
		image->setPixel(passehaut[i].x,passehaut[i].y,passehaut[i].color);
}
				//*************les gardients*/

/////////////////////soble
QRgb getsoble(pixeltype *tab)
{
	
	int resultxr=0,resultxg=0,resultxb=0,resultyr=0,resultyg=0,resultyb=0;
	int resultr,resultg,resultb;
	
	resultxr=tab[8].r+2*tab[0].r+tab[5].r-tab[7].r-2*tab[1].r-tab[6].r;
	resultyr=tab[8].r+2*tab[3].r+tab[7].r-tab[5].r-2*tab[4].r-tab[6].r;
	resultxg=tab[8].g+2*tab[0].g+tab[5].g-tab[7].g-2*tab[1].g-tab[6].g;
	resultyg=tab[8].g+2*tab[3].g+tab[7].g-tab[5].g-2*tab[4].g-tab[6].g;
	resultxb=tab[8].b+2*tab[0].b+tab[5].b-tab[7].b-2*tab[1].b-tab[6].b;
	resultyb=tab[8].b+2*tab[3].b+tab[7].b-tab[5].b-2*tab[4].b-tab[6].b;
	resultr=(int)sqrt((float)(resultxr*resultxr+resultyr+resultyr));
	resultg=(int)sqrt((float)(resultxg*resultxg+resultyg+resultyg));
	resultb=(int)sqrt((float)(resultxb*resultxb+resultyb+resultyb));
		
	return QColor(valid(resultr),valid(resultg),valid(resultb)).rgb();
}
pixeltype RImage::soblepixel(QImage *image,int x,int y)
{
	pixeltype soble;
	pixeltype tab[9];
	tab[0].color=image->pixel(x-1,y);
	tab[1].color=image->pixel(x+1,y);
	tab[2].color=image->pixel(x  ,y);
	tab[3].color=image->pixel(x  ,y-1);
	tab[4].color=image->pixel(x  ,y+1);
	tab[5].color=image->pixel(x-1,y+1);
	tab[6].color=image->pixel(x+1,y+1);
	tab[7].color=image->pixel(x+1,y-1);
	tab[8].color=image->pixel(x-1,y-1);	
	for(int i=0;i<9;i++)	remplir(tab[i],1);
	soble.x=x;
	soble.y=y;
	soble.color=getsoble(tab);
	return soble;
}

void RImage::soble(QImage *image)
{
	pixeltype *soble=new pixeltype[image->width()*image->height()];
	int f=0;
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			soble[f]=soblepixel(image,w,h);
			f++;
		}
	for(int i=0;i<f;i++)
		image->setPixel(soble[i].x,soble[i].y,soble[i].color);
}

/////////////////simple gradien
QRgb getgradien(pixeltype *tab)
{
	
	int resultxr=0,resultxg=0,resultxb=0,resultyr=0,resultyg=0,resultyb=0;
	int resultr,resultg,resultb;
	
	resultxr=tab[2].r-tab[3].r;
	resultyr=tab[0].r+tab[1].r;
	resultxg=tab[2].g-tab[3].g;
	resultyg=tab[0].g+tab[1].g;
	resultxb=tab[2].b-tab[3].b;
	resultyb=tab[0].b+tab[1].b;
	
	resultr=(int)sqrt((float)(resultxr*resultxr+resultyr+resultyr));
	resultg=(int)sqrt((float)(resultxg*resultxg+resultyg+resultyg));
	resultb=(int)sqrt((float)(resultxb*resultxb+resultyb+resultyb));
		
	return QColor(valid(resultr),valid(resultg),valid(resultb)).rgb();
}
pixeltype RImage::gradienpixel(QImage *image,int x,int y)
{
	pixeltype gradien;
	pixeltype tab[4];
	tab[0].color=image->pixel(x-1,y);
	tab[1].color=image->pixel(x+1,y);
	tab[2].color=image->pixel(x  ,y-1);
	tab[3].color=image->pixel(x  ,y+1);
	for(int i=0;i<4;i++)	remplir(tab[i],1);
	gradien.x=x;
	gradien.y=y;
	gradien.color=getgradien(tab);
	return gradien;
}

void RImage::gradien(QImage *image)
{
	pixeltype *gradien=new pixeltype[image->width()*image->height()];
	int f=0;
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			gradien[f]=gradienpixel(image,w,h);
			f++;
		}
	for(int i=0;i<f;i++)
		image->setPixel(gradien[i].x,gradien[i].y,gradien[i].color);
	
}
////////////////gradient améliorer
floatpixel RImage::NormeGrad(QImage *image,int x,int y)
{
	int r1,r,r_1,g1,g,g_1,b1,b,b_1;
	floatpixel	 gradx	= {0.0f,0.0f,0.0f};
	floatpixel	 grady	= {0.0f,0.0f,0.0f};
	int		nbcompx = 0;
	int		nbcompy = 0;
	QColor(image->pixel(x,y)).getRgb(&r,&g,&b);
	if (x != 0)
	{
		QColor(image->pixel(x-1,y)).getRgb(&r_1,&g_1,&b_1);
		gradx.r += (float) (int)r-(int)r_1;
		gradx.g += (float) (int)g-(int)g_1;
		gradx.b += (float) (int)b-(int)b_1;
		nbcompx++;
	}
	if (x != image->width())
	{
		QColor(image->pixel(x+1,y)).getRgb(&r1,&g1,&b1);
		gradx.r += (float) (int)r1-(int)r;
		gradx.g += (float) (int)g1-(int)g;
		gradx.b += (float) (int)b1-(int)b;
		nbcompx++;
	}
	gradx.r=gradx.r/(float)nbcompx;		//on fait la moyenne
	gradx.g=gradx.g/(float)nbcompx;
	gradx.b=gradx.b/(float)nbcompx;

	if (y != 0)
	{
		QColor(image->pixel(x,y-1)).getRgb(&r_1,&g_1,&b_1);
		grady.r += (float) (int)r-(int)r_1;
		grady.g += (float) (int)g-(int)g_1;
		grady.b += (float) (int)b-(int)b_1;
		nbcompy++;
	}
	if (y != image->height())
	{
		QColor(image->pixel(x,y+1)).getRgb(&r1,&g1,&b1);
		grady.r += (float) (int)r1-(int)r;
		grady.g += (float) (int)g1-(int)g;
		grady.b += (float) (int)b1-(int)b;
		nbcompy++;
	}
	grady.r=grady.r/(float)nbcompy;
	grady.g=grady.g/(float)nbcompy;
	grady.b=grady.b/(float)nbcompy;
	
	floatpixel result = {(float)sqrt(gradx.r*gradx.r+grady.r*grady.r),
			          (float)sqrt(gradx.g*gradx.g+grady.g*grady.g),
			          (float)sqrt(gradx.b*gradx.b+grady.b*grady.b)};
	return result;
}
float max(float x,float y)
{
	if(x>y) return x;
	else return y;
}
float min(float x,float y)
{
	if(x>y) return y;
	else return x;
}
void RImage::gradienami(QImage *image,float durete)
{
	int i,j;
	floatpixel	* grad =new floatpixel[image->width()*image->height()];
	pixeltype	* data =new pixeltype[image->height()*image->width()];

	for(j=0;j<image->height();j++)
	{
		for(i=0;i<image->width();i++)
		{
			grad[i+j*image->width()] = RImage::NormeGrad(image,i,j);
			data[i+j*image->width()].x=i;
			data[i+j*image->width()].y=j;
		}
	}

	//On cherche le max et on normalise entre 0 et 255 et on genere l'image
	floatpixel maxi={0.0f,0.0f,0.0f};
	for (i=0;i<image->height()*image->width();i++)
	{
		if (grad[i].r>maxi.r)
			maxi.r=grad[i].r;
		if (grad[i].g>maxi.g)
			maxi.g=grad[i].g;
		if (grad[i].b>maxi.b)
			maxi.b=grad[i].b;
	}
	//On genere ensuite l'image en normalisant
	
	durete = max(durete,1);
	for (i=0;i<image->height()*image->width();i++)
	{
		data[i].r = (int) min(grad[i].r/maxi.r*255.0f*durete,255.0f);
		data[i].g = (int) min(grad[i].g/maxi.g*255.0f*durete,255.0f);
		data[i].b = (int) min(grad[i].b/maxi.b*255.0f*durete,255.0f);
	}
	for (i=0;i<image->height()*image->width();i++)
	{
		remplir(data[i],2);
		image->setPixel(data[i].x,data[i].y,data[i].color);
	}
	
}			

//////////////////////////////////////perwit
QRgb getperwit(pixeltype *tab)
{
	
	int resultxr=0,resultxg=0,resultxb=0,resultyr=0,resultyg=0,resultyb=0;
	int resultr,resultg,resultb;
	
	resultxr=tab[8].r+tab[0].r+tab[5].r-tab[7].r-tab[1].r-tab[6].r;
	resultyr=tab[8].r+tab[3].r+tab[7].r-tab[5].r-tab[4].r-tab[6].r;
	resultxg=tab[8].g+tab[0].g+tab[5].g-tab[7].g-tab[1].g-tab[6].g;
	resultyg=tab[8].g+tab[3].g+tab[7].g-tab[5].g-tab[4].g-tab[6].g;
	resultxb=tab[8].b+tab[0].b+tab[5].b-tab[7].b-tab[1].b-tab[6].b;
	resultyb=tab[8].b+tab[3].b+tab[7].b-tab[5].b-tab[4].b-tab[6].b;
	resultr=(int)sqrt((float)(resultxr*resultxr+resultyr+resultyr));
	resultg=(int)sqrt((float)(resultxg*resultxg+resultyg+resultyg));
	resultb=(int)sqrt((float)(resultxb*resultxb+resultyb+resultyb));
		
	return QColor(valid(resultr),valid(resultg),valid(resultb)).rgb();
}
pixeltype RImage::perwitpixel(QImage *image,int x,int y)
{
	pixeltype perwit;
	pixeltype tab[9];
	tab[0].color=image->pixel(x-1,y);
	tab[1].color=image->pixel(x+1,y);
	tab[2].color=image->pixel(x  ,y);
	tab[3].color=image->pixel(x  ,y-1);
	tab[4].color=image->pixel(x  ,y+1);
	tab[5].color=image->pixel(x-1,y+1);
	tab[6].color=image->pixel(x+1,y+1);
	tab[7].color=image->pixel(x+1,y-1);
	tab[8].color=image->pixel(x-1,y-1);	
	for(int i=0;i<9;i++)	remplir(tab[i],1);
	perwit.x=x;
	perwit.y=y;
	perwit.color=getperwit(tab);
	return perwit;
}

void RImage::perwit(QImage *image)
{
	pixeltype *perwit=new pixeltype[image->width()*image->height()];
	int f=0;
	for(int h=1;h<image->height()-1;h++)
		for(int w=1;w<image->width()-1;w++)
		{
			perwit[f]=perwitpixel(image,w,h);
			f++;
		}
	for(int i=0;i<f;i++)
		image->setPixel(perwit[i].x,perwit[i].y,perwit[i].color);
}



/////////////////////////////////////
void gris(QRgb &pixe)
{
	int r,g,b,result;
	QColor color(pixe);
	color.getRgb(&r,&g,&b);
	result=(r*11+g*16+b*5)/32;
	color.setRgb(result,result,result);
	pixe=color.rgb();
}
void RImage::Niveau_Gris(QImage *image)
{
	for(int h=0;h<image->height();h++)
		for(int w=0;w<image->width();w++)
		{
			QRgb inter=image->pixel(w,h);
			gris(inter);
			image->setPixel(w,h,inter);
		}
}
void RImage::aide_detecte_objet(QImage *image,int i,int y,QRgb src)
{
	for(int x=i;x < image->width();x++)
		if(image->pixel(x,y) == qRgb(255,0,0))
		{
			RImage::detecte_objet(image,x,y,src);
			x=image->width();
		}
}
void RImage::detecte_objet(QImage *image,int x,int y,QRgb src)
{
	image->setPixel(x,y,qRgb(0,0,255));
	if(src==image->pixel(x+1,y-1))	detecte_objet(image,x+1,y-1,src);
	if(src==image->pixel(x+1,y))	detecte_objet(image,x+1,y,src);
	if(src==image->pixel(x+1,y+1))	detecte_objet(image,x+1,y+1,src);
	if(src==image->pixel(x,y-1))	detecte_objet(image,x,y-1,src);
	if(src==image->pixel(x,y+1))	detecte_objet(image,x,y+1,src);
	if(src==image->pixel(x-1,y))	detecte_objet(image,x-1,y,src);
	if(src==image->pixel(x-1,y+1))	detecte_objet(image,x-1,y+1,src);
	if(src==image->pixel(x-1,y-1))	detecte_objet(image,x-1,y-1,src);
}


double RImage::distance(QImage *image,int x1,int y1,int x2,int y2)
{
	double Dx=image->width()/image->widthMM();
	double Dy=image->height()/image->heightMM();
	int x=abs(x1-x2);
	int y=abs(y1-y2);
	double xx=(double)x/Dx;
	double yy=(double)y/Dy;
	double inter=xx*xx+yy*yy;
	if(Dx==3)	return sqrt(inter)*0.8;
	else		return sqrt(inter)*0.6;
}
void RImage::dupliquer(QImage *image,QRgb src)
{
	for(int h=0;h<image->height();h++)
		for(int w=0;w<image->width();w++)
			if(image->pixel(w,h)==src)
			{
				image->setPixel(w-1,h,src);
				image->setPixel(w,h-1,src);
			}
}

void RImage::surface(QImage *image,int x,int y,QRgb src)
{
	image->setPixel(x,y,src);
	if(src!=image->pixel(x+1,y-1))	surface(image,x+1,y-1,src);
	if(src!=image->pixel(x+1,y))	surface(image,x+1,y,src);
	if(src!=image->pixel(x+1,y+1))	surface(image,x+1,y+1,src);
	if(src!=image->pixel(x,y-1))	surface(image,x,y-1,src);
	if(src!=image->pixel(x,y+1))	surface(image,x,y+1,src);
	if(src!=image->pixel(x-1,y))	surface(image,x-1,y,src);
	if(src!=image->pixel(x-1,y+1))	surface(image,x-1,y+1,src);
	if(src!=image->pixel(x-1,y-1))	surface(image,x-1,y-1,src);
}
/*void RImage::recupercourbe(QImage *image,Qrgb src,pixeltype *pix,int &nbr)
{
	nbr=0;
	for(int w=0;w<image->width();w++)
		for(int h=0;h<image->height();h++)		
			if(image->pixel(w,h)==src)
			{
				pix[nbr].x=w;
				pix[nbr].y=h;
				nbr++;
			}
}
double RImage::distanceHoos(QImage *image,pixeltype *pixref,int index1,pixeltype *pixf,int index2)
{
	double dist=0;
	for(int i=0;i<index1;i++)
		for(int j=0;j<index2;j++)
		{
			double valeur=distance(image,pixref[i].x,pixref[i].y,pixf[j].x,pixf[j].y;
			if(valeur>dist)	dist=valeur;
		}
}*/
double RImage::surfaceMM(QImage *image,QRgb src)
{
	int nbr=0;
	for(int w=0;w<image->width();w++)
		for(int h=0;h<image->height();h++)		
			if(image->pixel(w,h)==src)
			nbr++;
	
	double Dx=image->width()/image->widthMM();
	double Dy=image->height()/image->heightMM();
	double Dxy=Dx*Dy;
	double inter=double(nbr)/Dxy;
	if(Dx==3)	return inter*2.25;
	else		return inter;
}