#pragma once

#define MAX_GUILD_LIST 10
#define GUILD_LEVEL_STEP 11

class KD3DStatic;
class KD3DScrollbar;
class KD3DSizingBox;
class KGCGuildBenefitListBar;

struct KGuildBenefitList
{   
    KGuildBenefitList()
    {
        strLevel = L"";
        iNumber = 0;
        iGpoint = 0;
        iNeedContribution = 0;
    }

    std::wstring strLevel;
    int iNumber;
    int iGpoint;
    int iNeedContribution;   
};

class KGCGuildLevelList : public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildLevelList );
    /// Default constructor
    KGCGuildLevelList( void );
    /// Default destructor
    virtual ~KGCGuildLevelList( void );
    /// Copy constructor
    KGCGuildLevelList( const KGCGuildLevelList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildLevelList& operator=( const KGCGuildLevelList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    

protected:
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState ( void );
    virtual void ActionPerformed( const KActionEvent& event );

public:
    void InitScroll( void );
    void ShowList( void );
    void SetBarIndex( int iIdx ) {  m_iBarIdx = iIdx;   }
    int GetGuildLevel( std::map < USHORT, int> m_mapGuildLevelTable );
    void LoadGuildlBenefitList( void );
    void GpointDiff( void );

public:
    int m_iBarIdx;

private:    
    KD3DScrollbar*    m_pkScrollBar;
    KGCGuildBenefitListBar* m_pkGuildBenifitList[ MAX_LIST ];

    KD3DSizingBox*    m_pkGuildLevelBack;
    KD3DSizingBox*    m_pkListBack;
    
    KD3DWnd*          m_pkGuildBenefit;
    KD3DWnd*          m_pkGuildPoint;
    KD3DWnd*          m_pkRequareContribution;
    KD3DWnd*          m_pkGuildLevelIcon;
    
    KD3DStatic*       m_pkContributionStr;
    KD3DStatic*       m_pkContributionScore;
    KD3DStatic*       m_pkGuildLevelStr;  
      
    int               iGlevel;
    std::vector<KGuildBenefitList> m_vecGuildBenefitList;
};

DEFINE_WND_FACTORY( KGCGuildLevelList );
DECLARE_WND_FACTORY( KGCGuildLevelList );
DECLARE_WND_FACTORY_NAME( KGCGuildLevelList );