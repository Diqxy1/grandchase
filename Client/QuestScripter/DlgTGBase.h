#pragma once
#include "stdafx.h"
#include "BaseTrigger.h"

class CDlgTGBase : public CDialog
{
public:
    CDlgTGBase(void);
    virtual ~CDlgTGBase(void);

    DECLARE_DYNAMIC(CDlgTGBase)

    virtual void InitValue(void) = 0;
    virtual void GetValue(CBaseTGFunction *pValue) = 0;
    virtual void SetValue(CBaseTGFunction *pTGFunction) = 0;
    virtual void Refresh(void) { }
    virtual void CalcResult(void) { }

    CString GetDesc(void) { return m_strDesc; }
    void SetDesc(CString strDesc) { m_strDesc = strDesc; }


protected:
    CString m_strDesc;
};
