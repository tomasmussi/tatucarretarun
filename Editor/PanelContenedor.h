#ifndef PANELCONTENEDOR_H_
#define PANELCONTENEDOR_H_
#include "SDL.h"
#include "SDL_image.h"
#include <SDL_opengl.h>
#include "Coordenada.h"
#include <list>
#include <map>
#include <string>
#include <math.h>
#include "Ventana.h"
#include "constantes.h"

class Ventana;

typedef unsigned long ulong;

class PanelContenedor
{
private:
	/* ancho pantalla */
	ulong ancho;
	/* alto pantalla */
	ulong alto;
	/* Posicion del panel en pantalla */
	Coordenada posicion;
	/* numero de textura que se imprime actualmente */
	int texturaActual;
	/* Referencia a la pantalla */
	SDL_Surface * pantalla;
	/* Visibilidad de la textura */
	bool visible;
	/* Lista de paneles contendidos */
	std::list <PanelContenedor*> _paneles;
	/* Indica si fue clickeado */
	bool clicked;
	/* Indica si tiene el mouse arriba */
	bool mouseOver;
	/* Nombre en la jerarquia */
	std::string nombre;
	/* Id de panel */
	int id;
	
	ulong getAncho(const std::string & rutaImg);
	ulong getAlto(const std::string & rutaImg);


protected:
	/* mapa de texturas */
	std::map<int, GLuint> _texturas;
	/* Referencia de la ventana contenedora */
	Ventana * ventana;
	/* Carga una textura al map */
	void loadTexture(const char * path, int tipoTextura);
	/* Indica si esta habilitado */
	bool habilitado;

	
public:
	/* Constructores y destructor */
	PanelContenedor(const std::string & nombre, const std::string & rutaImgDefecto, const std::string & rutaImgClickDown, const std::string & rutaImgMouseOver); 
	PanelContenedor(const std::string & nombre, const std::string & rutaImgDefecto, const std::string & rutaImgClickDown, const std::string & rutaImgMouseOver, double zoom); 
	virtual ~PanelContenedor();
	/* Imprime en pantalla el panel y subpaneles */
	virtual void imprimir();
	/* Imprime la textura actual */
	void imprimirImagenDeFondo();
	/* Agregan paneles a la lista */
	void agregarPanel(PanelContenedor *, Coordenada);
	void agregarPanel(PanelContenedor * panel, int lugar);
	/* Utilitarios para la conversion de cordenadas SDL a OpenGL */
	double cambioX(long x);
	double cambioY(long y);
	/* Devuelve true si la coordenada pertenece al panel */
	bool pertenece(Coordenada);
	/* Ejecuta evento o bien lo ejecuta en un subpanel */
	bool seEjecutaEventoEn(const Coordenada punto, int evento);

	//SETTERS Y GETTERS
	void setPantalla (SDL_Surface * pantalla);
	void setVentana(Ventana *);
	void setPosicion(const Coordenada posicion); //la setea el agregador de panel
	Coordenada getPosicion() const;
	void setImagenDeFondo(int);
	void setAltura(const ulong);
	void setAncho(const ulong);
	ulong getAltura() const;
	ulong getAncho() const;
	bool is_clicked() const;
	bool is_over() const;
	std::string getNombre() const;
	int getId() const;
	void setId(const int);
	void setMouseOver(const bool b);
	bool estaHabilitado() const;
	void setHabilitar(const bool);

	//EVENTOS
	void virtual evento_MouseDown(Coordenada punto);
	void virtual evento_MouseOver(Coordenada punto);
	void virtual evento_MouseUp(Coordenada punto);
	
	//VARIABLES DE CONVERSION DE SDL A OPENGL
	double TX;
	double TY;
};
#endif /*PANELCONTENEDOR_H_*/

