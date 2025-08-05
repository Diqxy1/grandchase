//#include "KLuaManager.h"
//#include "KGCLuabinder.h"
//#include <vector>
#pragma once

#define  TIME_GAP       2
//#define  TIME_GAP       0.01
#define  DELAY_GAP      0.01
//#define  DELAY_GAP      0.01

#define  BOX_MATERIAL_DISTANCE_X      17
#define  BOX_MATERIAL_DISTANCE_Y      20

#define NUM_MATERIAL_CNT   6


class KGCCrafting
{
public:
    KGCCrafting(void);
    ~KGCCrafting(void);
};


//제작 관련
class ManufactureMaterial
{
public:
    int nItemID;                //재료아이템 번호
    int NeedItemCount;          //필요한 양 
};

class ManufactureRecipe
{
public:
    int nRecipeID;              //레시피 아이템 번호
    int nItemID;                //생산 완료할때 주는 아이템 번호
    int nFactor;                // Factor
    std::vector<ManufactureMaterial> vecMaterials;     //필요 재료 아이템들 백터
};
