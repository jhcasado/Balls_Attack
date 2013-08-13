/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_tablero.cpp */


//#include <iostream.h>
#include <windows.h>

#include "t_piezas.h"
#include "t_tablero.h"


// definición de la clase T_TABLERO


BOOL T_TABLERO::B_Dibujar_Tablero(T_IMAGEN & destino, T_IMAGEN & pieza, int x_nd, int y_nd)
// dibuja el tablero
{
	int i_cox, i_cfx, i_coy, i_cfy;

	for (int y = 0; y < 12; y ++)
	{
		for (int x = 0; x < 9; x ++)
		{
			if (p_tablero [x][y] != NULL)
			{	
				if ((x_nd != x) || (y_nd != y))
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
			if (p_tablero [x][y] != NULL)
			{	
				if ((x_nd != x) || (y_nd != y))
				{
					i_cox = p_tablero [x][y]->i_x1;
					i_cfx = p_tablero [x][y]->i_x2;
					i_coy = p_tablero [x][y]->i_y1;
					i_cfy = p_tablero [x][y]->i_y2;

					pieza.B_Pegar(219 + (x * 40) - 30,
								  49 + (y * 40) - 30,
								  destino, TRUE,
								  i_cox, i_coy, i_cfx, i_cfy);
				}
                
			}
		}
	}

	return TRUE;
}


BOOL T_TABLERO::B_Anim_Pieza(int i_xo, int i_yo, int i_xd, int i_yd, T_IMAGEN & fondo, BOOL cred, 
							 T_TEXTO & texto, T_RATON & rt, T_IMAGEN & puntero_raton,
							 T_IMAGEN & destino, T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red, 
							 BOOL esquina)							 
// animación de la pieza
{
	int x = i_xo, y = i_yo;
	int i_cox, i_cfx, i_coy, i_cfy;
	int eq = 0;

	i_cox = p_tablero [i_xo][i_yo]->i_x1;
	i_cfx = p_tablero [i_xo][i_yo]->i_x2;
	i_coy = p_tablero [i_xo][i_yo]->i_y1;
	i_cfy = p_tablero [i_xo][i_yo]->i_y2;
	
	for (float c = 0.0f; c < 1.0f; c += 0.10f)
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

		texto.B_Escribir(220, 10, "REALIZANDO MOVIMIENTO", destino);
	
		rt.B_Actualizar(); // actualizar el raton

		B_Dibujar_Tablero(destino, pieza, i_xo, i_yo);
	
		// dibujamos la pieza que se mueve
		// interpolación lineal simple (esto se puede simplificar...)
		if (!esquina)
		{
			x = (int)(((219 + (i_xo * 40) - 30) * (1.0f - c)) + ((219 + (i_xd * 40) - 30) * c));
			y = (int)(((49 + (i_yo * 40) - 30) * (1.0f - c)) + ((49 + (i_yd * 40) - 30) * c));
		
			pieza.B_Pegar_Luminosidad(127, x - 2, y + 3, destino, 400, 0, 500, 100);
			pieza.B_Pegar(x, y, destino, TRUE, i_cox, i_coy, i_cfx, i_cfy);
		}
		else // esquina == la pieza desaparece
		{
			eq = (int)(255 * c);			

			pieza.B_Pegar_Transparencia(255 - eq,
				                        219 + (x * 40) - 30,
										49 + (y * 40) - 30,
										destino, TRUE,
										i_cox, i_coy, i_cfx, i_cfy);			
		}
                	
		// puntero del ratón
		puntero_raton.B_Pegar_Transparencia(200, rt.i_x, rt.i_y, destino, TRUE); 	
	
		destino.B_Pegar(0, 0, pantalla);
		pantalla.B_Flip();
	}

	return TRUE;
}


T_TABLERO::T_TABLERO() 
// entrada : -
// salida : -
// constructor de la clase
{
	i_x = -1;
	i_y = -1;

	for (int i_cy = 0; i_cy < 12; i_cy ++)
	{
		for (int i_cx = 0; i_cx < 9; i_cx ++)
		{
			p_tablero [i_cx][i_cy] = NULL;
		}
	}
}


void T_TABLERO::V_Colocar_Piezas()
// entrada : -
// salida : -
// coloca las piezas en el tablero
{
	for (int i_cx = 3; i_cx < 6; i_cx ++)
	{
		p_tablero [i_cx][1] = new T_SECUNDARIA(NEGRA, 199, 0, 300, 100);
		p_tablero [i_cx][8] = new T_SECUNDARIA(BLANCA, 0, 0, 100, 100);
	}

	for (int i_cx = 3; i_cx < 6; i_cx ++)
	{
		p_tablero [i_cx][3] = new T_SECUNDARIA(NEGRA, 199, 0, 300, 100);
		p_tablero [i_cx][10] = new T_SECUNDARIA(BLANCA, 0, 0, 100, 100);	
	}

	p_tablero [3][2] = new T_SECUNDARIA(NEGRA, 199, 0, 300, 100);
	p_tablero [5][2] = new T_SECUNDARIA(NEGRA, 199, 0, 300, 100);

	p_tablero [3][9] = new T_SECUNDARIA(BLANCA, 0, 0, 100, 100);
	p_tablero [5][9] = new T_SECUNDARIA(BLANCA, 0, 0, 100, 100);

	p_tablero [4][2] = new T_PRINCIPAL(NEGRA, 299, 0, 400, 100);
	p_tablero [4][9] = new T_PRINCIPAL(BLANCA, 99, 0, 200, 100);
}


BOOL T_TABLERO::B_Mover_Pieza(int i_xd, int i_yd,
							  T_IMAGEN & fondo, BOOL cred, 
							  T_TEXTO & texto, T_RATON & rt, T_IMAGEN & puntero_raton,
					          T_IMAGEN & destino, T_IMAGEN & pantalla, T_IMAGEN & pieza, char *mensaje_red)
// entrada : i_xd = pos x de la nueva pos
//			 i_yd = pos y de la nueva pos
// salida : TRUE si pudo moverla, si no FALSE
// mueve la pieza actualmente seleccionada hasta la nueva pos
{	
	if (!p_tablero [i_x][i_y]->p_movimientos [i_xd][i_yd].b_movimiento)
	{
		return FALSE;
	}

	p_tablero [i_x][i_y]->i_contador = 1; 

	if (p_tablero [i_xd][i_yd] != NULL) delete p_tablero [i_xd][i_yd];
	
	B_Anim_Pieza(i_x, i_y, i_xd, i_yd, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red);	

	p_tablero [i_xd][i_yd] = p_tablero [i_x][i_y];
		
	p_tablero [i_x][i_y] = NULL;

	// colisiones y desplazamientos de otras bolas

	int i_dif_x, i_dif_y, i_nx, i_ny, i_para_nx, i_para_ny,
		i_para_nx2, i_para_ny2;
	BOOL b_salida;

	for (;;) 
	{	
		b_salida = FALSE;

		if (p_tablero [i_xd][i_yd]->i_tipo == SECUNDARIA)
		{   // pieza secundaria		
			i_dif_x = i_xd - i_x;
			i_dif_y = i_yd - i_y;

			i_nx = i_xd + i_dif_x;
			i_ny = i_yd + i_dif_y;

			if ((i_nx >= 0) && (i_nx < 9) && (i_ny >= 0) && (i_ny < 12))
			{	
				if ((p_tablero [i_nx][i_ny] != NULL) && 
					(p_tablero [i_nx][i_ny]->i_tipo == SECUNDARIA)) // desplazamos a otra pieza
				{
					// comprobamos que no tiene otra detrás

					i_para_nx = i_xd + 2 * i_dif_x;
					i_para_ny = i_yd + 2 * i_dif_y;

					if ((i_para_nx >= 0) && (i_para_nx < 9) && 
						(i_para_ny >= 0) && (i_para_ny < 12)) 
					{
						if (p_tablero [i_para_nx][i_para_ny] != NULL)
						{
							break;
						}
					}

					// procedemos a desplazar a la pieza

					if ((i_para_nx >= 0) && (i_para_nx < 9) && 
						(i_para_ny >= 0) && (i_para_ny < 12)) 
					{
						B_Anim_Pieza(i_nx, i_ny, i_para_nx, i_para_ny, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red);

						p_tablero [i_para_nx][i_para_ny] = p_tablero [i_nx][i_ny];					
						i_xd = i_para_nx;
						i_yd = i_para_ny;
						i_x = i_nx;
						i_y = i_ny;						
					}				

					else
					{
						B_Anim_Pieza(i_nx, i_ny, i_para_nx, i_para_ny, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red, TRUE);
						delete p_tablero [i_nx][i_ny];
					}
				
					p_tablero [i_nx][i_ny] = NULL; // borramos la pos actual
				}

				else
				{
					break;
				}
			}

			else
			{
				break;
			}
		}
	
		if (p_tablero [i_xd][i_yd]->i_tipo == PRINCIPAL)
		{   // principal
			i_dif_x = i_xd - i_x;
			i_dif_y = i_yd - i_y;

			i_nx = i_xd + i_dif_x;
			i_ny = i_yd + i_dif_y;

			if ((i_nx >= 0) && (i_nx < 9) && (i_ny >= 0) && (i_ny < 12))
			{	
				if (p_tablero [i_nx][i_ny] != NULL) // desplazamos a otra pieza
				{
					// comprobamos que no tiene otra detrás

					i_para_nx = i_xd + 2 * i_dif_x;
					i_para_ny = i_yd + 2 * i_dif_y;

					if ((i_para_nx >= 0) && (i_para_nx < 9) && 
						(i_para_ny >= 0) && (i_para_ny < 12)) 
					{
						if (p_tablero [i_para_nx][i_para_ny] != NULL)
						{
							break;
						}
					}

					// desplazamos hasta encontrar otra bola

					for (;;)
					{
						i_para_nx2 = i_xd + 3 * i_dif_x;
						i_para_ny2 = i_yd + 3 * i_dif_y;

						if ((i_para_nx2 >= 0) && (i_para_nx2 < 9) && 
							(i_para_ny2 >= 0) && (i_para_ny2 < 12)) 
						{
							if (p_tablero [i_para_nx2][i_para_ny2] != NULL)
							{
								break;
							}
						}

						if ((i_para_nx >= 0) && (i_para_nx < 9) && 
							(i_para_ny >= 0) && (i_para_ny < 12)) 
						{
							B_Anim_Pieza(i_nx, i_ny, i_para_nx, i_para_ny, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red);

							p_tablero [i_para_nx][i_para_ny] = p_tablero [i_nx][i_ny];
							p_tablero [i_nx][i_ny] = NULL;
							i_x = i_xd;
							i_y	= i_yd;		
							i_xd = i_nx;
							i_yd = i_ny;							
							i_nx = i_para_nx;
							i_ny = i_para_ny;
							i_para_nx = i_para_nx2;
							i_para_ny = i_para_ny2;
						}
						
						else 
						{	
							if (p_tablero [i_nx][i_ny] != NULL)
							{
								B_Anim_Pieza(i_nx, i_ny, i_para_nx, i_para_ny, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red, TRUE);
							}
							delete p_tablero [i_nx][i_ny];
							p_tablero [i_nx][i_ny] = NULL;
							b_salida = TRUE;
							break;
						}
					}

					// procedemos a desplazar la pieza

					if ((i_para_nx >= 0) && (i_para_nx < 9) && 
						(i_para_ny >= 0) && (i_para_ny < 12)) 
					{
						B_Anim_Pieza(i_nx, i_ny, i_para_nx, i_para_ny, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red);

						p_tablero [i_para_nx][i_para_ny] = p_tablero [i_nx][i_ny];					
						i_xd = i_para_nx;
						i_yd = i_para_ny;
						i_x = i_nx;
						i_y	= i_ny;
					}				

					else
					{		
	//					B_Anim_Pieza(i_nx, i_ny, i_para_nx, i_para_ny, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, TRUE);
						delete p_tablero [i_nx][i_ny];
					}
			
					p_tablero [i_nx][i_ny] = NULL; // borramos la pos actual
				}
			
				else
				{
					B_Anim_Pieza(i_xd, i_yd, i_nx, i_ny, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red);
					
					p_tablero [i_nx][i_ny] = p_tablero [i_xd][i_yd];
					p_tablero [i_xd][i_yd] = NULL;
					i_x = i_xd;
					i_y = i_yd;
					i_xd = i_nx;
					i_yd = i_ny;
				}
			}

			else
			{
				B_Anim_Pieza(i_xd, i_yd, i_xd, i_yd, fondo, cred, texto, rt, puntero_raton, destino, pantalla, pieza, mensaje_red, TRUE);
				delete p_tablero [i_xd][i_yd];
				p_tablero [i_xd][i_yd] = NULL;
				break;
			}
		}

		if (b_salida)
		{
			break;
		}
	}

	i_x = -1;
	i_y = -1;
	
	return TRUE;
}


BOOL T_TABLERO::B_Seleccionar_Pieza(int i_color, int i_xd, int i_yd, BOOL b_con)
// entrada : i_xd = pos x de la nueva pos
//			 i_yd = pos y de la nueva pos
//           i_color = indica de quien es el turno, BLANCO, NEGRO
//           b_con = consulta
// selecciona una pieza del tablero
{

	if ((i_xd < 0) || (i_xd > 8) || (i_yd < 0) || (i_yd > 11))
	{
		return FALSE;
	}

	if ((p_tablero [i_xd][i_yd] != NULL) && 
		(p_tablero [i_xd][i_yd]->i_color == i_color) &&
		(p_tablero [i_xd][i_yd]->i_contador == 0))
	{
		if (!b_con)
		{
			i_x = i_xd;
			i_y = i_yd;

			p_tablero [i_x][i_y]->V_Generar_Movimientos(i_x, i_y, p_tablero);
		}

		return TRUE;
	}

	return FALSE;
}


void T_TABLERO::V_Limpiar_Movimientos()
// limpia los movimientos de las piezas
// entrada : -
// salida : -
{
	for (int i_cx = 0; i_cx < 9; i_cx ++)
	{
		for (int i_cy = 0; i_cy < 12; i_cy ++)
		{
			if (p_tablero [i_cx][i_cy] != NULL)
			{				
				p_tablero [i_cx][i_cy]->i_contador = 0;
			}
		}
	}
}


/* t_tablero.cpp */

