#ifndef Head_fene
#define Head_fene

#include <QApplication>
#include <QWidget>
#include <QTabWidget>
#include <QMainWindow>
#include <QScrollArea>
#include <QImage>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QWidgetAction>
#include <QToolBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QFormLayout>
#include <QMdiArea>
#include <QStatusBar>
#include <QSlider>
#include <QMdiSubWindow>
#include <QPixmap>
#include <QDockWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QCommandLinkButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QStack>
#include <QEvent>
#include <QMouseEvent>
#include <QImage>
#include <QColor>
#include <QScrollBar>
#include <QPainter>
#include <QTabBar>
#include <QMovie>
#include <QThread>


#include "paint.h"
#include "thread.h"


class fene : public QMainWindow
{
	Q_OBJECT
public :
	fene(QString fichier=0,bool test=false);
	void manubarcreate();
	void toolbarcreate();
	void dockcreate();
	void charger(QImage *image,int tab);
	void supprime(int val);
	void scaleImage(double factor);
	void Ouuvrir();
	void histogarmme(double *tab,double max,QRgb desine,int y,QString str);
	
	public slots:
		void ouvrir_une_image();
		void ouvrir_des_images();
		void enregistrer();
		void enregistrer_sous();
		void annuler();
		void retablir();
		void zoomin();
		void zoomout();
		void retabliir();
		void analyse();
		void analyses();
		void median();
		void gaussien();
		void passehaut();
		void repoussage();
		void soble();
		void perwit();
		void gradien();
		void gradien1();
		void detecterAC();
		void distanceAC();
		void active();
		void mouseAC(int,int);
		void Colorpen();
		void Colorobj();
		void changerimage(int);
		void Pllay();
		void Sttop();
		void afficheHist();
		void clearHist();
		void paraHist();
		void colorHist();
		
		
private :
	////
	QRadioButton *ls1,*ls2;
	QCommandLinkButton *Analyser;

	////
	QDockWidget *dockmanipulation,*dockdb,*levelset;
	QDialog *histo;
	
	/////
	QImage *image;
	Thread *simul;
	

	////
	QStack<QImage> *tab_pixmap_gauche;
	QStack<QImage> *tab_pixmap_droite;

	////
	QAction *Retablir,*Annuler,*Zoomout,*Zoomin,*Retabliir,*AAnalyser,*GGaussien,*MMedian,*PPasseHaut,*GGradien,*SSoble,*PPerwit,*RRepoussage,*GGradien1;
	QAction *Ouvrir,*Sauvgarder,*Sauvgarder_sous,*quitter,*Home,*Ouvrir_plus;
	QAction *Distance,*Detecte,*Pen,*colorpen,*colorobj;
	QAction *afficheHistogramme,*clearHistogramme,*paraHistogramme;
	QPushButton *simuler;

	///////
	QDoubleSpinBox *spinbox,*spindistance,*spinsurface,*spinechelle;
	QDoubleSpinBox *spinbox_landa1,*spinbox_landa2,*spinbox_mu,*spinbox_nu,*spinbox_dt;
	QSpinBox *spinbox_ray,*spinbox_iter,*spinbox_X,*spinbox_Y,*spinbox_mouse_X,*spinbox_mouse_Y;

	QSpinBox *histoY;
	QLineEdit *strg;
	QColor couleurHist;
	////
	QWidget *widget,*startpage;
	QGroupBox *simulation;
	QScrollArea *principale;
	QTabBar *tabbar;

	/////////
	QString ouvrir;
	double scaleFactor;
	bool test;
	bool simule;

	////
	QStringList ouvrirs;
	QVector<QImage> images;

	//////////
	paint *pain;
	
	fene *fene2;
}; 
#endif