#pragma once

#if defined( _TRUNK )
#define NATION_KOREA
#endif

// 아무 국가도 명시되어 있지 않다면 한국으로 하자.
#if !defined( NATION_KOREA ) && !defined( NATION_JAPAN ) && !defined( NATION_TAIWAN ) && !defined( NATION_HONGKONG ) && !defined( NATION_CHINA ) && !defined( NATION_PHILIPPINE ) && !defined( NATION_BRAZIL ) && !defined( NATION_USA ) && !defined( NATION_IDN ) && !defined( NATION_THAILAND ) && !defined(NATION_LATINAMERICA) && !defined(NATION_EU)
#   define NATION_KOREA
#endif

#include "Define_common.h"

// -----------------------------------------------------------------------------
// 국가별 로드
// -----------------------------------------------------------------------------

#if defined(NATION_TAIWAN)              // 대만 버전 (오픈베타/정식)
#include "Define_tw.h"

#elif defined(NATION_BRAZIL)            // 브라질 버전
#include "Define_br.h"

#elif defined(NATION_USA)             // 미국 버전
#include "Define_us.h"

#elif defined(NATION_PHILIPPINE)        // 필리핀 버전
#include "Define_ph.h"

#elif defined(NATION_IDN)			// 인도네시아 버전
#include "Define_id.h"

#elif defined(NATION_THAILAND)			// 태국 버전
#include "Define_th.h"

#elif defined(NATION_LATINAMERICA)               // 남미 버전
#include "Define_la.h"

#elif defined(NATION_EU)                //유럽 버전
#include "Define_eu.h"

#elif defined(NATION_CHINA)
#include "Define_cn.h"

#else // NATION_KOREA                   // 한국 버전
#include "Define_kr.h"

//#	if defined( _OPEN_TEST_ )
//#		define NATION_CODE L"KROP"
//#	else
//#	   define NATION_CODE L"KR"
//#	endif
#endif
