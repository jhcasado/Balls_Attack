/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_imagen.cpp */


#include "t_imagen.h"


// VARIABLES GLOBALES 


static BYTE pby_tabla_lum [MAX_TABLA_LUM]; // tabla pecalculada para las opr de luminosidad
static LPDIRECTDRAW7 lp_dd = NULL; // puntero al objeto direct draw


// FUNCIONES PRIVADAS 


static int I_Pos_Color(DWORD dw_masc)
// devuelve la pos de un compenete de color
// entrada : i_mask = mascara del color
// salida : la pos
{
	int i_c = 0;

	while (!(dw_masc & 1))
	{
		i_c ++;
		dw_masc >>= 1;		
	}

	return i_c;
}


static void V_Crear_Tablas()
// precalcula la tabla para las transparencias y para el brillo
// entrada : -
// salida : -
{
	int i_color;

	for (int i_b = 0; i_b < 128; i_b ++)
	{
		for (int i_c = 0; i_c < 256; i_c ++)
		{
			i_color = i_c - ((127 - i_b) * 2);

			if (i_color < 0) i_color = 0;

			pby_tabla_lum [(i_b << 8) + i_c] = (BYTE)(i_color); 
		}
	}

	for (int i_bm = 128; i_bm < 256; i_bm ++)
	{
		for (int i_c = 0; i_c < 256; i_c ++)
		{
			i_color = i_c + ((i_bm - 128) * 2);

			if (i_color > 255) i_color = 255;

			pby_tabla_lum [(i_bm << 8) + i_c] = (BYTE)(i_color); 
		}
	}
}


// FUNCIONES PUBLICAS //


T_IMAGEN::T_IMAGEN()
// constructor de la clase 
// entrada : -
// saldia : -
{
    i_ancho = -1;
	i_alto = -1;
    lp_datos = NULL;
	b_pantalla = FALSE;
	lp_pantalla = NULL;
	i_buffer_posterior = 0;
}


void T_IMAGEN::V_Liberar()
// libera los recursos
// entrada : -
// salida : -
{
    if ((lp_datos != NULL) && !(b_pantalla))
    {
	    lp_datos->Release();
		lp_datos = NULL;
	}

    if ((lp_dd != NULL) && (lp_pantalla != NULL) && (b_pantalla))
    {
		lp_pantalla->Release();
        lp_pantalla = NULL;
		lp_datos = NULL;
		b_pantalla = FALSE;
			
		lp_dd->Release();
		lp_dd = NULL;
    }    
}


BYTE T_IMAGEN::By_Bpp()
// devuelve el nº de bits por pixel
// entrada : -
// salida : el nº de bits por pixel
{
	return (BYTE)(ddsd.ddpfPixelFormat.dwRGBBitCount);
}


BOOL T_IMAGEN::B_Recortar(int i_xd, int i_yd, T_IMAGEN & destino, 
   				  	      int i_xo1, int i_yo1, int i_xo2, int i_yo2, 
						  RECT & r_o, RECT & r_d)
// recorta dos rectangulos que representan las dimensiones de las imagenes
// entrada : i_xd = desplazamiento en x
//			 i_yd = desplazamiento en y
//           destino = imagen de destino
//           (i_xo1, i_yo1, i_xo2, i_yo2) coordenadas que componen el rectángulo destino
//           r_o = rectángulo de origen
//           r_d = rectángulo de destino una vez ha sido recortado
// salida : TRUE = existe recorte
//          FALSE = no existe recorte, se sale completamente
{
	if (i_xo1 == -1) i_xo1 = 0;
	if (i_xo2 == -1) i_xo2 = i_ancho;	
	if (i_yo1 == -1) i_yo1 = 0;
	if (i_yo2 == -1) i_yo2 = i_alto;

	// para x

	if (i_xd > destino.i_ancho) // se sale por la derch todo
	{
	    return FALSE;
	}

	if (((i_xo2 - i_xo1) + i_xd) > destino.i_ancho) // se sale por la derch parte
	{
		i_xo2 = i_xo2 - (((i_xo2 - i_xo1) + i_xd) - destino.i_ancho);
	}
  
	if (((i_xo2 - i_xo1) + i_xd) < 0) // se sale por la izq todo
	{
		return FALSE;
	}

	if (i_xd < 0) // se sale por la izq parte
	{
		i_xo1 = i_xo1 - i_xd;
		i_xd = 0;
	}

	// para y

	if (i_yd > destino.i_alto) // se sale por abajo todo
	{
	    return FALSE;
	}

	if (((i_yo2 - i_yo1) + i_yd) > destino.i_alto) // se sale por abajo parte
	{
		i_yo2 = i_yo2 - (((i_yo2 - i_yo1) + i_yd) - destino.i_alto);
	}
  
	if (((i_yo2 - i_yo1) + i_yd) < 0) // se sale por arriba todo
	{
		return FALSE;
	}

	if (i_yd < 0) // se sale por arriba parte
	{
		i_yo1 = i_yo1 - i_yd; 
		i_yd = 0;		
	}

	// ajustamos los rectángulos

	r_o.left = i_xo1;
	r_o.top = i_yo1;
	r_o.right = i_xo2;
	r_o.bottom = i_yo2;
	
	r_d.left = i_xd;
	r_d.top = i_yd;
	r_d.right = i_xd + (i_xo2 - i_xo1);
	r_d.bottom = i_yd + (i_yo2 - i_yo1);

	return TRUE;
}


BOOL T_IMAGEN::B_Recortar_Origen(int i_xo1, int i_yo1, int i_xo2, int i_yo2, RECT & r_o)
// recorta dos rectangulos que representan las dimensiones de las imagenes
// entrada : i_xd = desplazamiento en x
//			 i_yd = desplazamiento en y
//           destino = imagen de destino
//           i_xo1, i_yo1, i_xo2, i_yo2, coordenadas que componen el rectángulo destino
//           r_o = rectángulo de origen
//           r_d = rectángulo de destino una vez ha sido recortado
// salida : TRUE = existe recorte
//          FALSE = no existe recorte, se sale completamente
{
	if (i_xo1 == -1) i_xo1 = 0;
	if (i_xo2 == -1) i_xo2 = i_ancho;	
	if (i_yo1 == -1) i_yo1 = 0;
	if (i_yo2 == -1) i_yo2 = i_alto;

	if (i_xo1 > i_ancho) // todo por la derecha
	{
	    return FALSE;
	}

	if (i_xo2 > i_ancho) // se sale por la derch parte
	{
		i_xo2 = i_ancho;
	}
  
	if (i_xo2 < 0) // se sale por la izq todo
	{
		return FALSE;
	}

	if (i_xo1 < 0) // se sale por la izq parte
	{
		i_xo1 = 0; 
	}

	// para y

	if (i_yo1 > i_alto) // se sale por abajo todo
	{
	    return FALSE;
	}

	if (i_yo2 > i_alto) // se sale por abajo parte
	{
		i_yo2 = i_alto;
	}
  
	if (i_yo2 < 0) // se sale por arriba todo
	{
		return FALSE;
	}

	if (i_yo1 < 0) // se sale por arriba parte
	{
		i_yo1 = 0; 
	}

	// ajustamos el rectángulo

	r_o.left = i_xo1;
	r_o.top = i_yo1;
	r_o.right = i_xo2;
	r_o.bottom = i_yo2;
	
	return TRUE;
}


BOOL T_IMAGEN::B_Crear(int i_x, int i_y)
// crea una imagen
// entrada : i_x = i_ancho
//           i_y = i_alto
// salida : TRUE = todo fue bien
//          FALSE = no se pudo crear la imagen
{
	if (lp_dd == NULL) return FALSE;

	ZeroMemory(&ddsd, sizeof(ddsd));

    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | 
		           DDSD_WIDTH | DDSD_CKSRCBLT; 
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwHeight = i_y; 
    ddsd.dwWidth = i_x;
    ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = 0;
    ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = 0;

    if (FAILED(lp_dd->CreateSurface(&ddsd, &lp_datos, NULL)))
	{
		return FALSE;
	}

	if (FAILED(lp_datos->GetSurfaceDesc(&ddsd)))
	{
		return FALSE;
	}

    i_ancho = i_x;
	i_alto = i_y;
	b_pantalla = FALSE;

	return TRUE;
}


BOOL T_IMAGEN::B_Cargar(char *s_nombre)
// carga una imagen desde el disco
// entrada : s_nombre = nombre del fichero
// salida : TRUE = pudo cargar la imagen
//          FALSE = no pudo cargar la imagen
{
    HBITMAP         hbm;
	BITMAP		    bm;
    HDC             hdc_image = NULL;
    HDC             hdc_surf = NULL;
    BOOL            b_return = FALSE;

    hbm = (HBITMAP)LoadImage(NULL, s_nombre, IMAGE_BITMAP, 0, 0,
		                     LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
    if (hbm == NULL) 
	{
        goto exit;
    }
    
	GetObject(hbm, sizeof(bm), &bm); 

  	if (!B_Crear(bm.bmWidth, bm.bmHeight))
	{
		goto exit;
	}

    hdc_image = CreateCompatibleDC(NULL);
    SelectObject(hdc_image, hbm);
	
    if (FAILED(lp_datos->GetDC(&hdc_surf)))
	{
        goto exit;
    }
      
    if (BitBlt(hdc_surf, 0, 0, bm.bmWidth, bm.bmHeight, hdc_image, 0, 0, SRCCOPY) == FALSE) 
	{
        goto exit;
    }

    b_return = TRUE;
    
	exit :

    if (hdc_surf) lp_datos->ReleaseDC(hdc_surf);
    if (hdc_image) DeleteDC(hdc_image);
    if (hbm) DeleteObject(hbm);

    return b_return;
}


BOOL T_IMAGEN::B_Borrar(DWORD dw_color, int i_xo1, int i_yo1, int i_xo2, int i_yo2)
// borra una zona de la imagen con un color determinado
// entrada : dw_color = color con el que se borrará la imagen
//           (i_xo1, i_yo1, i_xo2, i_yo2) = zona a borrar
// salida : TRUE = pudo borrar
//          FALSE = no pudo borrar
{
    RECT r_o;
    DDBLTFX ddbltfx;

    if (!B_Recortar_Origen(i_xo1, i_yo1, i_xo2, i_yo2, r_o)) 
	{
		return TRUE;
	}

    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = dw_color;
		
	if (FAILED(lp_datos->Blt(&r_o, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx)))
	{
       	return FALSE;
	}

	return TRUE;
}


BOOL T_IMAGEN::B_Pegar(int i_xd, int i_yd, T_IMAGEN & destino, BOOL b_sprite, 
				  	   int i_xo1, int i_yo1, int i_xo2, int i_yo2)
// pega una imagen en otra
// entrada : i_xd = desplazamiento en x
//           i_yd = desplazamiento en y
//           destino = imagen de destino
//           b_sprite = TRUE, es un sprite
//           (i_xo1, i_yo1, i_xo2, i_yo2) = zona de la imagen de origen a pegar
// salida : TRUE = pudo pegar
//          FALSE = hubo algún error
{
    RECT r_o, r_d;

	if (!B_Recortar(i_xd, i_yd, destino, i_xo1, i_yo1, i_xo2, i_yo2, r_o, r_d)) 
	{
		return TRUE;
	}

	if (b_sprite)
	{
	  	if (FAILED(destino.lp_datos->Blt(&r_d, lp_datos, &r_o, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			return FALSE;
		}
	}

	else
	{
	  	if (FAILED(destino.lp_datos->Blt(&r_d, lp_datos, &r_o, DDBLT_WAIT, NULL)))
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL T_IMAGEN::B_Luminosidad(int i_lum, int i_xo1, int i_yo1, int i_xo2, int i_yo2)
// realiza un cambio de luminosidad de una zona determinada
// entrada : i_lum = cantidad de luminosidad
//           (i_xo1, i_yo1, i_xo2, i_yo2) = zona de la imagen de origen
// salida : TRUE = pudo cambiar el brillo
//          FALSE = hubo algún error
{
    RECT r_o;
	DWORD i_bmr, i_bmg, i_bmb;
	int i_pr, i_pg, i_pb;		
	
	if ((i_lum < 0)  || (i_lum > 255)) 
	{
		return FALSE;
	}

    if (!B_Recortar_Origen(i_xo1, i_yo1, i_xo2, i_yo2, r_o))
	{
		return TRUE;
	}

	V_Pos_RGB(i_pr, i_pg, i_pb);
	V_Mascara_RGB(i_bmr, i_bmg, i_bmb);

	if (FAILED(lp_datos->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) 
	{	
		return FALSE;
	}

	DWORD *pdw_org = (DWORD *)ddsd.lpSurface + (i_ancho * r_o.top) + r_o.left;
	DWORD *pdw_final_org = (DWORD *)ddsd.lpSurface + (i_ancho * r_o.bottom) + r_o.right - i_ancho;
	DWORD *pdw_final_linea_org = pdw_org + (r_o.right - r_o.left) - i_ancho; 

	int i_incr_org = i_ancho - (r_o.right - r_o.left);
	int i_incr_linea_org = i_ancho;
	i_lum <<= 8;
	
	for (; pdw_org < pdw_final_org; pdw_org += i_incr_org)
	{
	    pdw_final_linea_org += i_incr_linea_org;

		for (; pdw_org < pdw_final_linea_org; pdw_org ++)
		{    	
    		*pdw_org = ((*(pby_tabla_lum + i_lum + ((*pdw_org & i_bmr) >> i_pr)) << i_pr) | 
				        (*(pby_tabla_lum + i_lum + ((*pdw_org & i_bmg) >> i_pg)) << i_pg) |
				   	    (*(pby_tabla_lum + i_lum + ((*pdw_org & i_bmb) >> i_pb)) << i_pb)); 
		}
	}

    if (FAILED(lp_datos->Unlock(NULL)))
	{
        return FALSE;
    }	
	
	return TRUE;
}


BOOL T_IMAGEN::B_Pegar_Luminosidad(int i_lum, int i_xd, int i_yd, 
						           T_IMAGEN & destino,
						           int i_xo1, int i_yo1, int i_xo2, int i_yo2)
// pega una imagen en otra con luminosidad
// entrada : i_lum = luminosidad de la imagen
//           i_xd = desplazamiento en x
//           i_yd = desplazamiento en y
//           destino = imagen de destino
//           (i_xo1, i_yo1, i_xo2, i_yo2) = zona de la imagen de origen a pegar
// salida : TRUE = pudo pegar
//          FALSE = hubo algún error
{
    RECT r_o, r_d;
	int  i_r, i_g, i_b;
    DWORD i_bmr, i_bmg, i_bmb;
	int i_pr, i_pg, i_pb;

	if ((i_lum < 0)  || (i_lum > 255)) 
	{
		return FALSE;
	}
	
	if (!B_Recortar(i_xd, i_yd, destino, i_xo1, i_yo1, i_xo2, i_yo2, r_o, r_d)) 
	{
		return TRUE;
	}

	V_Pos_RGB(i_pr, i_pg, i_pb);
	V_Mascara_RGB(i_bmr, i_bmg, i_bmb);

	if (FAILED(lp_datos->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) 
	{	
		return FALSE;
	}
   
	if (FAILED(destino.lp_datos->Lock(NULL, &destino.ddsd, DDLOCK_WAIT, NULL))) 
	{	
		return FALSE;
	}
	
	DWORD *pdw_org = (DWORD *)ddsd.lpSurface + (i_ancho * r_o.top) + r_o.left;
	DWORD *pdw_dest = (DWORD *)destino.ddsd.lpSurface + (destino.i_ancho * r_d.top) + r_d.left;
	DWORD *pdw_final_org = (DWORD *)ddsd.lpSurface + (i_ancho * r_o.bottom) + r_o.right - i_ancho;
	DWORD *pdw_final_linea_org = pdw_org + (r_o.right - r_o.left) - i_ancho; 

	int i_incr_org = i_ancho - (r_o.right - r_o.left);
	int i_incr_linea_org = i_ancho;
	int i_incr_dest = destino.i_ancho - (r_o.right - r_o.left);
	i_lum <<= 8;

	for (; pdw_org < pdw_final_org; pdw_org += i_incr_org, pdw_dest += i_incr_dest)
	{
	    pdw_final_linea_org += i_incr_linea_org;

		for (; pdw_org < pdw_final_linea_org; pdw_org ++, pdw_dest ++)
		{   			
			i_r = *(pby_tabla_lum + i_lum + ((*pdw_org & i_bmr) >> i_pr)) << 8;
			i_g = *(pby_tabla_lum + i_lum + ((*pdw_org & i_bmg) >> i_pg)) << 8;
			i_b = *(pby_tabla_lum + i_lum + ((*pdw_org & i_bmb) >> i_pb)) << 8;

  		    *pdw_dest = ((*(pby_tabla_lum + i_r + ((*pdw_dest & i_bmr) >> i_pr)) << i_pr) | 
			   	         (*(pby_tabla_lum + i_g + ((*pdw_dest & i_bmg) >> i_pg)) << i_pg) |
			 		     (*(pby_tabla_lum + i_b + ((*pdw_dest & i_bmb) >> i_pb)) << i_pb)); 	
	  	}
	}
	
    if (FAILED(lp_datos->Unlock(NULL)))
	{
        return FALSE;
    }	

    if (FAILED(destino.lp_datos->Unlock(NULL)))
	{
        return FALSE;
    }	
	
	return TRUE;
}


BOOL T_IMAGEN::B_Transparencia(int i_trans, DWORD dw_color_t, 
							   int i_xo1, int i_yo1, int i_xo2, int i_yo2)
// realiza una transparencia desde la imagen actual a un color color_t
// entrada : i_trans = cantidad de transparencia
//           dw_color_t = color hacia el que realizar la transparencia
//           (i_xo1, i_yo1, i_xo2, i_yo2) = zona de la imagen de origen
// salida : TRUE = pudo realizar la trans
//          FALSE = hubo algún error
{
    RECT r_o;
	int i_ctr, i_ctg, i_ctb;
	DWORD i_bmr, i_bmg, i_bmb;
	int i_pr, i_pg, i_pb,
		i_inv_trans;

	if ((i_trans < 0) || (i_trans > 255))
	{
		return FALSE;
	}
		 
	if (i_trans == 0) 
	{
		return TRUE;
	}
	
	if (i_trans == 255) 
	{
		return (B_Borrar(dw_color_t, i_xo1, i_yo1, i_xo2, i_yo2));
	}

    if (!B_Recortar_Origen(i_xo1, i_yo1, i_xo2, i_yo2, r_o))
	{
		return TRUE;
	}

	if (FAILED(lp_datos->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) 
	{	
		return FALSE;
	}

	V_Pos_RGB(i_pr, i_pg, i_pb);
	V_Mascara_RGB(i_bmr, i_bmg, i_bmb);

	i_inv_trans = 255 - i_trans;

	i_ctr = (((dw_color_t & i_bmr) >> i_pr) * i_trans);
	i_ctg = (((dw_color_t & i_bmg) >> i_pg) * i_trans);
	i_ctb = (((dw_color_t & i_bmb) >> i_pb) * i_trans);

	DWORD *pdw_org = (DWORD *)ddsd.lpSurface + (i_ancho * r_o.top) + r_o.left;
	DWORD *pdw_final_org = (DWORD*)ddsd.lpSurface + (i_ancho * r_o.bottom) + (r_o.right - i_ancho);
	DWORD *pdw_final_linea_org = pdw_org + ((r_o.right - r_o.left) - i_ancho); 

	int i_incr_org = (i_ancho - (r_o.right - r_o.left)); 
	int i_incr_linea_org = i_ancho;

	for (; pdw_org < pdw_final_org; pdw_org += i_incr_org)
	{
	    pdw_final_linea_org += i_incr_linea_org;

		for (; pdw_org < pdw_final_linea_org; pdw_org ++)
		{    
			*pdw_org = (((((((*pdw_org & i_bmr) >> i_pr) * i_inv_trans) + i_ctr) >> 8) << i_pr) |
				        ((((((*pdw_org & i_bmg) >> i_pg) * i_inv_trans) + i_ctg) >> 8) << i_pg) |
					    ((((((*pdw_org & i_bmb) >> i_pb) * i_inv_trans) + i_ctb) >> 8) << i_pb));		
		}
	}	

    if (FAILED(lp_datos->Unlock(NULL)))
	{
        return FALSE;
    }	
	
	return TRUE;
}


BOOL T_IMAGEN::B_Pegar_Transparencia(int i_trans, int i_xd, int i_yd, 
	  					             T_IMAGEN & destino, BOOL b_sprite, 
						             int i_xo1, int i_yo1, int i_xo2, int i_yo2)
// pega una imagen en otra con transparencia
// entrada : i_trans = cantidad de transparencia
//           i_xd = desplazamiento en x
//           i_yd = desplazamiento en y
//           destino = imagen de destino
//           b_sprite = TRUE, es un sprite
//           (i_xo1, i_yo1, i_xo2, i_yo2) = zona de la imagen de origen a pegar
// salida : TRUE = pudo pegar
//          FALSE = hubo algún error
{
    RECT r_o, r_d;
    int i_ctr, i_ctg, i_ctb;
  	DWORD i_bmr, i_bmg, i_bmb;
	int i_pr, i_pg, i_pb,
		i_inv_trans;		
    
	if ((i_trans < 0) || (i_trans > 255))
	{
		return FALSE;
	}

	if (i_trans == 0) 
	{
		return TRUE;
	}
	
	if (i_trans == 255) 
	{
		return (B_Pegar(i_xd, i_yd, destino, b_sprite, i_xo1, i_yo1, i_xo2, i_yo2));
	}

	if (!B_Recortar(i_xd, i_yd, destino, i_xo1, i_yo1, i_xo2, i_yo2, r_o, r_d))
	{
		return TRUE;
	}

	if (FAILED(lp_datos->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) 
	{	
		return FALSE;
	}

	if (FAILED(destino.lp_datos->Lock(NULL, &destino.ddsd, DDLOCK_WAIT, NULL))) 
	{	
		return FALSE;
	}

	V_Pos_RGB(i_pr, i_pg, i_pb);
	V_Mascara_RGB(i_bmr, i_bmg, i_bmb);

	i_inv_trans = 255 - i_trans;
	
	DWORD *pdw_org = (DWORD *)ddsd.lpSurface + (i_ancho * r_o.top) + r_o.left;
	DWORD *pdw_dest = (DWORD *)destino.ddsd.lpSurface + (destino.i_ancho * r_d.top) + r_d.left;
	DWORD *pdw_final_org = (DWORD *)ddsd.lpSurface + (i_ancho * r_o.bottom) + r_o.right - i_ancho;
	DWORD *pdw_final_linea_org = pdw_org + (r_o.right - r_o.left) - i_ancho; 

	int i_incr_org = i_ancho - (r_o.right - r_o.left);
	int i_incr_linea_org = i_ancho;
	int i_incr_dest = destino.i_ancho - (r_o.right - r_o.left);
	
	for (; pdw_org < pdw_final_org; pdw_org += i_incr_org, pdw_dest += i_incr_dest)
	{
	    pdw_final_linea_org += i_incr_linea_org;

		for (; pdw_org < pdw_final_linea_org; pdw_org ++, pdw_dest ++)
		{   			
			i_ctr = (((*pdw_org & i_bmr) >> i_pr) * i_trans);
			i_ctg = (((*pdw_org & i_bmg) >> i_pg) * i_trans);
			i_ctb = (((*pdw_org & i_bmb) >> i_pb) * i_trans);

			if (!b_sprite || (i_ctr != 0) && (i_ctg != 0) && (i_ctb != 0))
			{
				*pdw_dest = (((((((*pdw_dest & i_bmr) >> i_pr) * i_inv_trans) + i_ctr) >> 8) << i_pr) |
			 		         ((((((*pdw_dest & i_bmg) >> i_pg) * i_inv_trans) + i_ctg) >> 8) << i_pg) |
						     ((((((*pdw_dest & i_bmb) >> i_pb) * i_inv_trans) + i_ctb) >> 8) << i_pb));		
			}
		}
	}

    if (FAILED(lp_datos->Unlock(NULL)))
	{
        return FALSE;
    }	

    if (FAILED(destino.lp_datos->Unlock(NULL)))
	{
        return FALSE;
    }	
	
	return TRUE;
}


BOOL T_IMAGEN::B_Dibujar_Rectangulo(int i_trans, DWORD dw_color, 
							        int i_xo1, int i_yo1, int i_xo2, int i_yo2)
// dibuaja un rectángulo en pantalla
// entrada : i_trans = cantidad de transparencia
//           dw_color = color 
//           (i_xo1, i_yo1, i_xo2, i_yo2) = zona que representa las coordenadas del rectángulo
// salida : TRUE = pudo realizar el dibujo
//          FALSE = hubo algún error
{
	if (!B_Transparencia(i_trans, dw_color, i_xo1, i_yo1, i_xo2, i_yo1 + 1)) return FALSE;
	if (!B_Transparencia(i_trans, dw_color, i_xo1, i_yo2 - 1, i_xo2, i_yo2)) return FALSE;
	if (!B_Transparencia(i_trans, dw_color, i_xo1, i_yo1 + 1, i_xo1 + 1, i_yo2 - 1)) return FALSE;
	if (!B_Transparencia(i_trans, dw_color, i_xo2 - 1, i_yo1 + 1, i_xo2, i_yo2 - 1)) return FALSE;

	return TRUE;
}


BOOL T_IMAGEN::B_Pantalla()
// coloca la var actual apuntando a la pantalla
// entrada : -
// salida : TRUE, pudo realizar la asignación
//          FALSE, no pudo realizar la asignación
{
	if (lp_datos != NULL) return FALSE;

    b_pantalla = TRUE;
	i_ancho = -1;
	i_alto = -1;
    lp_datos = NULL;
	
	V_Crear_Tablas();

	return TRUE;
}


BOOL T_IMAGEN::B_Poner_Mondo_Video(HWND hwnd, int i_x, int i_y, BYTE by_bpp)
// cambia el modo de video actual
// entrada : hwnd = handle a la ventana del programa
//           i_x = ancho del modo de video
//           i_y = alto del modo de video
//           by_bpp = bits por pixel
// salida : TRUE, pudo cambiar el modo de video
//          FALSE, no pudo cambiar el modo de video
{
    DDSCAPS2 ddscaps;

	lp_dd = NULL;
	i_buffer_posterior = 2;
 
	if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&lp_dd, IID_IDirectDraw7, NULL)))
	{
		return FALSE;
	}

    if FAILED(lp_dd->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | 
				              				   DDSCL_FULLSCREEN))		                                             
	{										 
		return FALSE;
	}
	
	if (FAILED(lp_dd->SetDisplayMode(i_x, i_y, by_bpp, 0, 0))) 
	{
		return FALSE;
	}

	ZeroMemory(&ddsd, sizeof(ddsd));

    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | 
						  DDSCAPS_FLIP | 
                          DDSCAPS_COMPLEX |
                          DDSCAPS_VIDEOMEMORY;

    ddsd.dwBackBufferCount = i_buffer_posterior; // triple buffering
  
	if (FAILED(lp_dd->CreateSurface(&ddsd, &lp_pantalla, NULL)))
	{
		ddsd.dwBackBufferCount = i_buffer_posterior - 1;

		if (FAILED(lp_dd->CreateSurface(&ddsd, &lp_pantalla, NULL)))
		{
			return FALSE;
		}
	}

    ZeroMemory(&ddscaps, sizeof(ddscaps));    
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER; // obtener un puntero al buffer posterior
    
	if (FAILED(lp_pantalla->GetAttachedSurface(&ddscaps, &lp_datos)))
	{	
		return FALSE;
	}

	if (FAILED(lp_pantalla->GetSurfaceDesc(&ddsd)))
	{
		return FALSE;
	}

	i_ancho = i_x;
	i_alto = i_y;

	return TRUE;
}


BOOL T_IMAGEN::B_Flip()
// realiza el intercambio entre páginas
// entrada : -
// salida : TRUE, pudo realizar el intercambio
//          FALSE, hubo algún error
{    
    HRESULT result;

	if (!b_pantalla || (lp_pantalla == NULL))
	{
		return FALSE;
	}
	
	for (;;)
    {
        result = lp_pantalla->Flip(NULL, DDFLIP_WAIT);

        if (result == DD_OK)
        {
            break;
        }

        if (result == DDERR_SURFACELOST)
        {			
			if (FAILED(lp_pantalla->Restore()))
			{
				break;
			}

			B_Borrar(0);
        }

        if (result != DDERR_WASSTILLDRAWING)
        {
            break;
        }
    }

	return TRUE;
}


void T_IMAGEN::V_Pos_RGB(int &i_r, int &i_g, int &i_b)
// devuelve la pos de cada componente de color
// entrada : -
// salida : i_r = pos del rojo
//          i_g = pos del verde
//          i_b = pos del azul
{
	i_r = I_Pos_Color(ddsd.ddpfPixelFormat.dwRBitMask); 
	i_g = I_Pos_Color(ddsd.ddpfPixelFormat.dwGBitMask); 
	i_b = I_Pos_Color(ddsd.ddpfPixelFormat.dwBBitMask);
}


void T_IMAGEN::V_Mascara_RGB(DWORD &i_r, DWORD &i_g, DWORD &i_b)
// devuelve la mascara de cada componente de color
// entrada : -
// salida : i_r = mascara del componente rojo
//          i_g = mascara del componente verde
//          i_b = mascara del componente azul
{
	i_r = ddsd.ddpfPixelFormat.dwRBitMask;
	i_g = ddsd.ddpfPixelFormat.dwGBitMask;
	i_b = ddsd.ddpfPixelFormat.dwBBitMask;
}


DWORD T_IMAGEN::DW_RGB(DWORD dw_r, DWORD dw_g, DWORD dw_b)
// empaqueta los tres componentes de color
// entrada : by_r = compoenete rojo
//           by_g = componente verde
//           by_b = componente azul
// salida : los componentes empaquetados
{
	int	i_pr, i_pg, i_pb;
	DWORD color;

	V_Pos_RGB(i_pr, i_pg, i_pb);

	switch(By_Bpp())
	{
		case 32:

			color = ((dw_r << i_pr) | 
				     (dw_g << i_pg) | 
				     (dw_b << i_pb));

		break;		
	}

	return color;
}


DWORD T_IMAGEN::DW_Color(int i_px, int i_py)
// devuelve el color imagen [i_px][i_py]
// entrada : i_px = pos x
//           i_py = pos y
// salida : el color imagen [i_px][i_py]
{
	DWORD *pdw_color;

	if (FAILED(lp_datos->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) 
	{	
		return FALSE;
	}

	pdw_color = (DWORD *)ddsd.lpSurface + (i_ancho * i_py) + i_px;

    if (FAILED(lp_datos->Unlock(NULL)))
	{
        return FALSE;
    }	

	return *pdw_color;
}


BOOL T_IMAGEN::Restaurar_Modo_Video()
// restaura el modo de video por defecto
// entrada : -
// salida : FALSE si hubo problemas
{
	if (FAILED(lp_dd->RestoreDisplayMode())) 
	{
		return FALSE;
	}

	return TRUE;
}


/* t_imagen.cpp */

