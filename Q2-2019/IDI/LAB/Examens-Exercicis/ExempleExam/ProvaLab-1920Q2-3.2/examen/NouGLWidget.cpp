#include "NouGLWidget.h"

#include <iostream>

NouGLWidget::NouGLWidget(QWidget *parent) : MyGLWidget(parent) {
  grabKeyboard();
}

NouGLWidget::~NouGLWidget ()
{
}

void NouGLWidget::paintGL() {
  MyGLWidget::paintGL();
}

void NouGLWidget::resizeGL(int w, int h) {
  MyGLWidget::resizeGL(w, h);
}

void NouGLWidget::iniCamera ()
{
  MyGLWidget::iniCamera();
  angleY = 0.0;
  angleX = float(M_PI/6.0);

  fov = 2.0 * asin(0.5f);
  zn = radiEsc;
  zf = 3.0*radiEsc;
  rav = 1.0;

  ortho = false;

  obs = centreEsc + glm::vec3(0,2*radiEsc,0);
  up = glm::vec3(0,0,-1);
  vrp = glm::vec3(3,1,4.5);

  projectTransform ();
  viewTransform ();
}

void NouGLWidget::iniEscena ()
{
  MyGLWidget::iniEscena();

  centreEsc = glm::vec3(3,1,4.5);
  radiEsc = 5.5;
  posPil = glm::vec3(3,0,7);
  gol = false;
}

void NouGLWidget::ActPosfocus()
{
  glm::vec4 posFocus = MyGLWidget::View * glm::vec4(3.0, 4.0, 8.0,1.0);
  glm::vec3 colFocus;

  if(gol) colFocus = glm::vec3(1, 1, 0);
  else colFocus = glm::vec3(1, 1, 1);

  glUniform4fv (posFocusLoc, 1, &posFocus[0]);
  glUniform3fv (colFocusLoc, 1, &colFocus[0]);
}

void NouGLWidget::modelTransformTerra ()
{
  MyGLWidget::modelTransformTerra();

  glm::mat4 TG(1.0f); 

  TG = glm::scale(TG,glm::vec3 (2,1,3));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void NouGLWidget::modelTransformPilota ()
{

  MyGLWidget::modelTransformPilota();
  glm::mat4 pilTG = glm::mat4(1.0f);
  if(!gol){
    pilTG = glm::translate(pilTG,posPil);
    pilTG = glm::scale(pilTG, glm::vec3(0.5*escalaPil));
    pilTG = glm::translate(pilTG, -centreBasePil);
  }
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &pilTG[0][0]);
}

void NouGLWidget::modelTransformPatricio ()
{
  MyGLWidget::modelTransformPatricio();

  glm::mat4 patTG = glm::mat4(1.0f);

  patTG = glm::translate(patTG,glm::vec3(3,0,8));
  patTG = glm::rotate(patTG, float(M_PI), glm::vec3(0,1,0));
  patTG = glm::scale(patTG,glm::vec3 (escalaPat*2));
  patTG = glm::translate(patTG, -centreBasePat);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);

  ActPosfocus();
}

void NouGLWidget::viewTransform() {
  MyGLWidget::viewTransform();

  glm::mat4 View(1.f);

  if(ortho){
    View = glm::lookAt(obs,vrp,up);
  }
  else{
    View = glm::translate(View,glm::vec3(0,0,-2*radiEsc));
    View = glm::rotate(View,angleX,glm::vec3(1,0,0));
    View = glm::rotate(View,-angleY,glm::vec3(0,1,0));
    View = glm::translate(View,-centreEsc);
    ActPosfocus();
  }

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  
}

void NouGLWidget::projectTransform() {
  MyGLWidget::projectTransform();

  glm::mat4 Proj;

  Proj = glm::perspective(fov, rav, zn, zf);

  if (ortho) {
      if (rav > 1) Proj = glm::ortho (-radiEsc*rav, radiEsc*rav, -radiEsc, radiEsc, zn, zf);
      else Proj = glm::ortho (-radiEsc, radiEsc, -radiEsc/rav, radiEsc/rav, zn, zf);
    }

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_W: {

      posPil.z -= 0.5;

      if(posPil.z >= 0 and posPil.z <= 7) gol = false;
      else gol = true;
      sliderKeySet(70-posPil.z*10);
      break;
    }
    case Qt::Key_S: {

      if(posPil.z+ 0.5 <= 7.0) posPil.z += 0.5;

      if(posPil.z >= 0 and posPil.z <= 7) gol = false;
      else gol = true;
      sliderKeySet(70-posPil.z*10);
      break;
    }
    case Qt::Key_C: {

      ortho = !ortho;
      break;
    }
    default: {
      MyGLWidget::keyPressEvent(event);
      break;
    }
  }

  projectTransform();
  viewTransform();
  ActPosfocus();
  update();
}

void NouGLWidget::setCam()
{
  makeCurrent();
  ortho = !ortho;
  projectTransform();
  viewTransform();
  ActPosfocus();
  update();
}

void NouGLWidget::slidercamSet(int zpil)
{
  makeCurrent();
  zpil = 70 - zpil;
  posPil.z = (float)zpil / 10;
  if(posPil.z >= 0 and posPil.z <= 7) gol = false;
  else gol = true;

  projectTransform();
  viewTransform();
  ActPosfocus();
  update();
}

  
