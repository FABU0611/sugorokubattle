//piece.h	���
//20106_�c���@�@
//23_12_07
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
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