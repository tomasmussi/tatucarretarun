#include "ElementoEnlazable.h"

using namespace std;

ElementoEnlazable::ElementoEnlazable(const std::string & nombre,
		Coordenada posicion, const ulong magnitud, long angulo, Elemento * poseedor) :
	ElementoCircular(nombre, posicion, magnitud, angulo) {
	this->poseedor = poseedor;
	enlazado = false;
}

ElementoEnlazable::~ElementoEnlazable() {

}


Elemento * ElementoEnlazable::getPoseedor() const {
	return poseedor;
}



list <ElementoEnlazable*> ElementoEnlazable::getListaEnlaces() const {
	return _enlaces;
}

void ElementoEnlazable::eliminarEnlaces() {
	list <ElementoEnlazable*>::iterator it;
	for(it = _enlaces.begin(); it != _enlaces.end(); it++) {
		delete (*it);
	}
	_enlaces.clear();
}

bool ElementoEnlazable::conectarA(ElementoEnlazable* enganche) {
	if(enganche != this)
		if((this->getPosicion().x == enganche->getPosicion().x) && (this->getPosicion().y == enganche->getPosicion().y)) {
			_enlaces.push_back(enganche);
			return true;
		}
		
	return false;
}

