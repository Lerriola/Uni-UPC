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

  glBindVertexArray (VAO_Pat);
  // pintem el Patricio 2
  modelTransformPatricio2();
  glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);

  glBindVertexArray(0);
}

void NouGLWidget::iniCamera ()
{
  MyGLWidget::iniCamera();

  angleY = 0.0;
  angleX = float(M_PI/6.0);

  obs2 = glm::vec3(posPat2.x,3.5,posPat2.z);
  vrp2 = obs2 + glm::vec3(0,0,-2*radiEsc);
  up2 = glm::vec3(0,1,0);
  fov2 = float(M_PI/2.0);
  zn2 = 0.5;
  zf2 = 3.0*radiEsc;

  fov = 2.0 * asin(0.5f);
  zn = radiEsc;
  zf = 3.0*radiEsc;

  FPatricio = false;

  projectTransform ();
  viewTransform ();
}

void NouGLWidget::iniEscena ()
{
  MyGLWidget::iniEscena();

  centreEsc = glm::vec3(4,2,4);
  radiEsc = 6;
  posPat2 = glm::vec3(4,0,6);
}

void NouGLWidget::ActPosfocus()
{
  posFocus = glm::vec3(0, 0, 0);
  glUniform3fv (posFocusLoc, 1, &posFocus[0]);
}

void NouGLWidget::modelTransformPatricio ()
{
  MyGLWidget::modelTransformPatricio();

  glm::mat4 patTG = glm::mat4(1.0f);

  patTG = glm::translate(patTG,glm::vec3(2,0,1));
  patTG = glm::rotate(patTG, float(M_PI/2.0) , glm::vec3(0,1,0));
  patTG = glm::scale(patTG,glm::vec3 (escalaPat*2));
  patTG = glm::translate(patTG, -centreBasePat);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}

void NouGLWidget::modelTransformPatricio2 ()
{
  MyGLWidget::modelTransformPatricio();

  glm::mat4 patTG = glm::mat4(1.0f);
  
  patTG = glm::translate(patTG,posPat2);
  patTG = glm::rotate(patTG, float(M_PI) , glm::vec3(0,1,0));
  patTG = glm::scale(patTG,glm::vec3 (escalaPat*3));
  patTG = glm::translate(patTG, -centreBasePat);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}

void NouGLWidget::viewTransform() {
  MyGLWidget::viewTransform();

  glm::mat4 View(1.f);

  if(FPatricio){
    obs2 = glm::vec3(posPat2.x,2.5,posPat2.z);
    vrp2 = obs2 + glm::vec3(0,0,-2*radiEsc);
    View = glm::lookAt(obs2,vrp2,up2);
  }
  else
  {
    View = glm::translate(View,glm::vec3(0,0,-2*radiEsc));
    View = glm::rotate(View,angleX,glm::vec3(1,0,0));
    View = glm::rotate(View,-angleY,glm::vec3(0,1,0));
    View = glm::translate(View,-centreEsc);
  }

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void NouGLWidget::projectTransform() {
  MyGLWidget::projectTransform();

  glm::mat4 Proj;
  if (rav < 1){
    fov = 2.0 * atan(tan(asin(0.5f))/rav);
    fov2 = 2.0 * atan(tan(asin(float(sqrt(2.0)/2.0)))/rav);
  } 

  if(FPatricio) Proj = glm::perspective(fov2, rav, zn2, zf2);
  else Proj = glm::perspective(fov, rav, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

bool NouGLWidget::validPos(glm::vec3 pos){

  if(pos.x - 1.5 < 0 ) return false;
  else if(pos.x + 1.5 > 8 ) return false;
  else if(pos.z + 0.55 > 8) return false;
  else if(pos.z - 0.55 < 0) return false;
  else return true;
}

void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: {

      if(validPos(glm::vec3(posPat2.x,posPat2.y,posPat2.z+0.5))){
        posPat2.z += 0.5;
      }
      else posPat2.z = 7.45;

      break;
    }
    case Qt::Key_W: {

      if(validPos(glm::vec3(posPat2.x,posPat2.y,posPat2.z-0.5))){
        posPat2.z -= 0.5;
      }
      else posPat2.z = 0.55;

      break;
    }
    case Qt::Key_A: {

      if(validPos(glm::vec3(posPat2.x-0.5,posPat2.y,posPat2.z))){
        posPat2.x -= 0.5;
      }
      else posPat2.x = 1.5;

      break;
    }
    case Qt::Key_D: {

      if(validPos(glm::vec3(posPat2.x+0.5,posPat2.y,posPat2.z))){
        posPat2.x += 0.5;
      }
      else posPat2.x = 6.5;

      break;
    }
    case Qt::Key_C: {

      FPatricio = !FPatricio;
      if(FPatricio)TeclaSet("Primera Persona");
      else TeclaSet("Tercera Persona");     
      break;
    }
    default: {
      MyGLWidget::keyPressEvent(event);
      break;
    }
  }

  projectTransform();
  viewTransform();
  update();
}


void NouGLWidget::SetFocus()
{
  makeCurrent();
  FPatricio = !FPatricio;
  if(FPatricio)TeclaSet("Primera Persona");
  else TeclaSet("Tercera Persona");   
  projectTransform();
  viewTransform();
  update();
}
