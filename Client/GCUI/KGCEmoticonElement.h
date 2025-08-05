#ifndef _GC_EMOTICON_ELEMENT_H_
#define _GC_EMOTICON_ELEMENT_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCEmoticon;
class KD3DNumber;
class KGCEmoticonElement : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCEmoticonElement );
    /// Default constructor
    KGCEmoticonElement( void );
    /// Default destructor
    virtual ~KGCEmoticonElement( void );
    /// Copy constructor
    KGCEmoticonElement( const KGCEmoticonElement& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEmoticonElement& operator=( const KGCEmoticonElement& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void PostDraw( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnDestroyComplete( void );
    void InitNumberTextureCoordinate();
    void SetVolumn( UINT nVolume );
    bool IsExistEmoticon( void );
    void SetItemID( int iItemID );
    int GetItemId( void ) const { return m_iItemID; }
    void SetIsClick( bool bIsClick ) { m_bIsClick = bIsClick; }
    bool GetIsClick( void ) const { return m_bIsClick; }
    void SetIsExistEmoticon( bool bIsExist ) { m_bIsExist = bIsExist; }
    void OnClose( void );
    KGCEmoticon* GetEmoticonData( void );

private:
    KD3DNumber*         m_pkNumber10;
    KD3DNumber*         m_pkNumber1;
    KD3DWnd*            m_pkCenter;
    KD3DWnd*            m_pkEdge;
    bool                m_bIsClick;
    // 이모티콘 아템이 소트에 있으센?!
    bool                m_bIsExist;
    int                 m_iItemID;
    UINT                m_nVolume;
    KGCEmoticon*        m_pkEmoticonData;
};


DEFINE_WND_FACTORY( KGCEmoticonElement );
DECLARE_WND_FACTORY( KGCEmoticonElement );
DECLARE_WND_FACTORY_NAME( KGCEmoticonElement );

#endif
