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
    // MouseMoveEvent - Es crida al moure el ratoli
    virtual void mouseMoveEvent(QMouseEvent *event);

  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTransform (glm::vec3 pos, float angle);
    void viewTransform ();
    void terraTransform ();
    void init_camera ();
    void Attribute_Calc(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    void projectTransform(float FOV, float ra, float zNear, float zFar);
    void paintModel(glm::vec3 pos, float angle);

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc;
    GLuint projLoc;
    GLuint viewLoc;
    
    // VAO i VBO names
    GLuint VAO_Model;
    GLuint VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;

    // Internal vars
    float scale;
    glm::vec3 pos;
    float rota;
    float radi;
    float d;

    glm::vec3 posTerra;
    glm::vec3 color;

    //Cameras
    float FOV_Inicial;
    float FOV;
    float ra; 
    float zNear; 
    float zFar;


    //Angles Euler

    float theta; //AngleGir x
    float psi;   //AngleGir y

    float Xant;
    float Yant;

    glm::vec3 VRP;
    glm::vec3 UP;

    bool ortho;

    //Model
    Model mod;
 
};

