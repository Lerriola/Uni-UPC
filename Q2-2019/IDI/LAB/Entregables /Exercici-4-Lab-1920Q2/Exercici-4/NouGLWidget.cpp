#include "NouGLWidget.h"
#include <iostream>

NouGLWidget::NouGLWidget(QWidget *parent) : MyGLWidget(parent) {
  grabKeyboard();
}

NouGLWidget::~NouGLWidget ()
{
}

void NouGLWidget::initializeGL() {
  MyGLWidget::initializeGL();
  CarregaShadersFocus();
  iniFocus();
}

void NouGLWidget::modelTransformLego ()
{
  MyGLWidget::modelTransformLego();
  actPosFocus();
}

void NouGLWidget::viewTransform() {
  MyGLWidget::viewTransform();
}

void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: 
  {
    posFocus.x -= 0.2;
    if(posFocus.x <= 0) posFocus.x = 0;
    break;
  }
  case Qt::Key_D: 
  {
    posFocus.x += 0.2;
    if(posFocus.x >= 6) posFocus.x = 6;
    break;
	}
  case Qt::Key_F: 
  {
    TypeFocus = !TypeFocus;
    break;
	}
  default: {
    MyGLWidget::keyPressEvent(event);
    break;
	 }
  }
  actPosFocus();
  update();
}

void NouGLWidget::iniFocus ()
{
  posFocus = glm::vec4(3.0,3.8,0.5,1.0);
  colFocus = glm::vec3(1.0, 1.0, 1.0);

  glm::vec4 posFocusF = MyGLWidget::View * posFocus;  // SCA -> SCO

  TypeFocus = true;

  sliderKeySet((int)posFocus.x*10);
  glUniform3fv (colFocusLoc, 1, &colFocus[0]);
  glUniform4fv (posFocusLoc, 1, &posFocusF[0]);
}

void NouGLWidget::actPosFocus()
{
  glm::vec4 posFocusF;

  if(TypeFocus)
  {
    posFocusF = MyGLWidget::View * posFocus;  // SCA -> SCO
    colFocus = glm::vec3(1.0, 1.0, 1.0);
  }
  else
  {
    posFocusF = MyGLWidget::View * MyGLWidget::legoTG * glm::vec4(1.85,20.0,7.3,1.0);  // SCM -> SCO
    colFocus = glm::vec3(0.5,1.0,0.0);  
  }
  sliderKeySet(posFocus.x*10);
 
  glUniform4fv (posFocusLoc, 1, &posFocusF[0]);
  glUniform3fv (colFocusLoc, 1, &colFocus[0]);
}

void NouGLWidget::CarregaShadersFocus()
{
  posFocusLoc = glGetUniformLocation (program->programId(),"posFocusF");
  colFocusLoc = glGetUniformLocation (program->programId(),"colFocus");
}

void NouGLWidget::sliderFocusSet(int xfocus)
{
  makeCurrent();
  posFocus.x = (float)xfocus / 10;
  actPosFocus();
  update();
}
