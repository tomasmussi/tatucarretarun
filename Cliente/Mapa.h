#ifndef MAPA_H_
#define MAPA_H_
#include "Elemento.h"
#include <list>
#include "FabricaDeElementos.h"
#include "constantes.h"
typedef unsigned long ulong;

class Mapa {
private:
	/* Nombre del mapa. Sirve para buscar el archivo de imagen de fondo */
	std::string nombre;
	/* Ancho y alto del mapa */
	ulong ancho;
	ulong alto;
	/* Lista de elementos contenidos en mapa */
	std::list <Elemento*> _elementos;
	/* Es el elemento seleccionado. Todos los metodos trabajaran sobre el */
	Elemento * elemento;
	/* Es el elemento que se creará al llamar agregarElemento() */
	int nroElementoParaCrear;
	/* Indica por donde se a seleccionado el elemento */
	int tipoSeleccion;
	/* Vector de elementos habilitados */
	bool elementosHabilitados [CANTIDAD_ELEMENTOS];
	/* Indica los precios de los elementos de este mapa en particular */
	unsigned int precio[CANTIDAD_ELEMENTOS];
	/* Indica la plata que este mapa provee */
	int plata;
	/* Indica el puntaje */
	int puntos;
	int tiempoCohete; // milisegundos. 0 es tiempo infinito
	long tiempoResolucion; // milisegundos de tiempo de resolcion del mapa
public:
	/* Contructor con valores por default */
	Mapa();
	/* Contructor con parametros */
	Mapa(ulong ancho, ulong alto);
	/* Destructor virtual */
	virtual ~Mapa();
	/* Devuelve ancho */
	ulong getAncho() const;
	/* Devuelve alto */
	ulong getAlto() const;
	/* Setea el elemento que la fabrica contruira cuando se lo pida */
	void setNroElementoParaCrear(int);
	/* Devuelve el elemento a crear */
	int getNroElementoParaCrear() const;
	/* Agrega elemento al mapa. Devuelve true si fue posible */
	bool agregarElemento(Coordenada posicion);
	/* Selecciona el primer elemento que encuentre en esa coordenada.
	 * Devuelve False en caso de no haber ninguno
	 *  */
	 int seleccionarElemento(Coordenada);
	 /* Informa si hay algun elemento seleccionado */
	 bool hayElementoSeleccionado() const;
	 /* Devuelve por donde se ha seleccionado el elemento */
	 int getTipoSeleccion() const;
	 /* setea el tipo de seleccion de forma forzada */
	 void setTipoSeleccion(const int tipo);
	 /* Devuelve puntero constante al Elemento seleccionado */
	 const Elemento * getElementoSeleccionado() const;
	/* Devuelve cantidad de elementos en el mapa */
	size_t getCantidadElementos();
	/* Devuelve true si existe punto esta en el entorno 
	 * de algun punto de enlace.
	 * Retorna en puntoExacto el centro del punto de enlace 
	 * */
	bool entornoDePuntoDeEnlace(Coordenada punto, Coordenada * puntoExacto);
	/* Setea el extremo opuesto del elemento longitudinal seleccionado */
	void setExtremoOpuesto(const Coordenada punto);
	/* Devuelve el extremo opuesto del elemento longitudinal seleccionado */
	Coordenada getExtremoOpuesto() const;
	/* Mueve el elemento a destino. Devuelve true si fue posible */
	bool moverElementoA(Coordenada posicion);
	/* Rota el elemento a destino. Devuelve true si fue posible */
	bool rotarElementoA(long angulo);
	/* Agranda el elemento a longitud. Devuelve true si fue posible */
	bool agrandarElementoA(long longitud);
	/* Actualiza modulo y angulo en funcion del extremoOpuesto.
	 * Mantiene posicion constante.
	 * Retora la coordenada corregida.
	 *  */
	Coordenada actualizarModuloAngulo(Coordenada extremoOpuesto);
	/* Actualiza solo el angulo manteniendo LONGITUD_MINIMA */
	Coordenada actualizarAngulo(Coordenada extremoOpuesto);
	/* Actualiza solo el angulo manteniendo LONGITUD_MAXIMA */
	Coordenada actualizarAngulo2(Coordenada click);
	/* Elimina el elemento y extrae de la lista*/
	void eliminarElemento();
	/* Elimina todos y vacia la lista */
	void eliminarTodosLosElementos();
	/* Imprime todos los elementos */
	void imprimir();
	/* Devuelve la lista de elementos */
	std::list <Elemento*> getListaElementos() const;
	/* Actualiza los puntos de enlace de los elementos longitudinales */
	void actualizarEnlaces();
	/* Si el elemento seleccionado debera ser elmininado */
	bool elementoABorrar;
	/* Filtra los enlaces en todo el mapa y los del elementos longitudinales */
	std::list <ElementoEnlazable*> getTodosLosEnlaces();
	/* Avisa al Elemento Seleccionado si mostrar sus puntos de enlace */
	void setMostrarPuntosDeEnlaceElemento(const bool);
	/* Bloquea todos los elementos del mapa */
	void bloquearElementos();
	/* Desbloquea todos los elementos del mapa */
	void desbloquearElementos();
	/* Deshabilita un elemento */
	void deshabilitarElemento(const int nroElemento);
	/* Habilita un elemento */
	void habilitarElemento(const int nroElemento);
	/* Devueleve si el elemento esta habilitado */
	bool elementoHabilitado(const int nroElemento) const;
	/* Muestra por consola los enlaces */
	void imprimirTodosLosEnlaces();

	/* Otros getters y setter de menor importancia */
	void setNombre(const std::string&); 
	std::string getNombre() const;
	void setPlata(const int);
	int getPlata() const;
	void setPuntos(const int);
	int getPuntos() const;
	void setPrecio(int nroElemento, int precio);
	int getPrecio(int nroElemento) const;
	int getTiempoCohete() const; // milisegundos
	void setTiempoCohete(int tiempo); // milisegundos
	void setTiempoResolucion(const long);
	long getTiempoResolucion() const;
	int getSumaDePrecios() const;
	
	
	
};

#endif /*MAPA_H_*/
