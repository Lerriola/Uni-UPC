//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
    setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat

    num_triang = 8;
    num_vertx = num_triang*3;
    angularOffset =  2*M_PI/(float)num_triang;
}

MyGLWidget::~MyGLWidget ()
{
    if (program != NULL)
        delete program;
}

void MyGLWidget::initializeGL ()
{
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();
    glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    carregaShaders();
    creaBuffersUmbrella();
}

void MyGLWidget::pintaUmbrella ()  // AQUEST MÈTODE NO ES POT MODIFICAR !!!
{
    // Activem l'Array a pintar
    glBindVertexArray(VAO1);
    // Pintem
    glDrawArrays(GL_TRIANGLES, 0, num_vertx);
}

void MyGLWidget::paintGL ()
{
    // Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
    GLint vp[4];
    glGetIntegerv (GL_VIEWPORT, vp);
    ample = vp[2];
    alt = vp[3];
#endif

    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);

    glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);  // Esborrem el frame-buffer

    // Pintem paraigües 1
    modelTransformUmbrella (0, glm::vec3(-0.5, 0.0, 0.0), 0.5, false);
    pintaUmbrella ();

    // Pintem paraigues 2

    modelTransformUmbrella (0, glm::vec3(0.5, 0.0, 0.0), 0.5, true);
    pintaUmbrella ();

    // Desactivem el VAO
    glBindVertexArray(0);
}

void MyGLWidget::modelTransformUmbrella (float anglegir, glm::vec3 posicioCentre, float escala , bool rodaDreta)
{
    glm::mat4 TG (1.0); // Matriu de transformació, inicialment identitat

    // Transladem la posicio centre 
    TG = glm::translate (TG,posicioCentre);

    // Apliquem l'escala dessitjada
    TG = glm::scale(TG, glm::vec3(escala,escala,escala));

    //Apliquem la rotació en funciò de rodaDreta
    if(rodaDreta) TG = glm::rotate(TG, glm::radians(180.f), glm::vec3(0.0, 1.0, 0.0));
    
    // Apliquem la rotació de anglegir
    TG = glm::rotate(TG, anglegir , glm::vec3(0.0, 0.0, 1.0));

    glUniformMatrix4fv (TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::resizeGL (int w, int h)
{
    ample = w;
    alt = h;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
    makeCurrent();
    switch (event->key()) {
    case Qt::Key_R: { // rotació Umbrella 1
        break;
    }
    case Qt::Key_T: { // rotació Umbrella 2
        break;
    }
    default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::creaBuffersUmbrella ()
{
    // Dades de les coordenades dels vèrtexs
    glm::vec3 Vertices[num_vertx];
    
    // Dades dels colors de les cordenades
    glm::vec3 Colors[num_vertx];  

    float a = 0;
    for (int i=0; i<num_vertx; i+=3) {
        
        Vertices[i] = glm::vec3(0.0, 0.0, 0.0);
        Vertices[i+1]= glm::vec3(cos(a), sin(a), 0);
        a+= angularOffset;
        Vertices[i+2]= glm::vec3(cos(a), sin(a), 0);
        
        //Coloreig vertexs
        for(int j = 0; j < 3 ; ++j){
            if(i % 2 == 0) Colors[i+j] = glm::vec3(1,0,0);
            else Colors[i+j] = glm::vec3(1,1,1);
        }
            
    }

    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    GLuint VBOs[2];
    glGenBuffers(2, VBOs);
    
    // Creació del buffer amb les coordenades dels vèrtexs
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);
    
    //Creació del buffer amb colors vertexs
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    // Desactivem el VAO
    glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
    // Creem els shaders per al fragment shader i el vertex shader
    QOpenGLShader fs (QOpenGLShader::Fragment, this);
    QOpenGLShader vs (QOpenGLShader::Vertex, this);
    // Carreguem el codi dels fitxers i els compilem
    fs.compileSourceFile("shaders/basicShader.frag");
    vs.compileSourceFile("shaders/basicShader.vert");
    // Creem el program
    program = new QOpenGLShaderProgram(this);
    // Li afegim els shaders corresponents
    program->addShader(&fs);
    program->addShader(&vs);
    // Linkem el program
    program->link();
    // Indiquem que aquest és el program que volem usar
    program->bind();

    // Obtenim identificador per a l'atribut “vertex” del vertex shader
    vertexLoc = glGetAttribLocation (program->programId(), "vertex");
    // Obtenim identificador per a l'atribut "color" del vertex shader
    colorLoc = glGetAttribLocation (program->programId(), "color");
    // Obtenim identificador per a la trasn geometrica
    TGLoc = glGetUniformLocation (program->programId(), "TG");
 }
