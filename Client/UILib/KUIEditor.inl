//---------------------------------------------------------------------------
struct KIsCursorInRect
{
    KUIWidget*& m_pkInWidget;
    float m_fArea;

    KIsCursorInRect(KUIWidget*& pkInWidget)
        : m_pkInWidget(pkInWidget)
    {
        m_pkInWidget = NULL;
        m_fArea = FLT_MAX;
    }

    KIsCursorInRect& operator () (KUIWidget* pkWidget)
    {
        float fArea = pkWidget->GetArea();
        if (pkWidget->IsCursorInRect() &&
            m_fArea > fArea)
        {
            m_pkInWidget = pkWidget;
            m_fArea = fArea;
        }

        return *this;
    }    
};
//---------------------------------------------------------------------------