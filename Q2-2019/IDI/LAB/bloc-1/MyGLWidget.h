#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"
#define GLM_FORCE_RADIANS

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();

    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    
    //KeyPressEvent
    virtual void keyPressEvent(QKeyEvent *e);

  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTransforms(); // Funcio de trans de mat

    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    GLuint scaleLoc;
    GLuint TGLoc;
    
    // Program
    QOpenGLShaderProgram *program;
    
    // Variables Locals
    GLuint VAO1;
    GLint ample, alt;
    GLfloat factor;  //Factor escala
    float tx, ty;   //Translació x i y
};
