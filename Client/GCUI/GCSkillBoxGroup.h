#pragma once
#include "../uifx/D3DWnd.h"
#include "../uifx/D3DStatic.h"
#include "../uifx/D3DSizingBox.h"

class KGCSkillBoxGroup :	public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCSkillBoxGroup );
	/// Default constructor
	KGCSkillBoxGroup( void );
	/// Default destructor
	virtual ~KGCSkillBoxGroup( void );
	/// Copy constructor
	KGCSkillBoxGroup( const KGCSkillBoxGroup& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillBoxGroup& operator=( const KGCSkillBoxGroup& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void SetTitle( std::wstring strTitle );
	void SetSize( DWORD dwWidth, DWORD dwHeight );

private:
	KD3DStatic*		m_pkTitle;
	KD3DSizingBox*	m_pkBackground;
	KD3DSizingBox*	m_pkTitleBack;

};

DEFINE_WND_FACTORY( KGCSkillBoxGroup );
DECLARE_WND_FACTORY( KGCSkillBoxGroup );
DECLARE_WND_FACTORY_NAME( KGCSkillBoxGroup );
