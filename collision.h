//collision.h		当たり判定処理
//20106_田中　蓮
//23_11_21
#pragma once

#include "renderer.h"
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool CheckHitBS(const D3DXVECTOR3& s1_pos, const float& s1_r,
	const D3DXVECTOR3& s2_pos, const float& s2_r);
bool CheckHitBSSq(const D3DXVECTOR3& s1_pos, const float& s1_r,
	const D3DXVECTOR3& s2_pos, const float& s2_r);

bool CheckHitAABB(D3DXVECTOR3 b1_pos, D3DXVECTOR3 b1_size,
	D3DXVECTOR3 b2_pos, D3DXVECTOR3 b2_size);