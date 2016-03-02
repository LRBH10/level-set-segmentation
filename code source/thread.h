#ifndef head_thread
#define	head_thread

#include <QThread>
#include <QMessageBox>

class Thread : public QThread
{
    Q_OBJECT
    
public:
	Thread(QObject* parent = 0);
    virtual ~Thread();
    void run();
		
	public slots:
		void setparame(QImage imag,int x,int y,int ray,int nbr,double landa1,double landa2,double mu,double nu,double dt);	
		void setTest(bool Test);
 
signals:
	void signalIMG(QImage image);
        
private:
	bool test;
	QImage *image;
	int x,y,ray,nbr;
	double landa1,landa2,mu,nu,dt;
 };

#endif
