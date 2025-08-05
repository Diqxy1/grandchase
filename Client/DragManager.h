#pragma once

#include "KNCSingleton.h"

class KGCDragManager
{
    DeclareSingleton( KGCDragManager );

public:
	enum{
        KGDM_DRAG_READY = -1,
        KGDM_DRAG_START = 0,
        KGDM_DRAG_PROGRESS = 1,
        KGDM_DRAG_END = 2,
        KGDM_DRAG_CANCEL =3,

    };

    KGCDragManager(void);
    ~KGCDragManager(void);

public:
    int GetDragState ( ) {return m_iDragState;}
    void InitDragState( );
	void StartDrag( KItem *pItem , float fImageWidth_, float fImageHeght_, boost::function1<int,bool> a_funcProgress, KPetInfo *pPet = NULL);
	void ProgressDrag( );
    void EndDrag( );
    void CancelDrag( );
    void ReadyDrag( );
	void ClearDragState( );
	void Render( );
private:
	void SetDragState ( int iState );
	int m_iDragState;
    KGC2DObject *m_pkImage;
	KItem* m_pItem;

    float m_fImageWidth;
    float m_fImageHeght;

	boost::function1<int,bool> m_funcProgress;
};

DefSingletonInline( KGCDragManager );

