/** @file   KCSLockType.h
    @brief  syntactic safeguard for data in a critical section.
    @author robobeg@kogstudios.com
    @since  2009-1-12, 09:00
*/

#pragma once
#ifndef _KCSLOCKTYPE_H
#define _KCSLOCKTYPE_H

#include    <boost/noncopyable.hpp>
#include    <boost/static_assert.hpp>
#include    <boost/type_traits/is_const.hpp>
#include    <boost/type_traits/is_reference.hpp>
#include    <boost/type_traits/add_const.hpp>
#include    <boost/type_traits/add_reference.hpp>
#include    <boost/type_traits/remove_const.hpp>
#include    <boost/type_traits/remove_reference.hpp>
#include    <windows.h>
#include    "KPreBlock.h"


 
template<typename T>
class KCSLockType: private boost::noncopyable
{
public:

    BOOST_STATIC_ASSERT( !boost::is_const<T>::value );
    BOOST_STATIC_ASSERT( !boost::is_reference<T>::value );

    typedef T              TYPE;

    template<bool bIsConst>
    class   KCSLockedAccess;

    template<>
    class   KCSLockedAccess<false> : private boost::noncopyable
    {
    public:
        
        KCSLockedAccess( KCSLockType& kIn_ )
            : m_cs( kIn_.m_cs ), m_variable( kIn_.m_variable )
        {
            ::EnterCriticalSection( &m_cs );
        }//KCSLockedAccess()

        ~KCSLockedAccess()
        {
            ::LeaveCriticalSection( &m_cs );
        }//~KCSLockedAccess()

        T&                      AccessVariable() { return m_variable; }
    private:
        CRITICAL_SECTION&       m_cs;
        T&                      m_variable;    
    };//

    template<>
    class   KCSLockedAccess<true> : private boost::noncopyable
    {
    public:

        KCSLockedAccess( const KCSLockType& kIn_ )
            : m_cs( kIn_.m_cs ), m_variable( kIn_.m_variable )
        {
            ::EnterCriticalSection( &m_cs );
        }//KCSLockedAccess()

        ~KCSLockedAccess()
        {
            ::LeaveCriticalSection( &m_cs );
        }//~KCSLockedAccess()

        const T&                AccessVariable() const { return m_variable; }
    private:
        /*mutable*/ CRITICAL_SECTION&       m_cs;
        const T&                m_variable;    
    };//

    KCSLockType()           { ::InitializeCriticalSection( &m_cs ); }
    KCSLockType( const T& kIn_ ) : m_variable( kIn_ ) { ::InitializeCriticalSection( &m_cs ); }
    ~KCSLockType()          { ::DeleteCriticalSection( &m_cs ); }

private:
    /*mutable*/ CRITICAL_SECTION       m_cs;
    T                       m_variable;
};//KCSLockType

template<bool C, typename A, typename B> 
struct _kcslock_if_ {};
template<typename A, typename B> 
struct _kcslock_if_<true, A, B> { typedef A type; };
template<typename A, typename B> 
struct _kcslock_if_<false, A, B> { typedef B type; };

#define _KCSLOCK_TYPEDEF_(_type_)            \
    _kcslock_if_< boost::is_reference< _type_ >::value \
        , boost::add_reference< KCSLockType<boost::remove_reference< _type_ >::type> >::type \
        , KCSLockType<boost::remove_reference< _type_ >::type> >::type


template<bool C, typename A>
struct  _kcslock_add_const_if_ {};

template<typename A>
struct  _kcslock_add_const_if_<true, A> { typedef typename boost::add_const<A>::type type; };

template<typename A>
struct  _kcslock_add_const_if_<false, A> { typedef A type; };


/** @def   KCSLOCK_TYPE(_variable_)
    @brief  \a _variable_ 의 \c KCSLockType
*/
#define KCSLOCK_TYPE(_variable_)            __KCSLockType__##_variable_


/** @def   KCSLOCK_VARIABLE_TYPE(_variable_)
    @brief \a _variable_ 의 type
*/
#define KCSLOCK_VARIABLE_TYPE(_variable_)   boost::remove_reference<KCSLOCK_TYPE(_variable_)>::type::TYPE


/** @def   KCSLOCK_DECLARE(_type_,_variable_)
    @brief type \a _type_ 의 \a _variable_ 을 \c KCSLockType 으로 선언한다.

    예)
    @code
        KCSLOCK_DECLARE( std::set<int>, m_set )     // KCSLockType< std::set<int> > 타입의 변수 m_set 선언
        KCSLOCK_DECLARE( int&, m_refInt )           // KCSLockType< int >& 타입의 변수 m_refInt 선언
                                                    // 주의! KCSLockType< int& > 타입의 변수 선언이 아니다.
        static KCSLOCK_DECLARE( bool, ms_bInitialized )   // KCSLockType< bool > 타입의 static 변수 ms_bInitialized 선언
    @endcode
*/
#define KCSLOCK_DECLARE(_type_,_variable_)  _KCSLOCK_TYPEDEF_(KARG(_type_)) (_variable_);  typedef _KCSLOCK_TYPEDEF_(KARG(_type_)) KCSLOCK_TYPE(_variable_);


/** @def  KCSLOCK_SCOPE_IMPLEMENT(_scope_,_variable_)
    @brief static KCSLOCK_DECLARE( ... ) 형태로 선언하면 static member variable 이 된다. 이에 대한 implementation을 위해 사용된다.
    예)
    @code
        KCSLOCK_SCOPE_IMPLEMENT( KClassName, ms_variable );
    혹은
        KCSLOCK_SCOPE_IMPLEMENT( KClassName, ms_variable ) = ... ; // 초기화
    @endcode
*/
#define KCSLOCK_SCOPE_IMPLEMENT(_scope_,_variable_)   _scope_::KCSLOCK_TYPE(_variable_) _scope_::_variable_
#define KCSLOCK_IMPLEMENT(_variable_)   KCSLOCK_TYPE(_variable_) _variable_







/** @def KCSLOCK_BLOCK(_variable_)
*/
#define KCSLOCK_BLOCK(_variable_) \
    KPREBLOCK_DECLARE_INITIALIZE( _kcslock_add_const_if_<KPREBLOCK_IS_CONST(_variable_) KCOMMA() boost::remove_reference<KCSLOCK_TYPE(_variable_)>::type::KCSLockedAccess<KPREBLOCK_IS_CONST(_variable_)> >::type, \
        _KCSLOCK_access_, _variable_ ) \
    KPREBLOCK_BLOCK( boost::add_reference<_kcslock_add_const_if_<KPREBLOCK_IS_CONST(_variable_) KCOMMA() KCSLOCK_VARIABLE_TYPE(_variable_)>::type>::type (_variable_)( KLOCALVAR( _KCSLOCK_access_ ).AccessVariable() ) )


/** @example KCSLockType.h

1. 클래스 내에서의 KCSLockType 변수 정의

@code

#include <algorithm>
#include <vector>
#include "KCSLockType.h"


class   KMainThread
{
public:

    void    AccessKCSLock();
    void    ConstAccessKCSLock() const;
    void    PassToSubThread();

private:

    KCSLOCK_DECLARE( std::vector<int>, m_vecInt );  
    // KCSLockType< std::vector<int> > 타입의 변수 m_vecInt 을 선언

};//class   KMainThread

@endcode


2. KCSLockType 변수 접근

@code 

void    KMainThread::AccessKCSLock()
{
    KCSLOCK_BLOCK( m_vecInt )       // BEGIN, END 블럭 내에서는 자유로운 접근이 가능하다.
    {
        m_vecInt.push_back( 1 );
        m_vecInt.push_back( 2 );
        m_vecInt.push_back( 3 );
    }//KCSLOCK_BLOCK()

    // m_vecInt.push_back( 4 );     // BEGIN, END 블럭 밖에서 접근하려 하면 compile-time error 가 발생한다.
}//KMainThread::AccessKCSLock()


void    KMainThread::ConstAccessKCSLock() const
{
    KCSLOCK_BLOCK( m_vecInt )   // const 함수 내에서는 KCSLOCK_BLOCK을 사용하여 접근한다.
    {

        KCSLOCK_VARIABLE_TYPE( m_vecInt )::const_iterator iter 
            = std::find( m_vecInt.begin(), m_vecInt.end(), 3 );
        if ( iter == m_vecInt.end() )
        {
            return; // BEGIN, END 블럭 밖으로 빠져나갈 때 continue, break, return, goto를 사용해도 된다.
        }//if
        //...

    }//KCSLOCK_BLOCK()
}//KMainThread::ConstAccessKCSLock()

@endcode


3. KCSLockType 변수의 reference 전달

@code

class   KSubThread
{
private:

    KCSLOCK_DECLARE( std::vector<int>&, m_refVecInt );    
    // std::vector<int>& 타입이 아니라 KCSLockType< std::vector<int> >& m_refVecInt 를 선언하는 것임.

public:

    // KCSLockType의 reference를 파라미터로 받는다. 
    // KSubThread( KCSLockType< std::vector<int> >& refIn_ ); 과 의미가 같다.
    KSubThread( KCSLOCK_TYPE( m_refVecInt ) refIn_ );

    void    AccessKCSLock();    
};//class   KSubThread


KSubThread::KSubThread( KSubThread::KCSLOCK_TYPE( m_refVecInt ) refIn_ )
    : m_refVecInt( refIn_ )
{
}//KSubThread::KSubThread()


void    KMainThread::PassToSubThread()
{
    KSubThread  kSubThread( m_vecInt );  // KCSLockType 변수를 다른 클래스에 reference로 전달할 때는 BEGIN/END 블럭을 사용하지 않는다.
    //...
}//KMainThread::PassToSubThread()


void    KSubThread::AccessKCSLock()
{
    KCSLOCK_BLOCK( m_refVecInt )
    {
        // reference 역시 접근 방법은 동일하다.
    }//KCSLOCK_BLOCK()
}//KSubThread::AccessKCSLock()

@endcode

*/


#endif // #ifndef _KCSLOCKTYPE_H
