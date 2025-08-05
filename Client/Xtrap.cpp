#include "stdafx.h"
#include "KMailSender.h"
#include "Xtrap_C_XtrapCode.h"
#include "Xtrap.h"

#include "MyD3D.h"

void WINAPI XTRAP_CALLBACK__XtrapCallbackProcedure (
	IN unsigned int Reserve1,
	IN unsigned int Reserve2,
	IN unsigned int Reserve3,
	IN unsigned int Reserve4,
	IN unsigned char *pXtrapCode	// [in, out] Pointer to a XTRAP_CODE data structure
	)
{
//#ifndef USE_XTRAP
//	return;
//#endif

	XTRAP_CODE XtrapCode={0, };

	memcpy(&XtrapCode, pXtrapCode, sizeof(XTRAP_CODE));

	
	WCHAR szMsg1[MAX_PATH];
	WCHAR szMsg2[MAX_PATH];

	DWORD dwError = Convert_XTRAP_ERRORCODE( XtrapCode );

	switch( dwError )
 	{
	case XTRAPCODE_KINDS_CRACK:
	case XTRAPCODE_KINDS_CHEAT:
	case XTRAPCODE_KINDS_BOT:
	case XTRAPCODE_KINDS_ALERT:
	case XTRAPCODE_KINDS_DEBUG:	// 일단 여기다 포함... 
	case XTRAPCODE_KINDS_MACRO: // 이것도..
		::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_NPGG_CALLBACK_ERROR05 ).c_str() );
		::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM ).c_str() );
		break;
	case XTRAPCODE_UNDEFINED:
	case XTRAPCODE_KINDS_ETC:
	case XTRAPCODE_KINDS_KERNEL:
	case XTRAPCODE_KINDS_SYSTEM:
	case XTRAPCODE_KINDS_EXCEPT:
		lstrcpy( szMsg1,  g_pkStrLoader->GetString( STR_ID_NPGG_CALLBACK_ERROR01 ).c_str() );
		lstrcpy( szMsg2, L"" );
		break;
	case XTRAPCODE_KINDS_PATCH:
	case XTRAPCODE_KINDS_LAUNCHER:
		::lstrcpy( szMsg1,  g_pkStrLoader->GetString( STR_ID_XTRAP_PATCH ).c_str() );
		::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM ).c_str() );
		break;
	case XTRAPCODE_KINDS_ADMIN:
		//KSerBuffer buff;
		//buff.Write( ( void* )dwError, sizeof( GG_AUTH_DATA ) );
		// SiKP2P()->m_pkUserProxy->SendPacket( KUserEvent::EVENT_NPGG_AUTH_ACK, buff );
	default:
		break;
    }
	ExitWithHackAlert( L"XTRAP_ERROR" );
//#endif

}


DWORD Convert_XTRAP_ERRORCODE( XTRAP_CODE& XtrapCode_ )
{
//	XtrapCode
	if( XtrapCode_.MajorCode == 0x1 )
	{
		switch( XtrapCode_.MinorCode )
		{
		case 0x031:
		case 0x041:
		case 0x042:
		case 0x046:
		case 0x059:
		case 0x062:
		case 0x063:
		case 0x064:
		case 0x201:
		case 0x203:
		case 0x206:
		case 0x210:
		case 0x211:
		case 0x503:
		case 0x600:
			return XTRAPCODE_KINDS_SYSTEM;
		case 0x039:
			return XTRAPCODE_KINDS_CRACK;
		case 0x266:
		case 0x501:
		case 0x502:
			return XTRAPCODE_KINDS_CHEAT;
		case 0x700:
			return XTRAPCODE_KINDS_ALERT;			
		}
	}
	
	if( XtrapCode_.MajorCode == 0x2 )
	{	
		switch ( XtrapCode_.MinorCode )
		{
		case 0x017:
		case 0x031:
		case 0x504:
			return XTRAPCODE_KINDS_ADMIN;
		case 0x033:
		case 0x226:
		case 0x229:
		case 0x250:
		case 0x501:
		case 0x502:
		case 0x503:
		case 0x600:
			return XTRAPCODE_KINDS_SYSTEM;
		case 0x058:
		case 0x200:
		case 0x201:
		case 0x204:
		case 0x206:
		case 0x207:
		case 0x214:
		case 0x215:
		case 0x217:
		case 0x218:
		case 0x219:
		case 0x220:
		case 0x221:
		case 0x222:
		case 0x223:
		case 0x224:
		case 0x225:
		case 0x228:
		case 0x234:
		case 0x302:
		case 0x308:
		case 0x500:
		case 0x506:
		case 0x507:
		case 0x510:
		case 0x511:
		case 0x512:
			return XTRAPCODE_KINDS_KERNEL;
		case 0x211:
		case 0x227:
			return XTRAPCODE_KINDS_ETC;
		}	
	}


	if( XtrapCode_.MajorCode == 0x4 || XtrapCode_.MajorCode == 0x14 )
	{	
		switch ( XtrapCode_.MinorCode )
		{
		case 0x600:
			return XTRAPCODE_KINDS_SYSTEM;
		default:
			return XTRAPCODE_KINDS_MACRO;
		}
	}


	if( XtrapCode_.MajorCode == 0x5 )
	{	
		switch ( XtrapCode_.MinorCode )
		{
		case 0x600:
			return XTRAPCODE_KINDS_SYSTEM;
		default:
			return XTRAPCODE_KINDS_BOT;
		}
	}

	if( XtrapCode_.MajorCode == 0x6 || XtrapCode_.MajorCode == 0x13 )
		return XTRAPCODE_KINDS_SYSTEM;

	if( XtrapCode_.MajorCode == 0x7 )
	{
		switch ( XtrapCode_.MinorCode )
		{
		case 0x001:
		case 0x002:
		case 0x004:
			return XTRAPCODE_KINDS_CRACK;
		default:
			return XTRAPCODE_KINDS_SYSTEM;
		}
	}

	if( XtrapCode_.MajorCode == 0x8 )
		return XTRAPCODE_KINDS_CRACK;

	if( XtrapCode_.MajorCode == 0x9 )
		return XTRAPCODE_KINDS_DEBUG;

	if( XtrapCode_.MajorCode == 0x10 )
	{
		if( XtrapCode_.MinorCode == 0x010 ||  XtrapCode_.MinorCode == 0x200 )
			return XTRAPCODE_KINDS_PATCH;
		else
			return XTRAPCODE_KINDS_CRACK;
	}

	if( XtrapCode_.MajorCode == 0x11 )
	{
		switch ( XtrapCode_.MinorCode )
		{
		case 0x001:
			return XTRAPCODE_KINDS_SYSTEM;
		case 0x006:
			return XTRAPCODE_KINDS_CRACK;
		default:
			return XTRAPCODE_KINDS_CHEAT;
		}
	}

	if( XtrapCode_.MajorCode == 0x15 )
	{
		switch( XtrapCode_.MinorCode )
		{
		case 0x013:
			return XTRAPCODE_KINDS_ALERT;
		default:
			return XTRAPCODE_KINDS_BOT;
		}
	}

	if( XtrapCode_.MajorCode == 0x16 )
		return XTRAPCODE_KINDS_EXCEPT;

	if( XtrapCode_.MajorCode == 0x30 )
	{
		switch( XtrapCode_.MinorCode )
		{
		case 0x500:
			return XTRAPCODE_KINDS_LAUNCHER;
		case 0x600:
			return XTRAPCODE_KINDS_SYSTEM;
		default:
			return XTRAPCODE_KINDS_CHEAT;
		}
	}

	if( XtrapCode_.MajorCode == 0x31 )
	{
		switch( XtrapCode_.MinorCode )
		{
		case 0x206:
		case 0x207:
		case 0x210:
		case 0x211:
		case 0x215:
		case 0x216:
		case 0x217:
		case 0x223:
			return XTRAPCODE_KINDS_CHEAT;
		default:
			return XTRAPCODE_KINDS_CRACK;
		}
	}

	return XTRAPCODE_UNDEFINED;
}
