#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>


#define SOCKET_INVALIDO -1
#define BACKLOG 10
#define TAM_BUFFER 256

class Socket
{

	private:
		int id; //Identificador del Socket
		sockaddr_in dir; //Mi dirección
		char historial[TAM_BUFFER + 1]; //Bytes remanentes del recv
		unsigned int validosHistorial; //Cantidad de bytes remanentes del recv

	public:
		/* Constructor */
		Socket();

		/* Destructor */
		virtual ~Socket();

		/* Crea al socket. Devuelve true si fue una creación exitosa. */
		bool create();

		/* Envía a través del Socket el dato indicado.
		 * Devuelve true si pudo enviar correctamente */
		bool send(const std::string& dato) const;

		/* Recibe un mensaje a través del Socket y lo almacena en dato.
		 * Devuelve true si pudo recibir correctamente */
		bool recv(std::string& dato);

		/* Modifica el Id del Socket */
		void setId(const int id);

		/* Devuelve el Id del Socket */
		int getId() const;

		/* Devuelve mi dirección */
		sockaddr_in& getDir();

		/* Cierra el Socket */
		void close();

		void shutdown(int modo);

};

#endif

