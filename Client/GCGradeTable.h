#pragma once
#include "Lottery.h"
#include "KLuaManager.h"

class KGCGradeTable
{
public:
    KGCGradeTable(void);
    ~KGCGradeTable(void);

    enum{
        FAIL_GRADE = -1,

        GRADE_NORMAL = 0,
        GRADE_RARE = 1,
        GRADE_EPIC = 2,
        GRADE_LEGEND = 3,
        GRADE_MYSTIC = 4,

        GRADE_NUM
    };

    typedef std::map< int, float > ElementList;
private:
    int      m_iIndex;
    KLottery m_kLottery;
    ElementList m_mapGradeList;

    std::string GetTab( int iTab )
    {
        std::stringstream stm;
        while( iTab-- )
        {
            stm<<"\t";
        }
        return stm.str();
    }
    static std::string GetEnumString( int iEnum );
    
public:
    bool IsFixed();
    int GetGrade();
    bool LoadScript( int iIndex_, KLuaManager& luaMgr );
    bool SaveTable( IN OUT std::ofstream& file, int iTab );
    int GetIndex() const { return m_iIndex; }
    void SetIndex(int val) { m_iIndex = val; }

    bool SetGrade( int iGrade, float fProb );
    float GetTotalProb();
    float GetExtraProb();

    const ElementList& GetContainer() const { return m_mapGradeList; }

    static void SaveEnum( IN OUT std::ofstream& file );
};
