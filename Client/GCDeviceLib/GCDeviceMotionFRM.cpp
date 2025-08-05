#include "stdafx.h"
#include ".\gcdevicemotionfrm.h"

GCDeviceMotionFRM::GCDeviceMotionFRM(std::string DeviceID)
{
	m_strDeviceID = DeviceID;
	m_eDeviceType = GCDT_MOTION_FRM;
	m_aFrame = NULL;
	m_dwNumAngleBone = 0;
	m_dwNumFrame = 0;
	m_eFrmVer = FRM_VER_10;
}

GCDeviceMotionFRM::~GCDeviceMotionFRM(void)
{
	UnLoad();
}

bool GCDeviceMotionFRM::Load()
{
	GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(), m_strDeviceID);
	if (!memfile.IsLoaded()) {
		m_eDeviceState = GCDS_DISABLE;
		return false;
	}

	std::array<char, 12> strVersion{};
	memfile.ReadFile(strVersion.data(), strVersion.size());
	m_eFrmVer = CheckFrmVer(strVersion.data());

	if (FRM_VER_10 == m_eFrmVer) {
		memfile.FileSeek(0, FILESEEK_SET);
	}
	LoadNumFrameAndNumAnglebone(memfile);

	if (m_dwNumFrame == 0 || m_dwNumAngleBone == 0) {
		return false;
	}

	m_aFrame = new SFrame[m_dwNumFrame];

	if (m_eFrmVer == FRM_VER_12)
	{
		for (DWORD j = 0; j < m_dwNumFrame; j++)
		{
			if (memfile.ReadFile(&m_aFrame[j].Reserved2, sizeof(unsigned short)) == 0)
			{
				m_aFrame[j].Reserved2 = 0;
			}

			if (memfile.ReadFile(&m_aFrame[j].Reserved1, sizeof(unsigned short)) == 0)
			{
				m_aFrame[j].Reserved1 = 0;
			}

			m_aFrame[j].A_Bone = new D3DXMATRIX[m_dwNumAngleBone];
			memfile.ReadFile(m_aFrame[j].A_Bone, sizeof(D3DXMATRIX) * m_dwNumAngleBone);
			memfile.ReadFile(&m_aFrame[j].Pos_y, sizeof(float));
			memfile.ReadFile(&m_aFrame[j].Plus_x, sizeof(float));
			memfile.ReadFile(&m_aFrame[j].Option, sizeof(char));

			m_aFrame[j].dwNumAngleBone = m_dwNumAngleBone;
		}
	}
	else if(m_eFrmVer == FRM_VER_11 || m_eFrmVer == FRM_VER_10)
	{
		for (DWORD j = 0; j < m_dwNumFrame; j++) {
			memfile.ReadFile(&m_aFrame[j].Option, sizeof(m_aFrame[j].Option));
			memfile.ReadFile(&m_aFrame[j].Plus_x, sizeof(m_aFrame[j].Plus_x));
			memfile.ReadFile(&m_aFrame[j].Pos_y, sizeof(m_aFrame[j].Pos_y));

			m_aFrame[j].A_Bone = new D3DXMATRIX[m_dwNumAngleBone];
			memfile.ReadFile(m_aFrame[j].A_Bone, sizeof(D3DXMATRIX) * m_dwNumAngleBone);

			m_aFrame[j].dwNumAngleBone = m_dwNumAngleBone;
		}
	}
	else
	{
		m_eDeviceState = GCDS_NOT_LOADED;
		return false;
	}

	m_eDeviceState = GCDS_LOADED;
	return true;
}

DWORD GCDeviceMotionFRM::GetDeviceSize()
{
	return	sizeof( SFrame ) * m_dwNumFrame +
		sizeof( D3DXMATRIX ) * m_dwNumAngleBone * m_dwNumFrame;
}

void GCDeviceMotionFRM::UnLoad()
{
	if( m_eDeviceState != GCDS_LOADED )
		return;
	for(DWORD i = 0 ; i < m_dwNumFrame; i++)
	{
		SAFE_DELETE_ARRAY( m_aFrame[i].A_Bone );
	}
	SAFE_DELETE_ARRAY(m_aFrame);
	m_dwNumFrame = 0;
	m_dwNumAngleBone = 0;
	m_eDeviceState = GCDS_NOT_LOADED;
}

EGCFrmVer GCDeviceMotionFRM::CheckFrmVer( const char* strVer_ ) const
{
	if ( StrCmpA( strVer_, "FRM Ver 1.2" ) == 0 ) {
		return FRM_VER_12;
	}

	if( StrCmpA( strVer_, "Frm Ver 1.1" ) == 0 ) {
		return FRM_VER_11;
	}

	return FRM_VER_10;
}

void GCDeviceMotionFRM::LoadNumFrameAndNumAnglebone( GCMemoryFile& file_ )
{
	switch( m_eFrmVer ) {
	case FRM_VER_10:
		BYTE ver10Num;
		file_.ReadFile( &ver10Num, sizeof(BYTE) );
		m_dwNumFrame = ver10Num;
		file_.ReadFile( &ver10Num, sizeof(BYTE) );
		m_dwNumAngleBone = ver10Num;
		break;

	case FRM_VER_11:
		unsigned short ver11Num;
		file_.ReadFile( &ver11Num, sizeof(unsigned short) );
		m_dwNumFrame = ver11Num;
		file_.ReadFile( &ver11Num, sizeof(unsigned short) );
		m_dwNumAngleBone = ver11Num;
		break;

	case FRM_VER_12:
		int ver12Num;
		file_.ReadFile(&ver12Num, sizeof(ver12Num));
		m_dwNumAngleBone = ver12Num;
		file_.ReadFile(&ver12Num, sizeof(ver12Num));
		m_dwNumFrame = ver12Num;
		break;

	default:
#if !defined( __PATH__ )
		std::string strText;
		strText = m_strDeviceID + " 모션 파일 버전 정보 오류!!";
		MessageBoxA(NULL,strText.c_str() ,"", MB_OK );
#endif
		break;
	}
}
