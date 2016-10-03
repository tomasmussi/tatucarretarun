#ifndef __INTERFAZ_GRAFICA_H__
#define __INTERFAZ_GRAFICA_H__

#include <vector>
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include "common_Thread.h"
#include "client_ContactaServer.h"
#include "client_InterfazBatalla.h"
#include "client_InterfazJugarSolo.h"
#include "client_ChatDoble.h"
#include <iostream>
#include <string>


class InterfazGrafica: public Thread
{
	private:

		/* Elementos de la ventana */
		GtkWidget* ventana;
		GtkWidget* window;
		GtkWidget* entrada;
		GtkWidget* etiqueta;
		GtkWidget* cuadroMensajes;

		/* Contacto con el servidor */
		ContactaServer contacto;
		
		/* Interfaz para juego individual */
		InterfazJugarSolo* juegoSolo;

		/* Usuarios rivales */
		std::string adversarios;
		bool adversariosActualizados;
		
		std::vector<std::string> mensajesLeidos; //Mensajes que fueron escritos por el usuario
		
		/* Elementos de las batallas */
		bool enBatalla;
		InterfazBatalla* batalla;
		ChatDoble* chat;
		
		std::vector<std::string> mensajesAMostrar; //Se deben mostrar en el cuadro de mensajes
		

		/* Se invoca al emitirse la señal clicked del botón */
		static void apretar_boton_aceptar(GtkWidget *widget, gpointer data);

		/* Se invoca al emitirse la señal delete_event */
		static gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

		/* Se invoca al emitirse la señal destroy */
		static void destruir(GtkWidget *widget, gpointer data);

		/* Se invoca cada 100 milisegundos */
		static gboolean timeout_handler(gpointer data);

		/* Analiza si el texto está vacío o no 
		   Se considera vacío si es de longitud cero o solo tiene espacios */
		static bool textoVacio(const std::string& texto);

		/* Analiza cada uno de los mensajes recibidos y actúa en consecuencia de su tipo */
		static void analizarMensajesRecibidos(InterfazGrafica* p);

		/* Se ejecuta al hacer click en el botón conectar. Abre la ventana para setear datos de conexión */
		static void apretar_boton_conectar(GtkWidget *widget, gpointer data);

		/* Intenta conectar con el servidor con los datos de la ventana de conexión */
		static void conectar(GtkWidget *widget, gpointer data);

		/* Cierra la ventana dada por parámetro */
		static void cerrar_ventana(GtkWidget *widget, gpointer data);
		
		/* Se desconecta del Servidor */
		static void desconectar(GtkWidget *widget, gpointer data);		

		/* Se ejecuta al apretar en el boton Jugar Con.
		 * Pide los usuarios disponibles y permite elegir al usuario a quien desafiar */
		static void jugar_batalla(GtkWidget *widget, gpointer data);
		
		/* Obtiene los tokens de la cadena dada */
		static void obtenerTokens(std::string cadena, std::vector<std::string>& tokens);
		
		/* Muestra un GtkDialog con la leyenda dada por encima de la ventanaPadre */
		static void mostrarAlerta(std::string leyenda, GtkWidget* ventanaPadre);
		
		/* Envía solicitud de juego doble con el usuario elegido en la lista */
		static void elegirAdversario(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data);
		
		/* Se recibió una propuesta de batalla. 
		 * Consulta al usuario si desea jugar con esta persona o no */
		static void propuestaBatalla(InterfazGrafica* pthis, const std::string& rival);
		
		/* Envía notificación de rechazo de batalla */
		static void rechazarBatalla(GtkWidget *widget, gpointer data);
		
		/* Inicia una partida doble. Setea todo lo necesario para el comienzo de una interfaz batalla */
		static void iniciarBatalla(InterfazGrafica* pthis);
		
		/* Envía notificación de aceptación de batalla */
		static void aceptarBatalla(GtkWidget *widget, gpointer data);
		
		/* Inicializa una partida individual */
		static void jugar_solo(GtkWidget *widget, gpointer data);

		/* Función que se invoca al pulsar un botón utilizado para pruebas durante el desarrollo */
		static void apretar_boton_conectar_auto(GtkWidget *widget, gpointer data);
		
		/* Se invoca después de un tiempo una vez al principio del programa una sola vez.
		 * Cierra la presentación */
		static gboolean inicio(gpointer data);
		
		/* Genera la ventana que permite modificar la gravedad */
		static void modificar_gravedad(GtkWidget *widget, gpointer data);
		
		/* Modifica el archivo de configuración */
		static void modificar_archivo_gravedad(GtkWidget *widget, gpointer data);
		
		/* Pasa de una entero a un string */
		static void enteroAString(std::string& cadena, ulong entero);
		
		/* Muestra el resultado de un juego individual y consulta si desea seguir jugando o no */
		static void resultado_juego_solo(InterfazGrafica* pthis, const std::string& puntos);
		
		/* Pide al Servidor una tabla de posiciones */
		static void pedir_posiciones(GtkWidget *widget, gpointer data);

	public:

		/* Constructor */
		InterfazGrafica();

		/* Hilo que crea la interfaz gráfica y la ejecuta */
		void run();

		/* Carga un mensaje leído en el parámetro.
		 * Devuelve false si no hay mensajes leídos */
		bool obtenerMensajeLeido(std::string& mensaje);

		/* Agrega un mensaje para mostrar en el cuadro de texto de la interfaz */
		void agregarMensajeAMostrar(const std::string& mensaje);
		

};

#endif

