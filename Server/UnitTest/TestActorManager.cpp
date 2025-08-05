#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>

//#include "ActorManager.h"
//#include "ActorFactory.h"
//#include "User.h"
//#include "UserFSM.h"
//
//static const wchar_t* szName = L"TestName";

void TestActorManager()
{
    //std::wstring strName( szName );

    ////KSobManager< boost::shared_ptr<KActor> > kMgr;
    //KActorManager kMgr;
    //kMgr.SetActorFactory( IActorFactoryPtr( new KActorFactory<KUser,KUserFSM> ) );

    //BOOST_CHECK_EQUAL( kMgr.GetCount(), 0 );

    //KActorPtr spActor( kMgr.CreateActor() );

    //BOOST_CHECK_EQUAL( kMgr.GetCount(), 1 );
    //BOOST_CHECK_EQUAL( spActor.use_count(), 3 );

    //kMgr.Rename( strName, *spActor );

    //BOOST_CHECK( strName.compare( spActor->GetName() ) == 0 );

    //BOOST_CHECK_EQUAL( spActor.use_count(), 3 );

    //BOOST_CHECK_EQUAL( kMgr.GetByName( strName ), spActor );

    //KActorPtr spActor2( kMgr.GetByName( strName ) );

    //BOOST_CHECK_EQUAL( spActor, spActor2 );
    //BOOST_CHECK_EQUAL( spActor.use_count(), 4 );

    //spActor2 = kMgr.GetByIndex( 0 );

    //BOOST_CHECK_EQUAL( spActor, spActor2 );
    //BOOST_CHECK_EQUAL( spActor.use_count(), 4 );

    //kMgr.ReserveDelete( strName );
    //kMgr.Tick();

    //BOOST_CHECK_EQUAL( kMgr.GetCount(), 0 );
    //BOOST_CHECK_EQUAL( spActor.use_count(), 2 );
}