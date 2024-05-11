//piece.h	駒処理
//20106_田中　蓮
//23_12_07
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPiece(void);
void UninitPiece(void);
void UpdatePiece(void);
void DrawPiece(void);


float GetPieceFront(void);
void SetNextPos(D3DXVECTOR3 pos);
D3DXVECTOR3 GetNowPos(void);
void SetCanMove(const bool& canmove);
bool GetCanMove(void);
void SetMovenum(const int& movenum);
const int& GetMovenum(void);