#ifndef __CLIENT_CONTACTA_SERVER_H__
#define __CLIENT_CONTACTA_SERVER_H__

#include "common_SocketCliente.h"
#include <vector>
#include "common_Thread.h"
#include <iostream>


class ContactaServer: public Thread
{

	private:
		std::vector<std::string> mensajesRecibidos; //Mensajes recibidos desde el server
		bool conectado; //Indica si el socket está conectado o no
		SocketCliente socket; //Socket de contacto con el server


		/* Modifica el valor de conectado */
		void setConectado(bool valor);

	public:

		/* Constructor. Crea el Socket por el que se contactará al Server */
		ContactaServer();

		/* Destructor */
		virtual ~ContactaServer();

		/* Analiza si el socket está conectado o no */
		bool socketConectado();

		/* Función que ejecuta el hilo.
		 * Espera mensajes del Servidor y los va ubicando en una lista 
		 * para que sean posteriormente analizados */
		void run();

		/* Intenta conectarse con el host y el puerto dato.
		 * Retorna true si se pudo conectar */
		bool conectar(const std::string& host, const int puerto);

		/* Devuelve el primer mensaje que se recibió que esté disponible.
		 * Devuelve false si no hay ninguno */
		bool obtenerMensajeRecibido(std::string& mensaje);

		/* Envía un mensaje a través del socket */
		bool enviarMensaje(const std::string& mensaje);

		/* Desconecta al socket */
		void desconectarSocket();

		/* Agrega un mensaje recibido a la lista */
		void agregarMensaje(const std::string& mensaje);

		/* Vacía el vector de mensajes recibidos */
		void vaciarMensajesRecibidos();
		

		

};

#endif

