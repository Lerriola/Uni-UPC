#include "MyGLWidget.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class NouGLWidget : public MyGLWidget
{
 Q_OBJECT
	   
 public:
  NouGLWidget (QWidget *parent=NULL);
  ~NouGLWidget ();

 public slots:
 	void sliderFocusSet(int xfocus);

 signals:
 	void sliderKeySet(int xfocus);

 protected:
  virtual void initializeGL ();
  virtual void viewTransform ();
  virtual void modelTransformLego ();
  
  // keyPressEvent - Es cridat quan es prem una tecla
  virtual void keyPressEvent (QKeyEvent *event);
  
 private:

 	void normalizeTransform();
 	void iniFocus();
    void actPosFocus();
    void CarregaShadersFocus();


    GLuint posFocusLoc, colFocusLoc;
    bool TypeFocus;
    glm::vec4 posFocus;
    glm::vec3 colFocus;
};
