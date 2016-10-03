#ifndef BOTON_H_
#define BOTON_H_

#include "PanelContenedor.h"
#include "Mapa.h"
#include <string>

class Boton : public PanelContenedor
{
private:
	/* Rutinas */
	void (*pf_rutinaMouseDown)(Coordenada, Boton * );
	void (*pf_rutinaMouseUp)(Coordenada, Boton * );
	void (*pf_rutinaMouseOver)(Coordenada, Boton * );
	
	/* Buscadores de imagenes */
	std::string getRutaImgDefecto(int nroDeBoton);
	std::string getRutaImgMouseDown(int nroDeBoton);
	std::string getRutaImgMouseOver(int nroDeBoton);
	
	/* Referencia al mapa sobre el cual interactua */
	Mapa * mapa;
	/* Indica si esta presionado */
	bool presionado;
	
public:
	/* Constructores y destructores */
	Boton(int nroDeBoton, Mapa *);
	virtual ~Boton();
	/* Eventos del mouse */
	void evento_MouseDown(Coordenada punto);
	void evento_MouseUp(Coordenada punto);
	void evento_MouseOver(Coordenada punto);
	/* Setters de eventos */
	void setRutinaMouseDown(void (*rutina)(Coordenada, Boton *));
	void setRutinaMouseUp(void (*rutina)(Coordenada, Boton *));
	void setRutinaMouseOver(void (*rutina)(Coordenada, Boton *));
	/*  Setters, getters y Envios de seniales */
	bool esta_presionado() const;
	void desapretar();	
	void enviarSenialSalida();
	void enviarSenialSms();
	void enviarSenialSimulacion();
	long pedirTiempoResolucion();
	bool getSmsState();
	Mapa * getMapa() const;
};

#endif /*BOTON_H_*/

