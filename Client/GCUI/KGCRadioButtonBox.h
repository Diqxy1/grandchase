#ifndef _KGCRADIOBUTTONBOX_H_
#define _KGCRADIOBUTTONBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCRadioButtonBox: public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRadioButtonBox );

    KGCRadioButtonBox(void);
    virtual ~KGCRadioButtonBox(void);
    /// Copy constructor
    KGCRadioButtonBox( const KGCRadioButtonBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRadioButtonBox& operator=( const KGCRadioButtonBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EPETCARDSELECT
    {
        CHANGE_PET_CARD_LEFT,
        CHANGE_PET_CARD_RIGHT,
    };

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );	

private:
    KD3DWnd*            m_pkOriginalCheckBox;
    KD3DWnd*            m_pkOriginalCheck;
    KD3DStatic*         m_pkOriginalPetName;

    KD3DWnd*            m_pkNewCheckBox;
    KD3DWnd*            m_pkNewCheck;
    KD3DStatic*         m_pkNewPetName;

    bool                m_bCheckFirst;

    std::wstring        m_strOriginalPetName;
    std::wstring        m_strNewPetName;

public:
    void SetPetName( std::wstring strOriginalPetName, int iOriginalPetLevel, std::wstring strNewPetName, int iNewPetLevel, bool bCheckFirst );
    void ChangeCheck( bool bCheckFirst );
    bool IsCheckFirst(){ return m_bCheckFirst; }
};

DEFINE_WND_FACTORY( KGCRadioButtonBox );
DECLARE_WND_FACTORY( KGCRadioButtonBox );
DECLARE_WND_FACTORY_NAME( KGCRadioButtonBox );

#endif	//_KGCRADIOBUTTONBOX_H_