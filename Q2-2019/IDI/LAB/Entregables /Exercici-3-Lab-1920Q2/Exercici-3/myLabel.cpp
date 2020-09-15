#include "myLabel.h"

myLabel::myLabel(QWidget *parent):QLabel(parent) {}

void myLabel::ChangeStat()
{
	status = !status;

	if(status){
		setStyleSheet("background-color: green");
		setText("Funcionant...");
	}
	else {
		setStyleSheet("background-color: red");
		moves = 0;
		emit(enviamoves(moves));
		setText("Apagat");
	}
}
void myLabel::IncUp(){

	++moves;
	setText("Endavant");
	if(moves >= 10) setStyleSheet("background-color: yellow");
	if (moves == 15) emit(SwitchOff());
	emit(enviamoves(moves));
}

void myLabel::IncDown(){

	++moves;
	setText("Enrera");
	if(moves >= 10) setStyleSheet("background-color: yellow");
	if (moves == 15) emit(SwitchOff());
	emit(enviamoves(moves));
}

void myLabel::IncLeft(){

	++moves;
	setText("Esquerra");
	if(moves >= 10) setStyleSheet("background-color: yellow");
	if (moves == 15) emit(SwitchOff());
	emit(enviamoves(moves));
}

void myLabel::IncRight(){

	++moves;
	setText("Dreta");
	if(moves >= 10) setStyleSheet("background-color: yellow");
	if (moves == 15) emit(SwitchOff());
	emit(enviamoves(moves));
}

void myLabel::Gir(){

	setText("Gir");
}
