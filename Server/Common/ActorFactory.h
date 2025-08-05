#pragma once

#include "FSM/FSMClass.h"
#include <boost/utility.hpp>
#include <boost/pool/singleton_pool.hpp>
#include "Log4.h"
SmartPointer( IActorFactory );
//FILE_NAME_FOR_LOG
// 단순히 KActorFactory의 인터페이스 역할만을 한다. 동적생성을 위한 포인터를 선언할 때 쓰인다.
class KActor;
class IActorFactory
{
public:
    //IActorFactory() : m_iInitInput( -1 ) {}
    //virtual ~IActorFactory(void) {}

	IActorFactory& operator=(const IActorFactory&) = delete;
	//IActorFactory(const IActorFactory&) = delete;
	IActorFactory& operator&&(const IActorFactory&&) = delete;

    virtual std::shared_ptr<KActor> CreateActor()
	{
		START_LOG( clog, L"Virtual Function Call... Check Please...." ) << END_LOG;
		return std::shared_ptr<KActor>( (KActor*)(NULL) ); 
	}

    //void SetActorFSM( FSMclassPtr spFSM, int iInitInput )
    //{
    //    m_spFSM         = spFSM;
    //    m_iInitInput    = iInitInput;
    //}
};

template < class ActorType, class FSMType, class PoolTag >
class KActorFactory : public IActorFactory
{
    typedef boost::singleton_pool<PoolTag, sizeof(ActorType), boost::default_user_allocator_new_delete, boost::details::pool::default_mutex, 2500> ActorPool;
public:
    KActorFactory() : m_spFSM( new FSMType ) {}
    virtual ~KActorFactory(void)
    {
    }

    static void DelActor( ActorType* pBuff_ )
    {
        pBuff_->~ActorType();
        ActorPool::free(pBuff_);
    }

    virtual std::shared_ptr<KActor> CreateActor()
    {
        ActorType* pBuff = (ActorType*)ActorPool::malloc();
        if ( pBuff == NULL ) {
            START_LOG( cerr, L"Memory Alloc Fail.... !!" ) << END_LOG;
            return std::shared_ptr<KActor>();
        }

        ::new( (void *)pBuff ) ActorType; // 명시적 생성자 호출.

        std::shared_ptr<KActor> spActor( pBuff, DelActor );
        if( spActor )
        {
            spActor->SetFSM( m_spFSM );
            spActor->StateTransition( FSMType::INPUT_CONNECT );
        }
        else
        {
            START_LOG( cerr, L" new KActor Fail..!!!! ") << END_LOG;
        }

        return spActor;
    }

protected:
    FSMclassPtr m_spFSM;
};
