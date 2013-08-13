/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_raton.cpp */

#include "t_raton.h"


T_RATON::T_RATON()
// constructor de la clase
// entrada : -
// salida : -
{
	lp_di = NULL;
	lp_raton = NULL;
	i_x = 0;
	i_y = 0;
	i_z = 0;
	f_vel_x = 0.5;
	f_vel_y = 0.5;
	f_vel_z = 0.5;
	r_zona.top = 0;
	r_zona.left = 0;
	r_zona.bottom = 0;
	r_zona.right = 0;
	b_boton_der = FALSE;
	b_boton_cen = FALSE;
	b_boton_izq = FALSE;
}


BOOL T_RATON::B_Inicializar(HINSTANCE hinst, HWND hwnd)
// Inicializa el dispositivo
// entrada : hinst, instancia de la clase
//           hwnd, handle a la ventana de la aplicación
// salida : TRUE, pudo inicializarlo
//          FALSE, hubo algún error
{
/*	if (FAILED(DirectInputCreateEx(hinst, DIRECTINPUT_VERSION, IID_IDirectInput7, (void **)&lp_di, NULL)))
	{
		return FALSE;
	}*/

	if(FAILED(DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&lp_di, NULL)))
	{
		return FALSE;
	}
	
	if (FAILED(lp_di->CreateDevice(GUID_SysMouse, &lp_raton, NULL)))
	{
		return FALSE;
	}

    if (FAILED(lp_raton->SetDataFormat(&c_dfDIMouse)))
	{
        return FALSE;
	}

    if (FAILED(lp_raton->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		return FALSE;
	}

    return TRUE;
}


BOOL T_RATON::B_App_Activa(BOOL b_activa)
// toma o devuelve los privilegios sobre el ratón
// entrada : b_activa, var que indica si la app está activa
// salida : TRUE, no hubo errores
//          FALSE, hubo algún error
{
    if (lp_raton == NULL)
	{
        return FALSE;
	}

    if (b_activa) 
    {
        lp_raton->Acquire();
    }
	
    else 
    {
        lp_raton->Unacquire();
    }

    return TRUE;
}


BOOL T_RATON::B_Actualizar()
// actualiza los datos
// entrada : -
// salida : TRUE, no hubo errores
//          FALSE, hubo algún error
{
    if (lp_raton != NULL) 
    {
        DIMOUSESTATE dims;
        HRESULT hr;

        hr = DIERR_INPUTLOST;
        
        while (DIERR_INPUTLOST == hr) 
        {
            hr = lp_raton->GetDeviceState(sizeof(DIMOUSESTATE), &dims);
			
            if (hr == DIERR_INPUTLOST)
            {
                hr = lp_raton->Acquire();
                if (FAILED(hr))  
				{
                    return FALSE;
				}
            }
        }

        if (FAILED(hr))  
		{
            return FALSE;
		}

		i_x += (int)(dims.lX * f_vel_x); 
		i_y += (int)(dims.lY * f_vel_y); 
		i_z += (int)(dims.lZ * f_vel_x);

		if (i_x < r_zona.left) i_x = r_zona.left;
		if (i_y < r_zona.top) i_y = r_zona.top;
		if (i_x > r_zona.right) i_x = r_zona.right;
		if (i_y > r_zona.bottom) i_y = r_zona.bottom;

        (dims.rgbButtons [0] & 0x80) ? b_boton_izq = TRUE : b_boton_izq = FALSE;
		(dims.rgbButtons [1] & 0x80) ? b_boton_der = TRUE : b_boton_der = FALSE;
		(dims.rgbButtons [2] & 0x80) ? b_boton_cen = TRUE : b_boton_cen = FALSE;
	}

    return TRUE;
}


void T_RATON::V_Liberar()
// libera los recursos
// entrada : -
// salida : -
{
    if (lp_raton != NULL) 
    {
        lp_raton->Unacquire();

        lp_raton->Release();
        lp_raton = NULL;
    }

    if (lp_di != NULL) 
    {
        lp_di->Release();
        lp_di = NULL;
    }
}


/* t_raton.cpp */

