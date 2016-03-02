#ifndef paint_head
#define paint_head

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDoubleSpinBox>
#include <QPoint>
#include <QPen>
#include <QColor>



class paint : public QWidget
{
	Q_OBJECT
public:
	paint();
	void paintEvent(QPaintEvent *);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void reste();
	void setdetectation(bool test);
	void setdistance(bool test);
	void ouvrirr(QImage imag);

	public slots:
		void mod(QImage imag);
		void Dt(double );
		void Mu(double );
		void Nu(double );
		void Landa1(double );
		void Landa2(double );
		void Nbr(double );
		void Iter(int );
		void Ray(int);
		void Colorpen(QColor color);
		void Colorobj(QColor color);
	
	signals:
		void signaldistance(double);
		void moveMouse(int,int);
		void surfaceSGN(double);
		
	

private:
	QPoint mouse,dernier_point;
	QColor couleur_dis,couleur_obj;
	bool scar;
	bool detecte;
	bool distance;
	QImage *image;
	QImage *image_initiale;
	QImage *image_distance;
	double dt,mu,nu,landa1,landa2;
	int ray,nbr,iter;
	
	QDoubleSpinBox *spin;
	
};
#endif
