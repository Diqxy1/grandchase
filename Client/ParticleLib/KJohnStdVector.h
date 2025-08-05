#ifndef _KJOHNSTDVECTOR_H_
#define _KJOHNSTDVECTOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <vector>
//#include "KJohnGen.h"

template<class _Ty, int INITIAL_SIZE = 64, class _A = std::allocator<_Ty> >
class KJohnStdVector : public std::vector<_Ty, _A>
{
public:
	/// constructor.
	KJohnStdVector() { reserve(INITIAL_SIZE); }
	//~KJohnStdVector() {}

	bool Find(const _Ty& t)
	{
		iterator itor = begin();
		while (itor != end())
		{
			// @note    thers must be exist 't.operator==()'
			if (t == (*itor))
				return true;

			++itor;
		}//while

		return false;
	}//Find()

	bool InsertIfNotExist(const _Ty& t)
	{
		if (false == Find(t))
		{
			push_back(t);
			return true;
		}//if

		return false;
	}//InsertIfNotExist()
};//class KJohnStdVector<>

#endif // _KJOHNSTDVECTOR_H_

/** @example    class KJohnStdVector<>

  1) test case for std::vector<>

	#include <windows.h>
	#include <stdio.h>
	#include <cassert>
	#include <string.h>
	//#include <iostream>
	//#include <vector>
	#include "KJohnStdVector.h"
	#include "Win32Alloc.h"

	typedef std::vector<int, Win32Alloc<int> >              TVECTOR;
	typedef std::vector<int, Win32Alloc<int> >::iterator    TVECTOR_ITOR;

	void TestVector(TVECTOR& vecTest)
	{
		std::cout << "size=" << vecTest.size() << std::endl;
		for (TVECTOR_ITOR itor=vecTest.begin(); itor != vecTest.end(); ++itor)
		{
			std::cout << (*itor) << std::endl;
		}//for
	}//TestVector()

	void main()
	{
		TVECTOR         vecTest;

		//vecTest.reserve( 8 );
		for (int i=0; i < 9; ++i)
		{
			vecTest.push_back( i );
		}//for

		TestVector( vecTest );

		vecTest.clear();
	}//main()

//    9번의 push_back()의 경우 std::vector<>의 동적 메모리 할당
//    i=0, 1개 할당
//    i=1, 2개 할당
//    i=2, 4개 할당
//    i=4, 8개 할당
//    i=8, 16개 할당
//    총 5번의 메모리 할당

  2) test case for KJohnStdVector<>

	#include <windows.h>
	#include <stdio.h>
	#include <cassert>
	#include <string.h>
	//#include <iostream>
	//#include <vector>
	#include "KJohnStdVector.h"
	#include "Win32Alloc.h"

	typedef KJohnStdVector<int, 8, Win32Alloc<int> >            TVECTOR;
	typedef KJohnStdVector<int, 8, Win32Alloc<int> >::iterator  TVECTOR_ITOR;

	void TestVector(TVECTOR& vecTest)
	{
		std::cout << "size=" << vecTest.size() << std::endl;
		for (TVECTOR_ITOR itor=vecTest.begin(); itor != vecTest.end(); ++itor)
		{
			std::cout << (*itor) << std::endl;
		}//for
	}//TestVector()

	void main()
	{
		TVECTOR         vecTest;

		for (int i=0; i < 9; ++i)
		{
			vecTest.push_back( i );
		}//for

		TestVector( vecTest );

		vecTest.clear();
	}//main()

//    for loop에 진입하기전 8개 할당
//    i=8, 16개 할당
//    총 2번의 메모리 할당

  3) real use when data size expected to 9

	#include <windows.h>
	#include <stdio.h>
	#include <cassert>
	#include <string.h>
	//#include <iostream>
	//#include <vector>
	#include "KJohnStdVector.h"

	typedef KJohnStdVector<int, 9>              TVECTOR;
	typedef KJohnStdVector<int, 9>::iterator    TVECTOR_ITOR;

	void TestVector(TVECTOR& vecTest)
	{
		std::cout << "size=" << vecTest.size() << std::endl;
		for (TVECTOR_ITOR itor=vecTest.begin(); itor != vecTest.end(); ++itor)
		{
			std::cout << (*itor) << std::endl;
		}//for
	}//TestVector()

	void main()
	{
		TVECTOR         vecTest;

		for (int i=0; i < 9; ++i)
		{
			vecTest.push_back( i );
		}//for

		TestVector( vecTest );

		vecTest.clear();
	}//main()

//    for loop에 진입하기전 9개 할당
//    총 1번의 메모리 할당
//    참고: 실제 push_back()의 수가 17이라면, 9번째 push_back() 호출때 18개 할당
//    하므로 총 2번의 메모리 할당이 일어납니다.

*/
