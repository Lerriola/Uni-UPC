#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
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
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
  iniCamera ();
  iniCamera2();
}

void MyGLWidget::iniEscena ()
{
  creaBuffersTerra();
  creaBuffersCub();
  creaBuffersPatricio();
  
  centreEsc = glm::vec3(10,3,10);
  radiEsc = float(sqrt(20*20+20*20+6*6)/2);
  posPatricio = glm::vec3(10, 0, 10);
  rotaPatricio = 0.0f;
  dir = 0;
}

void MyGLWidget::iniCamera ()
{
  angleY = M_PI/6.0;
  angleX = 0.0f;
  obs = centreEsc + glm::vec3(0,0, 2*radiEsc);
  vrp = centreEsc;
  up = glm::vec3(0,1,0);
  ra = 1.0;
  fov = 2.0 * asin(0.5f);
  zn = radiEsc;
  zf = 3.0*radiEsc;

  projectTransform ();
  viewTransform ();
}

//Inicialitzacio dels valors de la camera de primera persona
void MyGLWidget::iniCamera2 ()
{
  obs2 = glm::vec3(posPatricio.x,2.5,posPatricio.z);
  vrp2 = obs2 + glm::vec3(0,0,2*radiEsc);
  fov2 = M_PI/3.0;
  zn2 = 0.5;
  zf2 = 3.0*radiEsc;

  firstPatricio = false;

  projectTransform ();
  viewTransform ();
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
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el Terra 
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Activem el VAO per a pintar la paret1
  glBindVertexArray (VAO_Cub);
  modelTransformParet1 ();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  
  // Activem el VAO per a pintar la paret2
  glBindVertexArray (VAO_Cub);
  modelTransformParet2 ();
  glDrawArrays(GL_TRIANGLES, 0, 36);


  // Activem el VAO per a pintar el Homer
  glBindVertexArray (VAO_Patricio);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, Patricio.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;

  ra = float(ample)/float(alt);
  if (ra < 1)
  {
    fov = 2.0 * atan(tan(asin(0.5f))/ra);
    fov2 = 2.0 * atan(tan(asin(0.5f))/ra);
  } 
  projectTransform ();
}

void MyGLWidget::modelTransformPatricio()
{
  float escala = 1.0f;
  calculaCapsaModel (Patricio,escala,centreBasePatricio);

  glm::mat4 TG(1.f);  // Matriu de transformació

  TG = glm::translate(TG,posPatricio);
  TG = glm::rotate(TG, rotaPatricio, glm::vec3(0,1,0));
  TG = glm::scale(TG,glm::vec3 (escala));
  TG = glm::translate(TG,-centreBasePatricio);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformParet1()  // Cal modificar aquesta funció...
{
  glm::mat4 TG(1.f);  // Matriu de transformació

  TG = glm::translate(TG,glm::vec3(10,0,16));
  TG = glm::scale(TG,glm::vec3 (0.6,6,8));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformParet2()  // Cal modificar aquesta funció...
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  
  TG = glm::translate(TG,glm::vec3(10,0,4));
  TG = glm::scale(TG,glm::vec3 (0.6f,6,8));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform () // Cal modificar aquesta funció...
{
  glm::mat4 Proj;  // Matriu de projecció
  
  //En funcio del boolea passem els parametres de la primera persona o no
  if(!firstPatricio)Proj = glm::perspective(fov, ra, zn, zf);
  else Proj = glm::perspective(fov2, ra, zn2, zf2);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () // Cal modificar aquesta funció...
{
  glm::mat4 View(1.f);  // Matriu de posició i orientació

  //Determino quina view aplicar en funcio del boolea que es modifica al premer C
  if(!firstPatricio)
  { // View amb angles euler
    View = glm::translate(View,glm::vec3(0.,0.,-2*radiEsc));
    View = glm::rotate(View,angleX,glm::vec3(1,0,0));
    View = glm::rotate(View,-angleY,glm::vec3(0,1,0));
    View = glm::translate(View,-vrp);
  }
  else // First Person View : Obs2 es declara a sobre el patricio i s'actualitza al moure'l
  // El vrp2 el declarem devant del obs, en funcio del angle de rotacio/dir es mou a una altre localitzaciò.
  //Aixi mitjançant despres el lookAt simulem la rotacio en la primera persona. 
  {
    obs2 = glm::vec3(posPatricio.x,2.5,posPatricio.z);

    switch(dir){

      case 0 : {
        vrp2 = obs2 + glm::vec3(0,0,2*radiEsc);
        break;
      }
      case 1:{
        vrp2 = obs2 + glm::vec3(2*radiEsc,0,0);
        break;
      }
      case 2:{
        vrp2 = obs2 + glm::vec3(0,0,-2*radiEsc);
        break;
      }
      case 3:{
        vrp2 = obs2 + glm::vec3(-2*radiEsc,0,0);
        break;
      }
    }
    View = glm::lookAt(obs2,vrp2,up);
  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)  // Cal modificar aquesta funció...
{

  makeCurrent();
  switch (event->key()) {
    // Per evitar problemes precalculo la posicio a on es desplaçaria i comprobo si es vàlida. Utilitzo
    // la variable distPatricio que es xmax-xmin/2 escalat a l'escala demanada per evitar que hi hagi overlaping
    case Qt::Key_Left: {
      if(posPatricio.x - 0.5 >= distPatricio and 
        not( (posPatricio.x - 0.5 <= 10.3 + distPatricio and posPatricio.x - 0.5 >= 9.7 - distPatricio) 
          and (posPatricio.z  <= 8 + distPatricio or posPatricio.z >= 12 - distPatricio)))
      {
        posPatricio.x -= 0.5;
      }
      break;
    }
    case Qt::Key_Right: {
      if (posPatricio.x + 0.5 <= 19 and 
        not((posPatricio.x + 0.5 <= 10.3 + distPatricio and posPatricio.x + 0.5 >= 9.7 - distPatricio)
         and (posPatricio.z  <= 8 + distPatricio or posPatricio.z >= 12 - distPatricio)))
      {
        posPatricio.x += 0.5;
      }
      break;
    }
    case Qt::Key_Up: {
      if(posPatricio.z - 0.5 >= 1 and 
        not((posPatricio.x <= 10.3 + distPatricio and posPatricio.x >= 9.7 - distPatricio)
         and (posPatricio.z - 0.5 <= 8 + distPatricio or posPatricio.z - 0.5 >=  12 - distPatricio)))
      {
      posPatricio.z -= 0.5;
      }
      break;
  }
    case Qt::Key_Down: {
      if (posPatricio.z + 0.5 <= 19 and 
        not((posPatricio.x <= 10.3 + distPatricio and posPatricio.x >= 9.7 - distPatricio)
         and (posPatricio.z + 0.5 <= 8 + distPatricio or posPatricio.z + 0.5 >=  12 - distPatricio)))
      {
      posPatricio.z += 0.5;
      }
      break;
  }
    case Qt::Key_R: {
      rotaPatricio += M_PI/2.0;
      ++dir;
      if (dir == 4) dir = 0;
      break;
  }
	  case Qt::Key_I: {
      posPatricio = glm::vec3(10,0,10);
      rotaPatricio = 0.0f;
      dir = 0;
	    break;
  }
    case Qt::Key_C: {
      firstPatricio = !firstPatricio;
	}
    default: event->ignore(); break;
  }

  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {

    if(e->x() > xClick) angleY += 0.05;
    else if(e->x() < xClick) angleY -= 0.05;

    if(e->y() > yClick) angleX += 0.05;
    else if(e->y() < yClick) angleX -= 0.05;

    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  
  escala = 2.0/(maxy-miny);
  distPatricio = escala * (maxx-minx)/2.0;
  centreBase[0] = (minx+maxx)/2.0; 
  centreBase[1] = miny; 
  centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::creaBuffersCub ()
{
  // Dades del cub
  // Vèrtexs del cub
  glm::vec3 vertexs[8] = {
       /* 0*/ glm::vec3( -0.5, 0.0, -0.5),  /* 1*/ glm::vec3( 0.5, 0.0, -0.5),
       /* 2*/ glm::vec3( -0.5, 1.0, -0.5),  /* 3*/ glm::vec3( 0.5, 1.0, -0.5),
       /* 4*/ glm::vec3( -0.5, 0.0, 0.5),  /* 5*/ glm::vec3( 0.5, 0.0, 0.5),
       /* 6*/ glm::vec3( -0.5, 1.0, 0.5),  /* 7*/ glm::vec3( 0.5, 1.0, 0.5)
  };

  // VBO amb la posició dels vèrtexs
  glm::vec3 poscub[36] = {  // 12 triangles
       vertexs[0], vertexs[2], vertexs[1],
       vertexs[1], vertexs[2], vertexs[3],
       vertexs[5], vertexs[1], vertexs[7],
       vertexs[1], vertexs[3], vertexs[7],
       vertexs[2], vertexs[6], vertexs[3],
       vertexs[3], vertexs[6], vertexs[7],
       vertexs[0], vertexs[4], vertexs[6],
       vertexs[0], vertexs[6], vertexs[2],
       vertexs[0], vertexs[1], vertexs[4],
       vertexs[1], vertexs[5], vertexs[4],
       vertexs[4], vertexs[5], vertexs[6],
       vertexs[5], vertexs[7], vertexs[6]
  };
  
  glm::vec3 color(0, 0, 1.0);

  glm::vec3 colorcub[36] = {
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color
  };
  
  // Creació del Vertex Array Object del cub
  glGenVertexArrays(1, &VAO_Cub);
  glBindVertexArray(VAO_Cub);

  GLuint VBO_Cub[2];
  glGenBuffers(2, VBO_Cub);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(poscub), poscub, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorcub), colorcub, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersPatricio ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  Patricio.load("./models/Patricio.obj");
  
  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Patricio);
  glBindVertexArray(VAO_Patricio);

  // Creació dels buffers del model
  GLuint VBO_Patricio[2];
  // Buffer de posicions
  glGenBuffers(2, VBO_Patricio);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Patricio.faces().size()*3*3, Patricio.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de component difusa usada com a color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Patricio.faces().size()*3*3, Patricio.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTerra ()
{
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[6] = {
	glm::vec3(0.0, 0.0, 20.0),
	glm::vec3(20.0, 0.0, 20.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(20.0, 0.0, 20.0),
	glm::vec3(20.0, 0.0, 0.0),
  }; 

  glm::vec3 color(0.0,0.6,0.0);

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 colterra[6] = {
	color, color, color, color, color, color
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("./shaders/basicShader.frag");
  vs.compileSourceFile("./shaders/basicShader.vert");
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

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
}

