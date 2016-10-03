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
	void addRope(int first, int last);	
	void addLona(int first, int last);
	void addMetalBar(int first, int last);
	void addPlataforma(int first, int last);	
	void addCohete(int first, int last);
	void addRueda(int first, int last);
	void addMainBall(int first,int last);	
	void addPuntoFijo(int first,int last);	
	void addZonaLlegada(double x,double y,double width,double heigth);
	void addSpringElement(int spring, int element);
	std::vector<Element*> getRopes();	
	std::vector<Element*> getRuedas();	
	std::vector<Element*> getMetalBars();
	std::vector<Element*> getPlataformas();	
	std::vector<Element*> getLonas();
	std::vector<Element*> getCohetes();
	std::vector<Element*> getMainBalls();
	std::vector<Element*> getPuntosFijos();
	std::vector<ZonaLlegada*> getZonasLlegada();
	std::vector< std::vector<Element*>* > vectoresElementos;
	std::map<int,int> getSpringElements();

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
	std::map<int,int> springElements;
	
};

#endif
