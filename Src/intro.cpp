/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* intro.cpp */


#include <math.h>

#include "ddraw.h"
#include "intro.h"


BOOL B_Modo_Actual(int & i_ancho, int & i_alto, BYTE & by_bpp)
// devuelve las caracteristicas del modo actual
// entrada : -
// salida : i_ancho = ancho del modo actual
//          i_alto = alto del modo actual
//          by_bpp = bpp del modo actual
//          FALSE = hubo algún problema
{
	DDSURFACEDESC2 ddsurfacedesc;
	LPDIRECTDRAW7 lp_dd7 = NULL;
	 
	if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&lp_dd7, IID_IDirectDraw7, NULL)))
	{
		return FALSE;
	}

	ZeroMemory(&ddsurfacedesc, sizeof(ddsurfacedesc));
    ddsurfacedesc.dwSize = sizeof(ddsurfacedesc);

	if (FAILED(lp_dd7->GetDisplayMode(&ddsurfacedesc)))
	{
		return FALSE;
	}

	by_bpp = (BYTE)(ddsurfacedesc.ddpfPixelFormat.dwRGBBitCount);
    i_ancho = ddsurfacedesc.dwWidth;
	i_alto = ddsurfacedesc.dwHeight;

	if (lp_dd7 != NULL)
	{
		lp_dd7->Release();
	}

	return TRUE;
}


BOOL B_Borrar_Pantalla(T_IMAGEN & pantalla)
// realiza el scroll de presentación
// entrada : -
// salida : TRUE, si no hubo problemas
//          FALSE, se produjo algún error
{    	
	T_IMAGEN copia;

	copia.B_Crear(pantalla.i_ancho, pantalla.i_alto);
		
	if (FAILED(copia.lp_datos->Blt(NULL, pantalla.lp_pantalla, NULL, DDBLT_WAIT, NULL)))
	{
		return FALSE;
	}

	for (int n = 90; n < 200; n ++)
	{
		pantalla.B_Borrar(0);
		copia.B_Pegar(0, (int)(copia.i_alto * (1.0 - sin(n * (3.141592654 / 180.0)))), pantalla);

		pantalla.B_Flip();
	}

	copia.V_Liberar();
	
	return TRUE;
}


void V_Scroll_Inicial(HWND hwnd, T_IMAGEN & pantalla)
// llama a la función que realiza el scroll de presentación
// entrada : -
// salida : -
{
	int i_ancho, i_alto; 
	BYTE by_bpp;
			
	if (B_Modo_Actual(i_ancho, i_alto, by_bpp))
	{	
		if (by_bpp != 24)
		{
			pantalla.B_Pantalla();

			if (pantalla.B_Poner_Mondo_Video(hwnd, i_ancho, i_alto, by_bpp)) 
			{
				B_Borrar_Pantalla(pantalla);		
			}	
		}
	}
}


/* intro.cpp */
