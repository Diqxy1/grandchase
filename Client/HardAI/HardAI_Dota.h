#pragma once

class HardAI_Core
{
public:
    HardAI_Core(void){};
    ~HardAI_Core(void){};

public:
    void Action_Wait(int iFrame, int iMonIdx);
    void Action_Die(int iFrame, int iMonIdx);

public:
    void CallBack_Die(int iMonIdx);
    void CallBack_Damage(int iMonIdx);
};

class HardAI_Ancient
{
public:
    HardAI_Ancient(void){};
    ~HardAI_Ancient(void){};

public:
    void Action_WaitFunc(int iFrame, int iMonIdx);
    void Action_WalkFunc(int iFrame, int iMonIdx);
    void Action_WalkFailFunc(int iMonIdx);
    void Ancient_Attack01Func(int iFrame, int iMonIdx);
    void Ancient_Attack02Func(int iFrame, int iMonIdx);
    void Ancient_JumpDownFunc(int iFrame, int iMonIdx);
    void Ancient_JumpToWait(int iFrame, int iMonIdx);
    void Ancient_DieFunc(int iFrame, int iMonIdx);
    void Ancient_StoneCurseFunc(int iFrame, int iMonIdx);
    void Ancient_IceCurseFunc(int iFrame, int iMonIdx);
    void Ancient_HoldFunc(int iFrame, int iMonIdx);

public:
    void CallBack_Land(int iMonIdx);
    void CallBack_Die(int iMonIdx);

public:
    void Trigger_Check_Targeting(int iFrame, int iMonIdx);
    void Trigger_Check_JumpDown(int iFrame, int iMonIdx);
    void Trigger_Check_Attack01(int iFrame, int iMonIdx);
    void Trigger_Check_Attack02(int iFrame, int iMonIdx);
    void Trigger_Check_Attack03(int iFrame, int iMonIdx);
};

class HardAI_GuardianTower
{
public:
    HardAI_GuardianTower(void){};
    ~HardAI_GuardianTower(void){};	

public:
    void Action_WaitFunc(int iFrame, int iMonIdx);
    void Action_AttackFunc(int iFrame, int iMonIdx);
    void Action_DieFunc(int iFrame, int iMonIdx);

public:
    void CallBack_Die(int iMonIdx);
    void CallBack_Damage(int iMonIdx);

public:
    void Trigger_Check_Targeting(int iFrame, int iMonIdx);
    void Trigger_Check_Attack(int iFrame, int iMonIdx);
};

class HardAI_GuardTower
{
public:
    HardAI_GuardTower(void){};
    ~HardAI_GuardTower(void){};

public:
    void Action_WaitFunc(int iFrame, int iMonIdx);
    void Action_AttackFunc(int iFrame, int iMonIdx);
    void Action_DieFunc(int iFrame, int iMonIdx);

public:
    void CallBack_Die(int iMonIdx);
    void CallBack_Damage(int iMonIdx);

public:
    void Trigger_Check_Targeting(int iFrame, int iMonIdx);
    void Trigger_Check_Attack(int iFrame, int iMonIdx);
};