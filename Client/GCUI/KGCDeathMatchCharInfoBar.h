#ifndef _KGCDeathMatchCharInfoBar_H_
#define _KGCDeathMatchCharInfoBar_H_

//#include "../uifx/D3DWnd.h"

class GCDeviceTexture;

class KD3DStatic;
class KGCGradeMark;
class KGCDeathMatchCharInfoBar : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCDeathMatchCharInfoBar );
	/// Default constructor
	KGCDeathMatchCharInfoBar( void );
	/// Default destructor
	virtual ~KGCDeathMatchCharInfoBar( void );
	/// Copy constructor
	KGCDeathMatchCharInfoBar( const KGCDeathMatchCharInfoBar& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCDeathMatchCharInfoBar& operator=( const KGCDeathMatchCharInfoBar& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }


    void UpdateGameData(); //UI에 현재 상태를 적용합니다

    void PostChildDraw();

    

public:
    int m_iPlayerIndex;
    int m_iGuildMark;

    KGCGradeMark*   m_pkGradeMark;
    KD3DWnd*        m_pkGuildMark;
    KD3DStatic*     m_pkGuildStatic;
    KD3DStatic*     m_pkNickNameStatic;
    KD3DStatic*     m_pkDamageStatic;
    KD3DStatic*     m_pkKillStatic;
    KD3DStatic*     m_pkDeathdStatic;
    KD3DStatic*     m_pkPingStatic;



};

DEFINE_WND_FACTORY( KGCDeathMatchCharInfoBar );
DECLARE_WND_FACTORY( KGCDeathMatchCharInfoBar );
DECLARE_WND_FACTORY_NAME( KGCDeathMatchCharInfoBar );

#endif //_KGCDeathMatchCharInfoBar_H_