#pragma once

class KD3DStatic;
class KGCCommonEditBox;

class KGCHeroDungeonInfoListS6 :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonInfoListS6 );
    /// Default constructor
    KGCHeroDungeonInfoListS6( void );
    /// Default destructor
    virtual ~KGCHeroDungeonInfoListS6( void );
    /// Copy constructor
    KGCHeroDungeonInfoListS6( const KGCHeroDungeonInfoListS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonInfoListS6& operator=( const KGCHeroDungeonInfoListS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DStatic*            	m_pkStaticName;
    KD3DStatic*            	m_pkStaticCondition;

    std::pair<std::wstring, std::vector<std::wstring>> m_prOpenTimeInfo;

public:
    void InitDungeonInfo();
    void SetDungeonDetailInfo( KGCGlobal::KDetailDungeonInfo kInfo_ );
};

DEFINE_WND_FACTORY( KGCHeroDungeonInfoListS6 );
DECLARE_WND_FACTORY( KGCHeroDungeonInfoListS6 );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonInfoListS6 );