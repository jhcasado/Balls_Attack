/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_texto.cpp */


#include "t_texto.h"


// FUNCIONES PUBLICAS //

	
void T_TEXTO::V_Liberar()
// libera los recursos ocupados por la clase
// entrada : -
// salida : -
{
	fuente.V_Liberar();
}


BOOL T_TEXTO::B_Cargar(char *s_nombre, int i_ancho_fuente, int i_alto_fuente)
// carga una fuente de texto
// entrada : s_nombre = nombre del fichero que contiene la fuente
// salida : FALSE si hubo algún problema si no TRUE
{
	i_ancho = i_ancho_fuente;
	i_alto = i_alto_fuente;

    return (fuente.B_Cargar(s_nombre));	
}
	
			
BOOL T_TEXTO::B_Escribir(int i_x, int i_y, char *s_texto, T_IMAGEN & destino, int i_trans) 
// escribe s_texto en las pos (i_x, i_y)
// entrada : i_x = pos inicial en x
//			 i_y = pos inicial en y
//			 s_texto = cadena de texto a escribir
//			 destino = imagen de destino
// salida : TRUE si todo fue bien si no FALSE
{
	BOOL b_result = FALSE;
	int i_tam;

	i_tam = strlen(s_texto);
	
	for (int c = 0; c < i_tam; c ++)
	{
 		b_result = fuente.B_Pegar_Transparencia(i_trans, i_x + (i_ancho * c), i_y, destino, TRUE, 
			                                    i_ancho * s_texto [c], 0, 
							   	                i_ancho * s_texto [c] + i_ancho, i_alto); 
		
		if (!b_result) return FALSE;
	}

	return b_result;
}


/* t_texto.cpp */

