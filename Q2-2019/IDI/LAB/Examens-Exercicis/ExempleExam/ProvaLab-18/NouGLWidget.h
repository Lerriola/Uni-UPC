#include "MyGLWidget.h"

class NouGLWidget : public MyGLWidget
{
 Q_OBJECT
	   
 public:
  NouGLWidget (QWidget *parent=NULL);
  ~NouGLWidget ();

 public slots:
 void SetFocus();

 signals:
 void TeclaSet(QString s);

 protected:
  void paintGL ();
  void iniCamera ();
  void iniEscena ();
  void viewTransform ();
  void projectTransform ();
  void modelTransformPatricio ();
  // keyPressEvent - Es cridat quan es prem una tecla
  void keyPressEvent (QKeyEvent *event);
  
 private:
  void modelTransformPatricio2 ();
  void ActPosfocus();
  bool validPos(glm::vec3);
  

  glm::vec3 posFocus;
  float fov, zn, zf;

  glm::vec3 posPat2;
  bool FPatricio;
  glm::vec3 vrp2, obs2, up2;
  float fov2, zn2, zf2;
};
