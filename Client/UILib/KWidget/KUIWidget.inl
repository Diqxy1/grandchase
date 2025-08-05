//---------------------------------------------------------------------------
template <class TFunc>
void KApplyUpWidget(KUIWidget* pkObject, TFunc pkFn)
{
    // Apply Function
    if (!pkObject)
        return;

    pkFn(pkObject);

    // Parent Widget
    KApplyUpWidget(pkObject->GetParent(), pkFn);
}
//---------------------------------------------------------------------------
class KUIWidget::KIsVisibleWidget
{
public:
    bool& m_bIsVisible;
    KIsVisibleWidget(bool& bIsVisible)
        : m_bIsVisible(bIsVisible)
    {
        m_bIsVisible = true;
    }

    KIsVisibleWidget& operator () (KUIWidget* pkWidget)
    {
        if (!pkWidget->m_bVisible)
            m_bIsVisible = false;

        return *this;
    }
};
//---------------------------------------------------------------------------
template <class TType>
void KUIWidget::Creates(KUIFrame* pkUIFrame, TiXmlNode* pkUIRoot, std::string strTag)
{
    TiXmlNode* pkNode = pkUIRoot->FirstChild(strTag.c_str());
    while (pkNode)
    {
        TType* pkWidget = new TType;
        pkUIFrame->AttachWidget(pkWidget);
        pkWidget->Load(pkNode);

        pkNode = pkNode->NextSibling(strTag.c_str());
    }
}
//---------------------------------------------------------------------------
template <class TType>
TType* KUIWidget::CreateOne(KUIFrame* pkUIParent, TiXmlNode* pkUIRoot, std::string strTag)
{
    TiXmlNode* pkNode = pkUIRoot->FirstChild(strTag.c_str());
    if (!pkNode)
        return NULL;

    TType* pkWidget = new TType;
    pkUIParent->AttachWidget(pkWidget);
    pkWidget->Load(pkNode);

    return pkWidget;
}
//---------------------------------------------------------------------------