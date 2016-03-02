#include "paint.h"
#include "levelset.h"
#include "image.h"


paint::paint():QWidget()
{
	
	spin=new QDoubleSpinBox();


	scar=false;
	detecte=false;
	distance=false;

	couleur_dis=qRgb(0,0,255);
	couleur_obj=qRgb(255,0,0);
	image =new QImage();

	landa1=1;
	landa2=1;
	mu=0.5*255*255;
	nu=0;
	dt=0.01;
	nbr=1;
	iter=10;
	ray=10;

	setCursor(QCursor(QPixmap("images/pen2.png"),0,23));
}
void paint::paintEvent(QPaintEvent * )
{
	QPainter painter(this);
	painter.drawImage(0, 0, *image);
}

void paint::ouvrirr(QImage imag)
{
	double Dx=imag.width()/imag.widthMM();
	//double Dy=imag.height()/imag.heightMM();

	spin->setValue(Dx);
	
	image_initiale=new QImage(*image);
	*image=imag;
	QPainter paintt(image);
	paintt.drawImage(0, 0, *image);
	update(image->rect());
}
void paint::mod(QImage imag)
{
	*image=imag;
	QPainter paintt(image);
	paintt.drawImage(0, 0, *image);
	update(image->rect());
}
void paint::mousePressEvent(QMouseEvent *event)
{
	emit moveMouse(event->x(),event->y());
	if(event->button()==Qt::LeftButton)
		if(detecte)
		{
			QImage *inter=new QImage(*image);
			levelset::essai(inter,event->x(),event->y(),ray,iter,nbr,landa1,landa2,mu,nu,dt);
			
			RImage::aide_detecte_objet(inter,event->x(),event->y(),qRgb(255,0,0));

			for(int w=0;w<image->width();w++)
				for(int h=0;h<image->height();h++)
					if(inter->pixel(w,h)==qRgb(0,0,255))	image->setPixel(w,h,couleur_obj.rgb());
			
			RImage::dupliquer(inter,qRgb(0,0,255));
			RImage::surface(inter,event->x(),event->y(),qRgb(0,0,255));
			
			emit surfaceSGN(RImage::surfaceMM(inter,qRgb(0,0,255)));
			mod(*image);	
		}
		else if(distance)
		{
			mod(*image_distance);
			QPainter painter(image);
			mouse.setX(event->x());
			mouse.setY(event->y());
			
			QPen *pen=new QPen(couleur_dis);
			pen->setWidth(5);
			painter.setPen(*pen);			

			painter.drawPoint(mouse);
			update(image->rect());
			scar=true;
		}
		else 
		{
			QPainter painter(image);

			painter.setPen(couleur_dis);
			dernier_point=QPoint(event->x(),event->y());

			painter.drawPoint(event->x(),event->y());
			update(image->rect());

			scar=true;
		}
}
void paint::mouseMoveEvent(QMouseEvent *event)
{
	emit moveMouse(event->x(),event->y());
	if ((event->buttons() & Qt::LeftButton) && scar && distance)
	{
		mod(*image_distance);
		QPainter painter(image);
		QPen *pen=new QPen(couleur_dis);
		pen->setWidth(5);
		painter.setPen(*pen);		
		painter.drawPoint(mouse);
		pen->setWidth(1);
		painter.setPen(*pen);		
		QPoint inter(event->x(),event->y());
		painter.drawLine(mouse,inter);

		emit signaldistance(RImage::distance(image,mouse.x(),mouse.y(),event->x(),event->y()));
		
		update(image->rect());
	}
    else if ((event->buttons() & Qt::LeftButton) && scar)
	{
		QPainter painter(image);
		painter.setPen(couleur_dis);
		painter.drawLine(dernier_point,QPoint(event->x(),event->y()));
		dernier_point=QPoint(event->x(),event->y());
		update(image->rect());
	}
	
}

void paint::mouseReleaseEvent(QMouseEvent *event)
{
	emit moveMouse(event->x(),event->y());
	if (event->button() == Qt::LeftButton && scar && distance) 
	 {
     	QPainter painter(image);
		
		QPen *pen=new QPen(couleur_dis);
		pen->setWidth(5);
		painter.setPen(*pen);		
		
		painter.drawPoint(event->x(),event->y());
		emit signaldistance(RImage::distance(image,mouse.x(),mouse.y(),event->x(),event->y()));

		update(image->rect());
		scar=false;
	 }
     else if (event->button() == Qt::LeftButton && scar) 
	 {
     	QPainter painter(image);
		painter.drawPoint(event->x(),event->y());
		update(image->rect());
		scar=false;
	 }    
}
void paint::setdistance(bool test)
{
	if(test==false)	*image=*image_distance;
	distance=test;
	image_distance =new QImage(*image);
}
void paint::setdetectation(bool test)
{
	detecte=test;
}
void paint::Dt(double ddt)
{
	dt=ddt;
}
void paint::Mu(double mmu)
{
	mu=mmu;
}
void paint::Nu(double nnu)
{
	nu=nnu;
}
void paint::Landa1(double landda1)
{
	landa1=landda1;
}
void paint::Landa2(double landda2)
{
	landa2=landda2;
}
void paint::Nbr(double nnbr)
{
	nbr=nnbr;
}
void paint::Iter(int iiter)
{
	iter=iiter;
}
void paint::Ray(int rray)
{
	ray=rray;
}
void paint::Colorobj(QColor color)
{
	couleur_obj=color;
}
void paint::Colorpen(QColor color)
{
	couleur_dis=color;
}