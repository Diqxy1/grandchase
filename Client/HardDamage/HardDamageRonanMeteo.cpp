#include "stdafx.h"
#include ".\HardDamageRonanMeteo.h"
#include "Stage.h"

#include "Controls.h"
#include "KGCRoomManager.h"
#include "DamageManager.h"
#include "LTPacket.h"
#include "LatencyTransfer.h"

CHardDamageRonanMeteo::CHardDamageRonanMeteo(void)
{
}

CHardDamageRonanMeteo::~CHardDamageRonanMeteo(void)
{
}

void CHardDamageRonanMeteo::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );

    if( pDamageInstance->m_What == DT_RONAN_MAGIC_SPECIAL2_READY )
    {
        if( pDamageInstance->m_X < g_MyD3D->MyStg->GetXLimit() &&
            pDamageInstance->m_X > 0.0f )
        {
            if( SiKGCRoomManager()->IsMonsterGameMode() )
            {
                KGCContectInfo kInfo( pDamageInstance->m_X, pDamageInstance->m_Y, 0, 0, 0, false );
                if( true == g_MyD3D->MyStg->Check_Contact( kInfo ) )
                {
                    pDamageInstance->m_Y = kInfo.m_y;

                }
                else
                {
                    D3DXVECTOR2 vPos = g_MyD3D->MyStg->Check_SafePostionX( pDamageInstance->m_X, pDamageInstance->m_Y );
                    pDamageInstance->m_X = vPos.x;
                    pDamageInstance->m_Y = vPos.y;
                }
            }
            else
            {
                D3DXVECTOR3 staticV;
                staticV = g_MyD3D->MyStg->Check_Contact(pDamageInstance->m_X, pDamageInstance->m_Y, 0 );
                if( staticV.y != -1.0f )
                {
                    pDamageInstance->m_Y = staticV.y;
                }
                else
                {
                    D3DXVECTOR2 vPos = g_MyD3D->MyStg->Check_SafePostionX( pDamageInstance->m_X, pDamageInstance->m_Y );
                    pDamageInstance->m_X = vPos.x;
                    pDamageInstance->m_Y = vPos.y + 0.12f;
                }
            }
        }
        else
        {
            pDamageInstance->m_Life = 1;
        }
    }
}

void CHardDamageRonanMeteo::BeginWithLocate( CDamageInstance* pDamageInstance, float fStartX , float fStartY )
{
    CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

    if( pDamageInstance->m_What == DT_RONAN_MAGIC_SPECIAL2_READY )
    {
        if( fStartX < g_MyD3D->MyStg->GetXLimit() &&
            fStartX > 0.0f )
        {
            pDamageInstance->m_X = fStartX;
            pDamageInstance->m_Y = fStartY;
        }
        else
        {
            pDamageInstance->m_Life = 1;
        }
    }
}

CHardDamageRonanMeteorLv3Rune::CHardDamageRonanMeteorLv3Rune(void)
{
}

CHardDamageRonanMeteorLv3Rune::~CHardDamageRonanMeteorLv3Rune(void)
{
}

void CHardDamageRonanMeteorLv3Rune::Begin( CDamageInstance* pDamageInstance )
{
    CDamage::Begin( pDamageInstance );

    if( pDamageInstance->m_X < g_MyD3D->MyStg->GetXLimit() &&
        pDamageInstance->m_X > 0.0f )
    {
        if( SiKGCRoomManager()->IsMonsterGameMode() )
        {
            KGCContectInfo kInfo( pDamageInstance->m_X, pDamageInstance->m_Y, 0, 0, 0, false );
            if( true == g_MyD3D->MyStg->Check_Contact( kInfo ) )
            {
                pDamageInstance->m_Y = kInfo.m_y;
                
            }
            else
            {
                D3DXVECTOR2 vPos = g_MyD3D->MyStg->Check_SafePostionX( pDamageInstance->m_X, pDamageInstance->m_Y );
                pDamageInstance->m_X = vPos.x;
                pDamageInstance->m_Y = vPos.y;
            }
        }
        else
        {
            D3DXVECTOR3 staticV;
            int yPos;
            staticV = g_MyD3D->MyStg->Check_Contact(pDamageInstance->m_X, pDamageInstance->m_Y, 0, false, NULL, &yPos );
            pDamageInstance->m_Y = yPos / 20.0f;
            //if( staticV.y != -1.0f )
            //{
            //    pDamageInstance->m_Y = staticV.y;
            //}
            //else
            //{
            //    D3DXVECTOR2 vPos = g_MyD3D->MyStg->Check_SafePostionX( pDamageInstance->m_X, pDamageInstance->m_Y );
            //    pDamageInstance->m_X = vPos.x;
            //    pDamageInstance->m_Y = vPos.y + 0.1;
            //}
        }
    }
    else
    {
        pDamageInstance->m_Life = 1;
    }

}

void CHardDamageRonanMeteorLv3Rune::BeginWithLocate( CDamageInstance* pDamageInstance, float fStartX , float fStartY )
{
    CDamage::BeginWithLocate( pDamageInstance, fStartX, fStartY );

    if( fStartX < g_MyD3D->MyStg->GetXLimit() &&
        fStartX > 0.0f )
    {
        pDamageInstance->m_X = fStartX;
        pDamageInstance->m_Y = fStartY;
    }
    else
    {
        pDamageInstance->m_Life = 1;
    }

}

void CHardDamageRonanMeteorLv3Rune::NotAngleFrameMove( CDamageInstance* pDamageInstance )
{
	
    CDamage::NotAngleFrameMove( pDamageInstance );

    if( pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() )
    {
        if( g_MyD3D->MyCtrl->k_Left )
            pDamageInstance->m_X -= 0.015f;
        else if( g_MyD3D->MyCtrl->k_Right )
            pDamageInstance->m_X += 0.015f;

        if( g_MyD3D->GetMyPlayer()->bIsRight )
        {
            if( pDamageInstance->m_X < g_MyD3D->GetMyPlayer()->vPos.x + 0.5f)
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x + 0.5f;
            }
            else if( pDamageInstance->m_X > g_MyD3D->GetMyPlayer()->vPos.x + 1.2f )
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x + 1.2f;
            }
        }
        else
        {
            if( pDamageInstance->m_X > g_MyD3D->GetMyPlayer()->vPos.x - 0.5f)
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x - 0.5f;
            }
            else if( pDamageInstance->m_X < g_MyD3D->GetMyPlayer()->vPos.x - 1.2f )
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x - 1.2f;
            }

        }

        

        if( pDamageInstance->m_Life == 83 || pDamageInstance->m_Life == 23 || pDamageInstance->m_Life == 53 )
        {
            LTP_DAMAGE packet;
            packet.bAbsoluePos = true;
            packet.iCharIndex = pDamageInstance->m_Who;
            packet.iDamage = DT_RONAN_METEO_LV3;
            packet.fx = pDamageInstance->m_X;
            packet.fy = pDamageInstance->m_Y;

            packet.bMonsterDamage = false;

            if( !g_MyD3D->GetMyPlayer()->bIsRight )
                packet.fx = -packet.fx;

            g_LatencyTransfer.PushPacket( &packet );
        }
    }
}

void CHardDamageRonanEnergyRune::NotAngleFrameMove( CDamageInstance* pDamageInstance )
{
    CDamage::NotAngleFrameMove( pDamageInstance );

    if( pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() )
    {
        if( g_MyD3D->MyCtrl->k_Left )
            pDamageInstance->m_X -= 0.015f;
        else if( g_MyD3D->MyCtrl->k_Right )
            pDamageInstance->m_X += 0.015f;

        if( g_MyD3D->GetMyPlayer()->bIsRight )
        {
            if( pDamageInstance->m_X < g_MyD3D->GetMyPlayer()->vPos.x + 0.5f)
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x + 0.5f;
            }
            else if( pDamageInstance->m_X > g_MyD3D->GetMyPlayer()->vPos.x + 1.2f )
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x + 1.2f;
            }
        }
        else
        {
            if( pDamageInstance->m_X > g_MyD3D->GetMyPlayer()->vPos.x - 0.5f)
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x - 0.5f;
            }
            else if( pDamageInstance->m_X < g_MyD3D->GetMyPlayer()->vPos.x - 1.2f )
            {
                pDamageInstance->m_X = g_MyD3D->GetMyPlayer()->vPos.x - 1.2f;
            }
        }

        if (pDamageInstance->m_Life == 1)
        {
            if (SiKGCRoomManager()->IsMonsterGameMode()) {

                LTP_DAMAGE packet;
                packet.bAbsoluePos = true;
                packet.iCharIndex = pDamageInstance->m_Who;
                packet.iDamage = DT_RONAN_ENERGY_CIRCLE_DUN;
                packet.fx = pDamageInstance->m_X;
                packet.fy = pDamageInstance->m_Y;

                packet.bMonsterDamage = false;

                if (!g_MyD3D->GetMyPlayer()->bIsRight)
                    packet.fx = -packet.fx;

                g_LatencyTransfer.PushPacket(&packet);

            }
            else {

                LTP_DAMAGE packet;
                packet.bAbsoluePos = true;
                packet.iCharIndex = pDamageInstance->m_Who;
                packet.iDamage = DT_RONAN_ENERGY_CIRCLE;
                packet.fx = pDamageInstance->m_X;
                packet.fy = pDamageInstance->m_Y;

                packet.bMonsterDamage = false;

                if (!g_MyD3D->GetMyPlayer()->bIsRight)
                    packet.fx = -packet.fx;

                g_LatencyTransfer.PushPacket(&packet);
            }
        }
    }
}

