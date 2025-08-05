#include "stdafx.h"
#include ".\gcsquarepeopleelesis.h"



#include "Controls.h"

#include "GCSquarePeopleCommon.h"

void GCSquarePeopleElesis::OnWait( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Up && pPeople->GetContact() )
        {
            g_MyD3D->MyCtrl->k_Up = false;
            pPeople->SetPeopleState( GCSquarePeople::JUMP_READY_PEOPLE );
            pPeople->SetFrame( 0 );
            return;
        }

        if( g_MyD3D->MyCtrl->k_Down )
        {
            g_MyD3D->MyCtrl->k_Down = false;
            //if( pPeople->GetContact() )
            //{
            //    // [12/26/2007] breadceo. 결국 나중에 포탈타는거 만들겠네
            //    pPlayer->SetPlayerState(MID_COMMON_INTO_THE_TELEPORT);
            //    pPlayer->cFrame=0;
            //    return 0;
            //}
            pPeople->SetYSpeed( -0.01f );
            pPeople->SetNoCheckContact( 40 );
            return;
        }

        pPeople->SetXSpeed( 0.0f );
        if( g_MyD3D->MyCtrl->k_Fresh_Right_Dash )
        {
            pPeople->SetFrame( 0 );
            pPeople->SetIsRight( true );
            if( CID_LEY1 != pPeople->GetExtraCharNum() && CID_LEY2 != pPeople->GetExtraCharNum() )
                pPeople->SetXSpeed( 0.015f );
            pPeople->SetPeopleState( GCSquarePeople::DASH_PEOPLE );
            return;
        }
        else if( g_MyD3D->MyCtrl->k_Fresh_Left_Dash )
        {
            pPeople->SetFrame( 0 );
            pPeople->SetIsRight( false );
            if( CID_LEY1 != pPeople->GetExtraCharNum() && CID_LEY2 != pPeople->GetExtraCharNum() )
                pPeople->SetXSpeed( -0.015f );
            pPeople->SetPeopleState( GCSquarePeople::DASH_PEOPLE );
            return;
        }
        else if( g_MyD3D->MyCtrl->k_Right )
        {
            if( pPeople->GetPeopleState() != GCSquarePeople::WALK_PEOPLE )
            {
                pPeople->SetFrame( 0 );
            }
            pPeople->SetIsRight( true );
            pPeople->SetXSpeed( 0.007f );
            pPeople->SetPeopleState( GCSquarePeople::WALK_PEOPLE );
            return;
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            if( pPeople->GetPeopleState() != GCSquarePeople::WALK_PEOPLE )
            {
                pPeople->SetFrame( 0 );
            }
            pPeople->SetIsRight( false );
            pPeople->SetXSpeed( -0.007f );
            pPeople->SetPeopleState( GCSquarePeople::WALK_PEOPLE );
            return;
        }
        else
        {
            if( pPeople->GetPeopleState() == GCSquarePeople::WALK_PEOPLE )
            {
                pPeople->SetFrame( 0 );
            }
            pPeople->SetPeopleState( GCSquarePeople::WAIT_PEOPLE );
            pPeople->SetXSpeed( 0.0f );
        }
        return;
    }
    GCSquarePeopleCommon::OnLoop( pPeople );
}

void GCSquarePeopleElesis::OnWalk( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        OnWait( pPeople, bKeyProc );
        return;
    }
    GCSquarePeopleCommon::OnLoop( pPeople );
}

void GCSquarePeopleElesis::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPeople->SetIsRight( true );
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPeople->SetIsRight( false );
        }
        return;
    }
    GCSquarePeopleCommon::OnLoop( pPeople );
}

void GCSquarePeopleElesis::OnJumpRemote( GCSquarePeople* pPeople )
{
    GCSquarePeopleCommon::OnRemoteProcess( pPeople );
    GCSquarePeopleCommon::OnLoop( pPeople );
}

void GCSquarePeopleElesis::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

	if( pPeople->GetFrame() == pPeople->GetMotionPtr()->GetNum_Frame() - 1 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleElesis::OnDash( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Up && pPeople->GetContact() )
        {
            g_MyD3D->MyCtrl->k_Up = false;
            pPeople->DashJump();
            return;
        }
    }

    if( pPeople->GetFrame() == 2 )
    {
        g_KDSound.Play( (DWORD)4, 0 );
    }
    else if( pPeople->GetFrame() == 3 )
    {
        pPeople->EffectDust();
    }
    // Run??

    GCSquarePeopleCommon::OnFrameEndState( pPeople, GCSquarePeople::WAIT_PEOPLE );
}

void GCSquarePeopleElesis::OnDashRemote( GCSquarePeople* pPeople )
{
    if( pPeople->GetOldPeopleState() != GCSquarePeople::DASH_PEOPLE )
    {
        if( pPeople->GetFrame() == 0 )
        {
            if( pPeople->GetIsRight() )
            {
                pPeople->SetXSpeed( 0.015f );
            }
            else
    {
                pPeople->SetXSpeed( -0.015f );
            }
        }
    }

    if( pPeople->GetFrame() == 2 )
    {
        g_KDSound.Play( (DWORD)4, 0 );
    }
    else if( pPeople->GetFrame() == 3 )
    {
        pPeople->EffectDust();
    }
    // Run??

    GCSquarePeopleCommon::OnFrameEndState( pPeople, GCSquarePeople::WAIT_PEOPLE );
}

void GCSquarePeopleElesis2::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 5 ||
        pPeople->GetFrame() == 6 ||
        pPeople->GetFrame() == 7 ||
        pPeople->GetFrame() == 8 ||
        pPeople->GetFrame() == 9 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleElesis3::OnJumpRemote( GCSquarePeople* pPeople )
{
    GCSquarePeopleCommon::OnRemoteProcess( pPeople );
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 25 );
}

void GCSquarePeopleElesis3::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPeople->SetIsRight( true );
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPeople->SetIsRight( false );
        }
        return;
    }
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 25 );
}

void GCSquarePeopleElesis4::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 10 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleLire3::OnJumpRemote( GCSquarePeople* pPeople )
{
    GCSquarePeopleCommon::OnRemoteProcess( pPeople );
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 21 );
}

void GCSquarePeopleLire3::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPeople->SetIsRight( true );
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPeople->SetIsRight( false );
        }
        return;
    }
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 21 );
}

void GCSquarePeopleLire4::OnJumpRemote( GCSquarePeople* pPeople )
{
    GCSquarePeopleCommon::OnRemoteProcess( pPeople );
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 20 );
}

void GCSquarePeopleLire4::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPeople->SetIsRight( true );
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPeople->SetIsRight( false );
        }
        return;
    }
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 20 );
}

void GCSquarePeopleArme::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 4 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleArme::OnWait( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return GCSquarePeopleElesis::OnWait( pPeople, true );
    }
    GCSquarePeopleCommon::OnWaitStop( pPeople, 49 );
}

void GCSquarePeopleArme::OnWaitRemote( GCSquarePeople* pPeople )
{
    pPeople->SetXSpeed( 0.0f );
    GCSquarePeopleCommon::OnWaitStop( pPeople, 49 );
}

void GCSquarePeopleArme2::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 14 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleArme2::OnWait( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return GCSquarePeopleElesis::OnWait( pPeople, true );
    }
    GCSquarePeopleCommon::OnWaitLoop( pPeople, 128 );
}

void GCSquarePeopleArme2::OnWaitRemote( GCSquarePeople* pPeople )
{
    pPeople->SetXSpeed( 0.0f );
    GCSquarePeopleCommon::OnWaitLoop( pPeople, 128 );
}

void GCSquarePeopleArme3::OnJumpRemote( GCSquarePeople* pPeople )
{
    GCSquarePeopleCommon::OnRemoteProcess( pPeople );
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 16 );
}

void GCSquarePeopleArme3::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPeople->SetIsRight( true );
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPeople->SetIsRight( false );
        }
        return;
    }
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 16 );
}

void GCSquarePeopleArme4::OnJumpRemote( GCSquarePeople* pPeople )
{
    GCSquarePeopleCommon::OnRemoteProcess( pPeople );
    if( pPeople->GetFrame() >= pPeople->GetMotionPtr()->GetNum_Frame() - 1 )
    {
        pPeople->SetFrame( pPeople->GetMotionPtr()->GetNum_Frame() - 2 );
    }
}

void GCSquarePeopleArme4::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPeople->SetIsRight( true );
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPeople->SetIsRight( false );
        }
        return;
    }
    if( pPeople->GetFrame() >= pPeople->GetMotionPtr()->GetNum_Frame() - 1 )
    {
        pPeople->SetFrame( pPeople->GetMotionPtr()->GetNum_Frame() - 2 );
    }
}

void GCSquarePeopleArme4::OnWait( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return GCSquarePeopleElesis::OnWait( pPeople, true );
    }
    GCSquarePeopleCommon::OnWaitLoop( pPeople, 69 );
}

void GCSquarePeopleArme4::OnWaitRemote( GCSquarePeople* pPeople )
{
    pPeople->SetXSpeed( 0.0f );
    GCSquarePeopleCommon::OnWaitLoop( pPeople, 69 );
}

void GCSquarePeopleLas::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 6 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleLas2::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 4 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleLas3::OnJumpRemote( GCSquarePeople* pPeople )
{
    GCSquarePeopleCommon::OnRemoteProcess( pPeople );
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 24 );
}

void GCSquarePeopleLas3::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        if( g_MyD3D->MyCtrl->k_Right )
        {
            pPeople->SetIsRight( true );
        }
        else if( g_MyD3D->MyCtrl->k_Left )
        {
            pPeople->SetIsRight( false );
        }
        return;
    }
    GCSquarePeopleCommon::OnJumpLoop( pPeople, 24 );
}

void GCSquarePeopleRyan2::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 8 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleRyan3::OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc )
    {
        return;
    }

    if( pPeople->GetFrame() == 7 )
    {
        pPeople->Jump();
    }
}

void GCSquarePeopleLey1::OnDash( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( bKeyProc && pPeople->GetFrame() > 25)
    {
        if( g_MyD3D->MyCtrl->k_Up && pPeople->GetContact() )
        {
            g_MyD3D->MyCtrl->k_Up = false;
            pPeople->DashJump();
            return;
        }
    }

    if( 16 < pPeople->GetFrame() && pPeople->GetFrame() < 32 )
    {
        if( pPeople->GetIsRight() ) pPeople->SetXSpeed( 0.05f );
        else                        pPeople->SetXSpeed( -0.05f );
    }
    else                            pPeople->SetXSpeed( 0.0f );

    if( pPeople->GetFrame() == 0 )
    {
        pPeople->AddTraceParticle( "Rey_Tel_in_01", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_in_02", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_out", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_in_05", 0.0f, 0.0f );   
    }
    else if( pPeople->GetFrame() == 30 )
    {
        pPeople->AddTraceParticle( "Rey_Tel_in_01_R", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_in_05", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_out", 0.1f, 0.0f );
    }

    GCSquarePeopleCommon::OnFrameEndState( pPeople, GCSquarePeople::WAIT_PEOPLE );
}

void GCSquarePeopleLey1::OnDashRemote( GCSquarePeople* pPeople )
{
    if( 16 < pPeople->GetFrame() && pPeople->GetFrame() < 32 )
    {
        if( pPeople->GetIsRight() ) pPeople->SetXSpeed( 0.05f );
        else                        pPeople->SetXSpeed( -0.05f );
    }
    else                            pPeople->SetXSpeed( 0.0f );

    if( pPeople->GetFrame() == 0 )
    {
        pPeople->AddTraceParticle( "Rey_Tel_in_01", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_in_02", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_out", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_in_05", 0.0f, 0.0f );   
    }
    else if( pPeople->GetFrame() == 30 )
    {
        pPeople->AddTraceParticle( "Rey_Tel_in_01_R", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_in_05", 0.0f, 0.0f );
        pPeople->AddTraceParticle( "Rey_Tel_out", 0.1f, 0.0f );
    }

    GCSquarePeopleCommon::OnFrameEndState( pPeople, GCSquarePeople::WAIT_PEOPLE );
}

//void GCSquarePeopleLey1::OnJumpRemote( GCSquarePeople* pPeople )
//{
//    if( pPeople->GetOldPeopleState() != GCSquarePeople::JUMP_PEOPLE )
//    {
//        if( pPeople->GetOldPeopleState() == GCSquarePeople::DASH_PEOPLE )
//        {
//            OnEndDash( pPeople, 0.5f );
//            pPeople->Jump();
//        }
//        else if( pPeople->GetOldPeopleState() == GCSquarePeople::JUMP_READY_PEOPLE )
//        {
//            pPeople->Jump();
//        }
//        else
//        {
//            pPeople->DownJump();
//        }
//    }
//
//    GCSquarePeopleCommon::OnLoop( pPeople );
//}