#pragma once

class KD3DStatic;
class KGCCommonEditBox;

class KGCHeroDungeonInfoList :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonInfoList );
    /// Default constructor
    KGCHeroDungeonInfoList( void );
    /// Default destructor
    virtual ~KGCHeroDungeonInfoList( void );
    /// Copy constructor
    KGCHeroDungeonInfoList( const KGCHeroDungeonInfoList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonInfoList& operator=( const KGCHeroDungeonInfoList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    struct KDetailDungeonInfo
    {
        int iGameMode;
        USHORT usMaxClearPerDay;
        std::wstring wstrName;
        std::wstring wstrLocate;
        std::vector<std::wstring> vecAllOpenTime;

        KDetailDungeonInfo()
        {
            iGameMode       = -1;
            usMaxClearPerDay= 0;
            wstrName        = L"";
            wstrLocate      = L"";
            vecAllOpenTime.clear();
        }
    };

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DStatic*            	m_pkStaticName;
    KD3DStatic*            	m_pkStaticLocate;
    KD3DStatic*            	m_pkStaticOpenTime;
    KD3DStatic*            	m_pkStaticClear;
    KD3DStatic*            	m_pkStaticInitTime;

    KD3DWnd*                m_pkCheckClearRed;
    KD3DWnd*                m_pkCheckClearGreen;
    KD3DWnd*                m_pkBtnDetailOpenTime;

    std::pair<std::wstring, std::vector<std::wstring>> m_prOpenTimeInfo;

public:
    void OnOpenTime();
    void SetCheckClear( bool bClear_ );
    void InitDungeonInfo();
    void SetDungeonDetailInfo( KDetailDungeonInfo kInfo_ );
};

DEFINE_WND_FACTORY( KGCHeroDungeonInfoList );
DECLARE_WND_FACTORY( KGCHeroDungeonInfoList );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonInfoList );