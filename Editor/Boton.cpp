#include "Boton.h"
#include <iostream>
#include "constantes.h"
using namespace std;

Boton::Boton(int nroDeBoton, Mapa * mapa) : PanelContenedor("Boton", getRutaImgDefecto(nroDeBoton), getRutaImgMouseDown(nroDeBoton), getRutaImgMouseOver(nroDeBoton)) {
	pf_rutinaMouseDown = NULL;
	pf_rutinaMouseOver = NULL;
	pf_rutinaMouseUp = NULL;
	this->mapa = mapa;
	presionado = false;
}

Boton::~Boton() {
}

bool Boton::esta_presionado() const {
	return presionado;
}

void Boton::desapretar() {
	presionado = false;
}

void Boton::evento_MouseDown(Coordenada punto) {
	PanelContenedor::evento_MouseDown(punto);
	ventana->desclickearBotones(this->getId());
	if(presionado == false)
		presionado = true;
	else
		presionado = false;
	if(pf_rutinaMouseDown != NULL)
		pf_rutinaMouseDown(punto, this);
}


void Boton::evento_MouseUp(Coordenada punto) {
	PanelContenedor::evento_MouseUp(punto);
		
	if(pf_rutinaMouseUp != NULL)
		pf_rutinaMouseUp(punto, this);
}


void Boton::evento_MouseOver(Coordenada punto) {
	PanelContenedor::evento_MouseOver(punto);
	if(pf_rutinaMouseOver != NULL)
		pf_rutinaMouseOver(punto, this);
}

void Boton::setRutinaMouseDown(void (*rutina)(Coordenada, Boton *)) {
	pf_rutinaMouseDown = rutina; 
}

void Boton::setRutinaMouseUp(void (*rutina)(Coordenada, Boton *)) {
	pf_rutinaMouseUp = rutina; 
}

void Boton::setRutinaMouseOver(void (*rutina)(Coordenada, Boton *)) {
	pf_rutinaMouseOver = rutina;
}

Mapa * Boton::getMapa() const {
	return mapa;
}

void Boton::enviarSenialSalida() {
	ventana->salir = true;
}

void Boton::enviarSenialSms() {
	if(!ventana->sms)	
		ventana->sms = true;
	else 
		ventana->sms = false;
}

bool Boton::getSmsState(){
	return ventana->sms;
}

string Boton::getRutaImgDefecto(int nroDeBoton) {
	switch(nroDeBoton) {
		case MASA: return "./ima/botones/icon_level_off.png";
		case RUEDA: return "./ima/botones/PlataformaMetalica_Defecto.bmp";
		case PUNTO_FIJO: return "./ima/botones/icon_node_off.png"; 
		case BARRA: return "./ima/botones/icon_metal_bar_off.png";
		case PLATAFORMA: return "./ima/botones/icon_metal_sheet_off.png";
		case SOGA: return "./ima/botones/icon_elastic_off.png";
		case CINTA: return "./ima/botones/icon_cloth_off.png"; 
		case COHETE: return "./ima/botones/icon_rocket_off.png";
		case BORRAR_TODO: return "./ima/botones/icon_clear_off.png";
		case BORRAR_ELEMENTO: return "./ima/botones/icon_rubber_off.png";
		case PLAY: return "./ima/botones/icon_start_off.png";
		case SALIR: return "./ima/botones/icon_salir_off.png";
		case SMS: return "./ima/botones/icon_sms_off.png";
		case ZONA_LLEGADA: return "./ima/botones/icon_llegada_off.png";
		case GUARDAR: return "./ima/botones/icon_save_off.png";
		case VOID: return "./letras/vacio.bmp";
	}
	
	return "";
}


string Boton::getRutaImgMouseDown(int nroDeBoton) {
	switch(nroDeBoton) {
		case MASA: return "./ima/botones/icon_level_on.png";
		case RUEDA: return "./ima/botones/PlataformaMetalica_MouseDown.bmp";
		case PUNTO_FIJO: return "./ima/botones/icon_node_on.png";
		case BARRA: return "./ima/botones/icon_metal_bar_on.png";
		case PLATAFORMA: return "./ima/botones/icon_metal_sheet_on.png";
		case SOGA: return "./ima/botones/icon_elastic_on.png";
		case CINTA: return "./ima/botones/icon_cloth_on.png";
		case COHETE: return "./ima/botones/icon_rocket_on.png";
		case BORRAR_TODO: return "./ima/botones/icon_clear_on.png";
		case BORRAR_ELEMENTO: return "./ima/botones/icon_rubber_on.png";
		case PLAY: return "./ima/botones/icon_start_on.png";
		case SALIR: return "./ima/botones/icon_salir_on.png";
		case SMS: return "./ima/botones/icon_sms_on.png";
		case GUARDAR: return "./ima/botones/icon_save_on.png";
		case ZONA_LLEGADA: return "./ima/botones/icon_llegada_on.png";
		case VOID: return "./letras/vacio.bmp";
	}
	
	return "";
}

string Boton::getRutaImgMouseOver(int nroDeBoton) {
	switch(nroDeBoton) {
		case MASA: return "./ima/botones/icon_level_over.png";
		case RUEDA: return "./ima/botones/PlataformaMetalica_MouseOver.bmp"; 
		case PUNTO_FIJO: return "./ima/botones/icon_node_off.png"; 
		case BARRA: return "./ima/botones/icon_metal_bar_over.png";
		case PLATAFORMA: return "./ima/botones/icon_metal_sheet_over.png";
		case SOGA: return "./ima/botones/icon_elastic_over.png";
		case CINTA: return "./ima/botones/icon_cloth_over.png";
		case COHETE: return "./ima/botones/icon_rocket_over.png";
		case BORRAR_TODO: return "./ima/botones/icon_clear_over.png";
		case BORRAR_ELEMENTO: return "./ima/botones/icon_rubber_over.png";
		case PLAY: return "./ima/botones/icon_start_over.png";
		case SALIR: return "./ima/botones/icon_salir_over.png";
		case SMS: return "./ima/botones/icon_sms_over.png";
		case GUARDAR: return "./ima/botones/icon_save_over.png";
		case ZONA_LLEGADA: return "./ima/botones/icon_llegada_over.png";
		case VOID: return "./letras/vacio.bmp";
	}
	
	return "";
}


