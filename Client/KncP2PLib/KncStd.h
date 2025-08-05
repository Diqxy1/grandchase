#ifndef _KNCSTD_H_
#define _KNCSTD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinSock2.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <map>
#include <list>
#include <Thread/Locker.h>

template <class KNCSTD_DATATYPE>
class KncVector
{
public:
    CRITICAL_SECTION                    csKncVector;
    std::vector<KNCSTD_DATATYPE>        vec;
    KncVector()
    {
        INITCS(csKncVector);
    }
    ~KncVector()
    {
        DELETECS(csKncVector);
    }
    KNCSTD_DATATYPE Get( unsigned int uiIndex )     // 인덱스를 미리 체크 하고 쓰도록 하자.
    {
        KLocker CSKncVector( csKncVector );
        return vec[uiIndex];
    }
    bool Get( unsigned int uiIndex, KNCSTD_DATATYPE* pData )
    {
        KLocker CSKncVector( csKncVector );
        unsigned int iCount = (unsigned int)vec.size();
        if ( iCount <= uiIndex )
            return false;
        *pData = vec[uiIndex];
        return true;
    }
    void RemoveAt( unsigned int uiIndex )
    {
        KLocker CSKncVector( csKncVector );
        if ( vec.size() == 0 )
            return;
        vec.erase( vec.begin() + uiIndex );
    }
    void Remove( KNCSTD_DATATYPE data )
    {
        KLocker CSKncVector( csKncVector );
        for ( int i = 0; i < (int)vec.size(); i++ )
        {
            if ( vec[i] == data )
            {
                vec.erase( vec.begin() + i );
                return;
            }
        }
    }
    void Push_Back( KNCSTD_DATATYPE data )
    {
        KLocker CSKncVector( csKncVector );
        vec.push_back( data );
    }
    int Size()
    {
        KLocker CSKncVector( csKncVector );
        return (int)vec.size();
    }
    void RemoveAll()
    {
        KLocker CSKncVector( csKncVector );
        vec.clear();
    }
    bool IsIn( KNCSTD_DATATYPE data_ )
    {
        int iCount = Size();
        KNCSTD_DATATYPE data;
        for ( int i = 0; i < iCount; i++ )
        {
            data = Get( i );
            if ( data == data_ )
                return true;
        }
        return false;
    }
    const KncVector<KNCSTD_DATATYPE>& operator = ( const KncVector<KNCSTD_DATATYPE>& vec_ )
    {
        for ( int i = 0; i < Size(); i++ )
        {
            Push_Back( vec_.Get(i) );
        }
    }
    const KncVector<KNCSTD_DATATYPE>& operator = ( const std::vector<KNCSTD_DATATYPE>& vec_ )
    {
        for ( int i = 0; i < Size(); i++ )
        {
            Push_Back( vec_[i] );
        }
    }
};

template<class KNCSTD_MAPKEY, class KNCSTD_DATATYPE>
class KncMap
{
public:
    CRITICAL_SECTION                        csKncMap;
    std::map<KNCSTD_MAPKEY,KNCSTD_DATATYPE> map;
    KncMap()
    {
        INITCS( csKncMap );
    }
    ~KncMap()
    {
        DELETECS( csKncMap );
    }
    bool Get( KNCSTD_MAPKEY key, KNCSTD_DATATYPE* pData )
    {
        KLocker CSKncMap( csKncMap );
        if ( map.find(key) == map.end() )
            return false;
        *pData = map[key];
        return true;
    }
    void Remove( KNCSTD_MAPKEY key )
    {
        KLocker CSKncMap( csKncMap );
        if ( map.find(key) == map.end() )
            return;
        map.erase( key );
    }
    void Add( KNCSTD_MAPKEY key ,KNCSTD_DATATYPE data)
    {
        KLocker CSKncMap( csKncMap );
        map[key] = data;
    }
    bool IsIn( KNCSTD_MAPKEY key )
    {
        KLocker CSKncMap( csKncMap );
        if ( map.find(key) == map.end() )
            return false;
        return true;
    }
    void RemoveAll()
    {
        KLocker CSKncMap( csKncMap );
        map.clear();
    }
    int  Size()
    {
        KLocker CSKncMap( csKncMap );
        return (UINT)map.size();
    }
};

#endif // _KNCSTD_H_