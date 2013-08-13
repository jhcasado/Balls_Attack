/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* apb.cpp */


#include <math.h>
#include <stdio.h>
#include <string.h>


#include "red.h"
#include "intro.h"
#include "tablero.h"
#include "apb.h"


static BYTE i_turno = BLANCA;
static int i_color, i_mc = 4, i_cp = -1, i_cpt, 
           i_victoria = 0, jugador;
static T_ESTILO_BOTON ebc, eb1;
static T_BOTON bc, bc1, bc2, bc3, bc4, bc5;
static T_TABLERO tablero;
static T_IMAGEN piezas, ventana, pantalla, fondo, puntero_raton, destino;;
static T_TEXTO texto;
static RED red;
static T_RATON rt;
static char mensaje_red [255];
static const GUID guid = { 0xcb19c7c0, 0x3294, 0x11d4, { 0x82, 0x45, 0x0, 0x40, 0x33, 0x9f, 0x96, 0xbe } };

static int i_xrt, i_yrt;
static float yd = - 260.0;
static float grados = -90.0;
static BOOL dir = 0;
static BOOL cred = TRUE;

	
void Error(char *s)
// muestra un dialogo de error
// entrada : s = mensaje de error
// salida : -
{
    MessageBox(NULL, s, "¡¡¡ ERROR !!!", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);  
}


void Consola_Mensaje(char *mensaje)
// escribre un mensaje en la consola
// entrada : mensaja = texto a escribir
// salida : -
{
	destino.B_Borrar(destino.DW_RGB(0, 0, 0));
	texto.B_Escribir(10, 10, mensaje, destino);
	destino.B_Pegar(0, 0, pantalla);
	pantalla.B_Flip();
}


BOOL Cargar_Ficheros()
// lee los datos desde el disco
// entrada : -
// salida : FALSE si hubo un error al cargar
{
	destino.B_Crear(800, 600);

	if (!texto.B_Cargar("Img\\TB_10x16_Normal.bmp", 10, 16))
	{
		return FALSE;
	}

	Consola_Mensaje("LEYENDO PIEZAS...");
	if (!piezas.B_Cargar("Img\\piezas.bmp"))
	{
		return FALSE;
	}

	Consola_Mensaje("LEYENDO BOTON...");
	if (!eb1.B_Cargar("Img\\boton.bmp")) 
	{
		return FALSE;
	}

	Consola_Mensaje("LEYENDO VENTANA...");
	if (!ventana.B_Cargar("Img\\ventana.bmp")) 
	{
		return FALSE;
	}

	Consola_Mensaje("LEYENDO FONDO...");
	if (!fondo.B_Cargar("Img\\fondo.bmp")) 
	{
		return FALSE;
	}

	Consola_Mensaje("LEYENDO PUNTERO...");
	if (!puntero_raton.B_Cargar("Img\\puntero.bmp")) 
	{
		return FALSE;	
	}
	
	eb1.pi_region [NORMAL][X1] = 0;
	eb1.pi_region [NORMAL][Y1] = 0;
	eb1.pi_region [NORMAL][X2] = 125;
	eb1.pi_region [NORMAL][Y2] = 35;
	
	eb1.pi_region [ARRIBA][X1] = 0;
	eb1.pi_region [ARRIBA][Y1] = 35;
	eb1.pi_region [ARRIBA][X2] = 125;
	eb1.pi_region [ARRIBA][Y2] = 70;

	eb1.pi_region [PULSADO][X1] = 0;
	eb1.pi_region [PULSADO][Y1] = 70;
	eb1.pi_region [PULSADO][X2] = 125;
	eb1.pi_region [PULSADO][Y2] = 105;
	
	return TRUE;
}


BOOL Ventana_Victoria(int x, int y, int t, T_RATON rt, T_IMAGEN & destino) 
// muestra la venta de victoria
// entrada : (x, y) = pos
//           t = transparencia
//           rt = ratón
//           destino = imagen de destino 
// salida : TRUE si se pulsa aceptar
{
	BOOL result = FALSE;

	ventana.B_Pegar_Transparencia((int)(t / 1.3), x, y, destino);
	texto.B_Escribir(10 + x, y + 15, "FINAL DE LA PARTIDA", destino, t);

	bc1.i_x = 87 + x;
	bc1.i_y = 180 + y;
	bc1.i_trans = t;

	if (bc1.B_Click(eb1, rt, destino))
	{
		result = TRUE;
	}

	texto.B_Escribir(x + 87 + 27, y + 190, "ACEPTAR", destino, t);

	if (i_victoria == BLANCA)
	{
		texto.B_Escribir(x + 10, y + 50, "LAS NEGRAS GANAN", destino, t);
	}

	if (i_victoria == NEGRA)
	{
		texto.B_Escribir(x + 10, y + 50, "LAS BLANCAS GANAN", destino, t);
	}

	return result;
}

	
int Ventana_Inicio(int x, int y, int t, T_RATON rt, T_IMAGEN & destino) 
// menu principal
// entrada : (x, y) = pos
//           t = transparencia
//           rt = ratón
//           destino = imagen de destino 
// salida : la opción pulsada
{
	int result = 0;

	ventana.B_Pegar_Transparencia((int)(t / 1.3), x, y, destino);
	texto.B_Escribir(x + 10, y + 15, "MENU", destino, t);

	bc1.i_x = 87 + x;
	bc1.i_y = 80 + y;
	bc1.i_trans = t;

	if (bc1.B_Click(eb1, rt, destino))
	{
		result = 1;
	}
	
	texto.B_Escribir(87 + 37 + x, y + 90, "NUEVO", destino, t);
			
	bc2.i_x = 87 + x;
	bc2.i_y = 130 + y;
	bc2.i_trans = t;

	if (bc2.B_Click(eb1, rt, destino))
	{
		result = 2;
	}

	texto.B_Escribir(87 + 32 + x, y + 140, "BUSCAR", destino, t);

	bc3.i_x = 87 + x;
	bc3.i_y = 180 + y;
	bc3.i_trans = t;

	if (bc3.B_Click(eb1, rt, destino))
	{
		result = 3;
	}

	texto.B_Escribir(87 + 37 + x, 190 + y, "SALIR", destino, t);

	return result;
}


BOOL Ventana_Jugador(int x, int y, int t, T_RATON rt, T_IMAGEN & destino)
// ventana que espera a que se conecte un jugador
// entrada : (x, y) = pos
//           t = transparencia
//           rt = ratón
//           destino = imagen de destino 
// salida : TRUE si se pulsa cancelar
{
	BOOL result = FALSE;

	i_color += i_mc;
	if (i_color > 200) 
	{ 
		i_mc = -i_mc; 
		i_color = 200; 
	}
	
	if (i_color < 25) 
	{ 
		i_mc = -i_mc ; 
		i_color = 25; 
	}

	ventana.B_Pegar_Transparencia((int)(t / 1.3), x, y, destino);
	texto.B_Escribir(x + 10, y + 15, "ESPERANDO JUGADOR", destino, t);

	texto.B_Escribir(x + 10 + i_color, y + 35, "ESPERE", destino, t);
			
	bc5.i_x = 87 + x;
	bc5.i_y = 180 + y;
	bc5.i_trans = t;

	if (bc5.B_Click(eb1, rt, destino))
	{
		result = TRUE;
	}

	texto.B_Escribir(87 + 22 + x, 190 + y, "CANCELAR", destino, t);

	return result;
}


BOOL Ventana_Partida(int x, int y, int t, T_RATON rt, T_IMAGEN & destino)
// ventana que busca una partida
// entrada : (x, y) = pos
//           t = transparencia
//           rt = ratón
//           destino = imagen de destino 
// salida : TRUE si se pulsa cancelar
{
	BOOL result = FALSE;

	i_color += i_mc;
	if (i_color > 200) 
	{ 
		i_mc = -i_mc; 
		i_color = 200; 
	}
	
	if (i_color < 25) 
	{ 
		i_mc = -i_mc ; 
		i_color = 25; 
	}

	ventana.B_Pegar_Transparencia((int)(t / 1.3), x, y, destino);
	texto.B_Escribir(x + 10, y + 15, "BUSCANDO PARTIDA", destino, t);

	texto.B_Escribir(x + 10 + i_color, y + 35, "ESPERE", destino, t);
			
	bc4.i_x = x + 87;
	bc4.i_y = y + 180;
	bc4.i_trans = t;

	if (bc4.B_Click(eb1, rt, destino))
	{
		result = TRUE;
	}

	texto.B_Escribir(x + 87 + 22, y + 190, "CANCELAR", destino, t);

	return result;
}


BOOL Iniciar_Conexion()
// inicia la conexion TPC/IP
// entrada : -
// salida : FALSE si no pudo realizar la conexión
{
	if (!red.B_TCPIP_Disponible())
	{
		return FALSE;
	}

	if(!red.B_Iniciar_Conexion_TCPIP())
	{		
		return FALSE;
	}

	strcpy_s(mensaje_red, "RED OK");

	return TRUE;
}


BOOL Iniciar_Raton(HINSTANCE hInstance, HWND hwnd)
// iniciliza el ratón
// entrada : hInstance = instancia 
//           hwnd = handle a la ventana
// salida : FALSE si hubo algún error
{
	if (!rt.B_Inicializar(hInstance, hwnd))
	{
		return FALSE;
	}

	rt.B_App_Activa(TRUE);

	return TRUE;
}


void App_Activa(BOOL b_activa)
// toma o devuelve los privilegios sobre el ratón
// entrada : b_activa = var que indica si la ap está activa
// salida : -
{
	rt.B_App_Activa(b_activa);
}


BOOL Iniciar_Modo_Video(HWND hwnd)
// iniciliza el ratón
// entrada : hwnd = handle a la ventana
// salida : FALSE si hubo algún error
{
	V_Scroll_Inicial(hwnd, pantalla);

    pantalla.V_Liberar();
	pantalla.B_Pantalla();
	
	if (!pantalla.B_Poner_Mondo_Video(hwnd, 800, 600, 32))
	{
		return FALSE;
	}
	
	pantalla.B_Borrar(pantalla.DW_RGB(0, 0, 0));
	pantalla.B_Flip();
	pantalla.B_Borrar(pantalla.DW_RGB(0, 0, 0));
	pantalla.B_Flip();
	
	// configuramos el ratón

	rt.i_x = pantalla.i_ancho / 2;
	rt.i_y = pantalla.i_alto / 2;
	
	rt.f_vel_x = 1.0;
	rt.f_vel_y = 1.0;
	rt.f_vel_z = 1.0;

	rt.r_zona.right = pantalla.i_ancho;
	rt.r_zona.bottom = pantalla.i_alto;

	return TRUE;
}


BOOL Contestar_Mensajes_Jugador(char *mensaje)
// constesta los mensajes del jugador
// entrada = mensaje = mensaje recibido
// salida = FALSE si hubo algún error
{
    LPVOID datos = NULL;
	DWORD tam;

	switch (red.DW_Recivir(datos, &tam))
	{
		case MSG_ERROR:
			i_cp = 0;
			strcpy_s(mensaje_red, "MENSAJE ERROR");
			red.B_Cerrar_Sesion();
		break;

		case MSG_DESCONOCIDO:
			i_cp = 0;
			strcpy_s(mensaje_red, "MENSAJE DESCONOCIDO");
			red.B_Cerrar_Sesion();							
		break;

		case MSG_NO_HAY:
						
		break;

		case MSG_CREA_JUGADOR:
			i_cp = 0;
			strcpy_s(mensaje_red, "CREAR JUGADOR");
			red.B_Cerrar_Sesion();							
		break;

		case MSG_DESTRUYE_JUGADOR:
			i_cp = 0;
			strcpy_s(mensaje_red, "DESTRUIR JUGADOR");
			red.B_Cerrar_Sesion();										
		break;

		case MSG_MENSAJE_ENVIADO:
			
		break;

		case MSG_SESION_PERDIDA:
			i_cp = 0;
			strcpy_s(mensaje_red, "SESION PERDIDA");
			red.B_Cerrar_Sesion();							
		break;

		case MSG_PERSONAL: // mis mensajes
			strcpy_s(mensaje_red, "RED OK");
			strcpy(mensaje, (char *)(datos));
			return TRUE;
		break;						
	}

	return FALSE;
}


int I_Seleccionar_Red()
// gestina la selección cuando jugamos en red
// entrada : -
// salida : 0
{
	// BLACAS MUEVEN

	if ((jugador == 1) && (i_turno == BLANCA))
	{
		texto.B_Escribir(420, 10, "TURNO BLANCAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR UNO", destino);

		if (B_Seleccionar_Pieza(i_turno, tablero, rt, destino))
		{
			char dato [2];
			DWORD size = 2;

			dato [0] = tablero.i_x;
			dato [1] = tablero.i_y; 

			red.B_Enviar(ID_TODOS, dato, size);
			i_cp = 5;
			V_Dibujar_Tablero(tablero, destino, piezas);
			return 0;
		}
	}

	if ((jugador == 2) && (i_turno == BLANCA))
	{
		char str [2];
		
		str [0] = 0;
		str [1] = 0;
		texto.B_Escribir(420, 10, "TURNO BLANCAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR DOS", destino);

		if (Contestar_Mensajes_Jugador(str)) // selecionamos las piezas del contrario
		{  
 		    tablero.B_Seleccionar_Pieza(i_turno, str [0], str [1]);								
			i_cp = 5;
			V_Dibujar_Tablero(tablero, destino, piezas);
			return 0;
		}
	}

	// NEGRAS MUEVEN

	if ((jugador == 1) && (i_turno == NEGRA))
	{
		char str [2];
	
		str [0] = 0;
		str [1] = 0;				
		texto.B_Escribir(420, 10, "TURNO NEGRAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR UNO", destino);

		if (Contestar_Mensajes_Jugador(str)) // selecionamos las piezas del contrario
		{ 				 
			tablero.B_Seleccionar_Pieza(i_turno, str [0], str [1]);							                               
			i_cp = 5;
			V_Dibujar_Tablero(tablero, destino, piezas);
			return 0;
		}
	}

	if ((jugador == 2) && (i_turno == NEGRA))
	{
		texto.B_Escribir(420, 10, "TURNO NEGRAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR DOS", destino);

		if (B_Seleccionar_Pieza(i_turno, tablero, rt, destino))
		{
			char dato [2];
			DWORD size = 2;

			dato [0] = tablero.i_x;
			dato [1] = tablero.i_y; 

			red.B_Enviar(ID_TODOS, dato, size);
			i_cp = 5;
			V_Dibujar_Tablero(tablero, destino, piezas);
			return 0;
		}
	}
	
	V_Dibujar_Tablero(tablero, destino, piezas);
	return 0;
}


int I_Mover_Red()
// gestiona los movimientos cuando jugamos en red
// entrada : -
// salida : 1 si se produce la victoria			
{	
	// BLANCAS MUEVEN
	
	if ((jugador == 1) && (i_turno == BLANCA))
	{
		texto.B_Escribir(420, 10, "TURNO BLANCAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR UNO", destino);

		if (B_Seleccionar_Movimiento(i_turno, tablero, rt, destino, i_xrt, i_yrt,
									 fondo, cred, texto, rt, 
						             puntero_raton, destino, pantalla, piezas, mensaje_red))
		{
			char dato [2];
			DWORD size = 2;

			dato [0] = i_xrt;
			dato [1] = i_yrt; 					

			red.B_Enviar(ID_TODOS, dato, size);
			i_cp = 4;
		
			if ((i_victoria = I_Victoria(tablero)) != 0)
			{
				return 1;
			}
			
			return 0;
		}
		
		V_Dibujar_Tablero(tablero, destino, piezas);
		return 0;
	}

	if ((jugador == 2) && (i_turno == BLANCA))
	{
		char str [2];
		str [0] = 0;
		str [1] = 0;

		texto.B_Escribir(420, 10, "TURNO BLANCAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR DOS", destino);

		if (Contestar_Mensajes_Jugador(str)) // movemos las piezas del contrario
		{  
			if ((tablero.i_x == str [0]) && (tablero.i_y == str [1]))
			{
				tablero.i_x = -1;
				tablero.i_y = -1;
			}

			else
			{
				//tablero.B_Mover_Pieza(str [0], str [1]);

				V_Mover_Pieza(i_turno, tablero, str [0], str [1], fondo, cred, 
							  texto, rt, puntero_raton, destino, pantalla, piezas, mensaje_red);
				
				if ((i_victoria = I_Victoria(tablero)) != 0)
				{
					return 1;
				}

				//tablero.B_Mover_Pieza(str [0], str [1], fondo, cred, texto, rt, 
				//		                puntero_raton, destino, pantalla, piezas);										     
			}

			i_cp = 4;
		}
		
		V_Dibujar_Tablero(tablero, destino, piezas);
		return 0;
	}

	// NEGRAS MUEVEN

	if ((jugador == 1) && (i_turno == NEGRA))
	{
		char str [2];
		str [0] = 0;
		str [1] = 0;

		texto.B_Escribir(420, 10, "TURNO NEGRAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR UNO", destino);

		if (Contestar_Mensajes_Jugador(str)) // selecionamos las piezas del contrario
		{
			if ((tablero.i_x == str [0]) && (tablero.i_y == str [1]))
			{
				tablero.i_x = -1;
				tablero.i_y = -1;
			}
			
			else
			{
			//	tablero.B_Mover_Pieza(str [0], str [1]);
			//  tablero.B_Mover_Pieza(str [0], str [1], fondo, cred, texto, rt, 
			//			              puntero_raton, destino, pantalla, piezas);
				
				V_Mover_Pieza(i_turno, tablero, str [0], str [1], fondo, cred, 
							  texto, rt, puntero_raton, destino, pantalla, piezas, mensaje_red);

				if ((i_victoria = I_Victoria(tablero)) != 0)
				{
					return 1;
				}
			}
					
			i_cp = 4;
		}

		V_Dibujar_Tablero(tablero, destino, piezas);
		return 0;
	}

	if ((jugador == 2) && (i_turno == NEGRA))
	{
		texto.B_Escribir(420, 10, "TURNO NEGRAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR DOS", destino);

		if (B_Seleccionar_Movimiento(i_turno, tablero, rt, destino, i_xrt, i_yrt,
									 fondo, cred, texto, rt, 
						             puntero_raton, destino, pantalla, piezas, mensaje_red))
		{
			char dato [2];
			DWORD size = 2;

			dato [0] = i_xrt;
			dato [1] = i_yrt; 

			red.B_Enviar(ID_TODOS, dato, size);
			i_cp = 4;
			
			if ((i_victoria = I_Victoria(tablero)) != 0)
			{
				return 1;
			}
		}
		
		else
		{
			V_Dibujar_Tablero(tablero, destino, piezas);
		}
		return 0;
	}

	return 0;
}


int I_Movimiento_Sin_Red()
// gestiona los movimientos cuando jugamos sin red
// entrada : -
// salida : 1 si se produce la victoria			
{
	int i_xps, i_yps;

	i_victoria = I_Victoria(tablero);

	if (i_victoria != 0)
	{
		return 1;
	}

	if (i_turno == BLANCA)
	{
		texto.B_Escribir(420, 10, "TURNO BLANCAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR UNO", destino);
	}

	if (i_turno == NEGRA)
	{
		texto.B_Escribir(420, 10, "TURNO NEGRAS", destino);
		texto.B_Escribir(220, 10, "JUGADOR DOS", destino);
	}

	switch (i_cp)
	{
		case 4 :
			if (B_Seleccionar_Pieza(i_turno, tablero, rt, destino))
			{
				i_cp = 5;
			}									
		break;

		case 5:
			if (B_Seleccionar_Movimiento(i_turno, tablero, rt, destino, i_xps, i_yps, 									 
									     fondo, cred, texto, rt, 
						                 puntero_raton, destino, pantalla, piezas, mensaje_red))
			{
				i_cp = 4;
				return 0;
			}
		break;
	}

	V_Dibujar_Tablero(tablero, destino, piezas);

	return 0;
}


BOOL Movimientos(T_RATON rt, T_IMAGEN & pantalla, T_IMAGEN & destino)
// función que controla al bucle pricipal 
// entrada : rt = ratón
//           pantalla = imagen que apunta a la pantalla
//           destino = imagen de destino
// salida : FALSE si abandonamos la aplicación
{
	if ((yd < 79.0) && (dir == 0))
	{
		grados += 6.0;
		yd = (float)(180.0 * (sin(grados * (3.141592654 / 180.0))) - 100);
	}

	if ((yd > -260.0) && (dir == 1))
	{
		grados -= 6.0;
		yd = (float)(180.0 * (sin(grados * (3.141592654 / 180.0))) - 100);
	}

	else if (dir == 1)
	{
		i_cp = i_cpt;
		dir = 0;
	}

	switch (i_cp)
	{
		case -1:
			V_Colocar_Piezas(tablero);
			i_cp = 0;		

		case 0: // menu			
			V_Dibujar_Tablero(tablero, destino, piezas);			
			i_cp = Ventana_Inicio(250, (int)(yd), (int)(grados * 2.5), rt, destino);
			
			if (i_cp != 0)
			{
				i_cpt = i_cp;
				i_cp = 0;			
				dir = 1;
			}
		break;
	
		case 1: // nuevo			
			if (!cred)
			{
				i_cp = 4;
			}
			
			if (yd > 79.0)
			{
				if (!Esperando_Jugador(250, (int)(yd), (int)(grados * 2.5)))
				{
					i_cpt = -1;				
					dir = 1;
				}
				else
				{
					jugador = 2;				
					i_cpt = 4;
					dir = 1;
				}
			}

			else
			{
				V_Dibujar_Tablero(tablero, destino, piezas);
				Ventana_Jugador(250, (int)(yd), (int)(grados * 2.5), rt, destino);
			}
		break;

		case 2: // buscar
			if (!cred)
			{
				i_cp = 4;
			}

			if (yd > 79.0)
			{	
				if (!Buscando_Partida(250, (int)(yd), (int)(grados * 2.5)))
				{
					i_cpt = -1;
					dir = 1;
				}

				else
				{
					jugador = 1;
					i_cpt = 4;
					dir = 1;
				}
			}

			else
			{
				V_Dibujar_Tablero(tablero, destino, piezas);
				Ventana_Partida(250, (int)(yd), (int)(grados * 2.5), rt, destino);
			}
		break;

		case 3: // salir			
			red.B_Cerrar_Conexion();
			return FALSE;
		break;

		case 4 :			
			if (cred)
			{
				I_Seleccionar_Red();
				break;
			}
		case 5 :
			int br;

			if (!cred) // jugamos sin red
			{
				br = I_Movimiento_Sin_Red();
			}

			else
			{
				br = I_Mover_Red();
			}
			
			if (br) // final de la partida
			{
				i_cp = 6;				
				yd = -260.0;
				grados = -90.0;
				dir = 0;
				V_Dibujar_Tablero(tablero, destino, piezas);
			}						
		break;

		case 6:
			V_Dibujar_Tablero(tablero, destino, piezas);
			
			if (Ventana_Victoria(250, (int)(yd), (int)(grados * 2.5), rt, destino))
			{
				V_Colocar_Piezas(tablero);				
				i_turno = BLANCA;			
				i_cpt = 0;
				dir = 1;
				if (cred)
				{
					red.B_Cerrar_Sesion();
				}
			}
		break;
	}

	return TRUE;
}


BOOL Esperando_Jugador(int x, int y, int t)
// espera a que se conecte un jugador
// entrada : (x, y) pos de la ventana
//           t = transparencia
// salida : FALSE si no se cancela la espera
{		
    LPVOID datos = NULL;
	DWORD tam;

	if (!red.B_Crear_Sesion(guid, "Partida"))
	{
		return FALSE;
	}

	if (!red.B_Crear_Jugador("jugador 1"))
	{
		return FALSE;
	}
	
	while (red.DW_Recivir(datos, &tam) != MSG_CREA_JUGADOR)
	{
		rt.B_Actualizar(); // actualizar el raton
					
		fondo.B_Pegar(0, 0, destino);
		texto.B_Escribir(10, 570, mensaje_red, destino);
		V_Dibujar_Tablero(tablero, destino, piezas);

		if (Ventana_Jugador(x, y, t, rt, destino))
		{
			red.B_Cerrar_Sesion();
			return FALSE;
		}

		// puntero del raton
		puntero_raton.B_Pegar_Transparencia(200, rt.i_x, rt.i_y, destino, TRUE); 
		
		destino.B_Pegar(0, 0, pantalla);
		pantalla.B_Flip();
	}

	return TRUE;	

	// empezamos a jugar
}


BOOL Buscando_Partida(int x, int y, int t)
// busca una partida
// entrada : (x, y) pos de la ventana
//           t = transparencia
// salida : FALSE si no se cancela la busqueda
{
	if (!red.B_Enumerar_Sesiones(guid))
	{
		return FALSE;
	}

	while (red.L_Tamano() == 0)
	{
		if (!red.B_Enumerar_Sesiones(guid))
		{
			red.B_Parar_Enumeracion(guid);
			return FALSE;
		}

		rt.B_Actualizar(); // actualizar el raton
		
		fondo.B_Pegar(0, 0, destino);
		texto.B_Escribir(10, 570, mensaje_red, destino);

		V_Dibujar_Tablero(tablero, destino, piezas);

		if (Ventana_Partida(x, y, t, rt, destino))
		{
			red.B_Parar_Enumeracion(guid);
			return FALSE;
		}

		puntero_raton.B_Pegar_Transparencia(200, rt.i_x, rt.i_y, destino, TRUE); 		
		
		destino.B_Pegar(0, 0, pantalla);
		pantalla.B_Flip();
	}

    if (!red.B_Abrir_Sesion(0))
	{
		return FALSE;
	}

	if (!red.B_Crear_Jugador("jugador 2"))
	{
		return FALSE;
	}

	return TRUE; // empezamos a jugar
}


BOOL MAIN(void)
// bucle principal de la aplicación
// entrada : -
// salida : FALSE si hubo algún error
{    		
	fondo.B_Pegar(0, 0, destino);
	
	if (!cred)
	{
		texto.B_Escribir(10, 570, "SIN RED", destino);
	}
	
	else
	{
		texto.B_Escribir(10, 570, mensaje_red, destino);
	}
		
	rt.B_Actualizar(); // actualizar el raton

	if (!Movimientos(rt, pantalla, destino))
	{
		return FALSE;
	}
	
	// puntero del ratón
	puntero_raton.B_Pegar_Transparencia(200, rt.i_x, rt.i_y, destino, TRUE); 	
	
	destino.B_Pegar(0, 0, pantalla);
	pantalla.B_Flip();
	
	return TRUE;
}


void Cerrar_Conexion()
// cierra la conexion de red
// entrada : -
// salida : -
{
	red.B_Cerrar_Conexion();
}


void Restaurar_Modo_Video()
// restaura el modo de video 
// entrada : -
// salida : -
{
	pantalla.Restaurar_Modo_Video();
}


void Jugar_Red(BOOL conexion)
// activa/desactiva la red
// entrada : conexion = estado de la connexion
// salida : -
{
	if (i_cp == 0)
	{
		cred = conexion;
	}
}


void Cancelar_Partida()
// cancela la parida actual volviendo al menu principal
// entrada : -
// salida : -
{
	if ((i_cp == 4) || (i_cp == 5))
	{
		yd = - 260.0;
		grados = -90.0;
		dir = 0;
		i_turno = BLANCA;
		jugador = 1;
		V_Colocar_Piezas(tablero);
		i_cp = 0;
		red.B_Cerrar_Sesion();
	}
}


/* apb.cpp */

