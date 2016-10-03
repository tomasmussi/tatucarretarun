#ifndef INTERFAZMAPA_H_
#define INTERFAZMAPA_H_

#include "Coordenada.h"
#include "PanelContenedor.h"
#include "Mapa.h"
#include <string>
#include "ElementoCircular.h"
#include "ElementoLongitudinal.h"
#include "Llegada.h"
class InterfazMapa : public PanelContenedor
{
private:
	/* Visibilidad del modelo mapa */
	Mapa * mapa;
	/* Rutinas del mouse */
	void (*pf_rutinaMouseDown)(Coordenada, InterfazMapa *);
	void (*pf_rutinaMouseUp)(Coordenada, InterfazMapa *);
	void (*pf_rutinaMouseOver)(Coordenada, InterfazMapa *);
	
	/* Metodos para imprimir cada tipo de elemento */
	void imprimirElementoCircular(ElementoCircular * circular);
	void imprimirElementoLongitudinal(ElementoLongitudinal * longitudinal);
	void imprimirZonaLlegada(Llegada * llegada);
	
	
public:
	/* Constructores y destructores, con y sin zoom de vision */
	InterfazMapa(const std::string & rutaImgDefecto, const std::string & rutaImgClickDown, Mapa * mapa);
	InterfazMapa(const std::string & rutaImgDefecto, const std::string & rutaImgClickDown, Mapa * mapa, double zoom);
	virtual ~InterfazMapa();
	
	/* Redefenición de llamadas a evnetos */
	void evento_MouseDown(Coordenada punto);
	void evento_MouseUp(Coordenada punto);
	void evento_MouseOver(Coordenada punto);
	
	/* setters para las rutinas de cada evento */
	void setRutinaMouseDown(void (*rutina)(Coordenada, InterfazMapa *));
	void setRutinaMouseUp(void (*rutina)(Coordenada, InterfazMapa *));
	void setRutinaMouseOver(void (*rutina)(Coordenada, InterfazMapa *));
	Coordenada vectorDiferencia;
	/* getter y setter del mapa */
	Mapa * getMapa() const;
	void setMapa(Mapa* mapa); //destruye el viejo
	/* Dibuja el mapa */
	void imprimir();

};
#endif
