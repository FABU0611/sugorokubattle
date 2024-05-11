//bullet.h	弾表示処理
//20106_田中　蓮
//23_12_07
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const int NUM_BULLET = 5;		//最大バレット数

struct BULLET
{
	D3DXVECTOR3		pos;		// ポリゴンの位置
	D3DXVECTOR3		rot;		// ポリゴンの向き(回転)
	D3DXVECTOR3		scl;		// ポリゴンの大きさ(スケール)

	D3DXMATRIX		mtxWorld;	// ワールドマトリックス

	bool			isUse;		//使っているか
	bool			isShot;		//撃たれ方
	int				frame;

	int				shadowID;	//影ID

	float			speed;	//移動速度
	float			front;	//向いている方向
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);


void SetBullet(const D3DXVECTOR3& pos);

D3DXVECTOR3 GetBulletPos(const int& idx);
float GetBulletRad(void);
bool GetBulletUse(const int& idx);
void EraseBullet(const int& idx);