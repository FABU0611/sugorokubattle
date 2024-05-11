//dice.h		サイコロ
//20106_田中　蓮 
//23_12_31
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct DICE
{
	D3DXVECTOR3		pos;		// ポリゴンの位置
	D3DXVECTOR3		rot;		// ポリゴンの向き(回転)
	D3DXVECTOR3		scl;		// ポリゴンの大きさ(スケール)
	float			rad;

	D3DXMATRIX		mtxWorld;	// ワールドマトリックス

	float			frame;		//フレーム
	int				deme;		//出目
	bool			stop;		//回っていないか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDice(void);
void UninitDice(void);
void UpdateDice(void);
void DrawDice(void);


void RooltheDice(void);
int GetDeme(void);
void DiceSetStop(const bool& stop);