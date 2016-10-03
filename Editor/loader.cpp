#include "PanelContenedor.h"
#include "loader.h"
#include "Angulo.h"
#include "constantes.h"
#include "constantesnico.h"

#include <ctime>
#include <list>
#define ROPES 5
#define LONAS 6
#define RUEDAS 2
#define COHETES 7
#define METAL 3
#define PLATAFORMAS 4
#define PTO_FIJO 1
#define TX 300.0
#define TB 600.0
#define TY 300.0
using namespace std;

void asociateSpring(SpringMassSystem& sms, int spring, int elemento){
	ElementManager* manager;
	manager = sms.getElementManager();
	manager->addSpringElement(spring, elemento);
}

unsigned calculaComp(double largo,double &sep){
	double min = 0.03;
	unsigned i = 0;
	while (true){
		if ( (i*min<largo) && ((i+1)*min>=largo) ) 
			break;
		i++;			
	}
	sep = largo / i;	
	return i;
}

void drawCintaLona(SpringMassSystem& sms, double x,double y,double largo,double angulo){
	ElementManager* manager;
	manager = sms.getElementManager();

	int ini, fin,fspr;
	double sep = 0.04;	
	int longitud = calculaComp(largo,sep);

	fspr = sms.getNumSprings();
	ini = sms.getNumMasses();
	fin = ini + longitud;
	for (int i=0;i<=longitud;i++) 
		sms.addCollMass(2.0, Vector2<>(x+i*sep*cos(angulo), y+i*sep*sin(angulo)), 0.01);

	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,LONA_K,0);	

	manager->addLona(ini,fin);
	sms.addEnganche(ini);
	sms.addEnganche(fin);
		
	ini = (manager->getLonas()).size()-1;
	for (size_t i=fspr; i<sms.getNumSprings();i++)
		asociateSpring(sms,i,ini);
}

void drawMetalBar(SpringMassSystem& sms, double x,double y,double largo,double angulo){
	ElementManager* manager;
	manager = sms.getElementManager();
	double sep = 0.04;	
	int longitud = calculaComp(largo,sep);
	
	int ini,fin,first,last;
	ini = sms.getNumMasses();	
	first = ini;
	fin = ini + longitud;
	 
	for (int i=0;i<=longitud;i++) 
		sms.addMass(2.0, Vector2<>(x+i*sep*cos(angulo), y+i*sep*sin(angulo)));  

	last = sms.getNumMasses()-1;
	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,METAL_K,0);	

	x += METAL_WIDTH  * sin(angulo);
	y -= METAL_WIDTH  * cos(angulo) ;
	
	ini = sms.getNumMasses();	
	fin = ini + longitud;
	
	for (int i=0;i<=longitud;i++) 
		sms.addMass(2.0, Vector2<>(x+i*sep*cos(angulo), y+i*sep*sin(angulo))); 

	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,METAL_K,0);	

	for (int i=ini ;i<=fin ;i++)	
		sms.addSpring(i,i-longitud-1,METAL_K,0);	
	for (int i=ini+1 ;i<=fin ;i++)	
		sms.addSpring(i,i-longitud-2,METAL_K,0);	


	manager->addMetalBar(first,sms.getNumMasses()-1);
	sms.addEnganche(first);
	sms.addEnganche(last);
}

void drawPlataforma(SpringMassSystem& sms, double x,double y,double largo,double angulo){
	ElementManager* manager;
	manager = sms.getElementManager();
	double sep = 0.04;	
	int longitud = calculaComp(largo,sep);
	
	int ini,fin,first,last;
	ini = sms.getNumMasses();	
	first = ini;
	fin = ini + longitud;
	 
	for (int i=0;i<=longitud;i++) 
		sms.addCollMass(2.0, Vector2<>(x+i*sep*cos(angulo), y+i*sep*sin(angulo)), 0.01);  

	last = sms.getNumMasses()-1;
	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,METAL_K,0);	

	x += METAL_WIDTH  * sin(angulo);
	y -= METAL_WIDTH  * cos(angulo) ;
	
	ini = sms.getNumMasses();	
	fin = ini + longitud;
	
	for (int i=0;i<=longitud;i++) 
		sms.addMass(2.0, Vector2<>(x+i*sep*cos(angulo), y+i*sep*sin(angulo))); 

	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,METAL_K,0);	

	for (int i=ini ;i<=fin ;i++)	
		sms.addSpring(i,i-longitud-1,METAL_K,0);	
	for (int i=ini+1 ;i<=fin ;i++)	
		sms.addSpring(i,i-longitud-2,METAL_K,0);	


	manager->addPlataforma(first,sms.getNumMasses()-1);
	sms.addEnganche(first);
	sms.addEnganche(last);
}

void drawRueda(SpringMassSystem& sms, double x,double y,double radio){
	ElementManager* manager;
	manager = sms.getElementManager();

	unsigned sides = 12 ;
	int ini = sms.getNumMasses();
	sms.addCollMass( 0,Vector2<>(x,y),0.05);
	double dx,dy;
	for (unsigned j = 0; j < sides; j++){
		dx = radio * cos(j * (2.0 * PI / sides));
		dy = radio * sin(j * (2.0 * PI / sides));
		sms.addCollMass( 1.5,
		Vector2<>(x + dx, y + dy),
		0.01);
		}
	for (unsigned j = ini+1; j < ini + sides; j++){
		sms.addSpring(j,j+1,RUEDA_K,0);	
		sms.addSpring(ini,j,RUEDA_K,0);	
	}

	sms.addSpring(ini+1,ini + sides,RUEDA_K,0);	
	sms.addSpring(ini,ini + sides,RUEDA_K,0);	
	manager->addRueda(ini,sms.getNumMasses()-1);	

	for (size_t i = ini + 1; i<=(ini + sides); i++)
		sms.addEnganche(i);

}

void drawCohete(SpringMassSystem& sms, double x,double y,double largo,double angulo){
	ElementManager* manager;
	double xprime,yprime;
	x-=ROCKET_WIDTH*0.5*sin(angulo);
	y+=ROCKET_WIDTH*0.5*cos(angulo);

	xprime = x;
	yprime = y;

	manager = sms.getElementManager();
	int ini, fin,first;
	ini = sms.getNumMasses();
	first = ini;
	double sep = 0.04;	
	int longitud = calculaComp(largo,sep);


	fin = ini + longitud - 1;
	for (int i=0;i<longitud;i++) 
		sms.addCollMass(1, Vector2<>(x+i*sep*cos(angulo), y+i*sep*sin(angulo)), 0.01); 

	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,ROCKET_K,0);	

	ini = sms.getNumMasses();
	fin = ini + longitud - 1;

	xprime = x + ROCKET_WIDTH * sin(angulo);
	yprime = y - ROCKET_WIDTH * cos(angulo);

	for (int i=0;i<longitud;i++)
		sms.addCollMass(1, Vector2<>(xprime+i*sep*cos(angulo), yprime+i*sep*sin(angulo)), 0.01); 

	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,ROCKET_K,0);	

	for (int i=ini ;i<=fin ;i++)	
		sms.addSpring(i,i-longitud,ROCKET_K,0);	

	for (int i=ini ;i<fin ;i++)
		sms.addSpring(i,i-longitud+1,ROCKET_K,0);

	xprime = x+ROCKET_WIDTH*0.5*sin(angulo);
	yprime = y-ROCKET_WIDTH*0.5*cos(angulo);

	sms.addCollMass(1, Vector2<>(xprime+longitud*sep*cos(angulo), yprime+longitud*sep*sin(angulo)), 0.03); 
	ini = sms.getNumMasses();
	sms.addSpring(ini-1,first+longitud-1,ROCKET_K,0);
	sms.addSpring(ini-1,first+2*longitud-1,ROCKET_K,0);

	sms.addCollMass(1, Vector2<>(xprime, yprime), 0.01);
	sms.addSpring(ini,first,ROCKET_K,0);
	sms.addSpring(ini,first+longitud,ROCKET_K,0);
	manager->addCohete(first,sms.getNumMasses()-1);	
	sms.addEnganche(sms.getNumMasses()-1);
	sms.addEnganche(sms.getNumMasses()-2);

}

void drawRope(SpringMassSystem& sms, double x,double y,double largo,double angulo){
	ElementManager* manager;
	manager = sms.getElementManager();
	int ini, fin;
	double sep = 0.04;	
	int longitud = calculaComp(largo,sep);

	ini = sms.getNumMasses();
	fin = ini + longitud;
	
	for (int i=0;i<=longitud;i++) 
		sms.addMass(3, Vector2<>(x+i*sep*cos(angulo), y+i*sep*sin(angulo)));  

	for (int i=ini ;i<fin ;i++)	
		sms.addSpring(i,i+1,ROPE_K,0);	

	manager->addRope(ini,fin);	
	sms.addEnganche(ini);
	sms.addEnganche(fin);
}

void drawFixedSpot(SpringMassSystem& sms, double x,double y){
	ElementManager* manager;
	manager = sms.getElementManager();
	sms.addMass(0,Vector2<>(x, y));
	manager->addPuntoFijo(sms.getNumMasses()-1,sms.getNumMasses()-1);
	sms.addEnganche(sms.getNumMasses()-1);
}

void drawCrashableFixedSpot(SpringMassSystem& sms, double x,double y){
	sms.addCollMass(0,Vector2<>(x, y),0.03);
}

void drawBall(SpringMassSystem& sms, double x,double y,double radio){
	ElementManager* manager;
	manager = sms.getElementManager();
	unsigned sides = 16 ;
	int ini = sms.getNumMasses();
	sms.addCollMass( 0.5,Vector2<>(x,y),0.05);
	double dx,dy;
	for (unsigned j = 0; j < sides; j++){
		dx = radio * cos(j * (2.0 * PI / sides));
		dy = radio * sin(j * (2.0 * PI / sides));
		sms.addCollMass( 1.5,
		Vector2<>(x + dx, y + dy),
		0.009);
		}
	for (unsigned j = ini+1; j < ini + sides; j++){
		sms.addSpring(j,j+1,BALL_K,0);	
		sms.addSpring(ini,j,BALL_K,0);	
	}

	sms.addSpring(ini+1,ini + sides,BALL_K,0);	
	sms.addSpring(ini,ini + sides,BALL_K,0);		
	manager->addMainBall(ini,sms.getNumMasses()-1);
}

void drawZonaLlegada(SpringMassSystem& sms,double x ,double y,double width,double heigth ){
	ElementManager* manager;
	manager = sms.getElementManager();
	
	manager->addZonaLlegada(x,y,width,heigth);
}
	
void fixMass(SpringMassSystem& sms,int pos){
	sms.fixMass(pos);
}

double cambioX(long x) {
	return (x - 1000/2.0)/TX;
}

double cambioY(long y) {
	return (-y + 600/2.0)/TY;
}
void instanciar(SpringMassSystem& sms)
{
	Mapa* mapa = sms.getMapa();



	list <Elemento*> elementos = mapa->getListaElementos();

	list <Elemento*>::iterator it;

	for(it = elementos.begin(); it != elementos.end(); it++) {
		
		if((*it)->es(MASA))
			drawBall(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX);
		if((*it)->es(RUEDA))
			drawRueda(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX);
		if((*it)->es(SOGA))
			drawRope(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX,Angulo::aRadianes((*it)->getAngulo()));
		if((*it)->es(CINTA))
			drawCintaLona(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX,Angulo::aRadianes((*it)->getAngulo()));
		if((*it)->es(PLATAFORMA))
			drawPlataforma(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX,Angulo::aRadianes((*it)->getAngulo()));
		if((*it)->es(BARRA))
			drawMetalBar(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX,Angulo::aRadianes((*it)->getAngulo()));
		if((*it)->es(COHETE))
			drawCohete(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX,Angulo::aRadianes((*it)->getAngulo()));
		if((*it)->es(PUNTO_FIJO))
			drawFixedSpot(sms, cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y));
		if((*it)->es(ZONA_LLEGADA))
			drawZonaLlegada(sms,cambioX((*it)->getPosicion().x) ,cambioY((*it)->getPosicion().y),(*it)->getMagnitud()/TX,(*it)->getMagnitud()/TX );

	}
	sms.checkRoutine();
	
}

void (*instance[])(SpringMassSystem&) = {instanciar};

void load(SpringMassSystem& sms)
{
	srand(static_cast<unsigned>(time(0)));

	int n = sizeof(instance) / sizeof(instance[0]);

	instance[rand() % n](sms);
}

