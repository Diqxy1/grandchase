#pragma once

class TiXmlNode;
class KUIWidget;

class KUIScript
{

public:
    enum EEventType
    {
        eOnLoad,
        eOnUpdate,
        eOnBtnDown,
        eOnKeyEnter,
        eEventTypeMax
    };

public:
    KUIScript(KUIWidget* pkWidget);

    void Load(TiXmlNode* pkNode);
    void LoadEvent(TiXmlNode* pkNode);

    void Update();
    void UpdateInput();
    void Execute(int iEventType);

    void CopyMember(KUIScript* pkScript);

protected:
    std::vector<std::string> m_vecEvents;

    KUIWidget* m_pkOwner;

};