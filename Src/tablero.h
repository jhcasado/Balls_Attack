/* ==============================================================================
	
    A.P.B.
    Versi�n: 1.0.0b

    Jos� Angel Montelongo Reyes
    Javier Honorio Casado Fern�ndez
    Gustavo Adolfo Negr�n Cerde�a

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* tablero.h */


#ifndef __TABLERO_H__
#define __TABLERO_H__


#include "t_imagen.h"
#include "t_tablero.h"
#include "t_raton.h"


void V_Colocar_Piezas(T_TABLERO & tablero_piezas);
// coloca las piezas en el tablero

BOOL B_Seleccionar_Pieza(BYTE by_turno, T_TABLERO & tablero_piezas, T_RATON raton, T_IMAGEN & destino);
// nos permite selecionar una pieza desde el tablero

BOOL B_Seleccionar_Movimiento(BYTE & by_turno, T_TABLERO & tablero_piezas, T_RATON raton, 
							  T_IMAGEN & destino, int & i_x_psel, int & i_y_psel, 
							  T_IMAGEN & fondo, BOOL cred, 
							  T_TEXTO & texto, T_RATON & rt, T_IMAGEN & puntero_raton, 
							  T_IMAGEN & destino_i, T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red);
// mueve una pieza ya seleccionada hacia una nueva posici�n

void V_Mover_Pieza(BYTE & by_turno, T_TABLERO & tablero_piezas, int i_xd, int i_yd, T_IMAGEN & fondo, BOOL cred, 
		           T_TEXTO & texto, T_RATON & rt, T_IMAGEN & puntero_raton, T_IMAGEN & destino, T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red);
// mueve una pieza dentro del tablero

void V_Dibujar_Tablero(T_TABLERO & tablero_piezas, T_IMAGEN & destino, T_IMAGEN & pieza, int x_nd = -1, int y_nd = -1);
// dibuaja el tablero en pantalla

int I_Victoria(T_TABLERO & tablero_piezas);
// devuelve quien ha ganado


#endif


/* tablero.h */
