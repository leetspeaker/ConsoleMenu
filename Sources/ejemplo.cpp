#include "ConsoleMenu.h"

using namespace ConsoleMenu;

int main(int argc, char* argv[])
{
	cout << endl << endl << "Otro texto del programa......." << endl << endl;
	CMenu *Menu = new CMenu("Titulo del menu", CMENU_TIPO_SELECCION, 2, true);

	Menu->CambiarColor(CMENU_COLOR_TITULO, Menu->Color_Azul);
	Menu->CambiarColor(CMENU_COLOR_MARCO, Menu->Color_Verde);
	Menu->CambiarColor(CMENU_COLOR_ITEMS, Menu->Color_Rojo);
	Menu->CambiarColor(CMENU_COLOR_GUIA, Menu->Color_Rosa);
	Menu->CambiarColor(CMENU_COLOR_DESCRIPCION, Menu->Color_Aqua);

	Menu->AgregarItem("Opcion uno", 1, "Descripcion opcion uno");
	Menu->AgregarItem("Opcion dos", 2, "Descripcion opcion dos");
	Menu->AgregarItem("Opcion tres",3, "Descripcion opcion tres");
	Menu->AgregarItem("Opcion cuatro", 4, "Descripcion opcion cuatro");
	Menu->AgregarItem("Opcion cinco", 5, "Descripcion opcion cinco");
	Menu->AgregarItem("Opcion seis", 6, "Descripcion elemento x");
	Menu->AgregarItem("Opcion noventa y nueve", 99); // A este no le quise poner descripcion, pero igual es válido!!
	cout << Menu->Ejecutar();
	return 0;
}

