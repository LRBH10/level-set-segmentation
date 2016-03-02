#include "fene1.h"
#include "manuvsdock.h"
#include "filtregradient.h"
#include "levelset.h"


//////////CONSTRUCTOR
fene::fene(QString fichier,bool testa) : QMainWindow()
{
	pain=new paint();
	pain->setStatusTip(tr("Zone de dessin"));
	manubarcreate();
	toolbarcreate();
	dockcreate();

	couleurHist=qRgb(255,0,0);
	
	strg=new QLineEdit;
	strg->setText("Pas de Titre");

	histoY=new QSpinBox();
	histoY->setValue(30);


	

					//alloctation
	principale=new QScrollArea(this);
	connect(pain,SIGNAL(moveMouse(int,int)),this,SLOT(mouseAC(int,int)));
		
	scaleFactor=1;
	image = new QImage();
	tabbar=new QTabBar();
	connect(tabbar,SIGNAL(currentChanged(int)),this,SLOT(changerimage(int)));

	statusBar()->showMessage(tr("Prêt"));

	QVBoxLayout *layout_mdi=new QVBoxLayout();
	layout_mdi->addWidget(tabbar);
	layout_mdi->addWidget(pain);
	
	simul=new Thread();
	connect(simul,SIGNAL(signalIMG(QImage)),pain,SLOT(mod(QImage)));

		
	test=testa;
	if(!fichier.isEmpty()&&test) 
	{
		ouvrir=fichier;
		Ouuvrir();
		showMaximized();
	}
	
	principale->setLayout(layout_mdi);
	addDockWidget(Qt::RightDockWidgetArea,dockmanipulation);
	addDockWidget(Qt::LeftDockWidgetArea,levelset);
	setCentralWidget(principale);
}
void fene::Ouuvrir()
{
	setWindowIcon(QIcon(ouvrir));
	setWindowTitle(nomFichier(ouvrir));

	image= new QImage(ouvrir);

	pain->ouvrirr(*image);
	
	tab_pixmap_gauche=new QStack<QImage>;
	tab_pixmap_droite=new QStack<QImage>;
		
	dockmanipulation->setVisible(true);
	levelset->setVisible(true);	
}

///////////ANALYSE
void fene::analyse()
{
	spinsurface->setValue(RImage::surfaceMM(image,qRgb(0,0,255)));
	charger(image);
	if(ls1->isChecked())		levelset::essai(image,spinbox_X->value(),spinbox_Y->value(),spinbox_ray->value(),spinbox_iter->value(),1,spinbox_landa1->value(),spinbox_landa2->value(),spinbox_mu->value(),spinbox_nu->value(),spinbox_dt->value());
	else						levelset::essai(image,spinbox_X->value(),spinbox_Y->value(),spinbox_ray->value(),spinbox_iter->value(),2,spinbox_landa1->value(),spinbox_landa2->value(),spinbox_mu->value(),spinbox_nu->value(),spinbox_dt->value());
	pain->mod(*image);
}
void fene::analyses()
{
	double tab[11];
	if(ls1->isChecked())
		for(int i=0;i<images.size();i++)
		{
			QImage *inter=new QImage(images.at(i));
			QImage *inter2=new QImage(images.at(i));

			levelset::essai(inter,spinbox_X->value(),spinbox_Y->value(),spinbox_ray->value(),spinbox_iter->value(),1,spinbox_landa1->value(),spinbox_landa2->value(),spinbox_mu->value(),spinbox_nu->value(),spinbox_dt->value());
			
			RImage::aide_detecte_objet(inter,spinbox_X->value(),spinbox_Y->value(),qRgb(255,0,0));
			
			for(int w=0;w<image->width();w++)
				for(int h=0;h<image->height();h++)
					if(inter->pixel(w,h)==qRgb(0,0,255))	inter2->setPixel(w,h,qRgb(255,255,0));

			RImage::dupliquer(inter,qRgb(0,0,255));
			RImage::surface(inter,spinbox_X->value(),spinbox_Y->value(),qRgb(0,0,255));

			tab[i]=RImage::surfaceMM(inter,qRgb(0,0,255));

			images.replace(i,*inter2);
		}

	else
		for(int i=0;i<images.size();i++)
		{
			QImage *inter=new QImage(images.at(i));
			QImage *inter2=new QImage(images.at(i));

			levelset::essai(inter,spinbox_X->value(),spinbox_Y->value(),spinbox_ray->value(),spinbox_iter->value(),2,spinbox_landa1->value(),spinbox_landa2->value(),spinbox_mu->value(),spinbox_nu->value(),spinbox_dt->value());
			
			RImage::aide_detecte_objet(inter,spinbox_X->value(),spinbox_Y->value(),qRgb(255,0,0));
			
			for(int w=0;w<image->width();w++)
				for(int h=0;h<image->height();h++)
					if(inter->pixel(w,h)==qRgb(0,0,255))	inter2->setPixel(w,h,qRgb(255,255,0));

			RImage::dupliquer(inter,qRgb(0,0,255));
			RImage::surface(inter,spinbox_X->value(),spinbox_Y->value(),qRgb(0,0,255));

			tab[i]=RImage::surfaceMM(inter,qRgb(0,0,255));

			images.replace(i,*inter2);
		}
	*image=images.at(tabbar->currentIndex());
	pain->mod(*image);
	
	double max=354.75;
	
	fene::histogarmme(tab,max,couleurHist.rgb(),histoY->value(),strg->text());
}

void fene::histogarmme(double *tab,double max,QRgb desine,int y,QString str)
{
	QImage inter("inter.bmp");
	QPainter painter(&inter);
	QPoint p;

	painter.drawLine(40,20,40,300);
	painter.drawLine(0,265,280,265);
	
	painter.drawText(45,35,"surface(mm²)");
	painter.drawText(260,255,"images");

	painter.setPen(desine);
	painter.drawLine(150,y,160,y);
	painter.drawText(170,y,str);

	int imin=0;
	int imax=0;
	for(int i=1;i<images.size();i++)
		if(tab[imin]>tab[i])		imin=i;
		else if(tab[imax]<tab[i])	imax=i;

	for(int i=0;i<images.size();i++)
	{
		int y=280-(tab[i]*200)/max;
		int x=60+i*20;

		/////
		painter.setPen(qRgb(0,0,0));
		char s[5];
		sprintf(s,"%d",i);
		painter.drawText(x,290,QString(s));
		painter.drawLine(x,264,x,266);

		/////
		if(i==imin || i==imax)
		{
			painter.setPen(qRgb(0,0,0));
			char s1[5];
			sprintf(s1,"%.2lf",tab[i]);
			painter.drawText(2,y,QString(s1));
			painter.drawLine(39,y,41,y);
		}
		
		////
		painter.setPen(desine);
		if(i!=0)
		{
			painter.drawLine(x,y,p.x(),p.y());
			painter.drawRect(x-1,y-1,2,2);
		}
		else
		{
			painter.drawPoint(x,y);
			painter.drawRect(x-1,y-1,2,2);
		}
		p.setX(x);
		p.setY(y);
	}
	inter.save("inter.bmp");
}
void fene::detecterAC()
{
	if(Detecte->isChecked())	pain->setdetectation(true);
	else						pain->setdetectation(false);
}

void fene::distanceAC()
{
	if(Distance->isChecked())	pain->setdistance(true);
	else						pain->setdistance(false);
}

void fene::mouseAC(int x,int y)
{
	spinbox_mouse_X->setValue(x);
	spinbox_mouse_Y->setValue(y);
}

void fene::active()
{
}

void fene::Colorobj()
{
	pain->Colorobj(QColorDialog::getColor(Qt::red,this));
}

void fene::Colorpen()
{
	pain->Colorpen(QColorDialog::getColor(Qt::blue,this));
}

void fene::changerimage(int id)
{
	*image=images.at(id);
	pain->mod(*image);
}

void fene::Pllay()
{
	simul->setTest(true);
	simul->setparame(*image,spinbox_X->value(),spinbox_Y->value(),spinbox_ray->value(),1,spinbox_landa1->value(),spinbox_landa2->value(),spinbox_mu->value(),spinbox_nu->value(),spinbox_dt->value());
	simul->start();
}

void fene::Sttop()
{
	simul->setTest(false);	
}

void fene::afficheHist()
{
	QDialog histoo(this);
	histoo.setFixedSize(300,300);
	histoo.setWindowTitle("l'histogramme des surface");
	QLabel *label=new QLabel(&histoo);
	QPixmap *pixmap=new QPixmap("inter.bmp");
	label->setPixmap(*pixmap);
	histoo.exec();
}
void fene::clearHist()
{
	QImage inter("inter.bmp");
	inter.fill(qRgb(255,255,255));
	inter.save("inter.bmp");
}
void fene::paraHist()
{
	histo=new QDialog(this);

	QPushButton *color=new QPushButton("Changer la Couleur");
	connect(color,SIGNAL(clicked()),this,SLOT(colorHist()));

	QPushButton *OK=new QPushButton("OK");
	connect(color,SIGNAL(clicked()),this,SLOT(OK()));

	QFormLayout *layout=new QFormLayout;
	layout->addRow("Le Titre de Ligne",strg);
	layout->addRow("La Postion Verticale",histoY);
	layout->addRow(color);

	histo->setLayout(layout);
	
	histo->show();
}
void fene::colorHist()
{
	couleurHist=QColorDialog::getColor(Qt::red,this);
}