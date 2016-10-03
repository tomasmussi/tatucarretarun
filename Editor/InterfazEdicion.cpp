#include "InterfazEdicion.h"
#include "sms_app.h"

#include "ParserXML.h"

using namespace std;


InterfazEdicion::InterfazEdicion(Mapa* mapa) {
	this->mapa = mapa;
}

InterfazEdicion::~InterfazEdicion() {

}


void InterfazEdicion::comenzar() {

	Ventana miVentana = Ventana(ANCHO_VENTANA, ALTO_VENTANA);

	agregarBotoneraInferior(&miVentana);	

	string rutaFondo = "./ima/mapa/fondoBasico.bmp";

	InterfazMapa * interfazMapa = new InterfazMapa(rutaFondo, rutaFondo, mapa);
	interfazMapa->setAltura(mapa->getAlto());
	interfazMapa->setAncho(mapa->getAncho());
	interfazMapa->setRutinaMouseDown(mapa_mouseDown);
	interfazMapa->setRutinaMouseOver(mapa_mouseOver);
	interfazMapa->setRutinaMouseUp(mapa_mouseUp);
	miVentana.agregarPanel(interfazMapa, Coordenada((long)0, (long)0));

	miVentana.ejecutar();
	
}

void InterfazEdicion::agregarBotoneraInferior(Ventana * miVentana) {
	
	/* Boton Masa */
	Boton * botonMasa = new Boton(MASA, mapa);
	botonMasa->setRutinaMouseDown(botonMasa_mouseDown);
	miVentana->agregarPanel(botonMasa, 0);
	
	/* Boton Rueda */
	Boton * botonRueda = new Boton(RUEDA, mapa);
	botonRueda->setRutinaMouseDown(botonRueda_mouseDown);
	miVentana->agregarPanel(botonRueda, 0);
	
	/* Boton PuntoFijo */
	Boton * botonPuntoFijo = new Boton(PUNTO_FIJO, mapa);
	botonPuntoFijo->setRutinaMouseDown(botonPuntoFijo_mouseDown);
	miVentana->agregarPanel(botonPuntoFijo, 0);
	
	/* Boton Barra */
	Boton * botonBarra = new Boton(BARRA, mapa);
	botonBarra->setRutinaMouseDown(botonBarra_mouseDown);
	miVentana->agregarPanel(botonBarra, 0);
	
	/* Boton PLataforma */
	Boton * botonPLataforma = new Boton(PLATAFORMA, mapa);
	botonPLataforma->setRutinaMouseDown(botonPlataforma_mouseDown);
	miVentana->agregarPanel(botonPLataforma, 0);
	
	/* Boton Soga */
	Boton * botonSoga = new Boton(SOGA, mapa);
	botonSoga->setRutinaMouseDown(botonSoga_mouseDown);
	miVentana->agregarPanel(botonSoga, 0);
	
	/* Boton Cinta */
	Boton * botonCinta = new Boton(CINTA, mapa);
	botonCinta->setRutinaMouseDown(botonCinta_mouseDown);
	miVentana->agregarPanel(botonCinta, 0);
	
	/* Boton ZonaLlegada */
	Boton * botonZonaLlegada = new Boton(ZONA_LLEGADA, mapa);
	botonZonaLlegada->setRutinaMouseDown(botonZonaLlegada_mouseDown);
	miVentana->agregarPanel(botonZonaLlegada, 0);
	
	
	/* Boton Cohete */
	Boton * botonCohete = new Boton(COHETE, mapa);
	botonCohete->setRutinaMouseDown(botonCohete_mouseDown);
	miVentana->agregarPanel(botonCohete, 0);
	
	/* Boton Borrar Elemento */
	Boton * botonBorrarElemento = new Boton(BORRAR_ELEMENTO, mapa);
	botonBorrarElemento->setRutinaMouseDown(botonBorrarElemento_mouseDown);
	botonBorrarElemento->setAltura(32);
	botonBorrarElemento->setAncho(32);
	miVentana->agregarPanel(botonBorrarElemento, 0);
	
	/* Boton Borrar Todo */
	Boton * botonBorrarTodo = new Boton(BORRAR_TODO, mapa);
	botonBorrarTodo->setRutinaMouseDown(botonBorrarTodo_mouseDown);
	botonBorrarTodo->setAltura(32);
	botonBorrarTodo->setAncho(32);
	miVentana->agregarPanel(botonBorrarTodo, 0);
	
	/* Boton Void */
	Boton * botonVoid = new Boton(VOID,mapa);
	botonVoid->setRutinaMouseDown(NULL);
	botonVoid->setAltura(32);
	botonVoid->setAncho(32);
	miVentana->agregarPanel(botonVoid, 0);
	
	
	/* Boton Guardar */
	Boton * botonGuardar = new Boton(GUARDAR, mapa); 
	botonGuardar->setRutinaMouseDown(botonGuardar_mouseDown);
	botonGuardar->setAltura(32);
	botonGuardar->setAncho(32);
	miVentana->agregarPanel(botonGuardar, 0);
	
	/* Boton PLAY */
	Boton * botonPlay = new Boton(PLAY, mapa);
	botonPlay->setRutinaMouseDown(botonPlay_mouseDown);
	botonPlay->setAltura(32);
	botonPlay->setAncho(32);
	miVentana->agregarPanel(botonPlay, 0);
		
	/* Boton SMS */
	Boton * botonSms = new Boton(SMS, mapa); 
	botonSms->setRutinaMouseDown(botonSms_mouseDown);
	botonSms->setAltura(32);
	botonSms->setAncho(32);
	miVentana->agregarPanel(botonSms, 0);
	
	/* Boton Salir */
	Boton * botonSalir = new Boton(SALIR, mapa); 
	botonSalir->setRutinaMouseDown(botonSalir_mouseDown);
	botonSalir->setAltura(32);
	botonSalir->setAncho(32);
	miVentana->agregarPanel(botonSalir, 0);

}


/* EVENTOS MAPA */


void mapa_mouseDown(Coordenada punto, InterfazMapa * interfaz) {
	Mapa * mapa = interfaz->getMapa();
	Coordenada puntoCorregido;
	const Elemento * elemento;

	if((mapa->entornoDePuntoDeEnlace(punto, &puntoCorregido) == true)) {
		if(mapa->getNroElementoParaCrear() > 2 ) { // si es longitudinal lo q se va  crear
			mapa->agregarElemento(puntoCorregido);
			if((elemento = mapa->getElementoSeleccionado()) == NULL) return;
			if(elemento->es(LONGITUDINAL))
				mapa->setTipoSeleccion(ELEMENTO_SELECCIONADO_POR_EXTREMO2);
			return;
		}
	}
	
	
	int resul = mapa->seleccionarElemento(punto);
	elemento = mapa->getElementoSeleccionado();
	if(elemento != NULL && elemento->estaBloqueado()) { 
		mapa->setTipoSeleccion(ELEMENTO_NO_SELECCIONADO); 
		return;
	}
	switch (resul) {
	
	case ELEMENTO_NO_SELECCIONADO:
		mapa->agregarElemento(punto);
		elemento = mapa->getElementoSeleccionado();
		mapa->setMostrarPuntosDeEnlaceElemento(false);
		if(elemento == NULL) { 
			interfaz->vectorDiferencia = punto - interfaz->getPosicion();
			return;
		}
		else if(elemento->es(LONGITUDINAL))
			mapa->setTipoSeleccion(ELEMENTO_SELECCIONADO_POR_EXTREMO2);
		else if(elemento->es(CIRCULAR)) {
			mapa->setTipoSeleccion(ELEMENTO_SELECCIONADO_POR_CENTRO);
			interfaz->vectorDiferencia = punto - elemento->getPosicion();
		}
		break;
	
	case ELEMENTO_SELECCIONADO_POR_CENTRO:	
		if(mapa->elementoABorrar == true) 
			mapa->eliminarElemento();
		else {
			mapa->setMostrarPuntosDeEnlaceElemento(false);
			interfaz->vectorDiferencia = punto - elemento->getPosicion();
		}
		break;	
	case ELEMENTO_SELECCIONADO_POR_EXTREMO2:
		mapa->setMostrarPuntosDeEnlaceElemento(false);
		break;
	}
	
	
}


void mapa_mouseOver(Coordenada click, InterfazMapa * interfaz) {
	Mapa * mapa = interfaz->getMapa();
			
	if(!interfaz->is_clicked() || !mapa->hayElementoSeleccionado()) return;
			
	Coordenada clickCorregido((long)0,(long)0);
	int tipoSeleccion = mapa->getTipoSeleccion();

	bool corregirClick =  mapa->entornoDePuntoDeEnlace(click, &clickCorregido);
	
	Coordenada punto = (corregirClick == true)? clickCorregido : click;
	
	switch (tipoSeleccion) {
	
	case ELEMENTO_SELECCIONADO_POR_CENTRO:
		mapa->moverElementoA(punto - interfaz->vectorDiferencia);
		break;
		
	case ELEMENTO_SELECCIONADO_POR_EXTREMO2: // solo los longitudinales caen aqui
		if(mapa->getElementoSeleccionado()->es(COHETE))
			mapa->actualizarAngulo(punto);
		else
			mapa->actualizarModuloAngulo(punto);
		break;
	
	}
		
}

void mapa_mouseUp(Coordenada click, InterfazMapa * interfaz) {
	Mapa * mapa = interfaz->getMapa();
	if(!mapa->hayElementoSeleccionado()) return;
	mapa->actualizarEnlaces();
	mapa->setMostrarPuntosDeEnlaceElemento(true);
	
}


/* EVENTOS BOTON MASA */

void botonMasa_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;
	
	if(miMapa->getNroElementoParaCrear() != MASA) {
		miMapa->setNroElementoParaCrear(MASA);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
}

/* EVENTOS BOTON RUEDA */

void botonRueda_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;

	if(miMapa->getNroElementoParaCrear() != RUEDA) {
		miMapa->setNroElementoParaCrear(RUEDA);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
}

/* EVENTOS BOTON PUNTOFIJO */

void botonPuntoFijo_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;

	if(miMapa->getNroElementoParaCrear() != PUNTO_FIJO) {
		miMapa->setNroElementoParaCrear(PUNTO_FIJO);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
}


/* EVENTOS BOTON BARRA */

void botonBarra_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;

	if(miMapa->getNroElementoParaCrear() != BARRA) {
		miMapa->setNroElementoParaCrear(BARRA);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
}

/* EVENTOS BOTON PLATAFORMA */

void botonPlataforma_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;

	if(miMapa->getNroElementoParaCrear() != PLATAFORMA) {
		miMapa->setNroElementoParaCrear(PLATAFORMA);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
	
	

}

/* EVENTOS BOTON SOGA */

void botonSoga_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;

	if(miMapa->getNroElementoParaCrear() != SOGA) {
		miMapa->setNroElementoParaCrear(SOGA);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
}

/* EVENTOS BOTON CINTA */

void botonCinta_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;

	if(miMapa->getNroElementoParaCrear() != CINTA) {
		miMapa->setNroElementoParaCrear(CINTA);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
}

/* EVENTOS BOTON ZONA_LLEGADA */

void botonZonaLlegada_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;

	if(miMapa->getNroElementoParaCrear() != ZONA_LLEGADA) {
		miMapa->setNroElementoParaCrear(ZONA_LLEGADA);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}
}

/* EVENTOS BOTON COHETE */

void botonCohete_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	
	miMapa->elementoABorrar = false;
	
	if(miMapa->getNroElementoParaCrear() != COHETE) {
		miMapa->setNroElementoParaCrear(COHETE);
	}else{
		miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	}	
}

/* EVENTOS BOTON BORRAR_TODO */

void botonBorrarTodo_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	miMapa->elementoABorrar = false;
	miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	miMapa->eliminarTodosLosElementos();
	
}

/* EVENTOS BOTON BORRAR_ELEMENTO */

void botonBorrarElemento_mouseDown(Coordenada cord, Boton * boton) {

	Mapa * miMapa = boton->getMapa();
	
	if(miMapa->elementoABorrar == false)
		miMapa->elementoABorrar = true;
	else
		miMapa->elementoABorrar = false;
	miMapa->setNroElementoParaCrear(NINGUN_ELEMENTO);

}

/* EVENTOS BOTON PLAY */

void botonPlay_mouseDown(Coordenada cord, Boton * boton) {
	Mapa * miMapa = boton->getMapa();
	SMSApp app(boton->getSmsState(), miMapa);
	app.run();

	
}

/* EVENTOS BOTON SALIR */

void botonSalir_mouseDown(Coordenada cord, Boton * boton) {
	boton->enviarSenialSalida();
}

/* EVENTOS BOTON SMS */

void botonSms_mouseDown(Coordenada cord, Boton * boton) {
	boton->enviarSenialSms();	
}


/* EVENTOS BOTON GUARDAR */

void botonGuardar_mouseDown(Coordenada cord, Boton * boton) {
	Mapa * miMapa = boton->getMapa();
	if(miMapa->esCorrecto()) {
		boton->getMapa()->setGuardar(true);
		boton->enviarSenialSalida();
	}
}


