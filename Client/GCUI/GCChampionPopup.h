#pragma once
class KGCChampionElement;
class KGCChampionPopup : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCChampionPopup );
    KGCChampionPopup( void );
    virtual ~KGCChampionPopup( void );
    KGCChampionPopup( const KGCChampionPopup& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    const KGCChampionPopup& operator=( const KGCChampionPopup& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    void OnClose();
private:
	enum {
        MAX_STATS  =  10,
	};
    KSafeArray< KGCChampionElement*, MAX_STATS > m_vecChampionElements;
};
DEFINE_WND_FACTORY( KGCChampionPopup );
DECLARE_WND_FACTORY( KGCChampionPopup );
DECLARE_WND_FACTORY_NAME( KGCChampionPopup );
