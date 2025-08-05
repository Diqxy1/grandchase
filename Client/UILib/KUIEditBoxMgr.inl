//---------------------------------------------------------------------------
class KUIEditBoxMgr::KComposeList
{
public:
    std::vector<KUIEditBox*>& m_vecList;
    KComposeList(std::vector<KUIEditBox*>& vecList)
        : m_vecList(vecList)
    {
        m_vecList.clear();
    }

    KComposeList& operator () (KUIWidget* pkWidget)
    {
        //if (NiIsKindOf(KUIEditBox, pkWidget))
        {
            if (pkWidget->IsVisible())
                m_vecList.push_back((KUIEditBox*)pkWidget);
        }

        return *this;
    }
};
//---------------------------------------------------------------------------