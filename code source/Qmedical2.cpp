#include <QApplication>
#include <QWidget>
#include <QIcon>
#include <QPlastiqueStyle>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QCDEStyle>


#include "fene1.cpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
		app.installTranslator(translator);

	//QApplication::setStyle(new QPlastiqueStyle);
	fene rab;
	
	//rab.setFont(QFont("Cambria", 10, QFont::Bold));
	//rab.setStyleSheet("background: rgb(255,255,0)");
	rab.setWindowIcon(QIcon("images/theme.png"));
	rab.setWindowTitle("fenetre principale");
	rab.showMaximized();
	return app.exec();
}

