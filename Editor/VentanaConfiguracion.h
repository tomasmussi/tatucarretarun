#ifndef __VENTANA_CONFIGURACION_H__
#define __VENTANA_CONFIGURACION_H__

#include "Mapa.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <string>


class VentanaConfiguracion;

/* Estructura de datos auxiliar para pasar parámetros */
typedef struct datosConfiguracion
{
	GtkWidget* textoNom;
	GtkWidget* textoPlata;
	VentanaConfiguracion* pthis;
	GtkWidget* ventana;
		
	//Botones de elección
	GtkWidget* elMasa;
	GtkWidget* elPuntoFijo;
	GtkWidget* elRueda;
	GtkWidget* elBarra;
	GtkWidget* elPlataforma;
	GtkWidget* elSoga;
	GtkWidget* elCinta;
	GtkWidget* elCohete;
	
	//Espacios de entrada
	GtkWidget* textoPlataMasa;
	GtkWidget* textoPlataPuntoFijo;
	GtkWidget* textoPlataRueda;
	GtkWidget* textoPlataBarra;
	GtkWidget* textoPlataPlataforma;
	GtkWidget* textoPlataSoga;
	GtkWidget* textoPlataCinta;
	GtkWidget* textoPlataCohete;
	GtkWidget* textoTiempoCohete;
	
}datosConfiguracion;


class VentanaConfiguracion
{
	private:
		Mapa* mapa; //Mapa editado
		datosConfiguracion* datos;

		/* Cierra una ventana pasada por parámetro */
		static void cerrar_ventana(GtkWidget *widget, gpointer data);
		
		/* Analiza si la cadena es un texto vacío o únicamente de espacios */
		static bool textoVacio(const std::string& texto);
		
		/* Muestra un GtkDialog con la leyenda dada */
		static void mostrarAlerta(std::string& leyenda, GtkWidget* ventanaPadre);
		
		/* Cierra Gtk */
		static void destruir(GtkWidget *widget, gpointer data);
		
		/* Pasa de entero a string */
		static void enteroAString(std::string& cadena, ulong entero);
		
		/* Analiza si la cadena está formada por números o no */
		static bool esNumero(const std::string& cadena);


	public:
		/* Constructor */
		VentanaConfiguracion(Mapa* mapa, GtkWidget* ventanaPpal);
		
		/* Destructor */
		virtual ~VentanaConfiguracion();
		
		/* Persiste el mapa que se está editando */
		static void guardar(GtkWidget *widget, gpointer data);

};

#endif

