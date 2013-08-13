/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_tablero.h */


#ifndef __T_TABLERO_H__
#define __T_TABLERO_H__


#include "t_imagen.h"
#include "t_raton.h"
#include "t_texto.h"
#include "t_piezas.h"


class T_TABLERO
{
private :

	BOOL B_Dibujar_Tablero(T_IMAGEN & destino, T_IMAGEN & pieza, int x_nd, int y_nd);
	// dibuja el tablero con las piezas

	BOOL B_Anim_Pieza(int i_xo, int i_yo, int i_xd, int i_yd, T_IMAGEN & fondo, BOOL cred, 
		              T_TEXTO & texto, T_RATON & rt, T_IMAGEN & puntero_raton,
					  T_IMAGEN & destino, T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red,
					  BOOL esquina = FALSE);
	// animación de la pieza

public :

	T_PIEZA *p_tablero [9][12]; // tablero con las piezas
	int i_x, i_y; // pos de la pieza actualmente seleccionada

	T_TABLERO(); 
	// constructor de la clase

	void V_Colocar_Piezas();
	// coloca las piezas en el tablero

	BOOL B_Mover_Pieza(int i_xd, int i_yd, T_IMAGEN & fondo, BOOL cred, 
		               T_TEXTO & texto, T_RATON & rt, T_IMAGEN & puntero_raton,
					   T_IMAGEN & destino, T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red);
	// mueve la pieza actualmente seleccionada hasta la nueva pos

	BOOL B_Seleccionar_Pieza(int i_color, int i_xd, int i_yd, BOOL b_con = FALSE);
	// selecciona una pieza del tablero

	void V_Limpiar_Movimientos();
	// limpia la tabla de movimientos

};


#endif


/* t_tablero.h */

