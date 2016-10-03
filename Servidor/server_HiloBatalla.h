#ifndef __HILO_BATALLA_H__
#define __HILO_BATALLA_H__

#include "server_HiloManejadorCliente.h"
#include "common_Thread.h"
#include "server_Mensaje.h"


class HiloBatalla: public Thread
{
	private:
	
		/* Guarda la referencia al la lista de mensajes recibidos del 
		 * administrador para devolverle el contro luego */
		std::vector<Mensaje*>& mensajesRecibidosAdministrador;
		Mutex& mMensajesRecibidosAdministrador;

		/* Mensajes recibidos propios */
		std::vector<Mensaje*> mensajesRecibidos;

		/* Referencia a jugadores participantes */
		HiloManejadorCliente* jug1;
		HiloManejadorCliente* jug2;
		
		/* True si la partida está en curso. False si terminó */
		bool jugando;
		
		/* Nombre del escenario a utilizar */
		std::string escenario;
		
		/* Devuelve la referencia del jugador con el nombre dado */
		HiloManejadorCliente* obtenerJugador(const std::string& nombre);
		
		/* Devuelve la referencia del jugador contrario al del nombre dado */
		HiloManejadorCliente* obtenerRival(const std::string& nombre);
	
		/* Espero a ver qué resultó de cada una de las simulaciones de los jugadores
		 * Devuelve true si debe terminarse la partida porque alguien ganó (por puntos o abandono) */
		bool analizarResultado();
		
		//resultado:
		bool jugador1conectado;
		bool jugador2conectado;
		int puntosJug1;
		int puntosJug2;
	
	public:
		/* Constructor */
		HiloBatalla(HiloManejadorCliente* jug1, HiloManejadorCliente* jug2, std::string escenario, Mutex& m, std::vector<Mensaje*>& mensajes);

		/* Preparación de la partida doble */
		bool iniciarPartida();
		
		/* Envía los archivos del escenario */
		bool enviarEscenario();
		
		/* True si el desafío sigue activo */
		bool activo();
		
		/* Lógica del hilo. Donde se juega la batalla */
		void run();
		
		/* Consulta de resultados */
		bool jug1Conectado();
		bool jug2Conectado();
		int jug1Puntos();
		int jug2Puntos();
		
		/* Obtención de las referencias de los jugadores */
		HiloManejadorCliente* obtenerJugador1();
		HiloManejadorCliente* obtenerJugador2();
};

#endif

