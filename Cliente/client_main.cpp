#include "common_SocketCliente.h"
#include <iostream>
using namespace std;

#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include "client_InterfazGrafica.h"

#define TITULO_VENTANA "TatuCarreta Run"



int main(int argc, char* argv[])
{
	gtk_init(&argc, &argv);
	InterfazGrafica inter;
	inter.start();
	
	//gtk_main();
	inter.join();

	/*SocketCliente socket;
	string mensaje = argv[2];
	socket.create();
	if (!socket.connect("127.0.0.1",atoi(argv[1]))) cout << "no se conecto" << endl;
	if (!socket.send(mensaje)) cout << "no envio" << endl;
	socket.recv(mensaje);
	cout << mensaje << endl;
	cin >> mensaje;
	while (mensaje.compare("salir") != 0)
	{
		socket.send(mensaje);
		cin >> mensaje;
	}

	socket.send("D");
	socket.close();	*/
}

