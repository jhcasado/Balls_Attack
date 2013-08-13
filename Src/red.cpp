/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

#include <stdio.h>

#include "red.h"

//////////////////////////////////////////////////////////////////////////////////////////
//		ZONA PRIVADA


BOOL RED::B_Inicializar_Direct_Play ()
{
	if (lp_direct_play)
		return FALSE;

	if (FAILED (CoCreateInstance (CLSID_DirectPlay, NULL, 
					              CLSCTX_ALL, IID_IDirectPlay4A, (void **)&lp_direct_play)))
		return FALSE;

	return TRUE;
}


BOOL FAR PASCAL EnumSessionsCallback2 
	(LPCDPSESSIONDESC2 lpThisSD, LPDWORD lpdwTimeOut, 
	DWORD dwFlags, LPVOID lpContext)
{
	RED *red;

	red = (RED *)lpContext;

	if (dwFlags & DPESC_TIMEDOUT)
		return(FALSE);

	if (!red->L_iSesion (lpThisSD))
		return FALSE;

	return TRUE;
}



BOOL FAR PASCAL EnumPlayersCallback2
	(DPID dpId, DWORD dwPlayerType, LPCDPNAME lpName, DWORD dwFlags,
	LPVOID lpContext)
{
	RED *red;

	red = (RED *)lpContext;

	if (!red->L_iJugador (dpId, lpName))
		return FALSE;

	return TRUE;
}


T_LISTA *RED::L_N (DWORD n)
{
	T_LISTA *sesion = NULL;

	if (!l_n)
		return NULL;

	if (n >= l_n)
		return NULL;

	sesion = &l_primero;
	for (DWORD i = 0; i < n; i++)
	{
		sesion = sesion->sig;
	}
	return sesion;
}


BOOL RED::L_Borra_Lista ()
{
	T_LISTA *sesion, *temp = NULL;

	sesion = l_primero.sig;
	while (sesion != &l_primero)
	{
		temp = sesion->sig;
//		if (sesion->nombre)
//			delete[] sesion->nombre;
		delete sesion;
		sesion = temp;
	}

	l_primero.sig = &l_primero;
	l_n = 0;
	return TRUE;
}


BOOL RED::L_iSesion (LPCDPSESSIONDESC2 lpThisSD)
{
	T_LISTA *sesion_nueva = NULL;

	if (l_n)
	{
		sesion_nueva = new T_LISTA;
		if (sesion_nueva == NULL)
			return FALSE;
		sesion_nueva->sig = l_primero.sig;
		l_primero.sig = sesion_nueva;
	}
	else
	{
		sesion_nueva = &l_primero;
	}

	sesion_nueva->guid = lpThisSD->guidInstance;
//	if (sesion_nueva->nombre)
//	{
//		delete[] sesion_nueva->nombre;
//		sesion_nueva->nombre = NULL;
//	}
	sprintf_s(sesion_nueva->nombre, "%s (%d/%d)", lpThisSD->lpszSessionNameA, 
		     lpThisSD->dwCurrentPlayers, lpThisSD->dwMaxPlayers );

	l_n ++;
	return TRUE;
}


BOOL RED::L_iJugador (DPID id, LPCDPNAME lpName)
{
	T_LISTA *jugador_nuevo = NULL;

	if (l_n)
	{
		jugador_nuevo = new T_LISTA;
		if (jugador_nuevo == NULL)
			return FALSE;
		jugador_nuevo->sig = l_primero.sig;
		l_primero.sig = jugador_nuevo;
	}
	else
	{
		jugador_nuevo = &l_primero;
	}

	jugador_nuevo->id = id;
//	if (jugador_nuevo->nombre)
//	{
//		delete[] jugador_nuevo->nombre;
//		jugador_nuevo->nombre = NULL;
//	}
	sprintf_s(jugador_nuevo->nombre, "%s", lpName->lpszLongNameA);

	l_n ++;
	return TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////
//		ZONA PUBLICA


RED::RED ()
{			
	//PARECE SER QUE ESTA LINEA HACE FALTA POR ALGO DE LOS OBJETOS COM
	CoInitialize(NULL);

	lp_direct_play = NULL;
	b_conexion_abierta = FALSE;
	b_sesion_abierta = FALSE;
	id_jugador = 0;
	l_n = 0;
	l_primero.sig = &l_primero;
}


RED::~RED ()
{
	//DESTRUIR EL OBJETO DIRECT PLAY
	if (id_jugador)
		B_Destruir_Jugador ();

	if (b_sesion_abierta)
		B_Cerrar_Sesion ();

	if (lp_direct_play) lp_direct_play->Release ();
	CoUninitialize();
}


BOOL RED::B_TCPIP_Disponible ()
{
	if (! (B_Iniciar_Conexion_TCPIP ()))
		return FALSE;

	B_Cerrar_Conexion ();

	return TRUE;
}


BOOL RED::B_IPX_Disponible ()
{
	if (! (B_Iniciar_Conexion_IPX ()))
		return FALSE;

	B_Cerrar_Conexion ();

	return TRUE;
}


BOOL RED::B_Iniciar_Conexion_TCPIP (char* c_direccion_ip, WORD w_puerto)
{
	LPDIRECTPLAYLOBBY3A lp_direct_play_lobby_temp = NULL;
	DPCOMPOUNDADDRESSELEMENT __datos_conexion[3];
	DWORD d_numero_datos = 0;
	void* v_buffer_datos;
	DWORD d_tamano_buffer = 0;


	if (b_conexion_abierta)
		return FALSE;

	//CREO UN OBJETO DIRECTPLAYLOBBY PARA CREAR UNA CONEXION PERSONALIZADA
	if (FAILED (CoCreateInstance(CLSID_DirectPlayLobby,NULL,CLSCTX_INPROC_SERVER,
										  IID_IDirectPlayLobby3A, (VOID**)&lp_direct_play_lobby_temp)))
		return FALSE;

	//RELLENAMOS LOS DATOS DE LA NUEVA CONEXION TCP/IP
	//
	//1) GUID DEL PROVEEDOR DE SERVICIO QUE USARE: DPSPGUID_TCPIP
	//2) DIRECCION IP A LA QUE SE INTENTARA CONECTAR: "" (POR DEFECTO)
	//3) PUERTO MEDIANTE EL QUE SE INICIARA LA CONEXION: 2333 (POR DEFECTO)

	//GUID TCPIP
	__datos_conexion[d_numero_datos].guidDataType = DPAID_ServiceProvider;
	__datos_conexion[d_numero_datos].dwDataSize = sizeof(GUID);
	__datos_conexion[d_numero_datos].lpData = (VOID*)&DPSPGUID_TCPIP;
	d_numero_datos ++;

	//DIRECCION IP
	__datos_conexion[d_numero_datos].guidDataType = DPAID_INet;
	__datos_conexion[d_numero_datos].dwDataSize = lstrlen(c_direccion_ip) + 1;
	__datos_conexion[d_numero_datos].lpData = c_direccion_ip;
	d_numero_datos ++;

	//PUERTO
	if (w_puerto > 0)
	{
		__datos_conexion[d_numero_datos].guidDataType = DPAID_INetPort;
		__datos_conexion[d_numero_datos].dwDataSize = sizeof(WORD);
		__datos_conexion[d_numero_datos].lpData = &w_puerto;
		d_numero_datos ++;
	}


	//CONSIGO EL TAMANO QUE TENDRA EL NUEVO BUFFER QUE CONTENDRA MI CONEXION PERSONALIZADA
	if (DPERR_BUFFERTOOSMALL !=(lp_direct_play_lobby_temp->CreateCompoundAddress
										(__datos_conexion, d_numero_datos, NULL, &d_tamano_buffer)))
		return FALSE;

	//HAGO HUECO PARA MI CONEXION
	v_buffer_datos = new BYTE [d_tamano_buffer];
	if (!v_buffer_datos)
		return FALSE;

	//EL BUFFER CONTENDRA MI NUEVA CONEXION TCP/IP
	if (FAILED (lp_direct_play_lobby_temp->CreateCompoundAddress
					(__datos_conexion, d_numero_datos, v_buffer_datos, &d_tamano_buffer)))
	{
		delete [] v_buffer_datos;
		return FALSE;
	}

	//LIBERO EL DIRECTPLAYLOBBY QUE YA NO ME HARA MAS FALTA
	lp_direct_play_lobby_temp->Release();

	if (!lp_direct_play)
		if (! (B_Inicializar_Direct_Play()))
		{
			delete [] v_buffer_datos;
			return FALSE;
		}

	//INICIALIZO MI CONEXION PERSONALIZADA TCP/IP
	if (FAILED (lp_direct_play->InitializeConnection(v_buffer_datos,0)))
	{
		delete [] v_buffer_datos;
		return FALSE;
	}

	b_conexion_abierta = TRUE;

	return TRUE;
}


BOOL RED::B_Iniciar_Conexion_IPX ()
{
	LPDIRECTPLAYLOBBY3A lp_direct_play_lobby_temp = NULL;
	DPCOMPOUNDADDRESSELEMENT __datos_conexion[3];
	DWORD d_numero_datos = 0;
	void* v_buffer_datos;
	DWORD d_tamano_buffer = 0;


	if (b_conexion_abierta)
		return FALSE;

	//CREO UN OBJETO DIRECTPLAYLOBBY PARA CREAR UNA CONEXION PERSONALIZADA
	if (FAILED (CoCreateInstance(CLSID_DirectPlayLobby,NULL,CLSCTX_INPROC_SERVER,
										  IID_IDirectPlayLobby3A, (VOID**)&lp_direct_play_lobby_temp)))
		return FALSE;

	//RELLENAMOS LOS DATOS DE LA NUEVA CONEXION TCP/IP
	//
	//1) GUID DEL PROVEEDOR DE SERVICIO QUE USARE: DPSPGUID_TCPIP
	//2) DIRECCION IP A LA QUE SE INTENTARA CONECTAR: "" (POR DEFECTO)
	//3) PUERTO MEDIANTE EL QUE SE INICIARA LA CONEXION: 2333 (POR DEFECTO)

	//GUID TCPIP
	__datos_conexion[d_numero_datos].guidDataType = DPAID_ServiceProvider;
	__datos_conexion[d_numero_datos].dwDataSize = sizeof(GUID);
	__datos_conexion[d_numero_datos].lpData = (VOID*)&DPSPGUID_IPX;
	d_numero_datos ++;

	//CONSIGO EL TAMANO QUE TENDRA EL NUEVO BUFFER QUE CONTENDRA MI CONEXION PERSONALIZADA
	if (DPERR_BUFFERTOOSMALL !=(lp_direct_play_lobby_temp->CreateCompoundAddress
										(__datos_conexion, d_numero_datos, NULL, &d_tamano_buffer)))
		return FALSE;

	//HAGO HUECO PARA MI CONEXION
	v_buffer_datos = new BYTE [d_tamano_buffer];
	if (!v_buffer_datos)
		return FALSE;

	//EL BUFFER CONTENDRA MI NUEVA CONEXION TCP/IP
	if (FAILED (lp_direct_play_lobby_temp->CreateCompoundAddress
					(__datos_conexion, d_numero_datos, v_buffer_datos, &d_tamano_buffer)))
	{
		delete [] v_buffer_datos;
		return FALSE;
	}

	//LIBERO EL DIRECTPLAYLOBBY QUE YA NO ME HARA MAS FALTA
	lp_direct_play_lobby_temp->Release();

	if (!lp_direct_play)
		if (! (B_Inicializar_Direct_Play()))
		{
			delete [] v_buffer_datos;
			return FALSE;
		}

	//INICIALIZO MI CONEXION PERSONALIZADA TCP/IP
	if (FAILED (lp_direct_play->InitializeConnection(v_buffer_datos,0)))
	{
		delete [] v_buffer_datos;
		return FALSE;
	}

	b_conexion_abierta = TRUE;

	return TRUE;
}


BOOL RED::B_Cerrar_Conexion ()
{
	if (!lp_direct_play)
		return TRUE;

	if (b_sesion_abierta)
		B_Cerrar_Sesion ();

	lp_direct_play->Release();
	lp_direct_play = NULL;
	b_conexion_abierta = FALSE;

	return TRUE;
}



BOOL RED::B_Crear_Sesion (GUID guid_app, char* c_nombre_sesion, DWORD d_max_jugadores)
{
	DPSESSIONDESC2 __descripcion_sesion;
	HRESULT hr;

	if (!lp_direct_play)
		return FALSE;

	if (b_sesion_abierta)
		return FALSE;

	ZeroMemory(&__descripcion_sesion, sizeof (DPSESSIONDESC2));
	__descripcion_sesion.dwSize = sizeof (DPSESSIONDESC2);
	__descripcion_sesion.dwFlags = DPSESSION_DIRECTPLAYPROTOCOL | /*DPSESSION_MIGRATEHOST |*/ DPSESSION_KEEPALIVE;
	__descripcion_sesion.guidInstance = GUID_NULL;
	__descripcion_sesion.guidApplication = guid_app;
	__descripcion_sesion.dwMaxPlayers = d_max_jugadores;
	__descripcion_sesion.lpszSessionNameA = c_nombre_sesion;
	__descripcion_sesion.lpszPasswordA = NULL;

	while (1)
	{
		hr = lp_direct_play->Open (&__descripcion_sesion, 
											DPOPEN_CREATE | DPOPEN_RETURNSTATUS);

		if (hr == DP_OK)
		{
			b_sesion_abierta = TRUE;
			return TRUE;
		}

		if (hr != DPERR_CONNECTING)
			return FALSE;
	}
}


BOOL RED::B_Enumerar_Sesiones (GUID guid_app)
{
	DPSESSIONDESC2 __descripcion_sesion;

	if (!b_conexion_abierta)
		return FALSE;

	if (b_sesion_abierta)
		return FALSE;

	L_Borra_Lista ();

	ZeroMemory(&__descripcion_sesion, sizeof (DPSESSIONDESC2));
	__descripcion_sesion.dwSize = sizeof (DPSESSIONDESC2);
	__descripcion_sesion.guidApplication = guid_app;

	if (FAILED (lp_direct_play->EnumSessions (&__descripcion_sesion, 0, EnumSessionsCallback2, this, 
															DPENUMSESSIONS_ASYNC | 
															DPENUMSESSIONS_ALL |
															DPENUMSESSIONS_RETURNSTATUS)))
		return FALSE;

	return TRUE;
}


BOOL RED::B_Cerrar_Sesion ()
{
	if (!b_sesion_abierta)
		return FALSE;

	if (id_jugador)
		B_Destruir_Jugador ();

	if (FAILED (lp_direct_play->Close()))
		return FALSE;

	b_sesion_abierta = FALSE;

	return TRUE;
}


BOOL RED::B_Crear_Jugador (char* c_nombre_jugador)
{
	DPNAME __jugador;

	if (!b_sesion_abierta)
		return FALSE;

	if (id_jugador)
		return FALSE;

	ZeroMemory (&__jugador, sizeof (DPNAME));
	__jugador.dwSize = sizeof (DPNAME);
	__jugador.lpszShortNameA = c_nombre_jugador;
	__jugador.lpszLongNameA = c_nombre_jugador;

	if (FAILED (lp_direct_play->CreatePlayer(&id_jugador, &__jugador, 
											NULL, NULL, 0, 0)))
		return FALSE;

	return TRUE;
}


BOOL RED::B_Enumerar_Jugadores ()
{
	if (!id_jugador)
		return FALSE;

	L_Borra_Lista ();

	if (FAILED (lp_direct_play->EnumPlayers 
		(NULL,EnumPlayersCallback2,this,DPENUMPLAYERS_REMOTE)))
		return FALSE;

	return TRUE;
}



BOOL RED::B_Destruir_Jugador ()
{
	if (!b_sesion_abierta)
		return FALSE;

	if (!id_jugador)
		return FALSE;

	if (FAILED (lp_direct_play->DestroyPlayer (id_jugador)))
		return FALSE;

	id_jugador = 0;

	return TRUE;
}

char *RED::L_Nombre (DWORD n)
{
	T_LISTA *sesion = NULL;

	sesion = L_N (n);

	if (sesion == NULL)
		return NULL;

	return sesion->nombre;
}

DPID RED::L_Id_Jugador (DWORD n)
{
	T_LISTA *sesion = NULL;

	sesion = L_N (n);

	if (sesion == NULL)
		return 0;

	return sesion->id;
}


DWORD RED::L_Tamano ()
{
	return l_n;
}


BOOL RED::B_Parar_Enumeracion (GUID guid_app)
{
	DPSESSIONDESC2 __descripcion_sesion;

	if (!b_conexion_abierta)
		return FALSE;

	if (b_sesion_abierta)
		return FALSE;

	L_Borra_Lista ();

	ZeroMemory(&__descripcion_sesion, sizeof (DPSESSIONDESC2));
	__descripcion_sesion.dwSize = sizeof (DPSESSIONDESC2);
	__descripcion_sesion.guidApplication = guid_app;

	if (FAILED (lp_direct_play->EnumSessions 
					(&__descripcion_sesion, 0, EnumSessionsCallback2, this, 
					DPENUMSESSIONS_STOPASYNC | DPENUMSESSIONS_RETURNSTATUS)))
		return FALSE;

	return TRUE;
}



BOOL RED::B_Abrir_Sesion (DWORD n)
{
	DPSESSIONDESC2 __desc_sesion;
	T_LISTA *temp;

	temp = L_N (n);

	if (temp == NULL)
		return FALSE;

	ZeroMemory (&__desc_sesion, sizeof (DPSESSIONDESC2));
	__desc_sesion.dwSize = sizeof (DPSESSIONDESC2);
	__desc_sesion.guidInstance = temp->guid;

	if (FAILED (lp_direct_play->Open (&__desc_sesion, DPOPEN_JOIN)))
		return FALSE;

	b_sesion_abierta = TRUE;
	
	return TRUE;
}


BOOL RED::B_Enviar (DPID id_to, LPVOID lp_data, DWORD dw_data_size)
{
	if (!id_jugador)
		return FALSE;

	if (DPERR_PENDING != lp_direct_play->SendEx 
						(id_jugador, id_to, DPSEND_ASYNC | DPSEND_GUARANTEED,
						lp_data, dw_data_size, 0, 0, NULL, NULL))
		return FALSE;

	return TRUE;
}


DWORD RED::DW_Recivir (LPVOID &lp_data, LPDWORD lpdw_data_size)
{
    DPID    id_from = 0;
    DPID    id_to = 0;
	HRESULT hr;

	if (!id_jugador)
		return 0;

	do
	{
		hr =lp_direct_play->Receive 
				(&id_from, &id_to, DPRECEIVE_ALL, lp_data, lpdw_data_size);

		if (hr == DPERR_BUFFERTOOSMALL)
		{
			if (lp_data)
				delete[] lp_data;
			lp_data = new BYTE[*lpdw_data_size];
			if (lp_data == NULL)
				return 0;
		}
	} while (hr == DPERR_BUFFERTOOSMALL);

    if( DPERR_NOMESSAGES == hr )
	    return MSG_NO_HAY;

    if( FAILED(hr) )
	{
		if (lp_data)
			delete[] lp_data;
		*lpdw_data_size = 0;
		lp_data = NULL;
        return MSG_ERROR;
	}

	if (id_from == DPID_SYSMSG)
	{
		LPDPMSG_GENERIC lp_msg = (LPDPMSG_GENERIC) lp_data;

		switch (lp_msg->dwType)
		{
			case DPSYS_CREATEPLAYERORGROUP:
				{
				DPMSG_CREATEPLAYERORGROUP *jugador = (DPMSG_CREATEPLAYERORGROUP *)lp_msg;
				char *nombre;

				*lpdw_data_size = jugador->dpId;
				nombre = new CHAR[strlen(jugador->dpnName.lpszShortNameA)+1];
				strcpy(nombre, jugador->dpnName.lpszShortNameA);
				delete jugador;
				lp_data = nombre;
				}
				return MSG_CREA_JUGADOR;

			case DPSYS_DESTROYPLAYERORGROUP:
				{
				DPMSG_DESTROYPLAYERORGROUP *jugador= (DPMSG_DESTROYPLAYERORGROUP *)lp_msg;
				char *nombre;

				*lpdw_data_size = jugador->dpId;
				nombre = new CHAR[sizeof(jugador->dpnName.lpszShortNameA)+1];
				strcpy(nombre, jugador->dpnName.lpszShortNameA);
				delete jugador;
				lp_data = nombre;
				}
				return MSG_DESTRUYE_JUGADOR;

			case DPSYS_SENDCOMPLETE:
				{
				HRESULT hr;
				DPMSG_SENDCOMPLETE *envio = (DPMSG_SENDCOMPLETE *)lp_msg;

				hr = envio->hr;
				delete envio;
				lp_data = NULL;
				if (envio->hr && DP_OK)
					*lpdw_data_size = envio->dwSendTime;
				}
				return MSG_MENSAJE_ENVIADO;

			case DPSYS_SESSIONLOST:
				{
				DPMSG_SESSIONLOST *envio = (DPMSG_SESSIONLOST *)lp_msg;

				delete envio;
				lp_data = NULL;
				*lpdw_data_size = 0;
				B_Cerrar_Sesion();
				}
				return MSG_SESION_PERDIDA;

			default:
				{
				delete[] lp_data;
				lp_data = NULL;
				*lpdw_data_size = 0;
				}
				return MSG_DESCONOCIDO;
		}
	}

	return MSG_PERSONAL;
}