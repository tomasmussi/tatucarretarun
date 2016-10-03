#include "elementManager.h"
ElementManager::ElementManager(){
	vectoresElementos.push_back(&ropes);	
	vectoresElementos.push_back(&cohetes);	
	vectoresElementos.push_back(&ruedas);	
	vectoresElementos.push_back(&plataformas);	
	vectoresElementos.push_back(&metalBars);	
	vectoresElementos.push_back(&mainBalls);	
	vectoresElementos.push_back(&lonas);	

}

ElementManager::~ElementManager(){
	for (size_t i=0; i< vectoresElementos.size(); i++)
		for (size_t j=0; j< vectoresElementos.at(i)->size(); j++)
			delete(vectoresElementos.at(i)->at(j));

	for (size_t i=0; i< zonasLlegada.size(); i++)
		delete(zonasLlegada.at(i));

}

void ElementManager::addRope(int first, int last){
	Element* elemento = new Element(first,last);	
	for(int i=first; i<=last;i++)
		elemento->masas.push_back(i);

	ropes.push_back(elemento);
}

void ElementManager::addCohete(int first, int last){
	Element* elemento = new Element(first,last);	
	for(int i=first; i<=last;i++)
		elemento->masas.push_back(i);

	cohetes.push_back(elemento);
}

void ElementManager::addRueda(int first, int last){
	Element* elemento = new Element(first,last);	
	for(int i=first; i<=last;i++)
		elemento->masas.push_back(i);

	ruedas.push_back(elemento);
}

void ElementManager::addMetalBar(int first, int last){
	Element* elemento = new Element(first,last);	
	for(int i=first; i<=last;i++)
		elemento->masas.push_back(i);

	metalBars.push_back(elemento);
}

void ElementManager::addPlataforma(int first, int last){
	Element* elemento = new Element(first,last);	
	for(int i=first; i<=last;i++)
		elemento->masas.push_back(i);

	plataformas.push_back(elemento);
}

void ElementManager::addMainBall(int first, int last){
	Element* elemento = new Element(first,last);	
	for(int i=first; i<=last;i++)
		elemento->masas.push_back(i);

	mainBalls.push_back(elemento);
}

void ElementManager::addLona(int first, int last){
	Element* elemento = new Element(first,last);	
	for(int i=first; i<=last;i++)
		elemento->masas.push_back(i);

	lonas.push_back(elemento);
}

void ElementManager::addPuntoFijo(int first, int last){
	Element* elemento = new Element(first,last);	
	elemento->masas.push_back(first);
	puntosFijos.push_back(elemento);
}

void ElementManager::addZonaLlegada(double x,double y,double width,double heigth){
	zonasLlegada.push_back(new ZonaLlegada(x,y,width,heigth));
}
	
std::vector<Element*> ElementManager::getRopes(){
	return ropes;
}
std::vector<Element*> ElementManager::getPuntosFijos(){
	return puntosFijos;
}
std::vector<Element*> ElementManager::getRuedas(){
	return ruedas;
}
std::vector<Element*> ElementManager::getMainBalls(){
	return mainBalls;
}
std::vector<Element*> ElementManager::getLonas(){
	return lonas;
}
std::vector<Element*> ElementManager::getCohetes(){
	return cohetes;
}
std::vector<Element*> ElementManager::getPlataformas(){
	return plataformas;
}
std::vector<Element*> ElementManager::getMetalBars(){
	return metalBars;
}
std::vector<ZonaLlegada*> ElementManager::getZonasLlegada(){
	return zonasLlegada;
}
