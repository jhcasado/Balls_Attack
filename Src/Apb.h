/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* apb.h */


#ifndef __APB_H__
#define __APB_H__


#include "t_imagen.h"
#include "t_boton.h"
#include "t_tablero.h"
#include "t_texto.h"
#include "t_raton.h"


void Error(char *s);
// muestra un dialogo de error

void Consola_Mensaje(char *mensaje);
// escribre un mensaje en la consola

BOOL Cargar_Ficheros();
// lee los datos desde el disco

BOOL Ventana_Victoria(int x, int y, int t, T_RATON rt, T_IMAGEN & destino);
// muestra la venta de victoria
	
int Ventana_Inicio(int x, int y, int t, T_RATON rt, T_IMAGEN & destino); 
// menu principal

BOOL Ventana_Jugador(int x, int y, int t, T_RATON rt, T_IMAGEN & destino);
// ventana que espera a que se conecte un jugador

BOOL Ventana_Partida(int x, int y, int t, T_RATON rt, T_IMAGEN & destino);
// ventana que busca una partida

BOOL Iniciar_Conexion();
// inicia la conexion TPC/IP

BOOL Iniciar_Raton(HINSTANCE hInstance, HWND hwnd);
// iniciliza el ratón

void App_Activa(BOOL b_activa);
// toma o devuelve los privilegios sobre el ratón

BOOL Iniciar_Modo_Video(HWND hwnd);
// iniciliza el ratón

BOOL Contestar_Mensajes_Jugador(char *mensaje);
// constesta los mensajes del jugador

int I_Seleccionar_Red();
// gestina la selección cuando jugamos en red

int I_Mover_Red();
// gestiona los movimientos cuando jugamos en red

int I_Movimiento_Sin_Red();
// gestiona los movimientos cuando jugamos sin red

BOOL Movimientos(T_RATON rt, T_IMAGEN & pantalla, T_IMAGEN & destino);
// función que controla al bucle pricipal 

BOOL Esperando_Jugador(int x, int y, int t);
// espera a que se conecte un jugador

BOOL Buscando_Partida(int x, int y, int t);
// busca una partida

BOOL MAIN(void);
// bucle principal de la aplicación

void Cerrar_Conexion();
// cierra la conexion de red

void Restaurar_Modo_Video();
// restaura el modo de video 

void Jugar_Red(BOOL conexion);
// activa/desactiva la red

void Cancelar_Partida();
// cancela la parida actual volviendo al menu principal


#endif


/* apb.h */

