/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* tablero.cpp */


#include "t_boton.h"
#include "tablero.h"


// VARIABLES GLOBALES


static int i_color = 0, i_color_pi = 0, i_inc_color = 4, i_inc_cpi = 8;
static T_BOTON boton;
static T_ESTILO_BOTON estilo_casilla;


void V_Colocar_Piezas(T_TABLERO & tablero_piezas)
// coloca las piezas en el tablero
// entrada : tablero_piezas = tablero para guardar las piezas
// salida : -
{
	for (int x = 0; x < 9; x ++) 
	{
		for (int y = 0; y < 12; y ++)
		{
			if (tablero_piezas.p_tablero [x][y] != NULL) 
			{
				delete tablero_piezas.p_tablero [x][y]; // borramos las piezas existentes
			}
			
			tablero_piezas.p_tablero [x][y] = NULL;
			tablero_piezas.i_x = -1;
			tablero_piezas.i_y = -1;
		}
	}

	tablero_piezas.V_Colocar_Piezas();
}


BOOL B_Seleccionar_Pieza(BYTE by_turno, T_TABLERO & tablero_piezas, T_RATON raton, T_IMAGEN & destino)
// nos permite selecionar una pieza desde el tablero
// entrada : tablero_piezas = tablero con las piezas
//           raton = variable que contiene información acerca del ratón
//           pantalla = dirección de la pantalla
//           destino = dirección de la imagen de destino
// salida : TRUE = se ha selecionado una pieza
//          FALSE = no se ha podido selecionar una pieza
{    
	DWORD dw_color_cuadro;
	BOOL b_selec;

	b_selec = tablero_piezas.B_Seleccionar_Pieza(by_turno, ((raton.i_x - 219) / 40), 
		                                         ((raton.i_y - 49) / 40), TRUE);

	if (b_selec)
	{
		dw_color_cuadro = destino.DW_RGB(0, 150, 100); // color para una selec correcta
	}

	else 
	{
		dw_color_cuadro = destino.DW_RGB(255, 0, 0); // color para una selec incorrecta
	}

	i_color += i_inc_color; // para el gradiante de color
	if (i_color > 150) 
	{ 
		i_inc_color = -i_inc_color; 
		i_color = 150; 
	}
	
	if (i_color < 50) 
	{ 
		i_inc_color = -i_inc_color; 
		i_color = 50; 
	}

	if ((raton.i_x >= 219) && (raton.i_y >= 49) && (raton.i_x <= 579) && (raton.i_y <= 529))
	{
		destino.B_Transparencia(i_color, dw_color_cuadro, 
								219 + 40 * ((raton.i_x - 219) / 40), 
								49 + 40 * ((raton.i_y - 49) / 40), 
								219 + 40 * ((raton.i_x - 219) / 40) + 40, 
								49 + 40 * ((raton.i_y - 49) / 40) + 40);

		destino.B_Dibujar_Rectangulo(255, dw_color_cuadro, 
						     	     219 + 40 * ((raton.i_x - 219) / 40), 
							         49 + 40 * ((raton.i_y - 49) / 40), 
							         219 + 40 * ((raton.i_x - 219) / 40) + 40, 
							         49 + 40 * ((raton.i_y - 49) / 40) + 40);

		boton.i_x = 219 + 40 * ((raton.i_x - 219) / 40);
		boton.i_y = 49 + 40 * ((raton.i_y - 49) / 40);

		estilo_casilla.pi_region [NORMAL][X1] = 0;
		estilo_casilla.pi_region [NORMAL][Y1] = 0;
		estilo_casilla.pi_region [NORMAL][X2] = 40;
		estilo_casilla.pi_region [NORMAL][Y2] = 40;
	
		estilo_casilla.pi_region [ARRIBA][X1] = 0;
		estilo_casilla.pi_region [ARRIBA][Y1] = 0;
		estilo_casilla.pi_region [ARRIBA][X2] = 40;
		estilo_casilla.pi_region [ARRIBA][Y2] = 40;

		estilo_casilla.pi_region [PULSADO][X1] = 0;
		estilo_casilla.pi_region [PULSADO][Y1] = 0;
		estilo_casilla.pi_region [PULSADO][X2] = 40;
		estilo_casilla.pi_region [PULSADO][Y2] = 40;		

		if (boton.B_Click(estilo_casilla, raton, destino, FALSE) && b_selec)
		{			
			tablero_piezas.B_Seleccionar_Pieza(by_turno, ((raton.i_x - 219) / 40), 
			                                   ((raton.i_y - 49) / 40));
			return TRUE; // hemos selecionado una pieza
		}
	}

	return FALSE;
}


BOOL B_Seleccionar_Movimiento(BYTE & by_turno, T_TABLERO & tablero_piezas, T_RATON raton, T_IMAGEN & destino, 
							  int & i_x_psel, int & i_y_psel,
							  T_IMAGEN & fondo, BOOL cred, 
							  T_TEXTO & texto, T_RATON & rt, 
							  T_IMAGEN & puntero_raton, T_IMAGEN & destino_i, 
							  T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red)
// mueve una pieza ya seleccionada hacia una nueva posición
// entrada : tablero_piezas = tablero con las piezas
//           raton = variable que contiene información acerca del ratón
//           pantalla = dirección de la pantalla
//           destino = dirección de la imagen de destino
// salida : TRUE = se ha selecionado una pieza
//          FALSE = no se ha podido selecionar una pieza
//          i_x_psel = pos x de la pieza seleccionada
//			i_y_psel = pos y de la pieza seleccionada
{    
	DWORD dw_color_cuadro;
	BOOL b_selec;
	int i_x_ps, i_y_ps;

	i_x_psel =  i_x_ps = ((raton.i_x - 219) / 40);
	i_y_psel =  i_y_ps = ((raton.i_y - 49) / 40);

	b_selec = tablero_piezas.p_tablero [tablero_piezas.i_x][tablero_piezas.i_y]->p_movimientos [i_x_ps][i_y_ps].b_movimiento;
	
	if ((tablero_piezas.i_x == i_x_ps) && (tablero_piezas.i_y == i_y_ps))
	{
		b_selec = TRUE;
	}

	if (b_selec)
	{
		dw_color_cuadro = destino.DW_RGB(0, 0, 200); // color hacia donde podemos mover
	}

	else 
	{
		dw_color_cuadro = destino.DW_RGB(255, 0, 0); // color hacia donde no podemos mover
	}

	i_color += i_inc_color; // realizamos el gradiante de colores
	if (i_color > 150) 
	{ 
		i_inc_color = -i_inc_color; 
		i_color = 150; 
	}
	
	if (i_color < 50) 
	{ 
		i_inc_color = -i_inc_color; 
		i_color = 50; 
	}

	if ((raton.i_x >= 219) && (raton.i_y >= 49) && (raton.i_x <= 579) && (raton.i_y <= 529))
	{
		destino.B_Transparencia(i_color, dw_color_cuadro, 
								219 + 40 * ((raton.i_x - 219) / 40), 
								49 + 40 * ((raton.i_y - 49) / 40), 
								219 + 40 * ((raton.i_x - 219) / 40) + 40, 
								49 + 40 * ((raton.i_y - 49) / 40) + 40);
	
		destino.B_Dibujar_Rectangulo(255, dw_color_cuadro, 
		        					 219 + 40 * ((raton.i_x - 219) / 40), 
								     49 + 40 * ((raton.i_y - 49) / 40), 
								     219 + 40 * ((raton.i_x - 219) / 40) + 40, 
								     49 + 40 * ((raton.i_y - 49) / 40) + 40);
	
		boton.i_x = 219 + 40 * ((raton.i_x - 219) / 40);
		boton.i_y = 49 + 40 * ((raton.i_y - 49) / 40);

		estilo_casilla.pi_region [NORMAL][X1] = 0;
		estilo_casilla.pi_region [NORMAL][Y1] = 0;
		estilo_casilla.pi_region [NORMAL][X2] = 40;
		estilo_casilla.pi_region [NORMAL][Y2] = 40;
	
		estilo_casilla.pi_region [ARRIBA][X1] = 0;
		estilo_casilla.pi_region [ARRIBA][Y1] = 0;
		estilo_casilla.pi_region [ARRIBA][X2] = 40;
		estilo_casilla.pi_region [ARRIBA][Y2] = 40;

		estilo_casilla.pi_region [PULSADO][X1] = 0;
		estilo_casilla.pi_region [PULSADO][Y1] = 0;
		estilo_casilla.pi_region [PULSADO][X2] = 40;
		estilo_casilla.pi_region [PULSADO][Y2] = 40;

		if (raton.b_boton_der) 
		{
			tablero_piezas.i_x = -1;
			tablero_piezas.i_y = -1; 	
			V_Dibujar_Tablero(tablero_piezas, destino, pieza);
			return TRUE; // deseleccionamos la pieza al pulsar el botón derecho
		}

		if (boton.B_Click(estilo_casilla, raton, destino, FALSE) && b_selec)
		{			
			if ((tablero_piezas.i_x == i_x_ps) && (tablero_piezas.i_y == i_y_ps))
			{
				tablero_piezas.i_x = -1;
				tablero_piezas.i_y = -1; // hemos deseleccionado la pieza
				V_Dibujar_Tablero(tablero_piezas, destino, pieza);				
			}

			else
			{
				V_Mover_Pieza(by_turno, tablero_piezas, i_x_ps, i_y_ps,
							  fondo, cred, texto, rt, 
						      puntero_raton, destino_i, pantalla, pieza, mensaje_red); // movemos la pieza
			}

			return TRUE;
		}
	}

	return FALSE;
}


void V_Mover_Pieza(BYTE & by_turno, T_TABLERO & tablero_piezas, int i_xd, int i_yd,
				   T_IMAGEN & fondo, BOOL cred, 
		           T_TEXTO & texto, T_RATON & rt, 
				   T_IMAGEN & puntero_raton, T_IMAGEN & destino, 
				   T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red)
// mueve una pieza dentro del tablero
// entrada : tablero_piezas = tablero con las piezas
//			 i_xd = pos x de la pieza
//           i_yd = pos y de la pieza
// salida : -
{
	BOOL b_principal = FALSE, b_movimiento = FALSE;			

	if (tablero_piezas.p_tablero [tablero_piezas.i_x][tablero_piezas.i_y]->i_tipo == PRINCIPAL)
	{
		b_principal = TRUE;
	}

	b_movimiento = tablero_piezas.B_Mover_Pieza(i_xd, i_yd,
		                                        fondo, cred, texto, rt, 
						                        puntero_raton, destino, pantalla, pieza, mensaje_red);

	if (b_movimiento && b_principal)				
	{
		by_turno ++;

		if (by_turno > 2) 
		{
			by_turno = BLANCA;
		}
	
		tablero_piezas.V_Limpiar_Movimientos();
	}
}


void V_Dibujar_Tablero(T_TABLERO & tablero_piezas, T_IMAGEN & destino, T_IMAGEN & pieza, int x_nd, int y_nd)
// dibuaja el tablero en pantalla
// entrada : tablero_piezas = tablero con las piezas
//           destino = imagen destino
//           pieza = imagen con el dibujo de las piezas
// salida : -
{
	int i_cox = 0, i_cfx = 0, i_coy = 0, i_cfy = 0; // coordenadas que componen el cuadro de la pieza

	i_color_pi += i_inc_cpi; // gradiante de color
	if (i_color_pi > 150) 
	{ 
		i_inc_cpi = -i_inc_cpi; 
		i_color_pi = 150; 
	}
	
	if (i_color_pi < 50) 
	{ 
		i_inc_cpi = -i_inc_cpi; 
		i_color_pi = 50; 
	}

	for (int y = 0; y < 12; y ++)
	{
		for (int x = 0; x < 9; x ++)
		{
			if (tablero_piezas.p_tablero [x][y] != NULL)
			{	
				if ((tablero_piezas.i_x != x) || (tablero_piezas.i_y != y))
				{
					pieza.B_Pegar_Luminosidad(127, 
											  219 + (x * 40) - 32, 
											  49 + (y * 40) - 27, 
											  destino, 
											  400, 0, 500, 100);
                }
			}
		}
	}
		
	for (int y = 0; y < 12; y ++)
	{
		for (int x = 0; x < 9; x ++)
		{
			if (tablero_piezas.p_tablero [x][y] != NULL)
			{	
				i_cox = tablero_piezas.p_tablero [x][y]->i_x1;
				i_cfx = tablero_piezas.p_tablero [x][y]->i_x2;
				i_coy = tablero_piezas.p_tablero [x][y]->i_y1;
				i_cfy = tablero_piezas.p_tablero [x][y]->i_y2;
	
				if ((tablero_piezas.i_x == x) && (tablero_piezas.i_y == y))
				{
					pieza.B_Pegar_Transparencia(i_color_pi, 219 + (x * 40) - 30,
												49 + (y * 40) - 30,
												destino, TRUE,
												i_cox, i_coy, i_cfx, i_cfy);
				}	

				else
				{
					pieza.B_Pegar(219 + (x * 40) - 30,
								  49 + (y * 40) - 30,
								  destino, TRUE,
								  i_cox, i_coy, i_cfx, i_cfy);
                }
			}
		}
	}
}


int I_Victoria(T_TABLERO & tablero_piezas)
// devuelve quien ha ganado
// entrada : tablero_piezas = tablero con las piezas
// salida : 0 = no ha ganado ninguno todavía
//          BLANCA = las blancas ganan
//          NEGRA = las negras ganan
{
	BOOL b_pr_ng = FALSE, b_pr_bl = FALSE;

	for (int y = 0; y < 12; y ++)
	{
		for (int x = 0; x < 9; x ++)
		{
			if ((tablero_piezas.p_tablero [x][y] != NULL) && 
				(tablero_piezas.p_tablero [x][y]->i_tipo == PRINCIPAL)) 
			{	
				if (tablero_piezas.p_tablero [x][y]->i_color == BLANCA)
				{
					b_pr_bl = TRUE;
				}

				if (tablero_piezas.p_tablero [x][y]->i_color == NEGRA)
				{
					b_pr_ng = TRUE;
				}
			}
		}
	}

	if (!b_pr_ng) return NEGRA;
	if (!b_pr_bl) return BLANCA;
	return 0;
}


/* tablero.cpp */


