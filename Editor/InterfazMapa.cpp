#include "InterfazMapa.h"
#include <iostream>
#include <list>
#include "Elemento.h"
#include "Barra.h"
#include "Masa.h"
#include "Rueda.h"
#include <list>
#include "PuntoDeEnlace.h"

using namespace std;

InterfazMapa::InterfazMapa(const string & rutaImgDefecto,
		const string & rutaImgClickDown, Mapa * mapa) :
	PanelContenedor("Mapa", rutaImgDefecto, rutaImgDefecto, rutaImgDefecto) {
	this->mapa = mapa;
	pf_rutinaMouseDown = NULL;
	pf_rutinaMouseOver = NULL;
	pf_rutinaMouseUp = NULL;
	

	loadTexture("./ima/mapa/ball.bmp", TEXTURA_MASA);
	loadTexture("./ima/mapa/punto_fijo.bmp", TEXTURA_PUNTO_FIJO);
	loadTexture("./ima/mapa/lona.bmp", TEXTURA_CINTA);
	loadTexture("./ima/mapa/rope.bmp", TEXTURA_SOGA);
	loadTexture("./ima/mapa/metal.bmp", TEXTURA_BARRA);
	loadTexture("./ima/mapa/rocket.bmp", TEXTURA_COHETE);
	loadTexture("./ima/mapa/platform.bmp", TEXTURA_PLATAFORMA);
	loadTexture("./ima/mapa/rocket.bmp", TEXTURA_RUEDA);
	loadTexture("./ima/mapa/llegada.bmp", TEXTURA_ZONA_LLEGADA);
	

}

InterfazMapa::InterfazMapa(const string & rutaImgDefecto,
		const string & rutaImgClickDown, Mapa * mapa, double zoom) :
	PanelContenedor("Mapa", rutaImgDefecto, rutaImgDefecto, rutaImgDefecto, zoom) {
	this->mapa = mapa;
	pf_rutinaMouseDown = NULL;
	pf_rutinaMouseOver = NULL;
	pf_rutinaMouseUp = NULL;
	

	loadTexture("./ima/mapa/ball.bmp", TEXTURA_MASA);
	loadTexture("./ima/mapa/punto_fijo.bmp", TEXTURA_PUNTO_FIJO);
	loadTexture("./ima/mapa/lona.bmp", TEXTURA_CINTA);
	loadTexture("./ima/mapa/rope.bmp", TEXTURA_SOGA);
	loadTexture("./ima/mapa/metal.bmp", TEXTURA_BARRA);
	loadTexture("./ima/mapa/rocket.bmp", TEXTURA_COHETE);
	loadTexture("./ima/mapa/platform.bmp", TEXTURA_PLATAFORMA);
	loadTexture("./ima/mapa/rocket.bmp", TEXTURA_RUEDA);
	loadTexture("./ima/mapa/llegada.bmp", TEXTURA_ZONA_LLEGADA);
	

}

InterfazMapa::~InterfazMapa() {

}

void InterfazMapa::evento_MouseDown(Coordenada punto) {
	PanelContenedor::evento_MouseDown(punto);
	if (pf_rutinaMouseDown != NULL)
		pf_rutinaMouseDown(punto, this);
}

void InterfazMapa::evento_MouseUp(Coordenada punto) {
	PanelContenedor::evento_MouseUp(punto);
	if (pf_rutinaMouseUp != NULL)
		pf_rutinaMouseUp(punto, this);

}

void InterfazMapa::evento_MouseOver(Coordenada punto) {
	PanelContenedor::evento_MouseOver(punto);
	if (pf_rutinaMouseOver != NULL)
		pf_rutinaMouseOver(punto, this);
}

void InterfazMapa::setRutinaMouseDown(void (*rutina)(Coordenada, InterfazMapa *)) {
	pf_rutinaMouseDown = rutina;
}

void InterfazMapa::setRutinaMouseUp(void (*rutina)(Coordenada, InterfazMapa *)) {
	pf_rutinaMouseUp = rutina;
}

void InterfazMapa::setRutinaMouseOver(void (*rutina)(Coordenada, InterfazMapa *)) {
	pf_rutinaMouseOver = rutina;
}

Mapa * InterfazMapa::getMapa() const {
	return mapa;
}

void InterfazMapa::setMapa(Mapa* mapa)
{
	this->mapa = mapa;
}

void InterfazMapa::imprimir() {
	PanelContenedor::imprimir();

	list<Elemento*> lista = this->mapa->getListaElementos();

	list<Elemento*>:: iterator it;

	for (it = lista.begin(); it != lista.end(); it++) {
		if ((*it)->es(CIRCULAR))
			imprimirElementoCircular( (ElementoCircular*) (*it) );

		else if ((*it)->es(LONGITUDINAL))
			imprimirElementoLongitudinal( (ElementoLongitudinal*) (*it) );
		else if((*it)->es(ZONA_LLEGADA))
			imprimirZonaLlegada( (Llegada*) (*it) );

	}

}

void InterfazMapa::imprimirElementoCircular(ElementoCircular * circular) {
	ulong radio = circular->getMagnitud();
	int textura= TEXTURA_MASA;
	
	if (circular->es(MASA))
		textura = TEXTURA_MASA;
	if (circular->es(PUNTO_FIJO)) {
		textura = TEXTURA_PUNTO_FIJO;	
		radio = 4;
	}
	if (circular->es(PUNTO_ENLACE)) {
		textura = TEXTURA_PUNTO_FIJO;	
		radio = 3;
	}
	if (circular->es(RUEDA))
		textura = TEXTURA_RUEDA; 

	glBindTexture(GL_TEXTURE_2D, _texturas[textura]);

	glBegin(GL_TRIANGLE_FAN);

	double x, y;
	x =cambioX(circular->getPosicion().x) + getPosicion().x/TX;
	y = cambioY(circular->getPosicion().y) - getPosicion().y/TY;

	for (int j = 0; j <= POLY_SIDES; j++) {

		glTexCoord2i( 0, 0);
		glVertex3f(x, y, 0.0f);
		glTexCoord2i( 1, 0);
		glVertex3f(x, y, 0.0f);

		glTexCoord2i( 1, 1);
		glVertex3f(x + radio/TX * cos(j * (2 * PI
				/ POLY_SIDES)), y + radio/TY* sin(j * (2
				* PI / POLY_SIDES)), 0.0f);

		j++;
		glTexCoord2i( 0, 1);
		glVertex3f(x + radio/TX * cos(j * (2 * PI
				/ POLY_SIDES)), y + radio/TY * sin(j * (2
				* PI / POLY_SIDES)), 0.0f);
		j--;

	}

	glEnd();
	
	if (circular->es(RUEDA) && circular->getMostrarPuntosDeEnlace()) {
		list <PuntoDeEnlace*> _enlaces = ((Rueda*)circular)->getListaEnlaces();
		list <PuntoDeEnlace*>::iterator it;
		for(it = _enlaces.begin(); it != _enlaces.end() ; it++) {
			imprimirElementoCircular(*it);
		}
	}
		

}

void InterfazMapa::imprimirElementoLongitudinal(ElementoLongitudinal * elem) {
	int punta = 0;
	int textura= TEXTURA_MASA;
	if (elem->es(BARRA))
		textura = TEXTURA_BARRA; 
	else if (elem->es(CINTA))
		textura = TEXTURA_CINTA; 
	else if (elem->es(SOGA))
		textura = TEXTURA_SOGA; 
	else if (elem->es(COHETE)) {
		textura = TEXTURA_COHETE; 
		punta = 10;
	}
	else if (elem->es(PLATAFORMA))
		textura = TEXTURA_PLATAFORMA; 
	
	
	Coordenada extrOpPrima = elem->getExtremoOpuesto() - 
	Coordenada((long) (punta * cos(Angulo::aRadianes(elem->getAngulo()))) , -(long)( punta * sin(Angulo::aRadianes(elem->getAngulo()))));
	


	double aX = cambioX(elem->getPosicion().x) - (elem->getAncho()/(2.0*TX)) * cos(Angulo::aRadianes(90-elem->getAngulo()) );
	double aY = cambioY(elem->getPosicion().y) + (elem->getAncho()/(2.0*TY)) * sin(Angulo::aRadianes(90-elem->getAngulo()) );
	
	double bX = cambioX(extrOpPrima.x) - 
		(elem->getAncho()/(2.0*TX)) * cos(Angulo::aRadianes(90-elem->getAngulo()) );
		
	double bY = cambioY(extrOpPrima.y) +
		(elem->getAncho()/(2.0*TY)) * sin(Angulo::aRadianes(90-elem->getAngulo()) );
		
	double cX = (elem->getAncho()/TX) *
		sin(Angulo::aRadianes(elem->getAngulo())) + aX;
	double cY = aY - (elem->getAncho()/TY) *
		cos(Angulo::aRadianes(elem->getAngulo()));
	
	double dX = cambioX(extrOpPrima.x) + 
		(elem->getAncho()/(2.0*TX)) * sin(Angulo::aRadianes(elem->getAngulo()) );
	
	double dY = cambioY(extrOpPrima.y) - 
		(elem->getAncho()/(2.0*TX)) * cos(Angulo::aRadianes(elem->getAngulo()) );

	aX += getPosicion().x/TX;
	bX += getPosicion().x/TX;
	cX += getPosicion().x/TX;
	dX += getPosicion().x/TX;
	
	
	aY -= getPosicion().y/TY;
	bY -= getPosicion().y/TY;
	cY -= getPosicion().y/TY;
	dY -= getPosicion().y/TY;
	
	glBindTexture(GL_TEXTURE_2D, _texturas[textura]);
	glBegin(GL_QUADS);

	glTexCoord2i( 0, 0);
	glVertex3f(cX, cY, 0.0f);
	glTexCoord2i( 1, 0);
	glVertex3f(aX, aY, 0.0f);
	glTexCoord2i( 1, 1);
	glVertex3f(bX, bY, 0.0f);
	glTexCoord2i( 0, 1);
	glVertex3f(dX, dY, 0.0f);

	glEnd();
	
if (elem->es(COHETE)) {
	glBegin(GL_QUADS);

	glTexCoord2i( 0, 0);
	glVertex3f(dX, dY, 0.0f);
	glTexCoord2i( 1, 0);
	glVertex3f(bX, bY, 0.0f);
	glTexCoord2i( 1, 1);
	glVertex3f(cambioX(elem->getPosicion().x + elem->getMagnitud() * cos(-Angulo::aRadianes(elem->getAngulo())) + getPosicion().x), cambioY(elem->getPosicion().y + elem->getMagnitud() * sin(-Angulo::aRadianes(elem->getAngulo())) + getPosicion().y), 0.0f);
	glTexCoord2i( 0, 1);
	glVertex3f(cambioX(elem->getPosicion().x + elem->getMagnitud() * cos(-Angulo::aRadianes(elem->getAngulo())) + getPosicion().x), cambioY(elem->getPosicion().y + elem->getMagnitud() * sin(-Angulo::aRadianes(elem->getAngulo())) + getPosicion().y), 0.0f);
	
	glEnd();
}

	if(elem->getMostrarPuntosDeEnlace() ) {
		list <PuntoDeEnlace*> _enlaces = elem->getListaEnlaces();
		list <PuntoDeEnlace*>::iterator it;
		for(it = _enlaces.begin(); it != _enlaces.end() ; it++) {
			imprimirElementoCircular(*it);
		}
	}
	
}


void InterfazMapa::imprimirZonaLlegada(Llegada * llegada) {
	glColor4d(1.0, 1.0, 4.0, 0.4);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture( GL_TEXTURE_2D, _texturas[TEXTURA_ZONA_LLEGADA]);
	glBegin(GL_QUADS);

	glTexCoord2i( 0, 0);
	glVertex3f(cambioX(llegada->getPosicion().x + getPosicion().x), cambioY(llegada->getPosicion().y + getPosicion().y), 0.0f);
	glTexCoord2i( 1, 0);
	glVertex3f(cambioX(llegada->getPosicion().x + getPosicion().x), cambioY(llegada->getPosicion().y  + getPosicion().y + (long)llegada->getMagnitud()), 0.0f);
	glTexCoord2i( 1, 1);
	glVertex3f(cambioX(llegada->getPosicion().x + getPosicion().x + (long)llegada->getMagnitud()), cambioY(llegada->getPosicion().y + getPosicion().y + (long)llegada->getMagnitud()), 0.0f);
	glTexCoord2i( 0, 1);
	glVertex3f(cambioX(llegada->getPosicion().x + getPosicion().x + (long)llegada->getMagnitud()), cambioY(llegada->getPosicion().y + getPosicion().y), 0.0f);

	glEnd();
	glColor4d(1.0, 1.0, 1.0, 1.0);

}

