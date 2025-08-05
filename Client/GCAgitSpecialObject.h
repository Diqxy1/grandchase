#pragma once

enum EM_AGIT_OBJECT_ACTION
{
    EM_AGIT_OBJECT_JUMP_ACTION      = 0, 
    EM_AGIT_OBJECT_DICE_ACTION      = 1,
    EM_AGIT_OBJECT_FLAG_ACTION      = 2,
    EM_AGIT_OBJECT_SIZE             = 3, 
};

struct KAgitJumpObject
{
    GCITEMID    m_ItemID;
    float       m_fJumpX;
    float       m_fJumpY;
    int         m_nSound;
    int         m_nBasicEffectNum;

    KAgitJumpObject()
    {
        m_ItemID = UINT_MAX;
        m_fJumpX = 0.0f;
        m_fJumpY = 0.0f;
        m_nSound = -1;
        m_nBasicEffectNum = 0;
        m_vecEffectName.clear();
    }

    std::vector<std::string>    m_vecEffectName;
};

struct KAgitDiceObject
{
    GCITEMID    m_ItemID;
    int         m_nSound;

    int         m_nBasicEffectNum;

    std::vector<std::string>    m_vecEffectName;

    KAgitDiceObject()
    {
        m_ItemID = UINT_MAX;
        m_nSound = -1;
        m_nBasicEffectNum = 0;
        m_vecEffectName.clear();
    }
};

struct KAgitFlagObject
{
    GCITEMID    m_ItemID;
    int         m_nSound;


    std::vector<std::string>    m_vecEffectName;

    KAgitFlagObject()
    {

        m_ItemID = UINT_MAX;
        m_nSound = -1;
        m_vecEffectName.clear();
    }
};


typedef std::map<GCITEMID, EM_AGIT_OBJECT_ACTION> MAP_SPECIAL_AGIT_OBJ;
typedef MAP_SPECIAL_AGIT_OBJ::iterator            MAP_SPECIAL_AGIT_OBJ_ITER;

typedef std::map<GCITEMID, KAgitJumpObject> MAP_AGIT_JUMP_OBJECT;
typedef MAP_AGIT_JUMP_OBJECT::iterator      MAP_AGIT_JUMP_OBJECT_ITER;

typedef std::map<GCITEMID, KAgitDiceObject> MAP_AGIT_DICE_OBJECT;
typedef MAP_AGIT_DICE_OBJECT::iterator      MAP_AGIT_DICE_OBJECT_ITER;


typedef std::map<GCITEMID, KAgitFlagObject> MAP_AGIT_FLAG_OBJECT;
typedef MAP_AGIT_FLAG_OBJECT::iterator      MAP_AGIT_FLAG_OBJECT_ITER;



