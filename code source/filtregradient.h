#include "fene1.h"
#include "image.cpp"

//les action de filtres
				/*median*/
void fene::median()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::median(image);
		pain->mod(*image);
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour appliquer le filtres Médian"));
}
			/*passehaut*/
void fene::passehaut()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::passehaut(image);
		pain->mod(*image);
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour appliquer le filtres Passe Haut"));
}
			/*gaussien*/
void fene::gaussien()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::gaussien(image);
		pain->mod(*image);		
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour appliquer le filtres Gaussien"));
}
			/*Repoussage*/
void fene::repoussage()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::Niveau_Gris(image);
		pain->mod(*image);
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour la conversion au Niveaux de gris"));
}
			//fin des action de filtres



					//les gradiens
				/*simple gradien*/
void fene::gradien()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::gradien(image);
		pain->mod(*image);
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour appliquer le simple gradient "));
}
			/*gradien améliorer*/
void fene::gradien1()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::gradienami(image,spinbox->value());
		pain->mod(*image);
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour appliquer le gradient améliorer"));
}
				/*soble*/
void fene::soble()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::soble(image);
		pain->mod(*image);
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour appliquer le gradient de Soble"));
}
				/*perwit*/
void fene::perwit()
{
	if(!image->isNull())
	{
		charger(image);
		RImage::perwit(image);
		pain->mod(*image);
	}
	else QMessageBox::warning(this,tr("avertissement"),tr("il y'a pas d'image pour appliquer le gradient de Perwit"));
}
		//***************fin des gardiens




