#include "sms_app.h"
#include "loader.h"
#include <cmath>
#include <fstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <sstream>
#include <vector>
#include <iostream>
#include "Angulo.h"
#include "constantes.h"
#include "constantes_sms.h"
#include "hiloCron.h"
#define SHOW_ALL 0
#define SHOW_TEXT 1
#define ANCHO 0.02

using namespace std;
namespace
{
	const double MASS_SIZE = 0.02;
	const int POLY_SIDE = 12;

	const double MAX_DT = 0.006;

	const std::string CONFIG_FILENAME = "config.txt";

}


SMSApp::SMSApp(int argc, Mapa* mapa) :
config_(CONFIG_FILENAME),
sms_(config_),
video_(config_,"./escenario.jpg"),
ticks_(0),
physMult_(config_.read<int>("App.PhysMultiplier"))
{
	cronometro = new HiloCron();
	sms_.setCronometro(cronometro);
	reloj.cargarTexturas();
	tiempoLlegada = NULL;
	estabaEnLlegada = false;
	sms = false;
	sms_.setMapa(mapa);
	if (argc == 0)
		load(sms_);
	else
		{
		sms=true;
		load(sms_);
		}
}

SMSApp::~SMSApp()
{
	delete(cronometro);
}

bool SMSApp::estaEnLlegada(){
	if ((sms_.getElementManager())->getMainBalls().size()){
	MassProxy masa = sms_.getMassProxy(((sms_.getElementManager())->getMainBalls()).at(0)->getFirst());
	Vector2<> m = masa.getPos();
	double rad = RADIOMASA;
	vector<ZonaLlegada*> llegadas = (sms_.getElementManager())->getZonasLlegada();
	ZonaLlegada* z;
	
	for (size_t i= 0 ; i<llegadas.size(); i++) 
	{
		z = llegadas.at(i);
		if ( 
		((m.x - rad) > z->getX()) && 
		((m.x + rad) < (z->getX() + z->getWidth()) ) &&
		((m.y + rad) < z->getY()) && 
		((m.y - rad) > (z->getY() - z->getHeigth() ) ) 
		)
			return true;
	}
	}
	return false;
}

bool SMSApp::ganaNivel(){

	if (estaEnLlegada()){
		if (!estabaEnLlegada){
			tiempoLlegada = new HiloCron();
			tiempoLlegada->start();
			estabaEnLlegada = true;
			}
		else
			if (tiempoLlegada->getMilisegundos() > TIEMPOGANAR)
				return true;
	}		
	else {
		estabaEnLlegada = false;
		if (tiempoLlegada){
			tiempoLlegada->terminar();
			tiempoLlegada->join();
			delete(tiempoLlegada);
			tiempoLlegada = NULL;
			}
		}
	return false;
}

double SMSApp::run()
{
	bool gano = false;
	//HiloCron cronometro;
	bool exit_now = false;
	ticks_ = SDL_GetTicks();

	SDL_Event ev;
	
	cronometro->start();
	while (!exit_now)
	{
		while (SDL_PollEvent(&ev)){
			if (ev.type == SDL_QUIT)
				exit_now = true;
			if (ev.type == SDL_KEYDOWN)
				if ( ev.key.keysym.sym == SDLK_ESCAPE )
					exit_now = true;
				else if (!strcmp(SDL_GetKeyName(ev.key.keysym.sym),"s"))
					sms = !sms;
		}
		uint32_t newTicks = SDL_GetTicks();
		double dt;

		dt = MAX_DT;
		
		for (int i = 0; i < physMult_; i++)
			sms_.rk4Step(dt / physMult_);
		ticks_ = newTicks;

		/*************IMPRIMIR RELOJ******************/
		double tiempo = cronometro->getMilisegundos() / TIME_C;
		stringstream ss;
		
		ss << fixed;
		ss.precision( 2 );
		ss << tiempo;
		
		
		/*************FIN IMPRIMIR RELOJ**************/
		string time = ss.str();
		time += " s.";
		drawFrame(time);
		if (gano = ganaNivel())
			exit_now = true;
			
	}
	double tiempo = cronometro->getMilisegundos() / TIME_C;
	cronometro->terminar();
	cronometro->join();

	if (gano)
		return tiempo;
	return 0;
}

	double adjX(long x) {
		return (x - WIDTH/2.0)/tX;
}

	double adjY(long y) {
		return (-y + HEIGTH/2.0)/tY;
}
void SMSApp::drawFrame(string &time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	
	glBindTexture( GL_TEXTURE_2D, video_.fondo);
	glBegin(GL_QUADS);

	glTexCoord2i( 0, 0);
	glVertex3f(adjX(0), adjY(0), 0.0f);
	glTexCoord2i( 1, 0);
	glVertex3f(adjX(0), adjY(HEIGTH), 0.0f);
	glTexCoord2i( 1, 1);
	glVertex3f(adjX(0 + WIDTH), adjY(HEIGTH), 0.0f);
	glTexCoord2i( 0, 1);
	glVertex3f(adjX(0 + WIDTH), adjY(0), 0.0f);

	glEnd();
	
	
	
	
	
	
	int status;
	status=SHOW_ALL;
	if (!sms)
	status=SHOW_TEXT;
	
	reloj.imprimirReloj(time, CLOCK_X, CLOCK_Y);
	
	// draws all springs
	glBegin(GL_LINES);
	if (status==SHOW_ALL)
	for (size_t i = 0; i < sms_.getNumSprings(); i++)
	{
		SpringProxy sp = sms_.getSpringProxy(i);

		Vector2<> startPos = sp.getStartPos();
		Vector2<> endPos = sp.getEndPos();

		double relElong =  sp.getRelElongation();
		double k = sp.getK();

		if (k == 0.0)
			continue;

		double color = relElong * k / 10.0;
		if (color < -1.0)
			color = -1.0;
		else if (color > 1.0)
			color = 1.0;

		if (color < 0.0)
		{
			glColor3d(-color / 2.0 + 0.5, 0.5 + color / 2.0, 
				0.5 + color / 2.0);
		}
		else
		{
			double green = 1.0 - 1.0 / (2.0 + 2.0 * color);
			glColor3d(1.0 - green, green, 1.0 - green);
		}

		glVertex2d(startPos.x, startPos.y);
		glVertex2d(endPos.x, endPos.y);
	}
	glEnd();

	// draws all masses
	
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3.0);
	glBegin(GL_POINTS);
	
	glColor3f(0, 0.1, 0.5);
	if (status==SHOW_ALL)
	for (size_t i = 0; i < sms_.getNumMasses(); i++)
	{
		MassProxy mp = sms_.getMassProxy(i);
		Vector2<> massPos = mp.getPos();
		glVertex2d(massPos.x, massPos.y);

	}
	
	glEnd();


	glColor4d(1.0, 1.0, 1.0, 1.0);
	unsigned it;

	//IMPRIME LAS LONAS
	vector<Element*> lonas = (sms_.getElementManager())->getLonas();
		
	
	glBindTexture( GL_TEXTURE_2D, video_.textures.at(CINTA) );	
	glBegin(GL_QUADS);
	if (status==SHOW_TEXT)
	for (it=0; it<lonas.size();it++){ 
		for (size_t i = 0; i < lonas.at(it)->masas.size()-1; i++)	
		{ 
		MassProxy mp = sms_.getMassProxy(lonas.at(it)->masas.at(i));
		Vector2<> massPos = mp.getPos();
		MassProxy mp2 = sms_.getMassProxy(lonas.at(it)->masas.at(i+1));
		Vector2<> massPos2 = mp2.getPos();
		double ang;
		double lseg = sqrt( pow(massPos.x-massPos2.x,2) + pow(massPos.y-massPos2.y,2) );		
		double dx,dy;
		ang = atan(ANCHO / lseg);
		dx = sin (ang) * ANCHO;
		dy = cos (ang) * ANCHO;

		
		glTexCoord2i( 0, 0 );
		glVertex3f( massPos.x, massPos.y, 0.0f );
		glTexCoord2i( 1, 0 );
		glVertex3f( massPos.x + dx, massPos.y - dy, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( massPos2.x + dx, massPos2.y - dy, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( massPos2.x, massPos2.y, 0.0f );
		}
	}
	glEnd();
 
	//IMPRIME LAS BARRAS DE METAL
	vector<Element*> metalBars = (sms_.getElementManager())->getMetalBars();
	glBindTexture( GL_TEXTURE_2D, video_.textures.at(BARRA) );	
	glBegin(GL_QUADS);
	if (status==SHOW_TEXT)
	for (it=0; it<metalBars.size();it++){ 
		MassProxy mp = sms_.getMassProxy(metalBars.at(it)->masas.at(0));
		Vector2<> massPos = mp.getPos();
		MassProxy mp2 = sms_.getMassProxy(metalBars.at(it)->masas.at(metalBars.at(it)->masas.size()/2-1));
		Vector2<> massPos2 = mp2.getPos();
		double ang;
		double lseg = sqrt( pow(massPos.x-massPos2.x,2) + pow(massPos.y-massPos2.y,2) );		
		
		ang = acos( (massPos2.x-massPos.x) / lseg);

		double aX, aY,bX,bY,cX,cY,dX,dY;
		
		if ( massPos2.y < massPos.y)
		{
			if (massPos2.x<massPos.x )
				ang =ang + 2*(PI-ang); 
			else
				ang = 2*PI - ang;
		
		}
	
		aX = massPos.x;
		aY = massPos.y;
		bX = (lseg)
				*cos(ang) + aX;
		bY = (lseg)
				*sin(ang) + aY;
		cX = (ANCHO)*sin(ang)
				+ aX;
		cY = aY - (ANCHO)*cos(ang);
		dX = cX + (lseg)
				*cos(ang);
		dY = cY + (lseg)
				*sin(ang);
			
		glTexCoord2i( 0, 0);
		glVertex3f(cX, cY, 0.0f);
		glTexCoord2i( 1, 0);
		glVertex3f(aX, aY, 0.0f);
		glTexCoord2i( 1, 1);
		glVertex3f(bX, bY, 0.0f);
		glTexCoord2i( 0, 1);
		glVertex3f(dX, dY, 0.0f);
	}
	glEnd();

	//IMPRIME LAS PLATAFORMAS
	vector<Element*> plataformas = (sms_.getElementManager())->getPlataformas();

	glBindTexture( GL_TEXTURE_2D, video_.textures.at(PLATAFORMA) );	
	glBegin(GL_QUADS);
	if (status==SHOW_TEXT)
	for (it=0; it<plataformas.size();it++){ 
		MassProxy mp = sms_.getMassProxy(plataformas.at(it)->masas.at(0));
		Vector2<> massPos = mp.getPos();
		MassProxy mp2 = sms_.getMassProxy(plataformas.at(it)->masas.at(plataformas.at(it)->masas.size()/2-1));
		Vector2<> massPos2 = mp2.getPos();
		double ang;
		double lseg = sqrt( pow(massPos.x-massPos2.x,2) + pow(massPos.y-massPos2.y,2) );		
		
		ang = acos( (massPos2.x-massPos.x) / lseg);

		double aX, aY,bX,bY,cX,cY,dX,dY;
		
		if ( massPos2.y < massPos.y)
		{
			if (massPos2.x<massPos.x )
				ang =ang + 2*(PI-ang); 
			else
				ang = 2*PI - ang;
		}
	
		aX = massPos.x;
		aY = massPos.y;
		bX = (lseg)
				*cos(ang) + aX;
		bY = (lseg)
				*sin(ang) + aY;
		cX = (ANCHO)*sin(ang)
				+ aX;
		cY = aY - (ANCHO)*cos(ang);
		dX = cX + (lseg)
				*cos(ang);
		dY = cY + (lseg)
				*sin(ang);
			
		glTexCoord2i( 0, 0);
		glVertex3f(cX, cY, 0.0f);
		glTexCoord2i( 1, 0);
		glVertex3f(aX, aY, 0.0f);
		glTexCoord2i( 1, 1);
		glVertex3f(bX, bY, 0.0f);
		glTexCoord2i( 0, 1);
		glVertex3f(dX, dY, 0.0f);
	}
	glEnd();

	//IMPRIME LAS SOGAS
	vector<Element*> ropes = (sms_.getElementManager())->getRopes();

	glBindTexture( GL_TEXTURE_2D, video_.textures.at(SOGA) );	
	glBegin(GL_QUADS);
	if (status==SHOW_TEXT)
	for (it=0; it<ropes.size();it++){  
		for (size_t i = 0; i < ropes.at(it)->masas.size()-1; i++)	
		{ 
		MassProxy mp = sms_.getMassProxy(ropes.at(it)->masas.at(i));
		Vector2<> massPos = mp.getPos();
		MassProxy mp2 = sms_.getMassProxy(ropes.at(it)->masas.at(i+1));
		Vector2<> massPos2 = mp2.getPos();

		if ( abs(massPos.x - massPos2.x) < 0.009){
			glTexCoord2i( 0, 0 );
			glVertex3f( massPos.x, massPos.y, 0.0f );
			glTexCoord2i( 1, 0 );
			glVertex3f( massPos2.x, massPos2.y-0.02, 0.0f );
			glTexCoord2i( 1, 1 );
			glVertex3f( massPos2.x+0.005, massPos2.y, 0.0f );
			glTexCoord2i( 0, 1 );
			glVertex3f( massPos.x+0.005, massPos.y, 0.0f );
		}
		else {
			glTexCoord2i( 0, 0 );
			glVertex3f( massPos.x, massPos.y, 0.0f );
			glTexCoord2i( 1, 0 );
			glVertex3f( massPos.x, massPos.y-0.02, 0.0f );
			glTexCoord2i( 1, 1 );
			glVertex3f( massPos2.x, massPos2.y-0.02, 0.0f );
			glTexCoord2i( 0, 1 );
			glVertex3f( massPos2.x, massPos2.y, 0.0f );
		}

	}
	}
	glEnd();

//IMPRIME LAS BOLAS

	vector<Element*> balls = (sms_.getElementManager())->getMainBalls();

	if (status==SHOW_TEXT)
	for (it=0; it<balls.size();it++){
		size_t cant = balls.at(it)->getLast() - balls.at(it)->getFirst();			
		MassProxy central = sms_.getMassProxy(balls.at(it)->getFirst());
		Vector2<> cenPos = central.getPos();

		glBindTexture( GL_TEXTURE_2D, video_.textures.at(MASA) );				
		glBegin(GL_QUADS);
		for (unsigned j = 1; j < cant; j++){
			MassProxy mp = sms_.getMassProxy(balls.at(it)->getFirst() + j);
			Vector2<> massPos = mp.getPos();
			MassProxy mp2 = sms_.getMassProxy(balls.at(it)->getFirst() + j + 1);
			Vector2<> massPos2 = mp2.getPos();
		
			glTexCoord2i( 0, 0 );			
			glVertex3f(massPos.x, massPos.y, 0.0f );		
			glTexCoord2i( 1, 0 );
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
					
			glTexCoord2i( 1, 1 );	
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
			
			glTexCoord2i( 0, 1 );
			glVertex3f(massPos2.x, massPos2.y, 0.0f );					
		}
			glEnd();
			glBindTexture( GL_TEXTURE_2D, video_.textures.at(9) );	
			glBegin(GL_QUADS);
			MassProxy mp = sms_.getMassProxy(balls.at(it)->getLast());
			Vector2<> massPos = mp.getPos();
			MassProxy mp2 = sms_.getMassProxy(balls.at(it)->getFirst() + 1);
			Vector2<> massPos2 = mp2.getPos();
		
			glTexCoord2i( 0, 0 );			
			glVertex3f(massPos.x, massPos.y, 0.0f );		
			glTexCoord2i( 1, 0 );
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
					
			glTexCoord2i( 1, 1 );	
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
			
			glTexCoord2i( 0, 1 );
			glVertex3f(massPos2.x, massPos2.y, 0.0f );


		glEnd();

	}
	
 //IMPRIME COHETES
 
 	vector<Element*> cohetes = (sms_.getElementManager())->getCohetes();
 
	int cant;
	int longitud;
	glBindTexture( GL_TEXTURE_2D, video_.textures.at(COHETE) );	
	glBegin(GL_QUADS);
if (status==SHOW_TEXT)
	for (it=0; it<cohetes.size();it++){  
		cant = cohetes.at(it)->masas.size() - 2;
		longitud = cant / 2;
		
		for (int i = 0; i < longitud-1; i++)	
		{ 
		MassProxy mp = sms_.getMassProxy(cohetes.at(it)->masas.at(i));
		Vector2<> massPos = mp.getPos();
		MassProxy mp2 = sms_.getMassProxy(cohetes.at(it)->masas.at(i)+1);
		Vector2<> massPos2 = mp2.getPos();
		MassProxy mp3 = sms_.getMassProxy(cohetes.at(it)->masas.at(i)+longitud);
		Vector2<> massPos3 = mp3.getPos();
		MassProxy mp4 = sms_.getMassProxy(cohetes.at(it)->masas.at(i)+longitud+1);
		Vector2<> massPos4 = mp4.getPos();

		
		glTexCoord2i( 1, 0 );
		glVertex3f( massPos2.x, massPos2.y, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( massPos.x, massPos.y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( massPos3.x, massPos3.y , 0.0f );
		glTexCoord2i( 0, 0 );
		glVertex3f( massPos4.x, massPos4.y, 0.0f );

		}
		MassProxy mp = sms_.getMassProxy(cohetes.at(it)->masas.at(cohetes.at(it)->masas.size()-2));
		Vector2<> massPos = mp.getPos();
		MassProxy mp2 = sms_.getMassProxy(cohetes.at(it)->masas.at(longitud-1));
		Vector2<> massPos2 = mp2.getPos();
		MassProxy mp3 = sms_.getMassProxy(cohetes.at(it)->masas.at(cohetes.at(it)->masas.size()-3));
		Vector2<> massPos3 = mp3.getPos();

		glTexCoord2i( 1, 0 );
		glVertex3f( massPos.x, massPos.y, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( massPos2.x, massPos2.y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( massPos3.x, massPos3.y , 0.0f );
		glTexCoord2i( 0, 0 );
		glVertex3f( massPos.x, massPos.y, 0.0f );
	
				

	}
	glEnd();

 //IMPRIME RUEDAS
	vector<Element*> ruedas = (sms_.getElementManager())->getRuedas();
	glBindTexture( GL_TEXTURE_2D, video_.textures.at(RUEDA) );	
	glBegin(GL_QUADS);
	if (status==SHOW_TEXT)
	for (it=0; it<ruedas.size();it++){ 
		MassProxy central = sms_.getMassProxy(ruedas.at(it)->getFirst());
		Vector2<> cenPos = central.getPos();
			
		for (unsigned j = 1; j < ruedas.at(it)->masas.size()-1; j++){
			MassProxy mp = sms_.getMassProxy(ruedas.at(it)->masas.at(j));
			Vector2<> massPos = mp.getPos();
			MassProxy mp2 = sms_.getMassProxy(ruedas.at(it)->masas.at(j+1));
			Vector2<> massPos2 = mp2.getPos();
		
			glTexCoord2i( 0, 0 );			
			glVertex3f(massPos.x, massPos.y, 0.0f );		
			glTexCoord2i( 0, 1);
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
					
			glTexCoord2i( 1, 1 );	
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
			
			glTexCoord2i( 1, 0 );
			glVertex3f(massPos2.x, massPos2.y, 0.0f );					
		}
	
			MassProxy mp = sms_.getMassProxy(ruedas.at(it)->masas.at(ruedas.at(it)->masas.size()-1));
			Vector2<> massPos = mp.getPos();
			MassProxy mp2 = sms_.getMassProxy(ruedas.at(it)->masas.at(1));
			Vector2<> massPos2 = mp2.getPos();
		
			glTexCoord2i( 0, 0 );			
			glVertex3f(massPos.x, massPos.y, 0.0f );		
			glTexCoord2i( 0, 1 );
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
					
			glTexCoord2i( 1, 1 );	
			glVertex3f(cenPos.x, cenPos.y, 0.0f );		
			
			glTexCoord2i( 1, 0 );
			glVertex3f(massPos2.x, massPos2.y, 0.0f );
	}	
	glEnd();
	
//DIBUJO PUNTOS FIJOS 
	glBindTexture( GL_TEXTURE_2D, video_.textures.at(PUNTO_FIJO) );
	if (status==SHOW_TEXT)		
	for (size_t i = 0; i < sms_.getNumMasses(); i++)
	{
		MassProxy mp = sms_.getMassProxy(i);
		Vector2<> massPos = mp.getPos();
		double invMass = mp.getInvMass();
		double massSize;
		massSize = MASS_SIZE/2;
	
		glBegin(GL_TRIANGLE_FAN);
		if (invMass==0)
	
		for (int j = 0; j <= POLY_SIDE; j++){
			glTexCoord2i( 0, 0 );			
			glVertex3f(massPos.x, massPos.y, 0.0f );		
			glTexCoord2i( 1, 0 );
			glVertex3f(massPos.x, massPos.y, 0.0f );		
					
			glTexCoord2i( 1, 1 );	
			glVertex3f(
				massPos.x + massSize *1.4* cos(j * (2 * PI / POLY_SIDE)),
				massPos.y + massSize *1.4* sin(j * (2 * PI / POLY_SIDE)), 0.0f );

			j++;
			glTexCoord2i( 0, 1 );
			glVertex3f(
				massPos.x + massSize *1.4* cos(j * (2 * PI / POLY_SIDE)),
				massPos.y + massSize *1.4* sin(j * (2 * PI / POLY_SIDE)), 0.0f );
			j--;
		}		
	
		glEnd();
	}
		glEnd();


//INDICA TRANSPARENCIAS PARA AREA DE LLEGADA
	glColor4d(1.0, 1.0, 4.0, 0.4);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
//DIBUJO AREAS DE LLEGADA

	vector<ZonaLlegada*> llegadas = (sms_.getElementManager())->getZonasLlegada();
	ZonaLlegada* zona;
	glBindTexture( GL_TEXTURE_2D, video_.textures.at(8) );	
	glBegin(GL_QUADS);	
	
	for (size_t i= 0 ; i<llegadas.size(); i++) 
	{
	zona = llegadas.at(i);
	glTexCoord2i( 0, 0 );
	glVertex3f( zona->getX(), zona->getY(), 0.0f );
	glTexCoord2i( 1, 0 );
	glVertex3f( zona->getX(),zona->getY()-zona->getHeigth(), 0.0f );
	glTexCoord2i( 1, 1 );
	glVertex3f( zona->getX() + zona->getWidth(), zona->getY()-zona->getHeigth(),0.0f );
	glTexCoord2i( 0, 1 );
	glVertex3f( zona->getX() + zona->getWidth(), zona->getY(), 0.0f );
	
	}
	
	glEnd();



	SDL_GL_SwapBuffers();
	glColor4d(1.0, 1.0, 1.0, 1.0);
}
