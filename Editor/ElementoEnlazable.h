#ifndef ELEMENTOENLAZABLE_H_
#define ELEMENTOENLAZABLE_H_

#include "ElementoCircular.h"
#include "Elemento.h"
#include "Coordenada.h"
#include <list>

class ElementoEnlazable : public ElementoCircular
{
private:
	Elemento * poseedor;
	std::list < ElementoEnlazable* > _enlaces;


public:
	ElementoEnlazable(const std::string & nombre,
		Coordenada posicion, const ulong magnitud, long angulo, Elemento * poseedor);
	virtual ~ElementoEnlazable();
	
	/* Devuelve el Elemento que lo contiene */
	Elemento * getPoseedor() const;
	/* Devuelve la lista de enlazables que tiene asociados */
	std::list <ElementoEnlazable*> getListaEnlaces() const;
	/* Vacia los enlaces y los libera */
	void eliminarEnlaces();
	/* Agrega un enlace a la lista verificando y devuelve si pudo 
	hacerlo (verifica que su posicion sea igual que la propia)
	*/
	bool conectarA(ElementoEnlazable*);
	bool enlazado;
		
};

#endif /*ELEMENTOENLAZABLE_H_*/
