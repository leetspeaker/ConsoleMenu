#pragma once
#ifndef _CONSOLEMENU_H_
#define _CONSOLEMENU_H_
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std;

#define CMENU_TIPO_NUMERO		0x01
#define CMENU_TIPO_SELECCION	0x02

// Constantes de elementos
#define CMENU_COLOR_MARCO		0x00
#define CMENU_COLOR_ITEMS		0x01
#define CMENU_COLOR_GUIA		0x02
#define CMENU_COLOR_DESCRIPCION	0x03
#define CMENU_COLOR_TITULO		0x04

namespace ConsoleMenu 
{
	class CItem 
	{
	private:
		string strTexto;
		string strDescripcion;
		int ID;
		bool Activo;
	public:
		CItem(string Texto, string Descripcion, int CarID, bool EstaActivo = true);
		string GetTexto();
		string GetDescripcion();
		int GetID();
		bool EstaActivo();
		void SetActividad(bool EstaActivo);
	};

	class CMenu 
	{
	private:
		enum TiposRelleno 
		{
			Derecho,
			Izquierdo,
			Medio_Vertical,
			Superior,
			Inferior,
			Medio_Horizontal
		};

		string strTitulo;
		int Elementos;
		int TipoMenu;
		int CantidadColumnas;
		int InicioY;
		bool MotrarDescripcion;
		char ColoresSistema[5];
		CItem **Items;

		bool EsValido(char ID);
		void PrintTitulo();
		void PrintRelleno();
		void PrintItems();
		char IniciarSeleccion();
	public:
		
		enum MenuColores 
		{
			Color_AzulOscuro=1,
			Color_VerdeOscuro,
			Color_AquaOscuro,
			Color_RojoOscuro,
			Color_RosaOscuro,
			Color_Mostaza,
			Color_BlancoOpaco,
			Color_Gris,
			Color_Azul,
			Color_Verde,
			Color_Aqua,
			Color_Rojo,
			Color_Rosa,
			Color_Amarillo,
			Color_Blanco
		};

		// CAMBIAR EL COLOR DE ALGUN ELEMENTO DEL MENU
		// Elemento: Cualquiera de las constantes de elementos definidas en este archivo
		// Color: Cualquiera de los enumerados arriba de este comentario
		// Esta funcion debe llamarse antes de ejecutar si se desea que los cambios surtan efecto
		void CambiarColor(int Elemento, int Color);

		//CONSTRUCTOR: ESPECIFICAR AQUI, LAS OPCIONES BASICAS
		//Título (obligatorio), máximo 100 caracteres (o no se mostrará)
		//Tipos (opcional):
		//		-CMENU_TIPO_NUMERO: El usuario debe seleccionar un numero para escoger la opcion (Tradicional)
		//		-CMENU_TIPO_SELECCION: El usuario puede seleccionar la opción con las teclas de dirección y enter
		//Columnas (1 o 2): Puede dividir el menu en dos columnas o dejarlo en una sola. Cualquier número distinto 
		//		a 1, se toma como si fueran 2 columnas. Si no se especifica este parámetro, se toma una sola comlunma.
		//Descripciones (No válido en CMENU_TIPO_NUMERO): Si es true, se mostrará el texto de ayuda del item.
		CMenu(string Titulo, int Tipo = CMENU_TIPO_SELECCION, int Columnas = 1, bool Descripciones = false);

		~CMenu();

		// Parámetros:
		// Texto (Obligatorio) nombre del item a mostrar en la lista. Máximo 50 caracteres.
		// ID (Obligatorio) numero del item. Éste numero será mostrado en pantalla junto al texto si se elige
		//		CMENU_TIPO_NUMERO. Será el numero devuelto si se selecciona esa opción.
		// Descripción (Opcional) Texto para describir el item, máximo 100 caracteres.
		// Activo (Opcional): Define si el item está activo. Si no se proporciona este parámetro, el item estará activo desde
		//		un principio. Cuando el item no está activo, no se mostrará en la lista y no se podrá seleccionar,
		//		sin embargo, ningún otro ítem podrá tener su mismo ID y se le reservará su espacio en la consola.
		// SI NO SE AGREGA NINGÚN ITEM, NO SE MOSTRARÁ EL MENU
		void AgregarItem(string Texto, int ID, string Descripcion = "", bool Activo = true);

		// Mostrar el menú... Se mostrará el menú en una línea más abajo de donde esté el cursor actualmente.
		// Si el usuario elige una opción, el número de la opción será devuelta.
		char Ejecutar();
	};
};// NAMESPACE CONSOLEMENU
#endif