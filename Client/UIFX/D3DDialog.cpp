#include "StdAfx.h"
#include "D3DDialog.h"

IMPLEMENT_CLASSNAME( KD3DDialog );
IMPLEMENT_WND_FACTORY( KD3DDialog );
IMPLEMENT_WND_FACTORY_NAME( KD3DDialog, "gc_kd3ddialog" );

KD3DDialog::KD3DDialog(void)
{
}

KD3DDialog::~KD3DDialog(void)
{
}

void KD3DDialog::CloseDlg()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KD3DDialog::BindToLua()
{
    lua_tinker::class_add<KD3DDialog>( KGCLuabinder::getInstance().GetLuaState(), "KD3DDialog" );
    lua_tinker::class_def<KD3DDialog>( KGCLuabinder::getInstance().GetLuaState(),  "CloseDlg", &KD3DDialog::CloseDlg );
    lua_tinker::class_inh<KD3DDialog,KD3DWnd>( KGCLuabinder::getInstance().GetLuaState() );
}