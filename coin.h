//coin.h	コイン
//20106_田中　蓮
//23_12_27
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const int NUM_COIN = 5;		//最大コイン枚数

struct COIN
{
	D3DXVECTOR3		pos;		// ポリゴンの位置
	D3DXVECTOR3		spos;		// 出現の始まり位置
	D3DXVECTOR3		epos;		// 出現の終わり位置
	D3DXVECTOR3		polylinepos;// ポリラインの位置
	D3DXVECTOR3		scl;		// ポリゴンの大きさ(スケール)

	D3DXMATRIX		mtxWorld;	// ワールドマトリックス

	bool			isUse;		//使っているか
	float			frame;		//フレーム

	bool			lerp_end;	//線形補間が終わったか

	int				polylineID;	//ポリラインID
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCoin(void);
void UninitCoin(void);
void UpdateCoin(void);
void DrawCoin(void);


void SetCoin(void);

D3DXVECTOR3 GetCoinPos(const int& idx);
float GetCoinRad(void);
bool GetCoinUse(const int& idx);
void EraseCoin(const int& idx);
void EraseCoinAll(void);