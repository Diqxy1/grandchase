#include "stdafx.h"
#include ".\gcsquarepeoplecommon.h"

#include "Controls.h"

GCSquarePeopleCommon::GCSquarePeopleCommon(void)
{
}

GCSquarePeopleCommon::~GCSquarePeopleCommon(void)
{
}

void GCSquarePeopleCommon::OnWaitRemote( GCSquarePeople* pPeople )
{
    pPeople->SetXSpeed( 0.0f );
    GCSquarePeopleCommon::OnLoop( pPeople );
}

void GCSquarePeopleCommon::OnWalkRemote( GCSquarePeople* pPeople )
{
    if( pPeople->GetOldPeopleState() == GCSquarePeople::DASH_PEOPLE )
    {
        pPeople->SetXSpeed( 0.0f );
    }
    else
    {
        if( pPeople->GetIsRight() )
        {
            pPeople->SetXSpeed( 0.007f );
        }
        else
        {
            pPeople->SetXSpeed( -0.007f );
        }
    }
    GCSquarePeopleCommon::OnLoop( pPeople );
}

bool GCSquarePeopleCommon::CheckDownJump( GCSquarePeople* pPeople )
{
    if( ( pPeople->GetOldPeopleState() == GCSquarePeople::WAIT_PEOPLE || pPeople->GetOldPeopleState() == GCSquarePeople::WALK_PEOPLE || pPeople->GetOldPeopleState() == GCSquarePeople::DASH_PEOPLE ) 
        && pPeople->GetPeopleState() == GCSquarePeople::JUMP_PEOPLE )
    {
        return true;
    }

    return false;
}

bool GCSquarePeopleCommon::CheckDashJump( GCSquarePeople* pPeople )
{
    if( pPeople->GetOldPeopleState() == GCSquarePeople::DASH_PEOPLE && pPeople->GetPeopleState() == GCSquarePeople::DASH_JUMP_PEOPLE )
    {
        return true;
    }

    return false;
}

void GCSquarePeopleCommon::OnRemoteProcess( GCSquarePeople* pPeople )
{
    if( pPeople->GetOldPeopleState() != GCSquarePeople::JUMP_PEOPLE &&
        pPeople->GetOldPeopleState() != GCSquarePeople::DASH_JUMP_PEOPLE )
    {
        if( GCSquarePeopleCommon::CheckDownJump(pPeople) )
        {
            pPeople->DownJump();
        }
        else if( GCSquarePeopleCommon::CheckDashJump(pPeople) )
        {
            pPeople->DashJump();
        }
        else if( pPeople->GetOldPeopleState() == GCSquarePeople::JUMP_READY_PEOPLE && pPeople->GetPeopleState() == GCSquarePeople::JUMP_PEOPLE )
        {
            pPeople->Jump();
        }
    }
}

void GCSquarePeopleCommon::OnLoop( GCSquarePeople* pPeople )
{
    if( pPeople->GetFrame() >= pPeople->GetMotionPtr()->GetNum_Frame() - 1 )
    {
        pPeople->SetFrame( 0 );
    }
}

void GCSquarePeopleCommon::OnFrameEndState( GCSquarePeople* pPeople, GCSquarePeople::PEOPLE_STATE eState )
{
    if( pPeople->GetFrame() >= pPeople->GetMotionPtr()->GetNum_Frame() - 1 )
    {
        pPeople->SetFrame( 0 );
        pPeople->SetPeopleState( eState );
    }
}

void GCSquarePeopleCommon::OnJump( GCSquarePeople* pPeople, bool bKeyProc )
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
	OnJumpLoop( pPeople, pPeople->GetMotionPtr()->GetNum_Frame()- 1 );
}

void GCSquarePeopleCommon::OnJumpRemote( GCSquarePeople* pPeople )
{
	OnRemoteProcess( pPeople );
	OnJumpLoop( pPeople, pPeople->GetMotionPtr()->GetNum_Frame()- 1 );
}

void GCSquarePeopleCommon::OnJumpLoop( GCSquarePeople* pPeople, int iFrame )
{
	//	상승
    if( pPeople->GetYSpeed() >= 0.0f )
    {
        if( pPeople->GetFrame() > iFrame )
        {
            pPeople->SetFrame(iFrame);
        }
    }
	//	낙하
    else
	{
		if( pPeople->GetFrame() >= iFrame )
		{
			 pPeople->SetFrame(iFrame);
		    pPeople->SetFrameLock(true);
		}
    }
}

void GCSquarePeopleCommon::OnWaitStop( GCSquarePeople* pPeople, int iFrame )
{
    if( pPeople->GetFrame() > iFrame )
    {
        pPeople->SetFrame( iFrame );
    }
}

void GCSquarePeopleCommon::OnWaitLoop( GCSquarePeople* pPeople, int iFrame )
{
    if( pPeople->GetFrame() > iFrame )
    {
        pPeople->SetFrame( 0 );
    }
}

void GCSquarePeopleCommon::OnJumpReadyRemote( GCSquarePeople* pPeople )
{
    if( pPeople->GetOldPeopleState() == GCSquarePeople::WALK_PEOPLE )
    {
        pPeople->SetXSpeed( 0.0f );
    }
}

void GCSquarePeopleCommon::OnSocialMotion( GCSquarePeople* pPeople, bool bKeyProc )
{
    if( pPeople->GetFrame() == 0 ) {
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON", false );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_0", false );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_1", false );
    } else if ( pPeople->GetFrame() >= pPeople->GetMotionPtr()->GetNum_Frame() - 1 ) {
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON", true );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_0", true );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_1", true );
    }

    OnFrameEndState( pPeople, GCSquarePeople::WAIT_PEOPLE );
}

void GCSquarePeopleCommon::OnSocialMotionRemote( GCSquarePeople* pPeople )
{   
    if( pPeople->GetFrame() == 0 ) {
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON", false );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_0", false );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_1", false );
    } else if ( pPeople->GetFrame() >= pPeople->GetMotionPtr()->GetNum_Frame() - 5 ) { //o3 프레임 단위로 끊어 먹어서 5프레임의 여유를 두겠음 
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON", true );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_0", true );
        pPeople->GetGCObject()->ToggleExtraMesh( "WEAPON_1", true );
    }

    OnFrameEndState( pPeople, GCSquarePeople::WAIT_PEOPLE );
}