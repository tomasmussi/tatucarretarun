#include "client_InterfazBatalla.h"
#include "ParserXML.h"
#include "InterfazEdicion.h"
#include <fstream>
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
using namespace std;

InterfazBatalla::InterfazBatalla(ContactaServer* contacto)
{
	this->contacto = contacto;
	jugando = true;
}


void InterfazBatalla::iniciarBatalla(ChatDoble* chat)
{
	this->chat = chat;

	//Recibo los archivos
	bool recibidojpg = false;
	bool recibidoxml = false;
	bool empezar = false;
	string mensaje;
	string archivo;
	
	ParserXML par;
	
	while ((!recibidojpg) || (!recibidoxml))
	{
		if (contacto->obtenerMensajeRecibido(mensaje))
		{
			
			//Pregunta por tipo listo
			if (mensaje[0] == 'A')
			{

				if (!recibidojpg)
				{
					desconvertir(archivo,mensaje.substr(1,mensaje.length()-1));
					ofstream arch("escenario.jpg");
					arch << archivo;
					arch.close();
					recibidojpg = true;	
				}
				else
				{
					desconvertir(archivo,mensaje.substr(1,mensaje.length()-1));
					mapa = par.obtenerMapaMemoria(archivo);
					recibidoxml = true;
				}
			}
		}
		sleep(0);
	}
	
	//Envia mensaje de listo
	contacto->enviarMensaje("L");

	//Espera mensaje de jugar
	while (!empezar)
	{
		if (contacto->obtenerMensajeRecibido(mensaje))
		{	
			//Pregunta por tipo listo
			if (mensaje[0] == 'J')
				empezar = true;
		}
		sleep(0);
	}
	
	
	movimientos = new HiloMovimientos(mapa);
	movimientos->start();

	
}

char InterfazBatalla::obtenerValor (char hexa)
{
	switch (hexa)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
	}
	return -1;
}

void InterfazBatalla::run()
{
	InterfazEdicion interfaz(mapa);
	interfaz.comenzar(chat);
	
	//set puntos;
	puntos = mapa->getPuntos();
	
	if (puntos < 0)
		// huyo
		contacto->enviarMensaje("H");
	
	movimientos->terminar();
	movimientos->join();
	
	setJugando(false);


}

void InterfazBatalla::desconvertir(string& resultado, const string& original)
{
	unsigned int i = 0;
	char buf;
	char bufaux;
	resultado = "";
	for ( ; i < original.length() ; i += 2 )
	{
		buf = obtenerValor(original[i]);
		buf = buf << 4;
		bufaux = obtenerValor(original[i+1]);
		buf = buf | bufaux;
		resultado += buf;
	}
}

bool InterfazBatalla::analizarMensajesRecibidos()
{

	string mensaje;
	string xml;
	GtkWidget *alerta;
	GtkWidget* label;
	GtkWidget* boton;

	while (chat->obtenerEntrada(mensaje))
	{
		contacto->enviarMensaje(mensaje);
	}
	
	// analizar los movimientos
	if (movimientos->obtenerMovimiento(xml))
		contacto->enviarMensaje("M" + xml);
	
	while (contacto->obtenerMensajeRecibido(mensaje))
	{

		switch (mensaje[0])
		{
			case 'C': //chat
				chat->agregarLinea(mensaje.substr(1,mensaje.length() - 1));
				break;
			case 'D': //desconexión
				chat->setSalir(true);
				return false; //Se desconectó el servidor
			case 'S':
				//simular
				chat->setSimular(true);
				break;
			case 'M': //movimiento
				chat->cambiarMovimiento(mensaje.substr(1,mensaje.length() - 1));
				break;
			case 'H': //huyó
				//avisa a la ventana que debe morir
				chat->setSalir(true);
				
				mapa->setPuntos(100); // premio consuelo porque escapa el otro
				
				this->join();
				
				alerta = gtk_dialog_new ();
				label = gtk_label_new ("Su rival abandonó la partida");
				gtk_window_set_title (GTK_WINDOW (alerta), "TatuCarreta");
	
				gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->vbox), label, FALSE, FALSE, 0);

				boton = gtk_button_new_from_stock("Aceptar");
				gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->action_area), boton, FALSE, FALSE, 0);

				gtk_window_set_position(GTK_WINDOW(alerta),GTK_WIN_POS_CENTER);

				gtk_signal_connect_object (GTK_OBJECT (boton), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT 							(alerta));

				gtk_widget_show_all (alerta);
				
				break;
				
			case 'I':
				//alguien gano
				chat->setSalir(true);
				
				this->join();
				
				alerta = gtk_dialog_new ();
				label = gtk_label_new ((mensaje.substr(1,mensaje.length()-1)).c_str());
				gtk_window_set_title (GTK_WINDOW (alerta), "TatuCarreta");	
				gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->vbox), label, FALSE, FALSE, 0);

				boton = gtk_button_new_from_stock("Aceptar");
				gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->action_area), boton, FALSE, FALSE, 0);

				gtk_window_set_position(GTK_WINDOW(alerta),GTK_WIN_POS_CENTER);

				gtk_signal_connect_object (GTK_OBJECT (boton), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT 							(alerta));

				gtk_widget_show_all (alerta);
				
				break;
			case 'X': //bloquear
				chat->setBloquear(true);
				break;
		}
	}
	return true;
}

bool InterfazBatalla::estaJugando()
{
	bool ret;
	bloquearEstado();
	ret = jugando;
	desbloquearEstado();
	return ret;
}

int InterfazBatalla::getPuntos()
{
	int ret;
	bloquearEstado();
	ret = puntos;
	desbloquearEstado();
	return ret;
}

void InterfazBatalla::setJugando(bool valor)
{
	bloquearEstado();
	jugando = valor;
	desbloquearEstado();
}


