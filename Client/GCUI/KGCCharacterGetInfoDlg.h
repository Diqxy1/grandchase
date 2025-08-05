#ifndef _CHARACTERGETINFODLG_H_
#define _CHARACTERGETINFODLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCCharacterName;

class KGCCharacterGetInfoDlg : public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCharacterGetInfoDlg );
    /// Default constructors
    KGCCharacterGetInfoDlg( void );
    /// Default destructor
    ~KGCCharacterGetInfoDlg( void );
    /// Copy constructor
    KGCCharacterGetInfoDlg( const KGCCharacterGetInfoDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharacterGetInfoDlg& operator=( const KGCCharacterGetInfoDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetInfo(bool bNewChar, int iCharType, int iPromotion);
    void SetShow(bool bShow);
    bool IsShow() { return m_bShow; }
    void SetMagic(bool bMagic = false) {    m_bMagic = true;    }
    bool GetMagic( ) {    return m_bMagic;    }
    void SetReward( const SMissionReward& sReward ){ m_sReward = sReward; }
    SMissionReward& GetReward(){ return m_sReward; }

private:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroy( void );
    virtual void ActionPerformed( const KActionEvent& ) { };
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

    bool LoadSpecialFaceScript( std::string strFileName, SpecialFace *pSpecialFace); 
    void RenderFace(D3DXVECTOR4 vecTempPos, int iChar, int iJob, bool bReverse);          
    
public:
    void LoadScript();

private:
    enum { NUM_TITLE_EFFECT = 6 };

    KD3DSizingBox*  m_pkCharacterFrame;
    KD3DSizingBox*  m_pkInfoFrame;
    KD3DWnd* m_pkClassGetMent;
    KSafeArray<KD3DWnd*, NUM_TITLE_EFFECT> m_apkTitleEffect;

    KGCCharGetInfoPanel* m_pkCharGetInfo;
    KGCPromotionChangePanel* m_pkPromotionChange;
    KGCCharacterName *m_pkCharacterNameTexture;

    bool m_bMagic;
    bool m_bShow;
    bool m_bNewChar;
    int m_iCharType;
    int m_iPromotion;
    DWORD m_dwTimeCheck;
    SMissionReward m_sReward;

    SpecialFace m_SpecialFace[GC_CHAR_NUM][NUM_JOB_LEVEL];

	KD3DWnd* m_pkImgDrawRect;
    KD3DWnd* m_pkPromotion;
    KD3DWnd* m_pkNewChar;

    KD3DWnd* m_pkContratulation;
};

DEFINE_WND_FACTORY( KGCCharacterGetInfoDlg );
DECLARE_WND_FACTORY( KGCCharacterGetInfoDlg );
DECLARE_WND_FACTORY_NAME( KGCCharacterGetInfoDlg );

#endif 