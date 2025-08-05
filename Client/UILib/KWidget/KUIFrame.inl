//---------------------------------------------------------------------------
template <class TFunc>
void KApplyDownWidget(KUIWidget* pkObject, TFunc pkFn)
{
    // Apply Function
    if (!pkObject)
        return;

    pkFn(pkObject);

    // Child Widget
   // if (NiIsKindOf(KUIFrame, pkObject))
    {
        KUIFrame* pkUIFrame = (KUIFrame*) pkObject;
        unsigned int uiWidgetCnt = pkUIFrame->GetWidgetCount();
        unsigned int iW;
        for (iW = 0; iW < uiWidgetCnt; ++iW)
        {
            KUIWidget* pkWidget = pkUIFrame->GetWidgetAt(iW);
            KApplyDownWidget(pkWidget, pkFn);
        }
    }    
}
//---------------------------------------------------------------------------
class KUIFrame::KGetWidgetByName
{
public:
    KUIWidget*& m_pkWidget;
    std::string m_strName;
    KGetWidgetByName(KUIWidget*& pkWidget, std::string strName)
        : m_pkWidget(pkWidget), m_strName(strName)
    {
        m_pkWidget = NULL;
    }

    KGetWidgetByName& operator () (KUIWidget* pkWidget)
    {
       // if (NiIsKindOf(KUIFrame, pkWidget) && !m_pkWidget)
        {
            KUIFrame* pkFrame = (KUIFrame*) pkWidget;

            unsigned int iW;
            for (iW = 0; iW < pkFrame->GetWidgetCount(); ++iW)
            {
                KUIWidget* pkChild = pkFrame->GetWidgetAt(iW);
                if (m_strName == pkChild->GetName())
                {
                    m_pkWidget = pkChild;
                }
            }
        }

        return *this;
    }
};
//---------------------------------------------------------------------------
class KUIFrame::KIsIncludedFrame
{
public:
    bool& m_bIsIncluded;
    KUIWidget* m_pkWidget;
    KIsIncludedFrame(KUIWidget* pkWidget, bool& bIsIncluded)
        : m_pkWidget(pkWidget), m_bIsIncluded(bIsIncluded)
    {
        m_bIsIncluded = false;
    }

    KIsIncludedFrame& operator () (KUIWidget* pkWidget)
    {
//        if (NiIsKindOf(KUIFrame, pkWidget) && !m_bIsIncluded)
        {
            KUIFrame* pkFrame = (KUIFrame*) pkWidget;

            unsigned int iW;
            for (iW = 0; iW < pkFrame->GetWidgetCount(); ++iW)
            {
                KUIWidget* pkChild = pkFrame->GetWidgetAt(iW);
                if (pkChild == m_pkWidget)
                    m_bIsIncluded = true;
            }
        }

        return *this;
    }
};
//---------------------------------------------------------------------------
