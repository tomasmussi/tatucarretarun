#ifndef __HILO_MANEJADOR_CLIENTE_H__
#define __HILO_MANEJADOR_CLIENTE_H__

#include "common_Socket.h"
#include "common_Thread.h"
#include "server_Mensaje.h"
#include <string>
#include <vector>

class HiloManejadorCliente: public Thread
{

	private:
		Socket socket; //Socket que contacta al cliente.
		std::string nombre; //Nombre del cliente
		Mutex& mMensajesRecibidos; //Mutex para la lista de mensajes recibidos
		std::vector<Mensaje*>* mensajesRecibidos; //Lista de mensajes recibidos
		
		/* Vector de escenarios ya jugados por el usuario */
		std::vector<std::string> escenariosJugados;

		/* Indica si está en una partida o no */
		bool jugando;
		
		/* Convierte un nibble en un caracter hexadecimal */
		char ntoh(char n);
		
		/* Codifica en hexadecimal */
		void convertir(std::string& mensaje, char buf);

	public:

		/* Constructor. Recibe el socket a través del cual se hará la conexión con el Cliente */
		HiloManejadorCliente(const std::string& nombre, const Socket& socket,Mutex& m, std::vector<Mensaje*>& mensajes);

		/* Destructor */
		~HiloManejadorCliente();

		/* Va recibiendo los mensajes desde el cliente y los analiza */
		void run();

		/* Devuelve true si el cliente está jugando una partida */
		bool estaJugando();

		/* Modifica el atributo que indica si el Cliente está jugando o no */
		void setJugando(bool valor);

		/* Envía un mensaje de Chat al cliente */
		void enviarMensajeChat(Mensaje* mensaje);	

		/* Envía un mensaje */
		void enviarMensaje(std::string mensaje);
		
		/* Redirecciona el flujo de almacenamiento de los mensajes recibidos */
		void setMensajesRecibidos(std::vector<Mensaje*>& mensajes);
		
		/* Devuelve la referencia a los mensajes recibidos */
		std::vector<Mensaje*>& getMensajesRecibidos();
		
		/* Envía un archivo codificado */
		void enviarArchivo(std::string ruta);
		
		/* Retorno el nombre del usuario */
		std::string& getNombre();
		
		/* Analiza si ya se jugó o no el escenario dado */
		bool escenarioJugado(const std::string& nombre);
		
		/* Agrega un escenario como ya jugado */
		void agregarEscenarioJugado(const std::string& nombre);
		
		/* Vacía la lista de escenarios jugados */
		void vaciarEscenariosJugados();
		
		/* Shutdown del Socket */
		void desconectar();
		

};

#endif
