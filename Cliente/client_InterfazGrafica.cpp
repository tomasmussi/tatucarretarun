#include "client_InterfazGrafica.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "ParserXML.h"
#include "InterfazEdicion.h"
#include "Mapa.h"
#include <fstream>
using namespace std;

#define TITULO_VENTANA "TatuCarreta Run"
#define TEXTO_INICIAL "Bienvenido! \n"

/* Estructuras de datos auxiliares para la invocación de funciones CallBack
 * con parámetros determinados. */

typedef struct datosConexion
{
	GtkWidget* entradaIp;
	GtkWidget* entradaPuerto;
	GtkWidget* entradaNombre;
	GtkWidget* eleccion;
	GtkWidget* ventanaConexion;
	InterfazGrafica* pthis;
} datosConexion;

typedef struct datosBatalla
{
	GtkWidget* ventanaEleccion;
	InterfazGrafica* pthis;
} datosBatalla;

typedef struct datosGravedad
{
	GtkWidget* ventanaGravedad;
	GtkWidget* escala;
} datosGravedad;

typedef struct datosDesafio
{
	GtkWidget* ventanaEleccion;
	InterfazGrafica* pthis;
	string rival;
} datosDesafio;
	

/* Constructor */
InterfazGrafica::InterfazGrafica()
{
	// inicializa atributos
	enBatalla = false;
	batalla = NULL;
	juegoSolo = NULL;
}

bool InterfazGrafica::textoVacio(const string& texto)
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

void InterfazGrafica::obtenerTokens(std::string cadena, std::vector<std::string>& tokens)
{
	string::size_type inicio = 0; //inicio del token
	string::size_type fin; //fin del token
    	string token;


	while (inicio < cadena.length())
	{
	    fin = cadena.find("\n",inicio);
        if (fin == string::npos)
            fin = cadena.length();
	    token = cadena.substr(inicio,fin-inicio);
	    tokens.push_back(token);
	    inicio = fin+1;

	}

}

void InterfazGrafica::rechazarBatalla(GtkWidget *widget, gpointer data)
{
	datosDesafio* datos = (datosDesafio*) data;
	datos->pthis->contacto.enviarMensaje("R" + datos->rival); //envía rechazo
	gtk_widget_destroy(datos->ventanaEleccion);
}

void InterfazGrafica::aceptarBatalla(GtkWidget *widget, gpointer data)
{
	datosDesafio* datos = (datosDesafio*) data;
	datos->pthis->contacto.enviarMensaje("A" + datos->rival); //envía aceptación
	gtk_widget_destroy(datos->ventanaEleccion);

}

void InterfazGrafica::iniciarBatalla(InterfazGrafica* pthis)
{
	pthis->enBatalla = true;
	pthis->batalla = new InterfazBatalla(&(pthis->contacto)); //genera InterfazBatalla
	pthis->chat = new ChatDoble(); //genera ChatDoble
	pthis->batalla->iniciarBatalla(pthis->chat); //inicializa Batalla
	pthis->batalla->start(); //corre batalla
}


void InterfazGrafica::apretar_boton_conectar_auto(GtkWidget *widget, gpointer data)
{

	InterfazGrafica* pthis = (InterfazGrafica*) data;
	string ip = "127.0.0.1";
	string puerto = "3000";
	string nombre = "Axel";

		string leyendaAlerta;
		if (pthis->contacto.conectar(ip,atoi(puerto.c_str())))
		{
			pthis->contacto.vaciarMensajesRecibidos();
			pthis->contacto.enviarMensaje("EAxel");
	

			string respuesta;
			while (true)
			{
				if (pthis->contacto.obtenerMensajeRecibido(respuesta))
					break;
				else
					sleep(0);
			}

			if (respuesta.compare("SSe ha conectado exitosamente") == 0)
			{
				string leyendaEtiqueta = "Conectado a servidor " + ip + " con nombre de usuario " + nombre;
				gtk_label_set_text(GTK_LABEL(pthis->etiqueta),leyendaEtiqueta.c_str());
			}
			mostrarAlerta(respuesta.substr(1, respuesta.length()-1),pthis->ventana);
			if (respuesta.compare("DSUsuario existente") == 0)
				mostrarAlerta("Usuario Existente",pthis->ventana);

		}
		else
			mostrarAlerta("No se pudo conectar al servidor",pthis->ventana);
}

void InterfazGrafica::mostrarAlerta(string leyenda, GtkWidget* ventanaPadre)
{
	GtkWidget *alerta = gtk_dialog_new ();
	GtkWidget* label = gtk_label_new (leyenda.c_str());
	
	gtk_window_set_title (GTK_WINDOW (alerta), "TatuCarreta");
	
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->vbox), label, FALSE, FALSE, 0);

	GtkWidget* boton = gtk_button_new_from_stock("Aceptar");
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alerta)->action_area), boton, FALSE, FALSE, 0);

	gtk_window_set_transient_for (GTK_WINDOW(alerta),GTK_WINDOW(ventanaPadre));
	gtk_window_set_position(GTK_WINDOW(alerta),GTK_WIN_POS_CENTER_ON_PARENT);

	g_signal_connect(G_OBJECT(boton), "clicked", G_CALLBACK(cerrar_ventana), alerta);

	gtk_widget_show_all (alerta);
}

void InterfazGrafica::elegirAdversario(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data)
{
	//Cast de los datos
	datosBatalla* datos = (datosBatalla*) data;

	//Obtengo el valor elegido
	gchar *elegido;
	gtk_clist_get_text(GTK_CLIST(clist), row, column, &elegido);
	
	string mensaje = "  Ha solicitado juego con ";
	mensaje += elegido;

	
	datos->pthis->agregarMensajeAMostrar(mensaje);

	//Envio solicitud de juego al servidor	
	mensaje = "B";
	mensaje += elegido;
	datos->pthis->contacto.enviarMensaje(mensaje.c_str());
	
	gtk_widget_destroy(datos->ventanaEleccion);

}


void InterfazGrafica::jugar_solo(GtkWidget *widget, gpointer data)
{

	InterfazGrafica* p = (InterfazGrafica*) data;
	
	if (!p->contacto.socketConectado())
		return ;
	
	Mapa* mapa = NULL;
	ParserXML par;

	p->contacto.enviarMensaje("J");

	//Recibo los archivos
	bool recibidojpg = false;
	bool recibidoxml = false;

	string mensaje;
	string archivo;
	
	while ((!recibidojpg) || (!recibidoxml))
	{
		if (p->contacto.obtenerMensajeRecibido(mensaje))
		{
			if (mensaje[0] == 'T')
			{
				mostrarAlerta("Ha conocido todos \n los niveles disponibles",p->ventana);
				break; //Termino todos los niveles
			}
			if (mensaje[0] == 'O')
			{
				mostrarAlerta("El Servidor está manejando su máximo \n de partidas simultáneas. Intente un rato más tarde",p->ventana);
				break;
			}
				
			if (mensaje[0] == 'A')
			{
				if (!recibidojpg)
				{
					//guardo imagen de fondo
					InterfazBatalla::desconvertir(archivo,mensaje.substr(1,mensaje.length()-1));
					ofstream arch("escenario.jpg");
					arch << archivo;
					arch.close();
					recibidojpg = true;	
				}
				else
				{
					InterfazBatalla::desconvertir(archivo,mensaje.substr(1,mensaje.length()-1));
					
					//Hacer el mapa en base al xml.
					mapa = par.obtenerMapaMemoria(archivo);
					recibidoxml = true;
				}
			}
		}
		sleep(0);
		
	}
	
	if ((recibidojpg) && (recibidoxml))
	{
		p->juegoSolo = new InterfazJugarSolo(mapa);
		p->juegoSolo->start(); //inicia juego solitario
	}
	
	
	
}

void InterfazGrafica::jugar_batalla(GtkWidget *widget, gpointer data)
{
	//Pide los usuarios conectados
	InterfazGrafica* p = (InterfazGrafica*) data;
	
	if (!p->contacto.socketConectado())
		return;		
	p->adversariosActualizados = false;
	p->contacto.enviarMensaje("U");
	while (!p->adversariosActualizados)
		analizarMensajesRecibidos(p);
	
	//Busco los usuarios
	vector<string> usuarios;
	obtenerTokens(p->adversarios,usuarios);
	
	//Analizo si hay usuarios conectados para desafiar
	if (usuarios.empty())
		mostrarAlerta("No hay otros usuarios conectados",p->ventana);
	else
	{
		//Arma la ventana
		GtkWidget* vent = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW (vent), "Jugar Con...");
		GtkWidget* label = gtk_label_new ("Seleccione un rival entre los usuarios conectados");
		GtkWidget* contenedorVertical = gtk_vbox_new(FALSE, 10);
		GtkWidget* lista = gtk_clist_new ( 1 );

		gtk_window_set_default_size (GTK_WINDOW (vent), 300, 300);
	
		gtk_clist_set_selection_mode( GTK_CLIST(lista), GTK_SELECTION_SINGLE );
	
		gtk_box_pack_start (GTK_BOX (contenedorVertical), label, FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (contenedorVertical), lista, TRUE, TRUE, 0);
		gtk_container_add(GTK_CONTAINER(vent), contenedorVertical);
	
		gtk_window_set_transient_for (GTK_WINDOW(vent),GTK_WINDOW(p->ventana));
		gtk_window_set_position(GTK_WINDOW(vent),GTK_WIN_POS_CENTER_ON_PARENT);
	
		//Agrego los usuarios a la lista
		unsigned int i;
		gchar* nomUsuario;
		for (i = 0 ; i < usuarios.size() ; i++)
		{
			nomUsuario = (gchar*) usuarios[i].c_str();
			gtk_clist_append( GTK_CLIST(lista), &nomUsuario );
		}
	
		//Datos para la siguiente pantalla
		datosBatalla* datos = new datosBatalla();
		datos->ventanaEleccion = vent;
		datos->pthis = p;

		//Conecto la señal de selección	
		gtk_signal_connect(GTK_OBJECT(lista), "select_row", GTK_SIGNAL_FUNC(elegirAdversario), datos);
	
		gtk_widget_show_all(vent);
	}
}

void InterfazGrafica::apretar_boton_aceptar(GtkWidget *widget, gpointer data)
{

	InterfazGrafica* pthis = (InterfazGrafica*) data;
	string mensaje = gtk_entry_get_text(GTK_ENTRY(pthis->entrada));

	//Si no dice nada en la caja de texto, no hace nada
	if (textoVacio(mensaje))
	{
		gtk_entry_set_text(GTK_ENTRY(pthis->entrada),"");
		return;
	}

	//Envía el mensaje
	if (!pthis->contacto.enviarMensaje("C" + mensaje))
		pthis->agregarMensajeAMostrar("No se pudo enviar el mensaje: " + mensaje);

	//Vacío la entrada
	gtk_entry_set_text(GTK_ENTRY(pthis->entrada),"");

}

void InterfazGrafica::cerrar_ventana(GtkWidget *widget, gpointer data)
{
	GtkWidget* vent = (GtkWidget*) data;
	gtk_widget_destroy(vent);
}


void InterfazGrafica::desconectar(GtkWidget *widget, gpointer data)
{
	InterfazGrafica* pthis = (InterfazGrafica*) data;

	if (pthis->contacto.socketConectado())
	{

		pthis->contacto.desconectarSocket();
		
		gtk_label_set_text(GTK_LABEL(pthis->etiqueta),"No está conectado a ningún servidor");
	}
}


void InterfazGrafica::conectar(GtkWidget *widget, gpointer data)
{

	datosConexion* datos = (datosConexion*) data;
	string ip = gtk_entry_get_text(GTK_ENTRY(datos->entradaIp));
	string puerto = gtk_entry_get_text(GTK_ENTRY(datos->entradaPuerto));
	string nombre = gtk_entry_get_text(GTK_ENTRY(datos->entradaNombre));

	//analizo si se comletaron todos los datos
	if ((textoVacio(ip)) || (textoVacio(puerto)) || (textoVacio(nombre)))
		mostrarAlerta("Faltan ingresar datos",datos->ventanaConexion);
	else
	{
		string leyendaAlerta;
		if (datos->pthis->contacto.conectar(ip,atoi(puerto.c_str())))
		{
			datos->pthis->contacto.vaciarMensajesRecibidos();
			
			//analiza botón de elección
			if (GTK_TOGGLE_BUTTON (datos->eleccion)->active)
				datos->pthis->contacto.enviarMensaje("N"+nombre);
			else
				datos->pthis->contacto.enviarMensaje("E"+nombre);
	

			string respuesta;
			while (true)
			{
				if (datos->pthis->contacto.obtenerMensajeRecibido(respuesta))
					break;
				else
					sleep(0);
			}

			if (respuesta.compare("SSe ha conectado exitosamente") == 0)
			{
				gtk_widget_destroy(datos->ventanaConexion);
				string leyendaEtiqueta = "Conectado a servidor " + ip + " con nombre de usuario " + nombre;
				gtk_label_set_text(GTK_LABEL(datos->pthis->etiqueta),leyendaEtiqueta.c_str());
			}
			mostrarAlerta(respuesta.substr(1, respuesta.length()-1),datos->pthis->ventana);
			if (respuesta.compare("DSUsuario existente") == 0)
				mostrarAlerta("Usuario Existente",datos->ventanaConexion);

		}
		else
			mostrarAlerta("No se pudo conectar al servidor",datos->ventanaConexion);
	}


}

void InterfazGrafica::apretar_boton_conectar(GtkWidget *widget, gpointer data)
{
	InterfazGrafica* pthis = (InterfazGrafica*) data;
	
	// elementos de la ventana
	GtkWidget* ventanaConexion;
	GtkWidget* contenedorHorizontalIp = gtk_hbox_new(FALSE, 10);
	GtkWidget* contenedorHorizontalPuerto = gtk_hbox_new(FALSE, 10);
	GtkWidget* contenedorHorizontalNombre = gtk_hbox_new(FALSE, 10);
	GtkWidget* contenedorHorizontalEleccion = gtk_hbox_new(FALSE, 10);
	GtkWidget* contenedorVertical = gtk_vbox_new(FALSE, 10);
	GtkWidget* etiquetaIp = gtk_label_new("IP:");
	GtkWidget* etiquetaPuerto = gtk_label_new("Puerto:");
	GtkWidget* etiquetaNombre = gtk_label_new("Nombre:");
	GtkWidget* entradaIp = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(entradaIp), 30);
	GtkWidget* entradaNombre = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(entradaNombre), 30);
	GtkWidget* entradaPuerto = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(entradaPuerto), 30);


	// botón de elección
	GtkWidget* eleccion = gtk_check_button_new_with_label ("Usuario Nuevo");
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalEleccion),eleccion, FALSE, FALSE, 3);

	gtk_box_pack_start (GTK_BOX(contenedorHorizontalIp), etiquetaIp, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalPuerto), etiquetaPuerto, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalNombre), etiquetaNombre, FALSE, FALSE, 3);

	gtk_box_pack_start (GTK_BOX(contenedorHorizontalIp), entradaIp, TRUE, TRUE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalPuerto), entradaPuerto, TRUE, TRUE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalNombre), entradaNombre, TRUE, TRUE, 3);

	GtkWidget* boton = gtk_button_new_from_stock("gtk-ok");
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalEleccion), boton, FALSE, FALSE, 3);

	ventanaConexion = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for (GTK_WINDOW(ventanaConexion),GTK_WINDOW(pthis->ventana));
	gtk_window_set_position(GTK_WINDOW(ventanaConexion),GTK_WIN_POS_CENTER_ON_PARENT);


	gtk_container_set_border_width(GTK_CONTAINER(ventanaConexion), 10); //Configura borde
	gtk_window_set_title (GTK_WINDOW (ventanaConexion), "Conexión a servidor"); //Pone título



	gtk_container_add(GTK_CONTAINER(ventanaConexion), contenedorVertical);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), contenedorHorizontalIp, FALSE, TRUE, 1);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), contenedorHorizontalPuerto, FALSE, TRUE, 1);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), contenedorHorizontalNombre, FALSE, TRUE, 1);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), contenedorHorizontalEleccion, FALSE, TRUE, 1);

	datosConexion * datos = new datosConexion();
	datos->entradaIp = entradaIp;
	datos->entradaPuerto = entradaPuerto;
	datos->entradaNombre = entradaNombre;
	datos->eleccion = eleccion;
	datos->ventanaConexion = ventanaConexion;
	datos->pthis = pthis;



	g_signal_connect(G_OBJECT(boton), "clicked", G_CALLBACK(conectar), datos);

	gtk_widget_show_all(ventanaConexion);

}

gboolean InterfazGrafica::on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{

	InterfazGrafica* pthis = (InterfazGrafica*) data;

	pthis->contacto.desconectarSocket();

	pthis->contacto.terminar();
	pthis->contacto.join();

	return FALSE;
}


void InterfazGrafica::destruir(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

void InterfazGrafica::propuestaBatalla(InterfazGrafica* pthis, const std::string& rival)
{
	string cadEtDesafio = rival;
	cadEtDesafio += " desea jugar una partida con usted.";

	// ventana de consulta
	GtkWidget* ventanaConsulta = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* etiquetaDesafio = gtk_label_new(cadEtDesafio.c_str());
	GtkWidget* etiquetaPregunta = gtk_label_new("¿Acepta el desafío?");
	GtkWidget* botonSi = gtk_button_new_from_stock("Sí");
	GtkWidget* botonNo = gtk_button_new_from_stock("No");
	GtkWidget* contenedorHorizontal = gtk_hbox_new(FALSE, 10);
	GtkWidget* contenedorVertical = gtk_vbox_new(FALSE, 10);
	
	gtk_window_set_title (GTK_WINDOW (ventanaConsulta), "Desafío"); //Pone título
	
	gtk_box_pack_start (GTK_BOX(contenedorVertical), etiquetaDesafio, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), etiquetaPregunta, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), contenedorHorizontal, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontal), botonSi, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontal), botonNo, FALSE, FALSE, 3);
	
	datosDesafio* datos = new datosDesafio();
	datos->ventanaEleccion = ventanaConsulta;
	datos->pthis = pthis;
	datos->rival = rival;
	
	g_signal_connect(G_OBJECT(botonNo), "clicked", G_CALLBACK(rechazarBatalla), datos);
	g_signal_connect(G_OBJECT(botonSi), "clicked", G_CALLBACK(aceptarBatalla), datos);
	
	gtk_container_add(GTK_CONTAINER(ventanaConsulta), contenedorVertical);
	
	gtk_window_set_transient_for (GTK_WINDOW(ventanaConsulta),GTK_WINDOW(pthis->ventana));
	gtk_window_set_position(GTK_WINDOW(ventanaConsulta),GTK_WIN_POS_CENTER_ON_PARENT);
	
	gtk_widget_show_all(ventanaConsulta);
}

void InterfazGrafica::analizarMensajesRecibidos(InterfazGrafica* p)
{
	// actúa en función al tipo de mensaje que llegó
	string mensaje,leyenda;
	while (p->contacto.obtenerMensajeRecibido(mensaje))
	{
		switch (mensaje[0])
		{
			case 'C': //chat
				p->agregarMensajeAMostrar(mensaje.substr(1,mensaje.length()-1));
				break;
			case 'D': //se deconectó el servidor
				gtk_label_set_text(GTK_LABEL(p->etiqueta),"No está conectado a ningún servidor");
				p->agregarMensajeAMostrar("  Se ha desconectado del servidor");
				break;
			case 'U': //usuarios disponibles
				p->adversarios = mensaje.substr(1,mensaje.length()-1);
				p->adversariosActualizados = true;
				break;
			case 'S': // señal del servidor
				p->agregarMensajeAMostrar(mensaje.substr(1,mensaje.length()-1));
				break;
			case 'B': // propuesta de batalla
				p->propuestaBatalla(p,mensaje.substr(1,mensaje.length()-1));
				p->agregarMensajeAMostrar("  Recibe desafío de " + mensaje.substr(1,mensaje.length()-1));
				break;
			case 'R': // rechazo de batalla
				leyenda = "No podrá jugar con ";
				leyenda += mensaje.substr(1,mensaje.length()-1);
				mostrarAlerta(leyenda,p->ventana);
				p->agregarMensajeAMostrar("  No pudo efectuarse desafío");
				break;
			case 'Z': // inicio de batalla
				iniciarBatalla(p);
				break;
			case 'I': // resultado de una batalla
				leyenda = mensaje.substr(1,mensaje.length()-1);
				mostrarAlerta(leyenda,p->ventana);
				break;
			case 'G': // tabla de posiciones
				leyenda = mensaje.substr(1,mensaje.length()-1);
				mostrarAlerta(leyenda,p->ventana);
				break;
			case 'O': //servidor ocupado
				mostrarAlerta("El Servidor está manejando su máximo \n de partidas simultáneas. Intente un rato más tarde",p->ventana);
				break;
		}
	}
}

void InterfazGrafica::modificar_archivo_gravedad(GtkWidget *widget, gpointer data)
{
	datosGravedad* datos = (datosGravedad*) data;
	string buffer,archivo;
	ifstream arch("config.txt");//,ios::in|ios::out);
	
	double valorNro = gtk_range_get_value(GTK_RANGE(datos->escala));
	valorNro /= (-100.0);
		
	std::ostringstream os;
	os << valorNro;
	string valor = os.str();

	getline(arch,buffer);
	
	archivo = buffer.replace(buffer.find_last_of(" ")+1,buffer.length()-buffer.find_last_of(" ")-1,valor.c_str(),valor.length());
	
	archivo += "\n";
	
	while (getline(arch,buffer))
	{
		archivo += buffer;
		archivo += "\n";
		buffer.clear();
	}
	
	arch.close();
	
	// reescritura del archivo
	ofstream arch2("config.txt");
	arch2.write(archivo.c_str(),archivo.length());
	arch2.close();
	
	gtk_widget_destroy(datos->ventanaGravedad);
	
	delete datos;
}


void InterfazGrafica::pedir_posiciones(GtkWidget *widget, gpointer data)
{
	InterfazGrafica* pthis = (InterfazGrafica*) data;
	pthis->contacto.enviarMensaje("G");
}


void InterfazGrafica::modificar_gravedad(GtkWidget *widget, gpointer data)
{
	float inicial = 0.0;
	string linea;
	ifstream arch("config.txt");
	getline(arch,linea);
	linea = linea.substr(linea.find_last_of(" ")+1,linea.length()-linea.find_last_of(" ")-1);
	arch.close();
	linea.replace(linea.find("."),1,",",1);
	inicial = strtod(linea.c_str(), NULL);
	InterfazGrafica* pthis = (InterfazGrafica*) data;
	GtkWidget* ventanaGravedad = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for (GTK_WINDOW(ventanaGravedad),GTK_WINDOW(pthis->ventana));
	gtk_window_set_position(GTK_WINDOW(ventanaGravedad),GTK_WIN_POS_CENTER_ON_PARENT);
	GtkWidget* escala = gtk_hscale_new(GTK_ADJUSTMENT(gtk_adjustment_new(inicial*(-100), 1.0, 31.0, 1.0, 1.0, 1.0 )));
	gtk_scale_set_draw_value( GTK_SCALE(escala),FALSE);

	gtk_window_set_default_size (GTK_WINDOW (ventanaGravedad), 200, 50);
	
	GtkWidget* botonOk = gtk_button_new_from_stock("Aceptar");
	GtkWidget* botonCancelar = gtk_button_new_from_stock("Cancelar");
	GtkWidget* contenedorHorizontal = gtk_hbox_new(FALSE, 10);
	GtkWidget* contenedorVertical = gtk_vbox_new(FALSE, 10);
	
	gtk_window_set_title (GTK_WINDOW (ventanaGravedad), "Gravedad"); //Pone título
	
	gtk_box_pack_start (GTK_BOX(contenedorVertical), escala, FALSE, FALSE, 10);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), contenedorHorizontal, FALSE, FALSE, 7);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontal), botonOk, FALSE, FALSE, 10);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontal), botonCancelar, FALSE, FALSE, 7);

	gtk_container_add(GTK_CONTAINER(ventanaGravedad), contenedorVertical);
	
	datosGravedad* datos = new datosGravedad();
	datos->ventanaGravedad = ventanaGravedad;
	datos->escala = escala;	
	
	g_signal_connect(G_OBJECT(botonOk), "clicked", G_CALLBACK(modificar_archivo_gravedad), datos);
	g_signal_connect(G_OBJECT(botonCancelar), "clicked", G_CALLBACK(cerrar_ventana), ventanaGravedad);
	
	gtk_widget_show_all (ventanaGravedad);

}

void InterfazGrafica::resultado_juego_solo(InterfazGrafica* pthis, const std::string& puntos)
{
	//Muestro ventana con resultado
	GtkWidget* ventanaResultado = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for (GTK_WINDOW(ventanaResultado),GTK_WINDOW(pthis->ventana));
	gtk_window_set_position(GTK_WINDOW(ventanaResultado),GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_title (GTK_WINDOW (ventanaResultado), "Resultado");
	
	GtkWidget* contenedorVertical = gtk_vbox_new(FALSE, 10);
	GtkWidget* contenedorHorizontal = gtk_hbox_new(FALSE, 10);
	
	GtkWidget* botonContinuar = gtk_button_new_from_stock("Continuar");
	GtkWidget* botonTerminar = gtk_button_new_from_stock("Terminar");
	
	string leyendaEtiqueta = "Ha completado el nivel con ";
	leyendaEtiqueta += puntos;
	leyendaEtiqueta += " puntos";
	GtkWidget* label = gtk_label_new(leyendaEtiqueta.c_str());
	
	gtk_box_pack_start (GTK_BOX(contenedorVertical), label, TRUE, TRUE, 10);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontal), botonContinuar, FALSE, FALSE, 10);
	gtk_box_pack_start (GTK_BOX(contenedorHorizontal), botonTerminar, FALSE, TRUE, 10);
	gtk_box_pack_start (GTK_BOX(contenedorVertical), contenedorHorizontal, FALSE, TRUE, 10);
	
	gtk_signal_connect(GTK_OBJECT(botonContinuar), "clicked", GTK_SIGNAL_FUNC(jugar_solo), pthis);
	gtk_signal_connect_object (GTK_OBJECT (botonContinuar), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT (ventanaResultado));


	gtk_signal_connect_object (GTK_OBJECT (botonTerminar), "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy), GTK_OBJECT (ventanaResultado));

	gtk_container_add(GTK_CONTAINER(ventanaResultado), contenedorVertical);
	
	gtk_widget_show_all (ventanaResultado);

}

gboolean InterfazGrafica::timeout_handler(gpointer data)
{



	GtkTextBuffer *buffer;
        GtkTextIter iter;
	InterfazGrafica* pthis = (InterfazGrafica*) data;
	string mensaje;
        
        if (pthis->batalla == NULL)
        {
        	
        	if (pthis->juegoSolo != NULL)
        	{
        		if (!pthis->juegoSolo->estaJugando())
        		{
        			// pide los puntos y los manda
        			int puntos = pthis->juegoSolo->getPuntos();
        			if (puntos > 0)
      				{
      					enteroAString(mensaje,puntos);
      					pthis->agregarMensajeAMostrar("  Ha obtenido " + mensaje + " puntos en un juego solitario");
        				mensaje = "P" + mensaje;
        				pthis->contacto.enviarMensaje(mensaje);
        			}
        			else
	        			pthis->contacto.enviarMensaje("P0");
	        			
	        		if (puntos == 0)
	        			mostrarAlerta("No ha completado correctamente el escenario",pthis->ventana);
        			
        			delete pthis->juegoSolo;
        			pthis->juegoSolo = NULL;
        			
        			if (puntos > 0)
	        			resultado_juego_solo(pthis,mensaje.substr(1,mensaje.length() - 1));
        			
        		}
        	}
      
		analizarMensajesRecibidos(pthis);

		//Muestra los mensajes en la pantalla del chat

		pthis->bloquearEstado();
		while (!pthis->mensajesAMostrar.empty())
		{
			mensaje = pthis->mensajesAMostrar[0];
			pthis->mensajesAMostrar.erase(pthis->mensajesAMostrar.begin());
	
			//Agrega el mensaje al buffer del cuadro de mensajes

			buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (pthis->cuadroMensajes));
			mensaje += "\n";       

			gtk_text_buffer_get_end_iter (buffer, &iter);
	       
			gtk_text_buffer_insert (buffer, &iter, mensaje.c_str(), -1);
	
			gtk_text_buffer_get_end_iter (buffer, &iter);

			gtk_text_view_scroll_to_iter (GTK_TEXT_VIEW (pthis->cuadroMensajes), &iter, 0.0, FALSE, 0, 0); 
		}
		pthis->desbloquearEstado();

	}
	else
	{
	       	if (!pthis->batalla->estaJugando())
        	{


        		// pide los puntos y los manda
        		int puntos = pthis->batalla->getPuntos();
        		if (puntos > 0)
      			{
      				enteroAString(mensaje,puntos);
      				pthis->agregarMensajeAMostrar("  Ha obtenido " + mensaje + " puntos en un juego doble");
        			mensaje = "P" + mensaje;
        			pthis->contacto.enviarMensaje(mensaje);
        		}
        		else
        			pthis->contacto.enviarMensaje("P0");

  	     		delete pthis->batalla;
  	     		delete pthis->chat;

        		pthis->batalla = NULL;
        		pthis->chat = NULL;

        	}
        	else
			//se tiene que hacer cargo la interfaz batalla
			if (!pthis->batalla->analizarMensajesRecibidos())
			{
				mostrarAlerta("Se ha desconectado del servidor",pthis->ventana);
				gtk_label_set_text(GTK_LABEL(pthis->etiqueta),"No está conectado a ningún servidor");
				pthis->agregarMensajeAMostrar("  Se ha desconectado del servidor");
			}
	}



	return true;

}

gboolean InterfazGrafica::inicio(gpointer data)
{
	InterfazGrafica * pthis = (InterfazGrafica*) data;
	sleep(1);
	gtk_widget_destroy(pthis->window); //quita presentación
	
	//inicia ventana principal
	gtk_widget_show_all (pthis->ventana);
	
	//agrega función que se ejecuta cada cierto intervalo de tiempo
	g_timeout_add(100, timeout_handler, pthis); 

	return false; //se ejecuta una sola vez
}



void InterfazGrafica::run()
{

	//carga imagen de presentación
	GdkPixbuf *pix = gdk_pixbuf_new_from_file_at_size("ima/armadillo.jpg",100,100,NULL);

	GtkWidget *imagen;
	GdkPixbuf *pixbuf;
	GdkPixmap *pixmap;
	GdkBitmap *mask;

	pixbuf = gdk_pixbuf_new_from_file_at_size("ima/armadillo.jpg",600,650,NULL);
	gdk_pixbuf_render_pixmap_and_mask (pixbuf, &pixmap, &mask, 0);
	imagen = gtk_pixmap_new( pixmap, mask );

	//ventana de presentación
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"TatuCarreta Run");
	gtk_window_set_decorated(GTK_WINDOW(window),FALSE);
	gtk_widget_show(imagen);
	gtk_container_add (GTK_CONTAINER (window), imagen);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_widget_show_all (window);


	//inicia entidad de contacto con los servidores
	contacto.start();



	GtkWidget* contenedorHorizontalChat;
	GtkWidget* contenedorVerticalChat;
	GtkWidget* contenedorHorizontalGrande;
	GtkWidget* contenedorVerticalBotones;
	GtkWidget* botonAceptar;

	GtkWidget* scroll;
	GtkWidget* botonConectar;
	GtkWidget* botonJugar;
	GtkWidget* botonJugarCon;
	GtkWidget* botonGravedad;
	GtkWidget* botonDesconectar;
	GtkWidget* botonRanking;


	gtk_set_locale(); //Configura idioma

	/****************************/
	/*** Configura la ventana ***/
	/****************************/

	ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(ventana),GTK_WIN_POS_CENTER);
	gtk_window_set_icon(GTK_WINDOW(ventana),GDK_PIXBUF(pix));
	//Conecta la función on_delete_event a la señal delete_event
	g_signal_connect(G_OBJECT(ventana), "delete_event", G_CALLBACK(on_delete_event), (gpointer)this );

	//Conecta la función destruir a la señal destroy
	g_signal_connect(G_OBJECT(ventana), "destroy", G_CALLBACK(destruir), NULL);

	gtk_container_set_border_width(GTK_CONTAINER(ventana), 10); //Configura borde
	gtk_window_set_title (GTK_WINDOW (ventana), TITULO_VENTANA); //Pone título
	gtk_window_set_default_size (GTK_WINDOW (ventana), 500, 500); //Configura tamaño de la ventana


	/*******************************/
	/*** Genera los contenedores ***/
	/*******************************/

	contenedorHorizontalChat = gtk_hbox_new(FALSE, 10);
	contenedorVerticalChat = gtk_vbox_new(FALSE,10);
	contenedorHorizontalGrande = gtk_hbox_new(FALSE, 10);
	contenedorVerticalBotones = gtk_vbox_new(FALSE,10);


	/**************************/
	/*** Genera la etiqueta ***/
	/**************************/

	etiqueta = gtk_label_new("No está conectado a ningún servidor");

	//Agrego la etiqueta al contenedor vertical de chat
	gtk_box_pack_start (GTK_BOX(contenedorVerticalChat), etiqueta, FALSE, FALSE, 3);

	/******************************/
	/*** Junto los contenedores ***/
	/******************************/


	//Coloca en la ventana al contenedor principal
	gtk_container_add(GTK_CONTAINER(ventana), contenedorHorizontalGrande);

	//Agrego el contenedor de chat en el principal
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalGrande), contenedorVerticalChat, TRUE, TRUE, 3);

	//Agrego el contenedor de botones en el principal
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalGrande), contenedorVerticalBotones, FALSE, FALSE, 3);


	/**************************************************************/
	/*** Genera el área de texto donde se muestran los mensajes ***/
	/**************************************************************/

	cuadroMensajes = gtk_text_view_new ();
	gtk_widget_set_usize (cuadroMensajes, 50, -2); //Configura tamaño
	gtk_text_view_set_editable(GTK_TEXT_VIEW(cuadroMensajes), FALSE); //Evita que sea texto editable
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(cuadroMensajes), FALSE);
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(cuadroMensajes), 5);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(cuadroMensajes), 3);


	/*************************************************/
	/*** Genera el scroll para el espacio de texto ***/
	/*************************************************/

	scroll = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll),cuadroMensajes); //Agrega el texto
	gtk_box_pack_start (GTK_BOX(contenedorVerticalChat), scroll, TRUE, TRUE, 3); //Agrega el scroll al contenedor vertical
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS); //Hace automático el scroll horizontal


	//Agrego el contenedor horizontal del chat en el vertical
	gtk_box_pack_start (GTK_BOX(contenedorVerticalChat), contenedorHorizontalChat, FALSE, TRUE, 3);


	/*********************************************/
	/*** Genera el espacio de entrada de texto ***/
	/*********************************************/

	entrada = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(entrada), 50); //Configura tamaño

	//Agrega la entrada de texto al contenedor horizontal del chat
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalChat), entrada, TRUE, TRUE, 3);



	/*****************************/
	/*** Genera label auxiliar ***/
	/*****************************/

	GtkWidget* labelAuxiliar = gtk_label_new("\n\n\n");

	//Agrego la etiqueta al contenedor de botones
	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), labelAuxiliar, FALSE, FALSE, 3);



	/********************************/
	/*** Genera el botón del chat ***/
	/********************************/

	botonAceptar = gtk_button_new_from_stock("gtk-ok");

	//Agrego el botón al contenedor horizontal
	gtk_box_pack_start (GTK_BOX(contenedorHorizontalChat), botonAceptar, FALSE, FALSE, 3);


	/**********************************************/
	/*** Genera el botón de conectar automatico ***/
	/**********************************************/
	/* Botón para agilizar las pruebas. No se ve actualmente en la pantalla */
	
	GtkWidget * botonConectarAuto = gtk_button_new_from_stock("ConectarAuto");

	//Agrego el botón al contenedor de botones
//	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), botonConectarAuto, FALSE, FALSE, 6);


	g_signal_connect(G_OBJECT(botonConectarAuto), "clicked", G_CALLBACK(apretar_boton_conectar_auto), this);


	/***********************************/
	/*** Genera el botón de conectar ***/
	/***********************************/

	botonConectar = gtk_button_new_from_stock("Conectar");

	//Agrego el botón al contenedor de botones
	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), botonConectar, FALSE, FALSE, 6);


	/**************************************/
	/*** Genera el botón de desconectar ***/
	/**************************************/

	botonDesconectar = gtk_button_new_from_stock("Desconectar");

	//Agrego el botón al contenedor de botones
	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), botonDesconectar, FALSE, FALSE, 6);


	/********************************/
	/*** Genera el botón de jugar ***/
	/********************************/

	botonJugar = gtk_button_new_from_stock("Jugar solo");

	//Agrego el botón al contenedor de botones
	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), botonJugar, FALSE, FALSE, 6);


	/************************************/
	/*** Genera el botón de jugar con ***/
	/************************************/

	botonJugarCon = gtk_button_new_from_stock("Jugar con...");

	//Agrego el botón al contenedor de botones
	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), botonJugarCon, FALSE, FALSE, 6);


	/*******************************************/
	/*** Genera el botón de cambiar gravedad ***/
	/*******************************************/

	botonGravedad = gtk_button_new_from_stock("Set Gravedad");

	//Agrego el botón al contenedor de botones
	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), botonGravedad, FALSE, FALSE, 6);
	
	
	/*******************************************/
	/*** Genera el botón de ranking ************/
	/*******************************************/

	botonRanking = gtk_button_new_from_stock("Posiciones");

	//Agrego el botón al contenedor de botones
	gtk_box_pack_start (GTK_BOX(contenedorVerticalBotones), botonRanking, FALSE, FALSE, 6);


	//Conexión de señales

	g_signal_connect(G_OBJECT(botonConectar), "clicked", G_CALLBACK(apretar_boton_conectar), this);

	g_signal_connect(G_OBJECT(botonAceptar), "clicked", G_CALLBACK(apretar_boton_aceptar), this);
	
	gtk_signal_connect (GTK_OBJECT(entrada), "activate", GTK_SIGNAL_FUNC(apretar_boton_aceptar), this);

	g_signal_connect(G_OBJECT(botonDesconectar), "clicked", G_CALLBACK(desconectar), this);
	
	g_signal_connect(G_OBJECT(botonJugar), "clicked", G_CALLBACK(jugar_solo), this);
	
	g_signal_connect(G_OBJECT(botonJugarCon), "clicked", G_CALLBACK(jugar_batalla), this);
	
	g_signal_connect(G_OBJECT(botonGravedad), "clicked", G_CALLBACK(modificar_gravedad), this);
	
	g_signal_connect(G_OBJECT(botonRanking), "clicked", G_CALLBACK(pedir_posiciones), this);


	// Función que cierra la presentación
	g_timeout_add(1000, inicio, this);



	gtk_main();
	
	contacto.terminar();
	contacto.join();

}


bool InterfazGrafica::obtenerMensajeLeido(string& mensaje)
{
	bool retorno;
	bloquearEstado();
	if (!mensajesLeidos.empty())
	{
		retorno = true;
		mensaje = mensajesLeidos[0]; //Da el primer mensaje leído disponible
		mensajesLeidos.erase(mensajesLeidos.begin()); //Lo saca del vector
	}
	else
		retorno = false;
	desbloquearEstado();
	return retorno;
}


void InterfazGrafica::agregarMensajeAMostrar(const std::string& mensaje)
{
	bloquearEstado();
	mensajesAMostrar.push_back(mensaje);
	desbloquearEstado();
}


void InterfazGrafica::enteroAString(std::string& cadena, ulong entero)
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

