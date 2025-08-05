#pragma once

class KGCScoreAnimUI : public KD3DWnd, public KActionListener
{
public:
    KGCScoreAnimUI(void);
    virtual ~KGCScoreAnimUI(void);

    DECLARE_CLASSNAME( KGCScoreAnimUI );

    /// Copy constructor
    KGCScoreAnimUI( const KGCScoreAnimUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCScoreAnimUI& operator=( const KGCScoreAnimUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void ActionPerformed( const KActionEvent& ) {}

private:
    KD3DUIVertex                        m_rectNumRender[4];
    char                                Letter_Text[31];
    std::vector<string>                 m_vecLetterText;

    GCDeviceTexture*                    m_pTex;
    D3DXVECTOR2                         m_vPos;        
    D3DXVECTOR2                         m_vOrigivalPos;        
    D3DXVECTOR2                         m_vDst;
    float                               m_fNumWidth;
    float                               m_fNumHeight;
    float                               m_fNumInterval;
    float                               m_fScale;
    DWORD                               m_dwColor;
    bool                                m_bRender;
    bool                                m_bAlphaAnim;
    bool                                m_bStartAlphaAnim;
    bool                                m_bEndAlphaAnim;
    bool                                m_bIsCompleteAnim;    

    int                                 m_iAlphaCount;
    int                                 m_iMoveWeight;

    int                                 m_iLetterTextCount;

public:
    void DrawLetter();
    void SetColor( DWORD dwColor ) { m_dwColor = dwColor; }
    void SetLetter( std::string letter, float fScale = 1.0f );
    void SetAnimMoveY( float fPosY, float iWeight, bool bAlphaAnim );
    void AnimationMoveY(float fPosY, int iWeight, bool bAlphaAnim );
    bool IsCompleteMoveAnim();
    bool IsCompleteAlphaAnim();

	void SetAlphaCount(int iCnt_ )	{ m_iAlphaCount = iCnt_; }
};

DEFINE_WND_FACTORY( KGCScoreAnimUI );
DECLARE_WND_FACTORY( KGCScoreAnimUI );
DECLARE_WND_FACTORY_NAME( KGCScoreAnimUI );
