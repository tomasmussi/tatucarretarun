#include "zonaLlegada.h"
		
ZonaLlegada::ZonaLlegada(double x, double y , double width, double heigth){
	this->x = x;
	this->y = y;
	this->width = width;
	this->heigth = heigth;
}
double ZonaLlegada::getX(){
	return x;
}
double ZonaLlegada::getY(){
	return y;
}
double ZonaLlegada::getWidth(){
	return width;
}
double ZonaLlegada::getHeigth(){
	return heigth;
}
