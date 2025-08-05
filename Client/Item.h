#ifndef _ITEM_H_
#define _ITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "GCUTIL/CRC/CRC_32.h"
#include "Packet.h"
#include "ProtectedType.h"

struct Item
{
	// 실제로 애들이 건드리는거는 What 뿐임.
	DWORD                           DropperUID;         // 드랍한 유저의 UID
	char                            DropperCharType;    // 드랍한 유저의 케릭터타입
	KProtectedType< unsigned char > What;
	WORD                            Life;
	KProtectedType<BOOL>		    IsEnable;
	bool                            IsRight;
	float			                x, y;
	float			                x_speed, y_speed;	//아이템 움직임에 물리 적용하기.
	float			                Start_x, Start_y;
	WORD			                Time;
	int				                iMatchGP;
	char			                cItemLord;		    //이 아이템을 먹을 수 있는 사람.
	bool			                bRenderMatchGP;     //숫자 그릴까요?
	float			                fRenderTime;	    //얼마 동안 그릴까요?

	Item::Item()
	{
		What = 0;
		Life = 0;
		IsEnable = FALSE;
		IsRight = true;
		x = 0.0f;
		y = 0.0f;
		x_speed = 0.0f;
		y_speed = 0.0f;
		Start_x = 0.0f;
		Start_y = 0.0f;
		Time = 0;
		iMatchGP = 0;
		cItemLord = -1;
		bRenderMatchGP = false;
		fRenderTime = 0.0f;
		DropperUID = ULONG_MAX;
		DropperUID = ULONG_MAX;
	}
};

class Items
{
private:
	char						Item_Cursor;
	char						Selected_Item;
	KSafeArray<int, GC_GAME_ITEM_NUM> m_aiItemNameID;
	NPC                         TransNPC;
	Item*                       MyItm;

public:
	KSafeArray<SLVERTEX, 4> V_Item;
	KSafeArray<GCDeviceTexture*, GC_GAME_ITEM_NUM> m_pTexItem;
	KSafeArray<SLVERTEX, 4> V_Letter;
	GCDeviceTexture*		m_pTexLetter;

public:
	inline void SetItemCursorPos(char cPos) { Item_Cursor = cPos; }
	inline int GetItemCursorPos(void) const { return Item_Cursor; }
	inline void MoveNextItemCursorPos(int iNumItem)
	{
		++Item_Cursor;
		if (Item_Cursor >= iNumItem) Item_Cursor = 0;
		if (0 == iNumItem) Item_Cursor = -1;
	}
	inline void SetSelectedItem(char cSelect) { Selected_Item = cSelect; }
	inline int GetSelectedItem(void) const { return Selected_Item; }

	const WCHAR* GetItemDesc(int iItem) const;

public:
	Items();
	~Items();

	void InitDevice();
	//void OnResetDevice();
	void DeleteDevice();

	void Add(char, const D3DXVECTOR2&, bool OnlyMe = false, WORD wItemLife = 500, float xspeed = 0.0f, float yspeed = 0.0f);
	void Clear_All();
	void Use_Item(char, char, int iTime = -1, bool bNoChat = false);
	void Through_Item();
	void Frame_Move();
	void Delete_Item();
	void Check_Player();	// Player가 완성되면 구현할 것(플레이어와 아이템간의 충돌 검사)
	void Alpha_Render();
	void Clear_TransNPC();
	void My_Item_Alpha_Render();
	void Through_MatchGP();
	void AddMatchGP(int iGP, const D3DXVECTOR2& vPos, bool OnlyMe, WORD wItemLife, float xspeed, float yspeed, char cItemLord, int nDropperIndex);
	void Num_Render(float x, float y, const TCHAR* Text, DWORD Color, float Width);
	void CatchMatchCoin(int iItemIndex, int iPlayerIndex);
};

#endif // _ITEM_H_
