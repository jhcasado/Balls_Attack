/* ==============================================================================
	
    A.P.B.
    Versi�n: 1.0.0b

    Jos� Angel Montelongo Reyes
    Javier Honorio Casado Fern�ndez
    Gustavo Adolfo Negr�n Cerde�a

    Multimedia
    Universidad de las Palmas de Gran Canaria	
	  	  
   ============================================================================== */

/* t_boton.cpp */

#include "t_boton.h"


// DEFINICION DE T_ESTILO_BOTON


void T_ESTILO_BOTON::V_Liberar()
// libera los recursos
// entrada : -
// salida : -
{
	imagen.V_Liberar();	
}


BOOL T_ESTILO_BOTON::B_Cargar(char *s_imagen) 
// carga un bot�n desde un fichero 
// entrada : s_imagen = nombre del fichero
// salida : TRUE = pudo cargar el fichero
//          FALSE = no pudo cargar el fichero
{
	return (imagen.B_Cargar(s_imagen));
}


// DEFINICION DE LA CLASE T_BOTON


T_BOTON::T_BOTON()
// constructor de la clase
// entrada : -
// salida : -
{
	i_x = 0;
	i_y = 0;
	i_trans = 255;
	b_pulsado = FALSE;
}


BOOL T_BOTON::B_Arriba(T_ESTILO_BOTON estilo, T_RATON raton, BOOL b_dibujar)
// nos informa si nos encontramos arriba del bot�n
// entrada : estilo = estilo del bot�n
//			 raton = variable con la informaci�n del rat�n
//           b_dibujar = repinta el bot�n
// salida : TRUE = arriba del rat�n
//          FALSE = no estamos arriba
{
	if ((raton.i_x > i_x) && 
		(raton.i_x < (i_x + (estilo.pi_region [NORMAL][X2] - estilo.pi_region [NORMAL][X1]))) &&
		(raton.i_y > i_y) &&
		(raton.i_y < (i_y + (estilo.pi_region [NORMAL][Y2] - estilo.pi_region [NORMAL][Y1]))))
	{
		if (b_dibujar)
		{    
		/*if (estilo.imagen.DW_Color(raton.i_x - i_x, raton.i_y - i_y) != 0)
			{
				return TRUE;
			}*/
			return TRUE;
		}
		
		else
		{
			return TRUE;
		}
	}
	
	return FALSE;
}


BOOL T_BOTON::B_Click(T_ESTILO_BOTON estilo, T_RATON raton, T_IMAGEN & destino, BOOL b_dibujar)
// conprueba si se ha pulsado el bot�n y lo dibuja
// entrada : estilo = estilo del bot�n
//			 raton = var con info del rat�n
//           destino = imagen de destino
//           b_dibujar = repinta el bot�n
// salida : TRUE = se ha pulsado el bot�n
//          FALSE = no se ha pulsado el bot�n
{
	BOOL b_result = FALSE;
	int i_tipo = NORMAL;

	if (B_Arriba(estilo, raton, b_dibujar) && b_pulsado && !raton.b_boton_izq)
	{
		i_tipo = ARRIBA;
		b_pulsado = FALSE;
		b_result = TRUE;
	}

	else if (B_Arriba(estilo, raton, b_dibujar) && raton.b_boton_izq)
	{
		i_tipo = PULSADO;
		b_pulsado = TRUE;
		b_result = FALSE;
	}

	else if (B_Arriba(estilo, raton, b_dibujar))
	{
		i_tipo = ARRIBA;
		b_pulsado = FALSE;
		b_result = FALSE;
	}

	else
	{
		i_tipo = NORMAL;
		b_pulsado = FALSE;
		b_result = FALSE;
	}

	if (b_dibujar)
	{
		estilo.imagen.B_Pegar_Transparencia(i_trans, i_x, i_y, destino, TRUE, 
		 									estilo.pi_region [i_tipo][X1], 
											estilo.pi_region [i_tipo][Y1],
											estilo.pi_region [i_tipo][X2],
											estilo.pi_region [i_tipo][Y2]);	
	}

	return b_result;
}


/* t_boton.cpp */


