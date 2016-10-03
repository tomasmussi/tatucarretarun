#ifndef SMS_APP_H
#define SMS_APP_H

#include "config_file.h"
#include "spring_mass_system.h"
#include "video.h"
#include "hiloCron.h"
#include <string>
#include "Reloj.h"
#include <SDL.h>

class SMSApp
{
	/*Muestra el tiempo de simulación en pantalla*/
	Reloj reloj;
	/*Carga el archivo de configuracion*/
	ConfigFile config_;
	/*Contiene información del sistema de masas y resortes*/
	SpringMassSystem sms_;
	/*Carga y maneja texturas y configuraciones gráficas*/
	Video video_;
	uint32_t ticks_;
	int physMult_;
	/*Dibuja la situación actual del SMR*/
	void drawFrame(std::string & time);
	/*Devuelve true si se cumple el objetivo del juego*/
	bool ganaNivel();
	/*Verifica la posición de la masa principal comparandola con
	las areas de llegada*/
	bool estaEnLlegada();
	bool estabaEnLlegada;
	/*Thread que controla el tiempo de simulación*/
	HiloCron* tiempoLlegada;
	HiloCron* cronometro;
public:
	SMSApp(int argc, Mapa* mapa);
	virtual ~SMSApp();
	double run();
	bool sms;
};

#endif
