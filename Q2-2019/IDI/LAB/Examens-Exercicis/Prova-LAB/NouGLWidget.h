#include "MyGLWidget.h"

class NouGLWidget : public MyGLWidget
{
 Q_OBJECT
	   
 public:
  NouGLWidget (QWidget *parent=NULL);
  ~NouGLWidget ();

 public slots:
 void setCam();
 void slidercamSet(int zpil);

 signals:
 void sliderKeySet(int zpil);

 protected:
  void paintGL ();
  void resizeGL (int w, int h);
  void iniCamera ();
  void iniEscena ();
  void viewTransform ();
  void projectTransform ();
  void modelTransformTerra ();
  void modelTransformPatricio ();
  void modelTransformPilota ();
  
  // keyPressEvent - Es cridat quan es prem una tecla
  void keyPressEvent (QKeyEvent *event);
  
 private:
  void ActPosfocus();

  float fov, zn, zf;

  glm::vec3 posPil;
  bool gol;

  bool ortho;
  glm::vec3 obs, up, vrp;
};
