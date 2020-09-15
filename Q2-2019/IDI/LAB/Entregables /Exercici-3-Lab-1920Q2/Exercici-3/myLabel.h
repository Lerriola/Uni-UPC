#include <QLabel>
class myLabel: public QLabel
{
  Q_OBJECT
  public:
  	myLabel(QWidget *parent);
  	int moves = 0;
  	bool status = false;
  	
  public slots:

  	void ChangeStat();

  	void IncUp();
  	void IncDown();
  	void IncLeft();
  	void IncRight();
  	void Gir();

  signals:
  	void enviamoves(int moves);
  	void SwitchOff();
};