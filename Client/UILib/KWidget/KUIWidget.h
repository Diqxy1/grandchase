#pragma once

#include "KUISize.h"
#include "KUIOffset.h"
#include "KUIScript.h"
//#include "KLuabinder.h"

class KUIFrame;
class TiXmlNode;

typedef KUIWidget* KUIWidgetPtr;

class KUIWidget : public lua_tinker::lua_value
{

    //NiDeclareRootRTTI(KUIWidget);
    KDeclareLuaInvoke;

public:
    static void RegisterLuabind();

    KUIWidget();
    virtual ~KUIWidget();

    template <class TType>
    static void Creates(KUIFrame* pkUIFrame, TiXmlNode* pkUIRoot, std::string strTag);

    template <class TType>
    static TType* CreateOne(KUIFrame* pkUIParent, TiXmlNode* pkUIRoot, std::string strTag);

    virtual void Load(TiXmlNode* pkNode);

    void LoadInheritance(TiXmlNode* pkNode);

    virtual void Render();    
    virtual void Update();
    virtual bool UpdateInput();

    // Clone
public:
    virtual KUIWidget* CreateObject();
    virtual void CopyMember(KUIWidget* pkWidget);
    KUIWidget* Clone(KUIFrame* pkParent);

    // Offset
public:
    virtual bool IsCursorInRect();

	D3DXVECTOR2 GetWorldPos();
    D3DXVECTOR2 GetLocalPos();
    void SetLocalPos(D3DXVECTOR2 kPos);

    D3DXVECTOR2 GetSize();
    float GetArea();

public:
    KUISize m_kSize;
    KUIOffset m_kOffset;

    // Parent
public:
    void AttachParent(KUIWidget* pkParent);
    void DetachParent();
    KUIWidget* GetParent();
    lua_tinker::lua_value* GetParent_();
    bool IsBackWidget();

protected:
    KUIWidget* m_pkParent;

    // Name
public:
    void LoadName(TiXmlNode* pkNode);
    const char* GetName();
    void SetName(const char* pcName);

protected:
    std::string m_strName;

    // Extra Name
public:
    void SetExtName(const char* pcName);
    const char* GetExtName();

protected:
    std::string m_strExtName;

    // Visible
public:
    void Hide();
    void Show();
    bool IsVisible();

protected:
    bool m_bVisible;

    // InputEnable
public:
    void LockInput();
    void UnlockInput();
    bool IsInputEnable();

protected:
    bool m_bInputEnable;

    // Script
public:
    void RegisterLuaVariable();

protected:
    KUIScript m_kScript;

    // Functor
public:
    class KIsVisibleWidget;
};

#include "KUIWidget.inl"