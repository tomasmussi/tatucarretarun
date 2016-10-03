#include "Mapa.h"
#include "ElementoEnlazable.h"
#include "ElementoLongitudinal.h"
#include "PuntoDeEnlace.h"
#include <math.h>
#include "Angulo.h"
using namespace std;

Mapa::Mapa() {
	this->ancho = 1024;
	this->alto = 550;
	elemento = NULL;
	nroElementoParaCrear = NINGUN_ELEMENTO;
	tipoSeleccion = ELEMENTO_NO_SELECCIONADO;
	elementoABorrar = false;
	
	for(int i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		elementosHabilitados[i] = true; // todos habilitados por defecto
		precio[i] = 0; // son gratis por default
	}
	
	plata = 100;
	puntos = -1;
}

Mapa::Mapa(ulong ancho, ulong alto) {
	this->ancho = ancho;
	this->alto = alto;
	elemento = NULL;
	nroElementoParaCrear = NINGUN_ELEMENTO;
	tipoSeleccion = ELEMENTO_NO_SELECCIONADO;
	elementoABorrar = false;

	for(int i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		elementosHabilitados[i] = true; // todos habilitados por defecto
		precio[i] = 0; // son gratis por default
	}
	
	plata = 100;
	puntos = -1;
}

Mapa::~Mapa() {
	desbloquearElementos();
	eliminarTodosLosElementos();
}

ulong Mapa::getAncho() const {
	return ancho;
}

ulong Mapa::getAlto() const {
	return alto;
}

void Mapa::setNroElementoParaCrear(int elemento) {
	nroElementoParaCrear = elemento;
	if(elemento== NINGUN_ELEMENTO) elemento = NULL;
}

int Mapa::getNroElementoParaCrear() const {
	return 	nroElementoParaCrear;
}

list <Elemento*> Mapa::getListaElementos() const {
	return _elementos;
}


bool Mapa::agregarElemento(Coordenada posicion) {
	FabricaDeElementos fabrica;
	Elemento * aux;
	aux = fabrica.crear(nroElementoParaCrear, posicion);
	if (aux !=NULL) {
		_elementos.push_back(aux);
		if (nroElementoParaCrear != ZONA_LLEGADA)
			plata -= precio[nroElementoParaCrear];
		elemento = aux;
		return true;
	} else
		return false;
}

size_t Mapa::getCantidadElementos() {
	return _elementos.size();
}

int Mapa::seleccionarElemento(Coordenada punto) {
	list<Elemento*>::iterator it;

	int resultado= ELEMENTO_NO_SELECCIONADO;

	for (it = _elementos.begin(); it != _elementos.end(); it++) 
		if ((resultado = (*it)->seleccionar(punto)) != ELEMENTO_NO_SELECCIONADO) {
			elemento = (*it);
			break;
		}
	

	if(resultado == ELEMENTO_NO_SELECCIONADO) {
		elemento = NULL;
	}

	tipoSeleccion = resultado;
	return resultado;
}

bool Mapa::hayElementoSeleccionado() const {
	return ((elemento != NULL)? true : false);
}

int Mapa::getTipoSeleccion() const {
	return tipoSeleccion;
}

void Mapa::setTipoSeleccion(const int tipo) {
	tipoSeleccion = tipo;
}

const Elemento * Mapa::getElementoSeleccionado() const {
	return elemento;
}

bool Mapa::entornoDePuntoDeEnlace(Coordenada punto, Coordenada * puntoExacto) {
	
	list <ElementoEnlazable*> _enlaces = getTodosLosEnlaces();
	list <ElementoEnlazable*>::iterator it;
	
	for (it = _enlaces.begin(); it != _enlaces.end(); it++) {
		if((*it)->seleccionar(punto) != ELEMENTO_NO_SELECCIONADO) {
			*puntoExacto = (*it)->getPosicion();
			return true;
		}
	}
	
	return false;
}

void Mapa::setExtremoOpuesto(const Coordenada punto) {
	if(elemento != NULL) {
		if(elemento->es(LONGITUDINAL))
			((ElementoLongitudinal*)elemento)->setExtremoOpuesto(punto);
	}
}

Coordenada Mapa::getExtremoOpuesto() const {
	if(elemento != NULL) {
		return ((ElementoLongitudinal*)elemento)->getExtremoOpuesto();
	}
	return Coordenada( (long) -1, (long)-1 );
}


bool Mapa::moverElementoA(const Coordenada posicion) {
	if(elemento != NULL)	
		elemento->setPosicion(posicion);
	return true;
}

bool Mapa::rotarElementoA(long angulo) {
	Angulo::corregir(&angulo);
	if(elemento != NULL)	
		elemento->setAngulo(angulo);
	return true;
}

bool Mapa::agrandarElementoA(long longitud) {
	if(elemento != NULL)
		elemento->setMagnitud(longitud);
	return true;
}

Coordenada Mapa::actualizarModuloAngulo(Coordenada click) {
    if(elemento != NULL) {
	if(  (click - elemento->getPosicion()).modulo < LONGITUD_MINIMA) 
		actualizarAngulo(click);
	else {
		if(elemento->es(BARRA) || elemento->es(PLATAFORMA)) {
			if( ( (click - elemento->getPosicion()).modulo < LONGITUD_MAXIMA)) 
				((ElementoLongitudinal *)elemento)->setExtremoOpuesto(click);
			else 
				actualizarAngulo2(click);
		} else
			((ElementoLongitudinal *)elemento)->setExtremoOpuesto(click);
	} 
	 
    }

    return click;
}

Coordenada Mapa::actualizarAngulo(Coordenada click) {
	if(elemento != NULL) {

		int longitudMinima = LONGITUD_MINIMA;

		if(elemento->es(COHETE))
			longitudMinima = LONGITUD_COHETE;
		Coordenada resta = click - elemento->getPosicion();
		if(resta.angulo > 180)
			resta.setAngulo(-resta.angulo);
		resta.setModulo(longitudMinima);
		((ElementoLongitudinal *)elemento)->setExtremoOpuesto(elemento->getPosicion() + resta);
	}
	return click;
}

Coordenada Mapa::actualizarAngulo2(Coordenada click) {
	if(elemento != NULL) {

		int longitudMinima = LONGITUD_MAXIMA;

		if(elemento->es(COHETE))
			longitudMinima = LONGITUD_COHETE;
		Coordenada resta = click - elemento->getPosicion();
		if(resta.angulo > 180)
			resta.setAngulo(-resta.angulo);
		resta.setModulo(longitudMinima);
		((ElementoLongitudinal *)elemento)->setExtremoOpuesto(elemento->getPosicion() + resta);
	}
	return click;
}


void Mapa::eliminarElemento() {
	if(elemento != NULL) {
		if(!elemento->estaBloqueado()) {
			_elementos.remove(elemento);
			plata += precio[elemento->getTipo()];
			delete elemento;
			elemento = NULL;
		}
	}
}

void Mapa::eliminarTodosLosElementos() {
	list<Elemento*>::iterator it;
	
	for (it = _elementos.begin(); it != _elementos.end(); ) {
		elemento = *it;
		if(!elemento->estaBloqueado()) {
			eliminarElemento();
			it = _elementos.begin();
		}
		else
			it++;
		
	}
	
	nroElementoParaCrear = NINGUN_ELEMENTO;
	elemento = NULL;

}

void Mapa::actualizarEnlaces() {
	list<Elemento*>::iterator it;
	for (it = _elementos.begin(); it != _elementos.end(); it++) {
		if((*it)->es(LONGITUDINAL)) {
			ElementoLongitudinal * ele = (ElementoLongitudinal *) *it;
			ele->actualizarPuntosDeEnlace(1);
		}
		else if( (*it)->es(RUEDA)) {
			((Rueda*)(*it))->actualizarPuntosDeEnlace();
		}
	}
}

list <ElementoEnlazable*> Mapa::getTodosLosEnlaces() {
	list <ElementoEnlazable*> lista;
	
	list<Elemento*>::iterator it;

	for (it = _elementos.begin(); it != _elementos.end(); it++) {
		if( (*it)->es(ENLAZABLE) )
			lista.push_back((ElementoEnlazable*) *it);
		else if( (*it)->es(LONGITUDINAL) ) {
			ElementoLongitudinal * eleLong = (ElementoLongitudinal *) *it;
			list <PuntoDeEnlace*> _puntosDeEnlace = eleLong->getListaEnlaces();
			list <PuntoDeEnlace*>::iterator it2;
			for(it2 = _puntosDeEnlace.begin(); it2 != _puntosDeEnlace.end(); it2++) {
				lista.push_back((ElementoEnlazable*) *it2);
			}
		}
		else if( (*it)->es(RUEDA) ) {
			list <PuntoDeEnlace*> _puntosDeEnlace = ((Rueda*)(*it))->getListaEnlaces();
			list <PuntoDeEnlace*>::iterator it2;
			for(it2 = _puntosDeEnlace.begin(); it2 != _puntosDeEnlace.end(); it2++) {
				lista.push_back((ElementoEnlazable*) *it2);
			}
		}
			
	}
	
	return lista;
}

void Mapa::imprimirTodosLosEnlaces() {
	list <ElementoEnlazable*> lista = getTodosLosEnlaces();
	
	list<ElementoEnlazable*>::iterator it;

	for (it = lista.begin(); it != lista.end(); it++) {
		(*it)->getPosicion().imprimir();
	}
}

void Mapa::imprimir() {
	cout << "PLATA: " << plata << endl;
	list<Elemento*>::iterator it;
	for (it = _elementos.begin(); it != _elementos.end(); it++) {
		(*it)->imprimir();
	}

}

void Mapa::setMostrarPuntosDeEnlaceElemento(const bool b) {
	if(elemento != NULL) {
		elemento->setMostrarPuntosDeEnlace(b);
	}
}

void Mapa::bloquearElementos() {
	list<Elemento*>::iterator it;
	for (it = _elementos.begin(); it != _elementos.end(); it++) {
		(*it)->bloquear();
	}
}

void Mapa::desbloquearElementos() {
	list<Elemento*>::iterator it;
	for (it = _elementos.begin(); it != _elementos.end(); it++) {
		(*it)->desbloquear();
	}
}

void Mapa::deshabilitarElemento(const int nroElemento) {
	elementosHabilitados[nroElemento] = false;
}

void Mapa::habilitarElemento(const int nroElemento) {
	elementosHabilitados[nroElemento] = true;
}

bool Mapa::elementoHabilitado(const int nroElemento) const {
	return elementosHabilitados[nroElemento];
}


void Mapa::setNombre(const string & nombre)  {
	this->nombre = nombre;
}

string Mapa::getNombre() const {
	return nombre;
}

void Mapa::setPlata(const int plata) {
	this->plata = plata;
}

int Mapa::getPlata() const {
	return plata;
}

void Mapa::setPuntos(const int puntos) {
	this->puntos = puntos;
}

int Mapa::getPuntos() const {
	return puntos;
}

void Mapa::setPrecio(int nroElemento, int precio) {
	this->precio[nroElemento] = precio;
}

int Mapa::getPrecio(int nroElemento) const{
	return this->precio[nroElemento];
}

int Mapa::getTiempoCohete() const {
	return tiempoCohete;
}
void Mapa::setTiempoCohete(int tiempo) {
	this->tiempoCohete = tiempo;
}

void Mapa::setTiempoResolucion(const long tiempo) {
	this->tiempoResolucion = tiempo;	
}

long Mapa::getTiempoResolucion() const {
	return tiempoResolucion;
}

int Mapa::getSumaDePrecios() const {
	int suma = 0;
	for(int i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		suma += precio[i];
	}
	return suma;
}

	
