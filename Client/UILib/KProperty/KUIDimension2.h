#pragma once

class TiXmlNode;

class KUIDimension2
{

public:
    KUIDimension2();

    virtual void Load(TiXmlNode* pkNode);

    virtual D3DXVECTOR2 GetDim();
    virtual void SetDim(D3DXVECTOR2 kDim);

    bool IsZero();

protected:
    float m_fX, m_fY;

};