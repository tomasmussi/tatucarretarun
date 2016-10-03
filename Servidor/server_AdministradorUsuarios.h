#ifndef __ADMINISTRADOR_USUARIOS_H__
#define __ADMINISTRADOR_USUARIOS_H__

#include <map>
#include <vector>
#include <string>
#include "common_Thread.h"
#include "server_HiloManejadorCliente.h"
#include "server_Mensaje.h"
#include "server_HiloBatalla.h"


#define MAX_PARTIDAS 10

class AdministradorUsuarios: public Thread
{
	private:
	
		/* Cantidad de partidas que se están jugando */
		unsigned int cantPartidas;
	
		/* Mapa de usuarios conectados. La clave corresponde al nombre del usuario */
		std::map<std::string,HiloManejadorCliente*> usuarios;

		/* Vector de escenarios disponibles para enviar a los usuarios */
		std::vector<std::string> escenarios;

		/* Ruta del archivo donde se almacenan los puntajes históricos de los usuarios */
		std::string rutaPuntajes;
		
		/* Mapa que almacena los puntajes de los usuarios. La clave es el nombre de usuario */ 
		std::map<std::string,unsigned int> puntajes;
		
		/* Vector que almacena todos los hilos de batallas en curso */
		std::vector<HiloBatalla*> desafios;

		/* Mutex que evita complicaciones en los hilos que leen o agregan mensajes recibidos
		 * desde los clientes */
		Mutex mMensajesRecibidos;
		/* Vector de mensajes recibidos por los clientes */
		std::vector<Mensaje*> mensajesRecibidos;

		/* Carga el vector de escenarios en base a los que encuentra en el archivo de ruta dada */
		void levantarEscenarios(const std::string& rutaEscenarios);
		
		/* Carga el vector de puntajes históricos en base a los datos que se encuentran en el
		 * archivo dado */
		void levantarPuntajes();
		
		/* Guarda el vector de puntajes en el archivo de puntajes históricos */
		void grabarPuntajes();

		/* Analiza el mensaje dado y actúa en consecuencia llamando al método que corresponda */
		void analizarMensaje(Mensaje* mensaje);

		/* Quita al jugador de la lista de usuarios conectados y frena el hilo receptor de datos
		 * correspondiente */
		void desconectarJugador(std::string nombre);
		
		/* Inicia un juego individual para el usuario dado */
		void iniciarJuegoSolitario(std::string nombre);
		
		/* Envía mensaje de chat a todos los usuarios que no están jugando */
		void enviarMensajeChat(Mensaje* mensaje);
		
		/* Indica que el usuario deja de estar jugando y actualiza el puntaje */
		void finPartidoSolitario(Mensaje* mensaje);
		
		/* Envía una lista de usuarios conectados al jugador dado por parámetro */
		void enviarUsuarios(const std::string& nombre);
		
		/* Envía notificación de invitación a desafío al usuario indicado en el texto del mensaje */
		void invitarDesafio(Mensaje* mensaje);
		
		/* Notifica al usuario del texto del mensaje que su propuesta de juego ha sido rechazada */
		void notificarRechazo(Mensaje* mensaje);
		
		/* Inicia el procedimiento de Juego Doble */
		void aceptarDesafio(Mensaje* mensaje);

		/* Devuelve un escenario al azar entre los disponibles */
		bool pedirEscenario(std::string& nombre);
		
		/* Elimina y cierra limpiamente los desafíos que se terminaron */
		void mantenerDesafios();
		
		/* Envía un mensaje a todos los usuarios que no están jugando */
		void enviarMensajeATodos(const std::string& mensaje);
		
		/* Devuelve una cadena con el ranking de posiciones cargadas */
		void obtenerRanking(std::string& ranking, const std::string& jugador);
		
		/* Obtiene el jugador que tiene menos puntos dentro del mapa dado */
		void obtenerMenor(std::map<std::string,unsigned int>& lideres, std::string& menor, unsigned int& ptosMenor);
		
		/* Envía el ranking al usuario dado */
		void enviarRanking(const std::string& nombre);

	public:

		/* Constructor. Recibe la ruta donde se almacenan los puntajes y donde se almacenan
		 * los escenarios */
		AdministradorUsuarios(const std::string rutaPuntajes, const std::string rutaEscenarios);
		
		/* Destructor */
		virtual ~AdministradorUsuarios();

		/* Guarda en el parámetro el nombre de un escenario aleatorio que no haya sido
		   utilizado por el cliente.
		 * Devuelve false si no hay escenarios disponibles */
		bool pedirEscenario(std::string& nombre, HiloManejadorCliente* cliente);

		/* Incrementa el puntaje del usuario en la cantidad indicada */
		bool actualizarPuntaje(std::string nombre, unsigned int puntos);

		/* Agrega un jugador entre los que actualmente están conectados */
		bool conectarJugador(const std::string nombre,  Socket& socket);

		/* Agrega un usuario nuevo a la base de datos de usuarios.
		 * Devuelve false si el nombre de usuario ya existe */
		bool agregarUsuario(std::string nombre);
		
		/* Analiza si el usuario existe o no */
		bool usuarioExiste(std::string nombre);

		/* Lógica principal del hilo */
		void run();

};

#endif

