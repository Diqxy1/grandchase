#pragma once
//#include "GCEnum.h"
//#include "../uifx/D3DWnd.h"

class KGCCharacterUI :  public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCCharacterUI );
    /// Default constructor
    KGCCharacterUI( void );
    /// Default destructor
    virtual ~KGCCharacterUI( void );
    /// Copy constructor
    KGCCharacterUI( const KGCCharacterUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharacterUI& operator=( const KGCCharacterUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
protected:
    virtual void OnCreate( void );

protected:
    KSafeArray<KD3DWnd*,GC_CHAR_NUM> m_pkChar;
    KD3DWnd*    m_pkAllChar;
    int m_iEnableChar;  //현재 표시 중인 케릭터
public:
    void SetToggleRender( bool bRender = false );
    void SetChar( int iCharIndex )
    {
        SetToggleRender();
        if( iCharIndex == -1 )
        {
            if( m_pkAllChar == NULL )
                return;
            m_pkAllChar->ToggleRender( true );
            m_iEnableChar = -1;
        }
        
        if( iCharIndex < 0 || iCharIndex >= GC_CHAR_NUM )
            return;

        if( m_pkChar[iCharIndex] == NULL )
            return;

        m_pkChar[iCharIndex]->ToggleRender( true );
        m_iEnableChar = iCharIndex;
    }
    int GetNowChar() { return m_iEnableChar; }
	void Active();
	void DeActive();
};

DEFINE_WND_FACTORY( KGCCharacterUI );
DECLARE_WND_FACTORY( KGCCharacterUI );
DECLARE_WND_FACTORY_NAME( KGCCharacterUI );