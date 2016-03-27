/*******************************************************************************
Este archivo es parte del proyecto ConsoleMenu
Autor: Carlos D. Alvarez
Fecha 160327
Caracas, Venezuela
*******************************************************************************/

#include "ConsoleMenu.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(WIN64)
#include <Windows.h>
#define Esperar Sleep
#define EstaPulsada GetAsyncKeyState
#define StringCopy lstrcpyA
#define gotoxy wingotoxy
#define ReducirCursor winReducirCursor
#define SetColorFont winSetColorFont
#define ObtenerY winObtenerY
#define TeclaArriba VK_UP
#define TeclaAbajo  VK_DOWN
#define TeclaIzquierda VK_LEFT
#define TeclaDerecha VK_RIGHT
#define TeclaEnter VK_RETURN
void winSetColorFont(int color)
{
	if (color > 0 && color < 16)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void winReducirCursor()
{
	CONSOLE_CURSOR_INFO ccinfo;
	ccinfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ccinfo);
}

void wingotoxy(int x, int y)
{
	COORD Posicion;
	Posicion.X = x;
	Posicion.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Posicion);
}

int winObtenerY() 
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	return SBInfo.dwCursorPosition.Y;
}
#else
#error ConsoleMenu es compatible sólo con Windows NT o posterior (por ahora...)
#endif

namespace ConsoleMenu
{
	CItem::CItem(string Texto, string Descripcion, int CarID, bool EstaActivo) 
	{
		Activo = EstaActivo;
		strTexto = Texto;
		strDescripcion = Descripcion;
		ID = CarID;
	}

	string CItem::GetTexto() 
	{
		return strTexto;
	}

	string CItem::GetDescripcion()
	{
		return strDescripcion;
	}

	int CItem::GetID() 
	{
		return ID;
	}

	void CItem::SetActividad(bool EstaActivo) 
	{
		Activo = EstaActivo;
	}

	bool CItem::EstaActivo()
	{
		return Activo;
	}

	CMenu::CMenu(string Titulo, int Tipo, int Columnas, bool Descripciones)
	{
		strTitulo = Titulo;
		TipoMenu = Tipo;
		CantidadColumnas = Columnas;
		MotrarDescripcion = Descripciones;
		Elementos = 0;
		Items = (CItem**) malloc(sizeof(CItem*));
		memset(ColoresSistema, 15, 5);
	}

	CMenu::~CMenu()
	{
		for(int i = 0; i < Elementos; i++)
		{
			delete Items[i];
		}
		free(Items);
	}

	bool CMenu::EsValido(char ID)
	{
		for (int i = 0; i < Elementos; i++)
		{
			if (Items[i]->GetID() == ID)
				return true;
		}
		return false;
	}

	void CMenu::CambiarColor(int Elemento, int Color)
	{
		if(Color < 16 && Color > 0)
			ColoresSistema[Elemento] = Color;
	}

	void CMenu::PrintTitulo()
	{
		char Buffer[121];
		int Medio = 60-(strTitulo.length()/2);
		memset(Buffer, ' ', 121);
		StringCopy(&Buffer[Medio], strTitulo.c_str());
		Buffer[Medio+strTitulo.length()] = ' ';
		Buffer[120] = (char) 0;
		gotoxy(0, InicioY+1);
		SetColorFont(ColoresSistema[CMENU_COLOR_TITULO]);
		cout << Buffer;
		SetColorFont(Color_Blanco);
	}

	void CMenu::PrintRelleno() 
	{
		SetColorFont(ColoresSistema[CMENU_COLOR_MARCO]);
		int FilasElementos = CantidadColumnas == 1 ? Elementos : Elementos / 2;
		for (int y = InicioY; y < InicioY + FilasElementos + 7; y++) 
		{
			for (int x = 2; x <= 118; x++)
			{
				gotoxy(x,y);
				if (x == 118) 
				{
					cout << endl;
					continue;
				}
				if ((x == 2) || (x == 60) || (x == 117)) 
				{
					switch(x)
					{
					case 2:
						if (y == InicioY)
							cout << (char)201;
						else if ((y == InicioY+2) || (y == InicioY+FilasElementos+4))
							cout << (char)204;
						else if (y == InicioY+FilasElementos+6)
							cout << (char)200;
						else
							cout << (char)186;
						break;

					case 60:
						if (CantidadColumnas == 2)
						{
							if (y == InicioY + 2)
								cout << (char)203;
							else if (y == InicioY+FilasElementos+4)
								cout << (char)202;
							else if (y > InicioY+2 && y < InicioY+FilasElementos+4) 
								cout << (char)186;
						}

						break;

					case 117:
						if (y == InicioY)
							cout << (char)187;
						else if ((y == InicioY+2) || (y == InicioY+FilasElementos+4))
							cout << (char)185;
						else if (y == InicioY+FilasElementos+6)
							cout << (char)188;
						else
							cout << (char)186;
						break;
					}
				}

				if ((x != 2) && (x != 117) && ((y == InicioY) || (y == InicioY+2) || (y == InicioY+FilasElementos+4) || (y == InicioY+FilasElementos+6)))
					cout << (char) 205; 

			}
		}
		SetColorFont(Color_Blanco);
	}

	void CMenu::AgregarItem(string Texto, int ID, string Descripcion, bool Activo)
	{
		if ((strcmp(Texto.c_str(), "")) &&
			(Descripcion.length() <= 100) &&
			(Texto.length() <= 50) &&
			(!EsValido(ID))) 
		{
			Items = (CItem**) realloc(Items, sizeof(CItem*)*++Elementos);
			Items[Elementos-1] = new CItem(Texto, Descripcion, ID, Activo);
		}
	}

	void CMenu::PrintItems()
	{
		SetColorFont(ColoresSistema[CMENU_COLOR_ITEMS]);
		if (CantidadColumnas == 1)
		{
			for (int i = 0; i < Elementos; i++) 
			{
				if (Items[i]->EstaActivo()) 
				{
					gotoxy(8, InicioY+3+i);
					if (TipoMenu != CMENU_TIPO_SELECCION)
						cout << Items[i]->GetID() << ". ";
					cout << Items[i]->GetTexto().c_str();

				}
			}
		} 
		else
		{
			int LineaActual = InicioY+3;
			for (int i = 0; i < Elementos; i++)
			{
				if (Items[i]->EstaActivo()) 
				{
					fflush(stdout);
					if (i > Elementos / 2)
						gotoxy(66, LineaActual);
					else
						gotoxy(8, LineaActual);

					if (TipoMenu != CMENU_TIPO_SELECCION)
						cout << Items[i]->GetID() << ". ";
					cout << Items[i]->GetTexto().c_str();
					LineaActual++;
					if (i == Elementos / 2)
						LineaActual = InicioY+3;
				}
			}
		}
		SetColorFont(Color_Blanco);
	}

	// FAVORECER CÓDIGO RÁPIDO SOBRE CÓDIGO PEQUEÑO
	char CMenu::IniciarSeleccion()
	{
		SetColorFont(ColoresSistema[CMENU_COLOR_GUIA]);
		int Seleccionado = 0;

		if (TipoMenu == CMENU_TIPO_NUMERO)
		{
			while(true){
				if (CantidadColumnas == 1)
					gotoxy(4, InicioY+Elementos+5);
				else
					gotoxy(4, InicioY+(Elementos/2)+5);
				cout << "Ingrese la opcion:                            ";

				if (CantidadColumnas == 1)
					gotoxy(23, InicioY+Elementos+5);
				else
					gotoxy(23, InicioY+(Elementos/2)+5);
				cin >> Seleccionado;

				for(int i = 0; i < Elementos; i++)	
					if(Items[i]->GetID() == Seleccionado)
						return i;
			}
		} 
		else
		{
			int Col = 0;
			const int ElemPos[2][3] = 
			{ // 0=Primero			//1	Ultimo			//2 PosX
				{0,					Elementos/2,		4},	// 0
				{(Elementos/2)+1,   Elementos-1,		62}	// 1
			};
			int Respaldo = -1;
			char Relleno[101];
			Relleno[100] = 0;

			gotoxy(4, InicioY+3+Seleccionado);
			cout << "=>>";
			while(true)
			{
				if ((Respaldo != Seleccionado) && MotrarDescripcion)
				{
					SetColorFont(ColoresSistema[CMENU_COLOR_DESCRIPCION]);
					int RY = ObtenerY();
					Respaldo = Seleccionado;
					if (CantidadColumnas == 1)
						gotoxy(4, InicioY+Elementos+5);
					else
						gotoxy(4, InicioY+(Elementos/2)+5);

					memset(Relleno, ' ', 100);
					StringCopy(Relleno, Items[Seleccionado]->GetDescripcion().c_str());
					Relleno[Items[Seleccionado]->GetDescripcion().length()] = ' ';
					if (strcmp(Items[Seleccionado]->GetDescripcion().c_str(), ""))
						cout << Relleno;
					gotoxy(ElemPos[Col][2], RY);
					SetColorFont(ColoresSistema[CMENU_COLOR_GUIA]);
				}

				Esperar(100);
				if (CantidadColumnas == 1)
				{
					if (EstaPulsada(TeclaAbajo))
					{
						if (Seleccionado != Elementos-1) 
						{
							gotoxy(4, InicioY + 3 + Seleccionado);
							cout << "   ";
							gotoxy(4, InicioY + 3 + ++Seleccionado);
							cout << "=>>";
						} 
						else
						{
							gotoxy(4, InicioY + 3 + Seleccionado);
							cout << "   ";
							gotoxy(4, InicioY + 3);
							cout << "=>>";
							Seleccionado = 0;
						}
					} 
					else if (EstaPulsada(TeclaArriba))
					{
						if (Seleccionado != 0)
						{
							gotoxy(4, InicioY + 3 + Seleccionado);
							cout << "   ";
							gotoxy(4, InicioY + 3 + --Seleccionado);
							cout << "=>>";
						}
						else
						{
							gotoxy(4, InicioY + 3 + Seleccionado);
							cout << "   ";
							gotoxy(4, InicioY + 3 + Elementos-1);
							cout << "=>>";
							Seleccionado = Elementos-1;
						}
					}
					else if (EstaPulsada(TeclaEnter))
						return Seleccionado;
				} 
				else
				{
					if (EstaPulsada(TeclaAbajo))
					{
						if (Seleccionado != ElemPos[Col][1])
						{
							gotoxy(ElemPos[Col][2], ObtenerY());
							cout << "   ";
							Seleccionado++;
							gotoxy(ElemPos[Col][2], ObtenerY()+1);
							cout << "=>>";
						} 
						else 
						{
							gotoxy(ElemPos[Col][2], ObtenerY());
							cout << "   ";
							Seleccionado = ElemPos[Col][0];
							gotoxy(ElemPos[Col][2], InicioY+3);
							cout << "=>>";
						}
					} 
					else if (EstaPulsada(TeclaArriba))
					{
						if (Seleccionado != ElemPos[Col][0])
						{
							gotoxy(ElemPos[Col][2], ObtenerY());
							cout << "   ";
							Seleccionado--;
							gotoxy(ElemPos[Col][2], ObtenerY()-1);
							cout << "=>>";
						} 
						else 
						{
							gotoxy(ElemPos[Col][2], ObtenerY());
							cout << "   ";
							Seleccionado = ElemPos[Col][1];
							gotoxy(ElemPos[Col][2], InicioY+3+(Seleccionado-ElemPos[Col][0]));
							cout << "=>>";
						}
					}
					else if (EstaPulsada(TeclaDerecha) || EstaPulsada(TeclaIzquierda))
					{
						int MaxY = Elementos - (Elementos/2) + InicioY + 1;
						int PosY = ObtenerY();
						gotoxy(ElemPos[Col][2], PosY);
						cout << "   ";
						Col ^= 1;
						if (Col == 1 && ObtenerY() > MaxY)
							gotoxy(ElemPos[1][2], MaxY);		
						else
							gotoxy(ElemPos[Col][2], PosY);
						cout << "=>>";
						Seleccionado = ObtenerY() - (InicioY+3) + ElemPos[Col][0];
					} 
					else if (EstaPulsada(TeclaEnter))
					{
						return Seleccionado;
					}
				}
			}
		}
	}

	char CMenu::Ejecutar() 
	{
		if (!Elementos) return NULL;
		InicioY = ObtenerY();

		if (Elementos < 2)
			CantidadColumnas = 1;

		cout << "\n";

		ReducirCursor();
		PrintTitulo();
		PrintRelleno();
		PrintItems();
		return Items[IniciarSeleccion()]->GetID();
	}
}; 
