#include "fene1.h"
#include "image.h"

////////////////////////pour recuperer le nom de fichier

QString nomFichier(QString string)
{
	QString result,alpha;
	for(int i=string.size()-1;i>=0;i--)
		if(string.at(i)!='/')	alpha+=string.at(i);
		else i=-1;
	for(int i=alpha.size()-1;i>=0;i--)	result+=alpha.at(i);
	return result;
}


/////////////ouvrir une image
void fene::ouvrir_une_image()
{
	ouvrir=QFileDialog::getOpenFileName(this,tr("&ouvrir une image "),QString(),tr("Images(*.*)"));
	if(!ouvrir.isEmpty()&&!test)	
	{		
		test=true;
		image->load(ouvrir);	
		
		pain->ouvrirr(*image);

		tab_pixmap_gauche=new QStack<QImage>;
		tab_pixmap_droite=new QStack<QImage>;
		
	}
	else if(ouvrir.isEmpty())	QMessageBox::warning(this,tr("avertissement"),tr("vous n'avez pas choisi une image"));
	else fene2=new fene(ouvrir,true);
	
}
void fene::ouvrir_des_images()
{
	ouvrirs=QFileDialog::getOpenFileNames(this,tr("ouvrir plusieurs image"),QString(),tr("Images(*.*)"));
	if(!ouvrirs.isEmpty() && !test)
	{
		test=true;
		for(int i=0;i<ouvrirs.size();i++)
		{
			QString inter=ouvrirs.at(i);
			images.insert(i,QImage(inter));
			tabbar->addTab(QIcon(inter),nomFichier(inter));
		}
		*image=images.at(0);

		simuler->setVisible(true);
		
		pain->ouvrirr(*image);

		tab_pixmap_gauche=new QStack<QImage>;
		tab_pixmap_droite=new QStack<QImage>;
	}
	else if(ouvrir.isEmpty())	QMessageBox::warning(this,tr("avertissement"),tr("vous n'avez pas choisi des images"));
	
}


//////////////////////enregestrer
void fene::enregistrer()
{
	if(!image->isNull())	image->save(ouvrir);
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a aucune image à enregistrer"));
}
void fene::enregistrer_sous()
{
	if(!image->isNull())	image->save(QFileDialog::getSaveFileName(this,tr("&enregistrer une image "),tr("sans titre"),tr("Images(*.png)")));
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a aucune image à enregistrer"));
}



///////////////charger dans la piles
void fene::charger(QImage *image,int tab=1)
{
	if(tab==1)
	{
		tab_pixmap_gauche->push(*image);
		Annuler->setEnabled(true);
	}
	else
	{
		tab_pixmap_droite->push(*image);
		Retablir->setEnabled(true);
	}
}

/////////////////////annuler et retablie
void fene::annuler()
{
	charger(image,0);
	*image=tab_pixmap_gauche->pop();
	if(tab_pixmap_gauche->isEmpty()) Annuler->setDisabled(true);
	pain->mod(*image);
}
void fene::retablir()
{
	charger(image);
	*image=tab_pixmap_droite->pop();
	if(tab_pixmap_droite->isEmpty()) Retablir->setDisabled(true);
	pain->mod(*image);
}



//////////////zoom avant et arriere
void fene::zoomin()
{
	scaleImage(0.8);
}
void fene::zoomout()
{
	scaleImage(1.2);
}
void fene::retabliir()
{
	scaleFactor=1;
	scaleImage(1);
}
void fene::scaleImage(double factor)
{
    scaleFactor *= factor;
	QImage *inter=new QImage(*image);
	inter->scaled(scaleFactor*inter->size());
	pain->mod(*inter);
}



///////////////////////////////////////////////////////////////////////////////////////

/////////////////////MANU BAR
void fene::manubarcreate()
{
		////declaration
	QMenu *fichier,*edition,*generer,*aide;
						/*	La bar de Menu	*/
	menuBar()->setStatusTip(tr("Prêt"));
				//fichier
	Ouvrir=new QAction(QIcon("images/ouvrirs.png"),tr("&Ouvrir"),this);
	Ouvrir->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
	Ouvrir->setStatusTip(tr("Ouvrir une image"));
	QObject::connect(Ouvrir,SIGNAL(triggered()),this,SLOT(ouvrir_une_image()));

	Ouvrir_plus=new QAction(QIcon("images/ouvrir.png"),tr("&Ouvrir plusieurs images"),this);
	
	Ouvrir_plus->setStatusTip(tr("Ouvrir une image"));
	QObject::connect(Ouvrir_plus,SIGNAL(triggered()),this,SLOT(ouvrir_des_images()));

	Home=new QAction(QIcon("images/Home.png"),tr("Page de démmarage"),this);
	Home->setStatusTip(tr("Retourner a la page de démmarage"));

	Sauvgarder=new QAction(QIcon("images/save.png"),tr("Enregistrer"),this);
	Sauvgarder->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
	Sauvgarder->setStatusTip(tr("Sauvgarder l'image encours"));
	QObject::connect(Sauvgarder,SIGNAL(triggered()),this,SLOT(enregistrer()));

	Sauvgarder_sous=new QAction(QIcon("images/saveas.png"),tr("Enregistrer sous ..."),this);
	Sauvgarder_sous->setStatusTip(tr("Sauvgarder l'image encours sous ..."));
	QObject::connect(Sauvgarder_sous,SIGNAL(triggered()),this,SLOT(enregistrer_sous()));

	quitter=new QAction(QIcon("images/quitter.png"),tr("&Quitter"),this);
	quitter->setStatusTip(tr("Quitter le programme"));
	quitter->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	QObject::connect(quitter,SIGNAL(triggered()),qApp,SLOT(quit()));

	fichier=menuBar()->addMenu(tr("&Fichier"));
	fichier->addAction(Ouvrir);
	fichier->addAction(Ouvrir_plus);
	fichier->addAction(Home);
	fichier->addSeparator();
	fichier->addAction(Sauvgarder);
	fichier->addAction(Sauvgarder_sous);
	fichier->addSeparator();
	fichier->addAction(quitter);
				//**********fin de fichier************//
			//Edition
	Annuler=new QAction(QIcon("images/prec.png"),tr("&Annuler"),this);
	Annuler->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
	Annuler->setStatusTip(tr("Retourner en arriére"));
	Annuler->setDisabled(true);
	QObject::connect(Annuler,SIGNAL(triggered()),this,SLOT(annuler()));

	Retablir=new QAction(QIcon("images/sui.png"),tr("&Rétablir"),this);
	Retablir->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Y));
	Retablir->setStatusTip(tr("Retablir l'image"));
	Retablir->setDisabled(true);
	QObject::connect(Retablir,SIGNAL(triggered()),this,SLOT(retablir()));

	Zoomin=new QAction(QIcon("images/zoom-.png"),tr("&Zoom Avant"),this);
	Zoomin->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Minus));
	Zoomin->setStatusTip(tr("Agrandir l'image"));
	//Zoomin->setDisabled(true);
	QObject::connect(Zoomin,SIGNAL(triggered()),this,SLOT(zoomin()));

	Zoomout=new QAction(QIcon("images/zoom+.png"),tr("&Zoom Arriére"),this);
	Zoomout->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Plus));
	Zoomout->setStatusTip(tr("Réduire l'image"));
	//Zoomout->setDisabled(true);
	QObject::connect(Zoomout,SIGNAL(triggered()),this,SLOT(zoomout()));

	Retabliir=new QAction(QIcon("images/zoom1.png"),tr("&La taille initiale"),this);
	Retabliir->setStatusTip(tr("Rétablir la taille initiale de l'image"));
	Retabliir->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_1));
	//Retabliir->setDisabled(true);
	QObject::connect(Retabliir,SIGNAL(triggered()),this,SLOT(retabliir()));


	edition=menuBar()->addMenu(tr("&Edition"));
	edition->setStatusTip(tr("Prêt"));
	edition->addAction(Annuler);
	edition->addAction(Retablir);
	edition->addSeparator();
	edition->addAction(Zoomin);
	edition->addAction(Zoomout);
	edition->addAction(Retabliir);

					//**********fin de Edition************//

				//Générer
	GGaussien=new QAction(QIcon("images/gaussien.png"),tr("appliquer le filtre &Gaussien"),this);
	GGaussien->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_G));
	GGaussien->setStatusTip(tr("Appliquer le filtre gaussien à l'image en cours"));
	QObject::connect(GGaussien,SIGNAL(triggered()),this,SLOT(gaussien()));

	MMedian=new QAction(QIcon("images/median.png"),tr("appliquer le filtre &Median"),this);
	MMedian->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_M));
	MMedian->setStatusTip(tr("Appliquer le filtre medianne à l'image en cours"));
	QObject::connect(MMedian,SIGNAL(triggered()),this,SLOT(median()));

	PPasseHaut=new QAction(QIcon("images/passehaut.png"),tr("appliquer le filtre &Passe Haut"),this);
	PPasseHaut->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
	PPasseHaut->setStatusTip(tr("Appliquer le filtre Passe Haut à l'image en cours"));
	QObject::connect(PPasseHaut,SIGNAL(triggered()),this,SLOT(passehaut()));

	GGradien=new QAction(QIcon("images/croix.png"),tr("appliquer le simple gradient"),this);
	GGradien->setShortcut(QKeySequence(Qt::ALT+Qt::Key_G));
	GGradien->setStatusTip(tr("Appliquer le simple gradient à l'image en cours"));
	QObject::connect(GGradien,SIGNAL(triggered()),this,SLOT(gradien()));

	GGradien1=new QAction(QIcon("images/gradient.png"),tr("appliquer le gradient amélioré"),this);
	GGradien1->setStatusTip(tr("Appliquer le gradient amélioré à l'image en cours"));
	QObject::connect(GGradien1,SIGNAL(triggered()),this,SLOT(gradien1()));

	SSoble=new QAction(QIcon("images/soble.png"),tr("appliquer le gradient de soble"),this);
	SSoble->setShortcut(QKeySequence(Qt::ALT+Qt::Key_S));
	SSoble->setStatusTip(tr("Appliquer le gradient de soble à l'image en cours"));
	QObject::connect(SSoble,SIGNAL(triggered()),this,SLOT(soble()));

	PPerwit=new QAction(QIcon("images/perwit.png"),tr("appliquer le gradient de perwit"),this);
	PPerwit->setShortcut(QKeySequence(Qt::ALT+Qt::Key_P));
	PPerwit->setStatusTip(tr("Appliquer le gradient de Perwit à l'image en cours"));
	QObject::connect(PPerwit,SIGNAL(triggered()),this,SLOT(perwit()));

	RRepoussage=new QAction(QIcon("images/repoussage.png"),tr("appliquer le Repoussage"),this);
	RRepoussage->setShortcut(QKeySequence(Qt::ALT+Qt::Key_R));
	RRepoussage->setStatusTip(tr("Appliquer le gradient de soble à l'image en cours"));
	QObject::connect(RRepoussage,SIGNAL(triggered()),this,SLOT(repoussage()));

	generer=menuBar()->addMenu(tr("&Prétaitement"));
	generer->addAction(MMedian);
	generer->addAction(GGaussien);
	generer->addAction(PPasseHaut);
	generer->addSeparator();
	generer->addAction(GGradien);
	generer->addAction(GGradien1);
	generer->addAction(SSoble);
	generer->addAction(PPerwit);
	
						//**********fin de genere************//
	////////////////////Aide
	QAction *AboutQt=new QAction(tr("A propos de Qt"),this);
	connect(AboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	
	aide=menuBar()->addMenu(tr("Aide"));
	aide->addAction(AboutQt);
}






////////////////////////////////TOOL BAR
void fene::toolbarcreate()
{
	QToolBar *gene_bar;
		//tool bar Generale
	gene_bar=addToolBar(tr("&Générales"));
	gene_bar->setIconSize(QSize(35,35));
	gene_bar->setStatusTip(tr("Prêt"));
	gene_bar->addAction(Ouvrir);
	gene_bar->addAction(Ouvrir_plus);
	gene_bar->addAction(Sauvgarder);
	gene_bar->addAction(Sauvgarder_sous);
	gene_bar->addSeparator();
	gene_bar->addAction(Annuler);
	gene_bar->addAction(Retablir);
	gene_bar->addAction(Zoomin);
	gene_bar->addAction(Zoomout);
	gene_bar->addAction(Retabliir);
	gene_bar->addSeparator();
	gene_bar->addAction(Home);

	/////////////////////////////
	Pen=new QAction(QIcon("images/pen.png"),tr("dessiner"),this);
	Pen->setStatusTip(tr("pour dessiner sur l'image"));

	Distance=new QAction(QIcon("images/distance.png"),tr("calculer les distance"),this);
	Distance->setStatusTip(tr("analyser"));
	Distance->setCheckable(true);
	Distance->setStatusTip(tr("pour calculer les distance sur l'image"));
	QObject::connect(Distance,SIGNAL(triggered()),this,SLOT(distanceAC()));

	Detecte=new QAction(QIcon("images/detecter.png"),tr("détection  d'objets"),this);
	Detecte->setStatusTip(tr("analyser"));
	Detecte->setCheckable(true);
	Detecte->setStatusTip(tr("pour selectioner un ou plusieurs objets sur l'image"));
	QObject::connect(Detecte,SIGNAL(triggered()),this,SLOT(detecterAC()));

	colorpen=new QAction(QIcon("images/color_pen.png"),tr("La couleur du dessin"),this);
	colorpen->setStatusTip(tr("pour selectioner la couleur du crayont"));
	connect(colorpen,SIGNAL(triggered()),this,SLOT(Colorpen()));

	colorobj=new QAction(QIcon("images/color_obj.png"),tr("La couleur des objet"),this);
	colorobj->setStatusTip(tr("la couleur de front de l'objet à detecter"));
	connect(colorobj,SIGNAL(triggered()),this,SLOT(Colorobj()));

	afficheHistogramme=new QAction(QIcon("images/histo.png"),tr("Afficher l'histogramme"),this);
	afficheHistogramme->setStatusTip(tr("afficher l'histogramme resultant"));
	connect(afficheHistogramme,SIGNAL(triggered()),this,SLOT(afficheHist()));

	clearHistogramme=new QAction(QIcon("images/effacer.png"),tr("Effacer l'histogramme"),this);
	clearHistogramme->setStatusTip(tr("effacer l'image de l'histogramme"));
	connect(clearHistogramme,SIGNAL(triggered()),this,SLOT(clearHist()));

	paraHistogramme=new QAction(QIcon("images/confige.png"),tr("configuration de l'histogramme"),this);
	paraHistogramme->setStatusTip(tr("configurer les paramétre de l'histogarme"));
	connect(paraHistogramme,SIGNAL(triggered()),this,SLOT(paraHist()));

	////////////////:
	QToolBar *painteer;
		//tool bar de painter
	painteer=new QToolBar("painter");
	painteer->setIconSize(QSize(30,30));
	painteer->setStatusTip(tr("Prêt"));
	addToolBar(Qt::BottomToolBarArea,painteer);
	painteer->addAction(Pen);
	painteer->addAction(Distance);
	painteer->addAction(Detecte);
	painteer->addAction(colorpen);
	painteer->addAction(colorobj);
	painteer->addSeparator();
	painteer->addAction(afficheHistogramme);
	painteer->addAction(clearHistogramme);
	painteer->addAction(paraHistogramme);

}




///////////////////////////DOCKS
void fene::dockcreate()
{
				//Dock manipulation
					//Declaration
	
	QCommandLinkButton *Median,*Gaussien,*PasseHaut,*NGrie,*Gradien1;

		//les filtres	

	Gaussien=new QCommandLinkButton(tr("&Rendre l'image plus\n        floue"));
	Gaussien->setStyleSheet(" min-height:45;");
	Gaussien->setIcon(QIcon("images/gaussien.png"));	
	Gaussien->setStatusTip(tr("Appliquer le filtre gaussien à l'image en cours"));
	QObject::connect(Gaussien,SIGNAL(clicked()),this,SLOT(gaussien()));

		
	Median=new QCommandLinkButton(tr("Eliminer le bruit \n        de l'image  "));
	Median->setStyleSheet(" min-height:45;");
	Median->setIcon(QIcon("images/median.png"));
	Median->setStatusTip(tr("Appliquer le filtre medianne à l'image encours"));
	QObject::connect(Median,SIGNAL(clicked()),this,SLOT(median()));

	PasseHaut=new QCommandLinkButton(tr("Augmenter\n        contraste"));
	PasseHaut->setIcon(QIcon("images/passehaut.png"));
	PasseHaut->setStyleSheet("min-height:45;");
	PasseHaut->setStatusTip(tr("Appliquer le filtre Passe Haut à l'image en cours"));
	QObject::connect(PasseHaut,SIGNAL(clicked()),this,SLOT(passehaut()));

	NGrie=new QCommandLinkButton(tr("&Convertir en \n        Niveau de gris "));
	NGrie->setIcon(QIcon("images/Ngris.png"));
	NGrie->setStyleSheet("min-height:45;");
	NGrie->setStatusTip(tr("Appliquer le Repoussage à l'image en cours"));
	QObject::connect(NGrie,SIGNAL(clicked()),this,SLOT(repoussage()));
	
						//fin des filtres

					//les gradient
	Gradien1=new QCommandLinkButton(tr("Gradient de l'image "));
	Gradien1->setIcon(QIcon("images/gradient.png"));
	Gradien1->setStyleSheet("min-height:45;");
	Gradien1->setStatusTip(tr("Appliquer le gardient amélioré à l'image encours"));
	QObject::connect(Gradien1,SIGNAL(clicked()),this,SLOT(gradien1()));

	spinbox=new QDoubleSpinBox();
	spinbox->setValue(2);
	spinbox->setRange(1,50);

	QFormLayout *layout_gra=new QFormLayout;
	layout_gra->addRow(" la dureté:",spinbox);
	layout_gra->addRow(Gradien1);

	QGroupBox *gradient=new QGroupBox("gradient");
	gradient->setStatusTip(tr("Prêt"));
	gradient->setLayout(layout_gra);

						//fin des gradient
	QGridLayout *layout_filtres=new QGridLayout();
	layout_filtres->addWidget(Median,0,0);
	layout_filtres->addWidget(Gaussien,2,0);
	layout_filtres->addWidget(PasseHaut,3,0);
	layout_filtres->addWidget(NGrie,4,0);
	
	QGroupBox *frame_filtre=new QGroupBox("Filtres et gradients:");
	frame_filtre->setStatusTip(tr("Prêt"));
	frame_filtre->setLayout(layout_filtres);

	/////////////Mouse
	spinbox_mouse_X=new QSpinBox();
	spinbox_mouse_X->setDisabled(true);
	spinbox_mouse_X->setMaximum(2000);

	spinbox_mouse_Y=new QSpinBox();
	spinbox_mouse_Y->setDisabled(true);
	spinbox_mouse_Y->setMaximum(2000);

	QLabel *xx=new QLabel("X :");
	QLabel *yy=new QLabel("Y :");
	
	QHBoxLayout *layout_mouse=new QHBoxLayout;
	layout_mouse->addWidget(xx);
	layout_mouse->addWidget(spinbox_mouse_X);
	layout_mouse->addWidget(yy);
	layout_mouse->addWidget(spinbox_mouse_Y);
	
	QGroupBox *move_souris=new QGroupBox("Les cordonnées du curseur :");
	move_souris->setStatusTip(tr("Prêt"));
	move_souris->setLayout(layout_mouse);

	///////////distance
	spindistance=new QDoubleSpinBox();
	spindistance->setMaximum(10000);
	spindistance->setDisabled(true);
	spindistance->setStatusTip(tr("la valeur de la dureté pour le gradient amélioré"));
	connect(pain,SIGNAL(signaldistance(double)),spindistance,SLOT(setValue(double)));

	QLabel *mm=new QLabel("mm");

	/////////surface
	spinsurface=new QDoubleSpinBox();
	spinsurface->setMaximum(10000000);
	spinsurface->setDisabled(true);
	//spinsurface->setValue(15.2341);
	spinsurface->setStatusTip(tr("la valeur de la dureté pour le gradient amélioré"));
	connect(pain,SIGNAL(surfaceSGN(double)),spinsurface,SLOT(setValue(double)));

	QLabel *mm2=new QLabel("mm²");

	spinechelle=new QDoubleSpinBox;
	spinechelle->setMaximum(1000000);
	spinechelle->setMinimum(0.01);
	spinechelle->setValue(1);
	spinechelle->setStatusTip(tr("l'echelle de dessin "));

	QHBoxLayout *layout_distance=new QHBoxLayout;
	layout_distance->addWidget(spindistance);
	layout_distance->addWidget(mm);

	QHBoxLayout *layout_surface=new QHBoxLayout;
	layout_surface->addWidget(spinsurface);
	layout_surface->addWidget(mm2);

	QFormLayout *layout_mesure=new QFormLayout();
	layout_mesure->addRow(tr("la distance:"),layout_distance);
	layout_mesure->addRow(tr("la surface :"),layout_surface);
	layout_mesure->addRow(tr("L'echelle :"),spinechelle);

	QGroupBox *mesure=new QGroupBox(tr("Les mesures calculées"));
	mesure->setStatusTip(tr("Prêt"));
	mesure->setLayout(layout_mesure);
	
	

	/////////////////////////
	QFormLayout *layout_dock=new QFormLayout();
	layout_dock->addWidget(move_souris);
	layout_dock->addWidget(mesure);
	layout_dock->addWidget(frame_filtre);
	layout_dock->addWidget(gradient);

	QWidget *Cdock=new QWidget();
	Cdock->setStatusTip(tr("Prêt"));
	Cdock->setLayout(layout_dock);

	dockmanipulation=new QDockWidget(tr("Manipulation"),this);
	dockmanipulation->setFixedWidth (200);
	//dockmanipulation->setStyleSheet("  QPushButton { background-color: red;  }");	
	dockmanipulation->setStatusTip(tr("Prêt"));
	dockmanipulation->setWidget(Cdock);
						//fin de Dock


////////////////DOCK LEVEL SET
	
	QPushButton *excuter,*play,*stop;
	
	
	///////////////////// widget simulation
	play=new QPushButton(QIcon("images/play.png"),"");
	play->setIconSize(QSize(40,45));
	connect(play,SIGNAL(clicked()),this,SLOT(Pllay()));
	
	stop=new QPushButton(QIcon("images/stop.png"),"");
	stop->setIconSize(QSize(40,45));
	connect(stop,SIGNAL(clicked()),this,SLOT(Sttop()));

	QHBoxLayout *layout_simuler=new QHBoxLayout();
	layout_simuler->addWidget(play);
	layout_simuler->addWidget(stop);

	simulation=new QGroupBox(tr("Simulation"));
	simulation->setStatusTip(tr("Prêt"));
	simulation->setLayout(layout_simuler);

	////////////
	excuter=new QPushButton(QIcon("images/excuter.png"),tr(" Excuter     "));
	excuter->setIconSize(QSize(45,30));
	excuter->setStatusTip(tr("segmenter l'image encours..."));
	QObject::connect(excuter,SIGNAL(clicked()),this,SLOT(analyse()));

	simuler=new QPushButton(QIcon("images/simuler.png"),tr("Excuter tout"));
	simuler->setStatusTip(tr("segmenter tout l'images selectionné"));
	simuler->setVisible(false);
	simuler->setIconSize(QSize(45,30));
	connect(simuler,SIGNAL(clicked()),this,SLOT(analyses()));

	ls1=new QRadioButton("Mumford 1");
	ls1->setChecked(true);
	ls2=new QRadioButton("Mumford 2");

	QVBoxLayout *ll=new QVBoxLayout();
	ll->addWidget(ls1);
	ll->addWidget(ls2);

	QGroupBox *lss=new QGroupBox(tr("le choix de level set"));
	lss->setStatusTip(tr("Prêt"));
	lss->setLayout(ll);

	//////////////////////////manuel

	spinbox_ray=new QSpinBox();
	spinbox_ray->setMinimum(5);
	spinbox_ray->setMaximum(300);
	spinbox_ray->setValue(10);
	spinbox_ray->setStatusTip(tr("la longeur du rayon de cercle"));
	connect(spinbox_ray,SIGNAL(valueChanged(int)),pain,SLOT(Ray(int)));

	spinbox_X=new QSpinBox();
	spinbox_X->setMinimum(0);
	spinbox_X->setMaximum(500);
	spinbox_X->setValue(115);
	spinbox_X->setStatusTip(tr("la postion selo l'axe des X"));
	
	spinbox_Y=new QSpinBox;
	spinbox_Y->setMinimum(0);
	spinbox_Y->setMaximum(500);
	spinbox_Y->setValue(115);
	spinbox_Y->setStatusTip(tr("la postion selon l'axe des Y"));

	spinbox_iter=new QSpinBox();
	spinbox_iter->setMinimum(0);
	spinbox_iter->setMaximum(300);
	spinbox_iter->setValue(10);
	spinbox_iter->setStatusTip(tr("le nombre d'itreation de level set"));
	connect(spinbox_iter,SIGNAL(valueChanged(int)),pain,SLOT(Iter(int)));
	
	QFormLayout *layout_manuel=new QFormLayout();
	layout_manuel->addRow("Nombre d'iteration:",spinbox_iter);
	layout_manuel->addRow("Le rayon :",spinbox_ray);
	layout_manuel->addRow("L'axe des X :",spinbox_X);
	layout_manuel->addRow("L'axe des Y :",spinbox_Y);

	QGroupBox *manuel=new QGroupBox(tr("Manuel"));
	manuel->setStatusTip(tr("Prêt"));
	manuel->setCheckable(true);
	manuel->setLayout(layout_manuel);

			///////////////////////Avencez
	spinbox_landa1=new QDoubleSpinBox();
	spinbox_landa1->setMinimum(0);
	spinbox_landa1->setValue(1);
	spinbox_landa1->setSingleStep(0.1);
	spinbox_landa1->setStatusTip(tr("Donner la valeur de landa1"));
	connect(spinbox_landa1,SIGNAL(valueChanged(double)),pain,SLOT(Landa1(double)));

	spinbox_landa2=new QDoubleSpinBox();
	spinbox_landa2->setMinimum(0);
	spinbox_landa2->setValue(1);
	spinbox_landa2->setSingleStep(0.1);
	spinbox_landa2->setStatusTip(tr("Donner la valeur de landa1"));
	connect(spinbox_landa2,SIGNAL(valueChanged(double)),pain,SLOT(Landa2(double)));

	spinbox_mu=new QDoubleSpinBox();
	spinbox_mu->setMaximum(1000000);
	spinbox_mu->setDecimals(4);
	spinbox_mu->setValue(0.5*255*255);
	spinbox_mu->setStatusTip(tr("Donner la valeur de Mu"));
	connect(spinbox_mu,SIGNAL(valueChanged(double)),pain,SLOT(Mu(double)));
	
	spinbox_nu=new QDoubleSpinBox();
	spinbox_nu->setMinimum(0);
	spinbox_nu->setDecimals(4);
	spinbox_nu->setSingleStep(0.1);
	spinbox_nu->setValue(0);
	spinbox_nu->setStatusTip(tr("Donner la valeur de Nu"));
	connect(spinbox_nu,SIGNAL(valueChanged(double)),pain,SLOT(Nu(double)));

	spinbox_dt=new QDoubleSpinBox();
	spinbox_dt->setMinimum(0);
	spinbox_dt->setSingleStep(0.1);
	spinbox_dt->setValue(0.1);
	spinbox_dt->setDecimals(4);
	spinbox_dt->setStatusTip(tr("Donner la valeur de dt"));
	connect(spinbox_dt,SIGNAL(valueChanged(double)),pain,SLOT(Dt(double)));

	QFormLayout *layout_avancee=new QFormLayout();
	layout_avancee->addRow("Landa 1 :",spinbox_landa1);
	layout_avancee->addRow("Landa 2 :",spinbox_landa2);
	layout_avancee->addRow("Mu :",spinbox_mu);
	layout_avancee->addRow("Nu :",spinbox_nu);
	layout_avancee->addRow("dt :",spinbox_dt);

	QGroupBox *avancee=new QGroupBox(tr("Avancée"));
	avancee->setStatusTip(tr("Prêt"));
	avancee->setCheckable(true);
	avancee->setLayout(layout_avancee);

	//////////////////////////dock level set
	QFormLayout *layout_dock2=new QFormLayout();
	layout_dock2->addWidget(lss);
	layout_dock2->addWidget(manuel);
	layout_dock2->addWidget(avancee);
	layout_dock2->addWidget(excuter);
	layout_dock2->addWidget(simuler);
	layout_dock2->addWidget(simulation);
	
	QWidget *LSdock=new QWidget();
	LSdock->setStatusTip(tr("Prêt"));
	LSdock->setLayout(layout_dock2);

	levelset=new QDockWidget(tr("Level Set"));
	levelset->setStatusTip(tr("Prêt"));
	levelset->setFixedWidth (200);
	levelset->setWidget(LSdock);
					//fin de Dock
}
