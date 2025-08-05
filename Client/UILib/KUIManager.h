#pragma once

#include "KUIFrame.h"
//#include "KLuabinder.h"

class TiXmlNode;

class KUIManager;
extern KUIManager* g_pUIManager;

//singletone
class KUIManager
{

public:
    KUIManager();
    virtual ~KUIManager();

    static void RegisterLuabind();

    //void Save(const char* strFilename);
    void Load();
    void Load(const char* strFilename);
    void AddXMLFile(const char* strFilename);
    void ClearXMLFile();
    void LoadBackWidgets(TiXmlNode* pkNode);

    void Render();
    void Update();
    void UpdateInput();


public:
	static KUIManager* GetInstance() { return g_pUIManager; }
    void InitFrame();
    void SetCurFrame(KUIFrame* pkUIFrame);

    KUIFrame* GetCurFrame();
    KUIFrame* GetBackFrame();
    bool IsBackWidget(KUIWidget* pkWidget);
    KUIWidget* GetWidgetByName(const char* pcName);
    lua_tinker::lua_value* GetWidgetByName_(const char* pcName);

protected:
    KUIFrame* m_spCurFrame;
    KUIFrame* m_spBackFrame;

    //std::vector<std::string> m_vecScriptFilename;
    std::vector<std::string> m_vecXMLFilename;
};