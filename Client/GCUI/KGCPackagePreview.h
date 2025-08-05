#ifndef _KGCPACKAGEPREVIEW_H_
#define _KGCPACKAGEPREVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCPackageItemBox.h"

#define SCROLL_LIMIT_CNT 12

class KGCPackageItemBox;
class KGCPackagePreview : public KD3DDialog,
    public KActionListener
{
public:
    enum {
        EM_INVEN_COLUMN = 4,
        EM_INVEN_ROW = 3
    };

    DECLARE_CLASSNAME(KGCPackagePreview);

    KGCPackagePreview(void);
    virtual ~KGCPackagePreview(void);
    /// Copy constructor
    KGCPackagePreview(const KGCPackagePreview&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCPackagePreview& operator=(const KGCPackagePreview&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

    virtual bool CheckCondition(IN const KDialogInfo& kInfo_);
    virtual void ActionPerformed(const KActionEvent& event);
    virtual void FrameMoveInEnabledState(void);

private:
    KD3DSizingBox* m_pkBack1;
    KD3DSizingBox* m_pkBack2;
    KD3DScrollbar* m_pkScroll;
    KD3DWnd* m_pkClose;
    KD3DWnd* m_pkTitle;
    std::vector<KGCPackageItemBox*> m_vecItemBox;
    std::vector<KDropItemInfo> m_vecItemInfo;

private:
    int                     m_iScrollIdx;
    int                     m_iShowItemStartIdx;
    D3DXVECTOR2             m_vOriPos;

public:
    void InitControll();
    void RefreshItemBox();
    void SetItemList(std::vector< KDropItemInfo > vecItemInfo);
    void CreateItemBox(int iHeight, int iWidth, D3DXVECTOR2& vPos, int& iCount, KDropItemInfo* paItemInfo = NULL);

public:
    void OnEnterCursor();
    void OnClickClsoe();
};

DEFINE_WND_FACTORY(KGCPackagePreview);
DECLARE_WND_FACTORY(KGCPackagePreview);
DECLARE_WND_FACTORY_NAME(KGCPackagePreview);

#endif	//_KGCPACKAGEPREVIEW_H_