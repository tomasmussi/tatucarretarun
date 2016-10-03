#ifndef INTERFAZEDICION_H_
#define INTERFAZEDICION_H_


#include <iostream>
#include "Angulo.h"
#include <string>
#include "Mapa.h"
#include "Masa.h"
#include "Barra.h"
#include "Ventana.h"
#include "PanelContenedor.h"
#include "Boton.h"
#include "InterfazMapa.h"
#include "constantes.h"
#include "Elemento.h"

/* Eventos sobre el mapa */
void mapa_mouseDown(Coordenada punto, InterfazMapa *);
void mapa_mouseOver(Coordenada punto, InterfazMapa *);
void mapa_mouseUp(Coordenada punto, InterfazMapa *);
/* Eventos de la botonera */
void botonMasa_mouseDown(Coordenada cord, Boton *);
void botonRueda_mouseDown(Coordenada cord, Boton *);
void botonPuntoFijo_mouseDown(Coordenada cord, Boton * boton);
void botonBarra_mouseDown(Coordenada cord, Boton *);
void botonPlataforma_mouseDown(Coordenada cord, Boton * boton);
void botonSoga_mouseDown(Coordenada cord, Boton * boton);
void botonCinta_mouseDown(Coordenada cord, Boton * boton);
void botonZonaLlegada_mouseDown(Coordenada cord, Boton * boton);
void botonCohete_mouseDown(Coordenada cord, Boton * boton);
void botonBorrarTodo_mouseDown(Coordenada cord, Boton * boton);
void botonBorrarElemento_mouseDown(Coordenada cord, Boton * boton);
void botonPlay_mouseDown(Coordenada cord, Boton * boton);
void botonSalir_mouseDown(Coordenada cord, Boton * boton);
void botonSms_mouseDown(Coordenada cord, Boton * boton);
void botonGuardar_mouseDown(Coordenada cord, Boton * boton);

class InterfazEdicion {

private:
	/* Referencia al modelo del mapa sobre el cual editara */
	Mapa * mapa;
	/* Agrega los botones */
	void agregarBotoneraInferior(Ventana *);

public:
	InterfazEdicion(Mapa*);
	~InterfazEdicion();
	void comenzar();

};

#endif /*INTERFAZEDICION_H_*/

