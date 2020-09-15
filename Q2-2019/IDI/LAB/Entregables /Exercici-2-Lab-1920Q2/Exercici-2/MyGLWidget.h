#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void creaBuffersTerra ();
    void creaBuffersCub ();
    void creaBuffersPatricio ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformParet1 ();
    void modelTransformParet2 ();
    void modelTransformPatricio ();
    void calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase);
    void iniEscena ();
    void iniCamera ();
    void iniCamera2();

    // VAO names
    GLuint VAO_Cub, VAO_Terra, VAO_Homer, VAO_Patricio;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, colorLoc;

    GLint ample, alt;

    // model
    Model homer;
    Model Patricio;

    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 posHomer, centreBaseHomer;
    glm::vec3 posPatricio, centreBasePatricio;

    //Rotacio i dist per evitar overlapping
    float rotaPatricio;
    float distPatricio;
    // Indicador de rotacio en primera persona
    int dir;

    //Camera 1
    glm::vec3 centreEsc, obs, vrp, up;
    float radiEsc, ra, fov, zn, zf;
    
    //Euler
    float angleX, angleY;

    //Camera 2
    glm::vec3 obs2, vrp2, up2;
    float fov2, zn2, zf2;

    //Bool primera persona
    bool firstPatricio;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
};

