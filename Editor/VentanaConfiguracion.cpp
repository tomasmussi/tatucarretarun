#include "VentanaConfiguracion.h"
#include "ParserXML.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <string>
#include <ctype.h>

using namespace std;


bool VentanaConfiguracion::textoVacio(const string& texto)
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

bool VentanaConfiguracion::esNumero(const std::string& cadena)
{
	//analiza letra por letra si es no cadena
	for (unsigned int i = 0; i < cadena.length(); i++)
	{
		if (!isdigit(cadena[i]))
			return false;
	}
	return true;
}

void VentanaConfiguracion::mostrarAlerta(string& leyenda, GtkWidget* ventanaPadre)
{
	GtkWidget *alerta = gtk_dialog_new ();
	GtkWidget* label = gtk_label_new (leyenda.c_str());
	
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->vbox), label, FALSE, FALSE, 0);

	GtkWidget* boton = gtk_button_new_from_stock("Aceptar");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->action_area), boton, FALSE, FALSE, 0);
	
	gtk_window_set_title(GTK_WINDOW(alerta),"TatuCarreta");

	gtk_window_set_transient_for (GTK_WINDOW(alerta),GTK_WINDOW(ventanaPadre));
	gtk_window_set_position(GTK_WINDOW(alerta),GTK_WIN_POS_CENTER_ON_PARENT);

	g_signal_connect(G_OBJECT(boton), "clicked", G_CALLBACK(cerrar_ventana), alerta);

	gtk_widget_show_all (alerta);
}

void VentanaConfiguracion::guardar(GtkWidget *widget, gpointer data)
{

	datosConfiguracion* datos = (datosConfiguracion*) data;
	
	ParserXML parser;
	
	// obtiene todas las entradas de datos
	string nombre = gtk_entry_get_text(GTK_ENTRY(datos->textoNom));
	string plata = gtk_entry_get_text(GTK_ENTRY(datos->textoPlata));
	string plataMasa = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataMasa));
	string plataPuntoFijo = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataPuntoFijo));
	string plataRueda = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataRueda));
	string plataBarra = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataBarra));
	string plataPlataforma = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataPlataforma));
	string plataSoga = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataSoga));
	string plataCinta = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataCinta));
	string plataCohete = gtk_entry_get_text(GTK_ENTRY(datos->textoPlataCohete));
	
	// entradas en entero
	int plataEnteroMasa;
	int plataEnteroPuntoFijo;
	int plataEnteroRueda;
	int plataEnteroBarra;
	int plataEnteroPlataforma;
	int plataEnteroSoga;
	int plataEnteroCinta;
	int plataEnteroCohete;
	
	// datos del tiempo de cohete
	string tiempoCohete = gtk_entry_get_text(GTK_ENTRY(datos->textoTiempoCohete));
	int tiempoEnteroCohete;
	
	int plataEntero;
	string archivo = "./escenarios/" + nombre + ".xml";

	
	// analiza si le faltó ingresar algún dato (casilla vacía)
	if ((textoVacio(nombre)) || (textoVacio(plata)) || (textoVacio(plataMasa)) || (textoVacio(plataPuntoFijo)) || (textoVacio(plataRueda)) || (textoVacio(plataBarra)) || (textoVacio(plataPlataforma)) || (textoVacio(plataSoga)) || (textoVacio(plataCinta)) || (textoVacio(plataCohete)))
	{
		string leyenda = "Faltan ingresar datos";
		mostrarAlerta(leyenda,datos->ventana);
		return;
	}
	
	// analiza si los datos ingresados en casillas numéricas son efectivamente numéricos
	if (!esNumero(plata) || !esNumero(plataMasa) || !esNumero(plataPuntoFijo) 
	      || !esNumero(plataRueda) || !esNumero(plataBarra) || !esNumero(plataPlataforma) 
	      || !esNumero(plataSoga) || !esNumero(plataCinta) || !esNumero(plataCohete) 
	      || !esNumero(tiempoCohete))
	{
		string leyenda = "Datos numéricos incorrectos";
		mostrarAlerta(leyenda,datos->ventana);
		return;
	}

	// obtiene los datos ingresados en forma de entero
	plataEntero = atoi(plata.c_str());
	plataEnteroMasa = atoi(plataMasa.c_str());
	plataEnteroPuntoFijo = atoi(plataPuntoFijo.c_str());
	plataEnteroRueda = atoi(plataRueda.c_str());
	plataEnteroBarra = atoi(plataBarra.c_str());
	plataEnteroPlataforma = atoi(plataPlataforma.c_str());
	plataEnteroSoga = atoi(plataSoga.c_str());
	plataEnteroCinta = atoi(plataCinta.c_str());
	plataEnteroCohete = atoi(plataCohete.c_str());
	tiempoEnteroCohete = atoi(tiempoCohete.c_str());
	
	// analiza si fue un ingreso válido en entero
	if ((plataEntero < 0) || (plataEnteroMasa < 0) || (plataEnteroPuntoFijo < 0) || (plataEnteroBarra < 0) || (plataEnteroPlataforma < 0) || (plataEnteroSoga < 0) || (plataEnteroCinta < 0) || (plataEnteroCohete < 0))
	{
		string leyenda = "Dinero incorrecto";
		mostrarAlerta(leyenda,datos->ventana);
		return;
	}
	
	if (tiempoEnteroCohete < 0)
	{
		string leyenda = "Tiempo de cohete incorrecto";
		mostrarAlerta(leyenda,datos->ventana);
		return;
	}
	
	
	
	//Modifica el mapa
	datos->pthis->mapa->setNombre(nombre);
	datos->pthis->mapa->setPlata(plataEntero);
	
	//Analiza si se habilitó o no cada elemento
	if (!GTK_TOGGLE_BUTTON (datos->elMasa)->active)
		datos->pthis->mapa->deshabilitarElemento(MASA);
	else
		datos->pthis->mapa->habilitarElemento(MASA);
	if (!GTK_TOGGLE_BUTTON (datos->elPuntoFijo)->active)
		datos->pthis->mapa->deshabilitarElemento(PUNTO_FIJO);
	else
		datos->pthis->mapa->habilitarElemento(PUNTO_FIJO);
	if (!GTK_TOGGLE_BUTTON (datos->elRueda)->active)
		datos->pthis->mapa->deshabilitarElemento(RUEDA);
	else
		datos->pthis->mapa->habilitarElemento(RUEDA);
	if (!GTK_TOGGLE_BUTTON (datos->elBarra)->active)
		datos->pthis->mapa->deshabilitarElemento(BARRA);
	else
		datos->pthis->mapa->habilitarElemento(BARRA);
	if (!GTK_TOGGLE_BUTTON (datos->elPlataforma)->active)
		datos->pthis->mapa->deshabilitarElemento(PLATAFORMA);
	else
		datos->pthis->mapa->habilitarElemento(PLATAFORMA);
	if (!GTK_TOGGLE_BUTTON (datos->elSoga)->active)
		datos->pthis->mapa->deshabilitarElemento(SOGA);
	else
		datos->pthis->mapa->habilitarElemento(SOGA);
	if (!GTK_TOGGLE_BUTTON (datos->elCinta)->active)
		datos->pthis->mapa->deshabilitarElemento(CINTA);
	else
		datos->pthis->mapa->habilitarElemento(CINTA);
	if (!GTK_TOGGLE_BUTTON (datos->elCohete)->active)
		datos->pthis->mapa->deshabilitarElemento(COHETE);
	else
		datos->pthis->mapa->habilitarElemento(COHETE);
		
	//Setea el costo de cada elemento
	datos->pthis->mapa->setPrecio(MASA,plataEnteroMasa);
	datos->pthis->mapa->setPrecio(PUNTO_FIJO,plataEnteroPuntoFijo);
	datos->pthis->mapa->setPrecio(RUEDA,plataEnteroRueda);
	datos->pthis->mapa->setPrecio(BARRA,plataEnteroBarra);
	datos->pthis->mapa->setPrecio(PLATAFORMA,plataEnteroPlataforma);
	datos->pthis->mapa->setPrecio(SOGA,plataEnteroSoga);
	datos->pthis->mapa->setPrecio(CINTA,plataEnteroCinta);
	datos->pthis->mapa->setPrecio(COHETE,plataEnteroCohete);
	
	//Setea el tiempo del cohete
	datos->pthis->mapa->setTiempoCohete(tiempoEnteroCohete);
		
	parser.obtenerXMLArchivo(archivo,datos->pthis->mapa);
	
	gtk_widget_destroy(datos->ventana);

	gtk_main_quit();
}


VentanaConfiguracion::VentanaConfiguracion(Mapa* mapa, GtkWidget* ventanaPpal)
{
	this->mapa = mapa;
	
	// ventana
	GtkWidget* ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(ventana),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(ventana), 10);
	gtk_window_set_title (GTK_WINDOW (ventana), "Editor :: TatuCarreta Run");
	gtk_window_set_default_size (GTK_WINDOW (ventana), 300, 100);
	gtk_window_set_resizable(GTK_WINDOW (ventana),FALSE);
	
	// etiquetas
	GtkWidget* etNombre = gtk_label_new ("Nombre del escenario");
	GtkWidget* etPlata = gtk_label_new ("Dinero disponible");
	GtkWidget* etTitulo = gtk_label_new ("Elementos disponibles");
	
	GtkWidget* etCostoMasa = gtk_label_new (" | Costo:");
	GtkWidget* etCostoPuntoFijo = gtk_label_new (" | Costo:");
	GtkWidget* etCostoRueda = gtk_label_new (" | Costo:");
	GtkWidget* etCostoBarra = gtk_label_new (" | Costo:");
	GtkWidget* etCostoPlataforma = gtk_label_new (" | Costo:");
	GtkWidget* etCostoSoga = gtk_label_new (" | Costo:");
	GtkWidget* etCostoCinta = gtk_label_new (" | Costo:");
	GtkWidget* etCostoCohete = gtk_label_new (" | Costo:");
	GtkWidget* etTiempoCohete = gtk_label_new (" | Tiempo en ms (cero para infinito):");
	
	// casillas de entrada de texto
	GtkWidget* textoNom = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoNom), 20); 
	GtkWidget* textoPlata = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlata), 20); 
	GtkWidget* textoPlataMasa = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataMasa), 20);
	GtkWidget* textoPlataPuntoFijo = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataPuntoFijo), 20);
	GtkWidget* textoPlataRueda = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataRueda), 20);
	GtkWidget* textoPlataBarra = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataBarra), 20);
	GtkWidget* textoPlataPlataforma = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataPlataforma), 20);
	GtkWidget* textoPlataSoga = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataSoga), 20);
	GtkWidget* textoPlataCinta = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataCinta), 20);
	GtkWidget* textoPlataCohete = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoPlataCohete), 20);
	GtkWidget* textoTiempoCohete = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(textoTiempoCohete), 20);

	// botones de elección
	GtkWidget* elMasa = gtk_check_button_new_with_label  ("Masas");
	GtkWidget* elPuntoFijo = gtk_check_button_new_with_label  ("Puntos fijos");
	GtkWidget* elRueda = gtk_check_button_new_with_label ("Ruedas");
	GtkWidget* elBarra = gtk_check_button_new_with_label  ("Barras metálicas");
	GtkWidget* elPlataforma = gtk_check_button_new_with_label  ("Plataformas metálicas");
	GtkWidget* elSoga = gtk_check_button_new_with_label  ("Sogas");
	GtkWidget* elCinta = gtk_check_button_new_with_label  ("Cintas de lona");
	GtkWidget* elCohete = gtk_check_button_new_with_label  ("Cohetes");
	
	
	gtk_entry_set_text(GTK_ENTRY(textoNom), (mapa->getNombre()).c_str());
	
	// inicializa los botones de elección
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elMasa),mapa->elementoHabilitado(MASA));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elPuntoFijo),mapa->elementoHabilitado(PUNTO_FIJO));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elRueda),mapa->elementoHabilitado(RUEDA));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elBarra),mapa->elementoHabilitado(BARRA));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elPlataforma),mapa->elementoHabilitado(PLATAFORMA));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elSoga),mapa->elementoHabilitado(SOGA));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elCinta),mapa->elementoHabilitado(CINTA));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(elCohete),mapa->elementoHabilitado(COHETE));
	
	string numero;
	
	// inicializa las entradas
	enteroAString(numero,mapa->getPlata());
	gtk_entry_set_text(GTK_ENTRY(textoPlata), numero.c_str());
	enteroAString(numero,mapa->getPrecio(MASA));
	gtk_entry_set_text(GTK_ENTRY(textoPlataMasa), numero.c_str());
	enteroAString(numero,mapa->getPrecio(PUNTO_FIJO));
	gtk_entry_set_text(GTK_ENTRY(textoPlataPuntoFijo), numero.c_str());
	enteroAString(numero,mapa->getPrecio(RUEDA));
	gtk_entry_set_text(GTK_ENTRY(textoPlataRueda), numero.c_str());
	enteroAString(numero,mapa->getPrecio(BARRA));
	gtk_entry_set_text(GTK_ENTRY(textoPlataBarra), numero.c_str());
	enteroAString(numero,mapa->getPrecio(PLATAFORMA));
	gtk_entry_set_text(GTK_ENTRY(textoPlataPlataforma), numero.c_str());
	enteroAString(numero,mapa->getPrecio(SOGA));
	gtk_entry_set_text(GTK_ENTRY(textoPlataSoga), numero.c_str());
	enteroAString(numero,mapa->getPrecio(CINTA));
	gtk_entry_set_text(GTK_ENTRY(textoPlataCinta), numero.c_str());
	enteroAString(numero,mapa->getPrecio(COHETE));
	gtk_entry_set_text(GTK_ENTRY(textoPlataCohete), numero.c_str());
	enteroAString(numero,mapa->getTiempoCohete());
	gtk_entry_set_text(GTK_ENTRY(textoTiempoCohete), numero.c_str());
	
	
	// botón guardar
	GtkWidget* boton = gtk_button_new_from_stock("Guardar escenario");
	
	GtkWidget* contenedor = gtk_vbox_new(FALSE,10);
	GtkWidget* contenedorNom = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorPlata = gtk_hbox_new(FALSE,10);
	
	//Contenedores
	GtkWidget* contenedorMasa = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorPuntoFijo = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorRueda = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorBarra = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorPlataforma = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorSoga = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorCinta = gtk_hbox_new(FALSE,10);
	GtkWidget* contenedorCohete = gtk_hbox_new(FALSE,10);
	
	
	// empaqueta los elementos
	gtk_box_pack_start (GTK_BOX(contenedorNom), etNombre, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorNom), textoNom, TRUE, TRUE, 3);
	
	gtk_box_pack_start (GTK_BOX(contenedorPlata), etPlata, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorPlata), textoPlata, TRUE, TRUE, 3);
	
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorNom, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorPlata, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), etTitulo, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorMasa, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorPuntoFijo, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorRueda, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorBarra, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorPlataforma, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorSoga, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorCinta, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedor), contenedorCohete, FALSE, FALSE, 3);
	
	gtk_box_pack_start (GTK_BOX(contenedorMasa), elMasa, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorMasa), etCostoMasa, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorMasa), textoPlataMasa, TRUE, TRUE, 3);
	
	gtk_box_pack_start (GTK_BOX(contenedorPuntoFijo), elPuntoFijo, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorPuntoFijo), etCostoPuntoFijo, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorPuntoFijo), textoPlataPuntoFijo, TRUE, TRUE, 3);
	
	gtk_box_pack_start (GTK_BOX(contenedorRueda), elRueda, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorRueda), etCostoRueda, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorRueda), textoPlataRueda, TRUE, TRUE, 3);

	gtk_box_pack_start (GTK_BOX(contenedorBarra), elBarra, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorBarra), etCostoBarra, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorBarra), textoPlataBarra, TRUE, TRUE, 3);

	gtk_box_pack_start (GTK_BOX(contenedorPlataforma), elPlataforma, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorPlataforma), etCostoPlataforma, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorPlataforma), textoPlataPlataforma, TRUE, TRUE, 3);
	
	gtk_box_pack_start (GTK_BOX(contenedorSoga), elSoga, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorSoga), etCostoSoga, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorSoga), textoPlataSoga, TRUE, TRUE, 3);

	gtk_box_pack_start (GTK_BOX(contenedorCinta), elCinta, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorCinta), etCostoCinta, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorCinta), textoPlataCinta, TRUE, TRUE, 3);

	gtk_box_pack_start (GTK_BOX(contenedorCohete), elCohete, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorCohete), etTiempoCohete, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorCohete), textoTiempoCohete, TRUE, TRUE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorCohete), etCostoCohete, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorCohete), textoPlataCohete, TRUE, TRUE, 3);


	
	gtk_box_pack_start (GTK_BOX(contenedor), boton, FALSE, FALSE, 3);
	
	gtk_container_add(GTK_CONTAINER(ventana), contenedor);
	
	datos = new datosConfiguracion();
	datos->textoNom = textoNom;
	datos->textoPlata = textoPlata;
	datos->pthis = this;
	datos->elMasa = elMasa;
	datos->elPuntoFijo = elPuntoFijo;
	datos->elRueda = elRueda;
	datos->elBarra = elBarra;
	datos->elPlataforma = elPlataforma;
	datos->elSoga = elSoga;
	datos->elCinta = elCinta;
	datos->elCohete = elCohete;
	datos->ventana = ventana;
	
	datos->textoPlataMasa = textoPlataMasa;
	datos->textoPlataPuntoFijo = textoPlataPuntoFijo;
	datos->textoPlataRueda = textoPlataRueda;
	datos->textoPlataBarra = textoPlataBarra;
	datos->textoPlataPlataforma = textoPlataPlataforma;
	datos->textoPlataSoga = textoPlataSoga;
	datos->textoPlataCinta = textoPlataCinta;
	datos->textoPlataCohete = textoPlataCohete;
	datos->textoTiempoCohete = textoTiempoCohete;
	
	// conexión de señales
	g_signal_connect(G_OBJECT(boton), "clicked", G_CALLBACK(guardar), datos);
	g_signal_connect(G_OBJECT(ventana), "destroy", G_CALLBACK(destruir), datos);
	
	gtk_widget_show_all (ventana);
	
}

VentanaConfiguracion::~VentanaConfiguracion()
{
	delete datos->pthis->mapa;
	delete datos;
}

void VentanaConfiguracion::destruir(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

void VentanaConfiguracion::cerrar_ventana(GtkWidget *widget, gpointer data)
{
	GtkWidget* vent = (GtkWidget*) data;
	gtk_widget_destroy(vent);
}


void VentanaConfiguracion::enteroAString(std::string& cadena, ulong entero)
{
	if (entero == 0)
		cadena = "0";
	else
		cadena = "";
	while (entero != 0)
	{
		switch (entero %10)
		{
			case 0: cadena="0"+cadena; break;
			case 1: cadena="1"+cadena; break;
			case 2: cadena="2"+cadena; break;
			case 3: cadena="3"+cadena; break;
			case 4: cadena="4"+cadena; break;
			case 5: cadena="5"+cadena; break;
			case 6: cadena="6"+cadena; break;
			case 7: cadena="7"+cadena; break;
			case 8: cadena="8"+cadena; break;
			case 9: cadena="9"+cadena; break;
		}
		entero /= 10;
	}
}


