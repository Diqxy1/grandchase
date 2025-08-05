enum EAttributeType
{
    ATTRTYPE_ATK_POINT,                           // 공격력 + @1
    ATTRTYPE_DEF_POINT,                          // 방어력 + @1
    ATTRTYPE_HP_POINT,                               // MAX HP + @1
    ATTRTYPE_SPECIAL_ATK_POINT,                   // 필살기 공격력 + @1
    ATTRTYPE_SPECIAL_DEF_POINT,                  // 필살기 방어력 + @1
    ATTRTYPE_HP_DRAIN_RATE,               // 일반 공격 데미지의 @1 %를 HP로 흡수
    ATTRTYPE_MP_DRAIN_RATE,               // 일반 공격 데미지의 @1 %를 MP로 흡수
    ATTRTYPE_HP_RECOVERY_RATE,                       // HP 회복량 + @1 %
    ATTRTYPE_MP_RECOVERY_RATE,                       // MP 회복량 + @1 %
    ATTRTYPE_EQUIP_LIMIT_LEVEL_DOWN,                    // 장착 레벨 -@1 %
    ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE,              // GP 추가획득량
    ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE,             // EXP 추가획득량
    ATTRTYPE_CRITICAL_ATTACK_RATE,                      // 일반/필살기 공격 치명타 확률 + @1 %
    ATTRTYPE_COUNTER_DEF,   // 카운터 피격시 MP소모량 %감소

    ATTRTYPE_ROLLING,                            // 롤링 어택 가능
    ATTRTYPE_ATTACK_PERCENT,                         // 공격력 +@1% 상승
    ATTRTYPE_DEFENCE_PERCENT,                        // 방어력 +@1% 상승
    ATTRTYPE_HP_PERCENT,                             // 생명력 +@1% 상승
    ATTRTYPE_FAST_HP_RECOVERY_DURING_LIMIT_TIME,            // 대전 시작 시 @1초 동안 HP를 빠르게 회복
    ATTRTYPE_IGNORE_HASTE,                              // 헤이스트 효과 무효화
    ATTRTYPE_AUTO_RECOVERY_HP_LIMIT_IN_DUNGEON,        // 던전 내 HP 회복 한계치 +@
    ATTRTYPE_ABNORMAL_CONDITION_RESISTANCE,        // 상태 이상 공격 저항력 +@1%
    ATTRTYPE_ARENA_ABSENCE_TIME,                        // 아레나 무적시간 +@1%
    ATTRTYPE_SKILL_POINT_EXP_BONUS,                     // 스킬포인트 보너스
    ATTRTYPE_ALL_STAT_POINT,                         // 공방생 + @1
    ATTRTYPE_ALL_STAT_PERCENT,                       // 공방생 + @1%


    ATTRTYPE_MAX,        
};