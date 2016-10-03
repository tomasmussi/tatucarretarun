#ifndef ELEMENTOLONGITUDINAL_H_
#define ELEMENTOLONGITUDINAL_H_

#include "Elemento.h"
#include <string>
#include <list>
#include "PuntoDeEnlace.h"

#define LONGITUD_DE_ENLACE_MINIMA  50

class ElementoLongitudinal : public Elemento {

private:
	std::list <PuntoDeEnlace*> _enlaces;
	Coordenada extremoOpuesto;
		
protected:
	ulong ancho;
public:
	ElementoLongitudinal();
	ElementoLongitudinal(const std::string & nombre, Coordenada posicion,
			const ulong magnitud, long angulo, ulong ancho);
	virtual ~ElementoLongitudinal();
	
	/* Agrega a la lista los puntos de enlaces en funcion de la longitud
	 * determinando su posicion */
	void actualizarPuntosDeEnlace(int modo);
	/* Imprime los enlaces */
	void imprimirEnlaces();
	/* Devuelve puntero a lista de enlaces */
	std::list<PuntoDeEnlace*> getListaEnlaces();
	/* Elimina los enlaces */
	void eliminarEnlaces();
	/* Devuelve anccho */
	ulong getAncho() const;
	/* Setea el extremo opuesto y actualiza el modulo y angulo */
	void setExtremoOpuesto(const Coordenada);
	/* Devuelve el extremo Opuesto */
	Coordenada getExtremoOpuesto() const;
	/* Devuelve true si la Coordenada pertenece al Elemento */
	virtual int seleccionar(Coordenada) const;
	/* Redefino setPosicion. Actualizo extremoOpuesto */
	void setPosicion(const Coordenada nuevaPosicion);
	/* Redefino setAngulo. Actualizo extremoOpuesto */
	void setAngulo(const long angulo);
};

#endif /*ELEMENTOLONGITUDINAL_H_*/
