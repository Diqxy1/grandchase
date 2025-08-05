#pragma once

#define MINIGAME_WEB_EVENT 9999

class KGCEventMinigameList : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEventMinigameList );
    /// Default constructor
    KGCEventMinigameList( void );
    /// Default destructor
    virtual ~KGCEventMinigameList( void );
    /// Copy constructor
    KGCEventMinigameList( const KGCEventMinigameList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEventMinigameList& operator=( const KGCEventMinigameList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    struct SEventMiniGameList{
        DWORD dwEventID;
        KD3DWnd *pkEventBox;
        KGC2DObject* pkImage;

        SEventMiniGameList()
        {
            dwEventID = 0;
            pkEventBox = NULL;
            pkImage = NULL;
        }
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void PostChildDraw( void );
    void OnEndBtn( void );
    void SetEventGameList( void );
    bool OnClickEventBox( const KActionEvent& event );
    void ClearEventList();
    void OnDestroy( void );
private:
    void SetEventGame( KEventOpenInfo& pInfo );
    void SetEventPosition( KEventOpenInfo& pInfo ,KD3DWnd *pBtnWnd, int iPos);
    KD3DWnd *m_pkEndBtn;
    KD3DWnd *m_pkEventBox;
    KD3DWnd* m_pkBtnClose;
    KD3DSizingBox *m_pkOutBackBox;
    KD3DSizingBox *m_pkInBackBox;
    std::vector<SEventMiniGameList> m_vecEventBoxList;
};

DEFINE_WND_FACTORY( KGCEventMinigameList );
DECLARE_WND_FACTORY( KGCEventMinigameList );
DECLARE_WND_FACTORY_NAME( KGCEventMinigameList );
