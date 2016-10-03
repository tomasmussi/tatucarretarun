#ifndef ELEMENTMANAGER_H
#define ELEMENTMANAGER_H

#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include "element.h"
#include "zonaLlegada.h"

class ElementManager
{
public:
	ElementManager();
	~ElementManager();
	/*Agrega una rueda al sistema*/
	void addRope(int first, int last);
	/*Agrega una cinta de lona al sistema*/	
	void addLona(int first, int last);
	/*Agrega una barra de metal al sistema*/
	void addMetalBar(int first, int last);
	/*Agrega una plataforma metalica al sistema*/
	void addPlataforma(int first, int last);
	/*Agrega un cohete al sistema*/	
	void addCohete(int first, int last);
	/*Agrega una rueda al sistema*/
	void addRueda(int first, int last);
	/*Agrega la masa principal al sistema*/
	void addMainBall(int first,int last);	
	/*Agrega un punto fijo*/
	void addPuntoFijo(int first,int last);	
	/*Agrega una zona de llegada*/
	void addZonaLlegada(double x,double y,double width,double heigth);
	/*Devuelve el vector conteniendo las sogas*/
	std::vector<Element*> getRopes();	
	/*Devuelve el vector conteniendo las ruedas*/
	std::vector<Element*> getRuedas();	
	/*Devuelve el vector conteniendo las barras*/
	std::vector<Element*> getMetalBars();
	/*Devuelve el vector conteniendo las plataformas*/
	std::vector<Element*> getPlataformas();	
	/*Devuelve el vector conteniendo las cintas de lona*/
	std::vector<Element*> getLonas();
	/*Devuelve el vector conteniendo los cohetes*/
	std::vector<Element*> getCohetes();
	/*Devuelve el vector conteniendo las masas*/
	std::vector<Element*> getMainBalls();
	/*Devuelve el vector conteniendo los puntos fijos*/
	std::vector<Element*> getPuntosFijos();
	/*Devuelve el vector conteniendo las zonas de llegada*/
	std::vector<ZonaLlegada*> getZonasLlegada();
	/*El vector que contiene todos los vectores
	de elementos (cohetes, ruedas, barras, etc.) */
	std::vector< std::vector<Element*>* > vectoresElementos;

private:
	std::vector<Element*> ropes;
	std::vector<Element*> cohetes;
	std::vector<Element*> ruedas;
	std::vector<Element*> plataformas;
	std::vector<Element*> metalBars;
	std::vector<Element*> mainBalls;
	std::vector<Element*> lonas;
	std::vector<Element*> puntosFijos;
	std::vector<ZonaLlegada*> zonasLlegada;
	
};

#endif
