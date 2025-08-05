#pragma once

class KGCGuildBattleInfo : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGuildBattleInfo );
    /// Default constructor
    KGCGuildBattleInfo( void );
    /// Default destructor
    virtual ~KGCGuildBattleInfo( void );
    /// Copy constructor
    KGCGuildBattleInfo( const KGCGuildBattleInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildBattleInfo& operator=( const KGCGuildBattleInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event  );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();
    
public:
    void SetInformation(std::wstring wstrGuildMarkName_, std::wstring wstrGuildName_, DWORD dwBattlePoint_ );
    void CheckGuildMark( std::wstring wstrGuildMarkName_ , std::wstring wstrGuildName_ );
    
protected:
    KD3DImageIcon* m_pkGuildImage;

    KD3DStatic* m_pkGuildName;
    KD3DStatic* m_pkdBattlePointNum;
    KD3DStatic* m_pkBattlePointStr;
    KD3DWnd* m_pkNoGuildImg;

};

DEFINE_WND_FACTORY( KGCGuildBattleInfo );
DECLARE_WND_FACTORY( KGCGuildBattleInfo );
DECLARE_WND_FACTORY_NAME( KGCGuildBattleInfo );
