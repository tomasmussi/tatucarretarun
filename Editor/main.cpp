#include "InterfazEdicion.h"
#include "Mapa.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <string>
#include "ParserXML.h"
#include "VentanaConfiguracion.h"
#include <sys/stat.h> 
using namespace std;

VentanaConfiguracion* configuracion;

// estructura de datos auxiliar para pasaje de parámetros
typedef struct datosAbrir
{
	GtkWidget* ventanaPpal;
	GtkWidget* entrada;
	GtkWidget* ventanaAbrir;
}datosAbrir;

/* Creación de escenario desde cero */
void crear_nuevo(GtkWidget *widget, gpointer data)
{
	GtkWidget* ventana = (GtkWidget*) data;
	Mapa* mapa = new Mapa(); // mapa nuevo
	
	// se ejecuta la edición
	InterfazEdicion inter(mapa);
	inter.comenzar();
	
	// si el usuario pidió guardar se pasa a la configuración
	if (mapa->debeGuardarse())
		configuracion = new VentanaConfiguracion(mapa,ventana);
	else
	{
		delete mapa;
		gtk_widget_destroy(ventana);
	}
}

/* Devuelve true si el texto es vacío o solamente tiene espacios */
bool textoVacio(const string& texto)
{
	unsigned int i;
	bool retorno = true;
	for (i = 0 ; i < texto.length() ; i ++)
	{
		if (texto[i] != ' ')
		{
			retorno = false;
			break;
		}
	}
	return retorno;
}

/* Analiza si existe el archivo del escenario dado 
 * Devuelve true si existe, false si no */
bool existeEscenario(const string& escenario)
{
	struct stat stFileInfo;
	string strFilename; 
	int intStat;

	strFilename = "escenarios/" + escenario +".xml";

	intStat = stat(strFilename.c_str(),&stFileInfo);
	if(intStat != 0)
		return false;
  
	return true;
}

/* Se intenta abrur un escenario existente */
void abrir_escenario(GtkWidget *widget, gpointer data)
{
	datosAbrir* datos = (datosAbrir*) data;

	string nomArchivo;
	string escenario = gtk_entry_get_text(GTK_ENTRY(datos->entrada));

	gtk_entry_set_text(GTK_ENTRY(datos->entrada),"");

	if (textoVacio(escenario))
		return;
		
	//validar archivo inexistente
	if (!existeEscenario(escenario))
	{
		GtkWidget *alerta = gtk_dialog_new ();
		GtkWidget* label = gtk_label_new ("Escenario inexistente");
	
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->vbox), label, FALSE, FALSE, 0);

		GtkWidget* boton = gtk_button_new_from_stock("Aceptar");
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->action_area), boton, FALSE, FALSE, 0);
	
		gtk_window_set_title(GTK_WINDOW(alerta),"TatuCarreta");

		gtk_window_set_transient_for (GTK_WINDOW(alerta),GTK_WINDOW(datos->ventanaAbrir));
		gtk_window_set_position(GTK_WINDOW(alerta),GTK_WIN_POS_CENTER_ON_PARENT);
		gtk_window_set_title(GTK_WINDOW(alerta),"Error");

		gtk_signal_connect_object (GTK_OBJECT (boton), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT 					(alerta));

		gtk_widget_show_all (alerta);
		return;
	}
		
	
	nomArchivo = "escenarios/" + escenario + ".xml";
	ParserXML par;
	Mapa* mapa;
	
	// se obtiene el mapa desde el parser
	mapa = par.obtenerMapaArchivo(nomArchivo);
	
	gtk_widget_destroy(datos->ventanaAbrir);

	// inicia edición
	InterfazEdicion inter(mapa);
	inter.comenzar();
	
	// si el usuario pide guardarse, se llama a la ventana de configuración
	if (mapa->debeGuardarse())
		configuracion = new VentanaConfiguracion(mapa,datos->ventanaPpal);
	else
	{
		delete mapa;
		gtk_widget_destroy(datos->ventanaPpal);
	}

	delete datos;
}

/* Ventana que permite abrir un escenario existente */
void abrir_existente(GtkWidget *widget, gpointer data)
{
	configuracion = NULL;

	GtkWidget* ventana = (GtkWidget*) data;
	GtkWidget* ventanaAbrir = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* contenedor = gtk_vbox_new(FALSE,10);
	GtkWidget* label = gtk_label_new("Nombre de escenario");
	GtkWidget* botonAceptar = gtk_button_new_from_stock("Abrir");
	
	GtkWidget* entrada = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(entrada), 30);

	gtk_window_set_transient_for (GTK_WINDOW(ventanaAbrir),GTK_WINDOW(ventana));
	gtk_window_set_position(GTK_WINDOW(ventanaAbrir),GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_title(GTK_WINDOW(ventanaAbrir),"TatuCarreta");
	
	gtk_container_add(GTK_CONTAINER(ventanaAbrir), contenedor);

	gtk_box_pack_start (GTK_BOX(contenedor), label, FALSE, FALSE, 10);
	gtk_box_pack_start (GTK_BOX(contenedor), entrada, TRUE, TRUE, 10);
	gtk_box_pack_start (GTK_BOX(contenedor), botonAceptar, FALSE, FALSE, 10);
	
	datosAbrir* datos = new datosAbrir();
	datos->ventanaPpal = ventana;
	datos->ventanaAbrir = ventanaAbrir;
	datos->entrada = entrada;
	
	g_signal_connect(G_OBJECT(botonAceptar), "clicked", G_CALLBACK(abrir_escenario), datos);
	
	gtk_widget_show_all(ventanaAbrir);
}

/* Cierra el Gtk */
void destruir(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}


int main(int argc, char* argv[])
{

	gtk_init(&argc, &argv);
	
	/* Ventana con menú principal */
	GtkWidget* ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* contenedor = gtk_vbox_new(FALSE,10);
	GtkWidget* botonNuevo = gtk_button_new_from_stock("Crear nuevo escenario");
	GtkWidget* botonAbrir = gtk_button_new_from_stock("Abrir escenario existente");
	GtkWidget* botonCerrar = gtk_button_new_from_stock("Cerrar editor");
	
	gtk_window_set_position(GTK_WINDOW(ventana),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(ventana), 10);
	gtk_window_set_title (GTK_WINDOW (ventana), "Editor :: TatuCarreta Run");
	gtk_window_set_default_size (GTK_WINDOW (ventana), 300, 100);
	gtk_window_set_resizable(GTK_WINDOW (ventana),FALSE);
	
	gtk_container_add(GTK_CONTAINER(ventana), contenedor);

	gtk_box_pack_start (GTK_BOX(contenedor), botonNuevo, FALSE, FALSE, 10);
	gtk_box_pack_start (GTK_BOX(contenedor), botonAbrir, FALSE, FALSE, 10);
	gtk_box_pack_start (GTK_BOX(contenedor), botonCerrar, FALSE, FALSE, 10);
	
	g_signal_connect(G_OBJECT(botonNuevo), "clicked", G_CALLBACK(crear_nuevo), ventana);	
	g_signal_connect(G_OBJECT(botonAbrir), "clicked", G_CALLBACK(abrir_existente), ventana);
	g_signal_connect(G_OBJECT(botonCerrar), "clicked", G_CALLBACK(destruir), NULL);
	g_signal_connect(G_OBJECT(ventana), "destroy", G_CALLBACK(destruir), NULL);
	
	gtk_widget_show_all (ventana);
	
	gtk_main();
	
	if (configuracion != NULL)
		delete configuracion;
	
}

