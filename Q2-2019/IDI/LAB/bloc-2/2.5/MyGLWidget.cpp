#include "MyGLWidget.h"
#include <cmath>
#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 0.0f;
  rota = 0.0f;
  ortho = false;
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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable (GL_DEPTH_TEST); // activa Zbuffer

  // Carrega de MODELS
  mod.load("./models/Patricio.obj");
  
  carregaShaders();
  creaBuffers();
  
  init_camera();
}

void MyGLWidget::Attribute_Calc(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{

    radi = sqrt((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)+(zmax-zmin)*(zmax-zmin))/2.0;
    d = 2 * radi;

    VRP = glm::vec3((xmax+xmin)/2.0,(ymax+ymin)/2.0,(zmax+zmin)/2.0); 
    UP = glm::vec3(0,1,0);

    theta = 0.0f;
    psi = 0.0f;

    FOV_Inicial = 2.0 * asin(radi / d);
    FOV = FOV_Inicial;
    ra = 1.0f;
    zNear = d - radi;
    zFar = d + radi;
}

void MyGLWidget::init_camera ()
{

  Attribute_Calc(-2.5f, 2.5f, 0.0f, 1.0f, -2.5f, 2.5f);
  projectTransform(FOV, ra, zNear, zFar);
 	viewTransform();

}

void MyGLWidget::paintModel(glm::vec3 pos,float angle)
{
    // Carreguem la transformació de model
  modelTransform (pos,angle);
  
  // Activem el VAO per a pintar el homero
  glBindVertexArray (VAO_Model);
  //Pintem
  glDrawArrays(GL_TRIANGLES, 0, sizeof(GLfloat) * mod.faces().size() * 3);

  glBindVertexArray(0);
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
  
  // Esborrem el frame-buffer i el z-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Pintem l'objecte els cops necessesaris

  paintModel(glm::vec3 (2,0,2),0);

  paintModel(glm::vec3 (0,0,0),float(M_PI/2));

  paintModel(glm::vec3 (-2,0,-2),M_PI);
  
  // Carreguem la transfromacio del terra
  terraTransform();
 
  //Activem el VAO per pintar el terra
  glBindVertexArray (VAO_Terra);

  //Pintem
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform (glm::vec3 pos, float angle) 
{
	// Calcul de la capsa mínima contenidora
	float xmin, ymin, zmin, xmax, ymax, zmax;

	xmin = xmax = mod.vertices()[0];
    ymin = ymax = mod.vertices()[1];
    zmin = zmax = mod.vertices()[2];

    for(unsigned int i = 3;i<mod.vertices().size();i+=3) {
        double x = mod.vertices()[i];
        double y = mod.vertices()[i+1];
        double z = mod.vertices()[i+2];
        if(x<xmin) xmin = x;
        else if (x>xmax) xmax = x;
        if(y<ymin) ymin = y;
        else if (y>ymax) ymax = y;
        if(z<zmin) zmin = z;
        else if (z>zmax) zmax = z;
    }

  	// Matriu de transformació de model
  	glm::mat4 transform (1.0f);

    transform = glm::translate(transform,pos);
    transform = glm::rotate(transform, angle, glm::vec3(0,1,0));
    
  	transform = glm::scale(transform,glm::vec3 (1/(xmax-xmin))+scale);
  	transform = glm::rotate(transform, rota, glm::vec3(0,1,0));
  	transform = glm::translate(transform,-glm::vec3((xmax+xmin)/2.0,ymin, (zmax+zmin)/2.0));
  	
  	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::terraTransform ()
{
    //Matriu transform terraTransform
    glm::mat4 terratransform(1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &terratransform[0][0]);
}

void MyGLWidget::projectTransform (float FOV, float ra, float zNear, float zFar)
{    
    glm::mat4 proj = glm::perspective (FOV, ra, zNear, zFar);
    if (ortho) {
    	if (ra > 1) proj = glm::ortho (-radi*ra, radi*ra, -radi, radi, zNear, zFar);
    	else proj = glm::ortho (-radi, radi, -radi/ra, radi/ra, zNear, zFar);
    }
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
    glm::mat4 View (1.0f);
    View = glm::translate(View,glm::vec3(0.,0.,-d));
    View = glm::rotate(View,theta,glm::vec3(1,0,0));
    View = glm::rotate(View,-psi,glm::vec3(0,1,0));
    View = glm::translate(View,-VRP);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
  float ra = float(w)/float(h);
  if (ra < 1) FOV = 2.0 * atan(tan(FOV_Inicial/2.0)/ra);
  projectTransform(FOV, ra, zNear, zFar);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: { // escalar a més petit
      rota += 0.45;
      break;
    }
    case Qt::Key_O: {
    	ortho = !ortho;
    	projectTransform(FOV,ra,zNear,zFar);
      break;
    }
    case Qt::Key_Z: { // escalar a més petit
      FOV -= 0.05;
      projectTransform(FOV,ra,zNear,zFar);
      break;
    }
    case Qt::Key_X: { // escalar a més petit
      FOV += 0.05;
      projectTransform(FOV,ra,zNear,zFar);
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) 
{
  makeCurrent();
  
  if(e->x() > Xant) psi += 0.02;
  else if(e->x() < Xant) psi -= 0.02;

  if(e->y() > Yant) theta += 0.02;
  else if(e->y() < Yant) theta -= 0.02;
  
  Xant = e->x();
  Yant = e->y();
    
  viewTransform();
  update();
}

void MyGLWidget::creaBuffers () 
{
  
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Model);
  glBindVertexArray(VAO_Model);

  GLuint VBO_Model[2];
  glGenBuffers(2, VBO_Model);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mod.faces().size() * 3 * 3 , mod.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mod.faces().size() * 3 * 3 , mod.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
 

  glm::vec3 posTerra[6] = {
	glm::vec3(-2.5, 0, -2.5),
    glm::vec3(2.5, 0, -2.5),
    glm::vec3(-2.5, 0, 2.5),
    glm::vec3(2.5, 0, -2.5),
    glm::vec3(-2.5, 0, 2.5),
    glm::vec3(2.5, 0, 2.5)
  };

    glm::vec3 color[6] = {
          glm::vec3(1,0,1),
          glm::vec3(1,0,1),
          glm::vec3(1,0,1),

          glm::vec3(1,0,1),
          glm::vec3(1,0,1),
          glm::vec3(1,0,1),
    };
  
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posTerra) , posTerra, GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color) , color, GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
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
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");


}

