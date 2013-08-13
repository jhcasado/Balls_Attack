/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_piezas.cpp */


#include <windows.h>

#include "t_piezas.h"


// definición de la clase T_TABLERO_MOV


T_TABLERO_MOV::T_TABLERO_MOV()
// constructor de la clase
// entrada : -
// salida : -
{	
	i_ex = -1;
	i_ey = -1;
	b_movimiento = FALSE;
}


// definición de la clase T_PIEZA


T_PIEZA::T_PIEZA(int i_tipo_p, int i_color_p, 
	             int i_x1_p, int i_y1_p, int i_x2_p, int i_y2_p)
// contructor de la clase
// entrada : -
// salida : -
{
	i_tipo = i_tipo_p;
	i_color = i_color_p,
	i_contador = 0;
	i_x1 = i_x1_p;
	i_y1 = i_y1_p;
	i_x2 = i_x2_p;
	i_y2 = i_y2_p;
}


// definición de la clase T_SECUNDARIA


void T_SECUNDARIA::V_Generar_Movimientos(int i_sx, int i_sy, T_PIEZA *p_tablero [9][12])
// genera los movimientos válidos
// entrada : i_sx = coordenada x de la pieza
//           i_sy = coordenada y de la pieza
//           p_tablero = tablero con las piezas
// salida : -
{
	if ((i_sx != -1) && (i_sy != -1))
	{		
		for (int i_cy = 0; i_cy < 12; i_cy ++) // borramos los movimientos anteriores
		{
			for (int i_cx = 0; i_cx < 9; i_cx ++)
			{
				p_tablero [i_sx][i_sy]->p_movimientos [i_cx][i_cy].b_movimiento = FALSE;
			}
		}
				
		if (p_tablero [i_sx][i_sy + 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx][i_sy + 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx][i_sy - 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx][i_sy - 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx - 1][i_sy] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx - 1][i_sy].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx + 1][i_sy] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx + 1][i_sy].b_movimiento = TRUE;
		}	

		// movimientos diagonales

		if (p_tablero [i_sx + 1][i_sy + 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx + 1][i_sy + 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx - 1][i_sy - 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx - 1][i_sy - 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx - 1][i_sy + 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx - 1][i_sy + 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx + 1][i_sy - 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx + 1][i_sy - 1].b_movimiento = TRUE;
		}	
	}
}


// definición de la clase T_PRINCIPAL


void T_PRINCIPAL::V_Generar_Movimientos(int i_sx, int i_sy, T_PIEZA *p_tablero [9][12])
// genera los movimientos válidos
// entrada : i_sx = coordenada x de la pieza
//           i_sy = coordenada y de la pieza
//           p_tablero = tablero con las piezas
// salida : -
{
	if ((i_sx != -1) && (i_sy != -1))
	{		
		for (int i_cy = 0; i_cy < 12; i_cy ++) // borramos los movimientos anteriores
		{
			for (int i_cx = 0; i_cx < 9; i_cx ++)
			{
				p_tablero [i_sx][i_sy]->p_movimientos [i_cx][i_cy].b_movimiento = FALSE;
			}
		}
				
		if (p_tablero [i_sx][i_sy + 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx][i_sy + 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx][i_sy - 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx][i_sy - 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx - 1][i_sy] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx - 1][i_sy].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx + 1][i_sy] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx + 1][i_sy].b_movimiento = TRUE;
		}	

		// movimientos diagonales

		if (p_tablero [i_sx + 1][i_sy + 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx + 1][i_sy + 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx - 1][i_sy - 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx - 1][i_sy - 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx - 1][i_sy + 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx - 1][i_sy + 1].b_movimiento = TRUE;
		}

		if (p_tablero [i_sx + 1][i_sy - 1] == NULL) // está vacía la siguiente pos
		{
			p_movimientos [i_sx + 1][i_sy - 1].b_movimiento = TRUE;
		}	
	}
}


/* t_pieza.cpp */


