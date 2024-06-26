//collision.cpp			当たり判定処理
//20106_田中　蓮
//23_11_21
#include "collision.h"

//四角
bool CheckHitAABB(D3DXVECTOR3 b1_pos, D3DXVECTOR3 b1_size,
	D3DXVECTOR3 b2_pos, D3DXVECTOR3 b2_size) {
	float boxa_left = b1_pos.x - (b1_size.x * 0.5f);
	float boxa_right = b1_pos.x + (b1_size.x * 0.5f);
	float boxa_bottom = b1_pos.y - (b1_size.y * 0.5f);
	float boxa_top = b1_pos.y + (b1_size.y * 0.5f);
	float boxa_front = b1_pos.z - (b1_size.z * 0.5f);
	float boxa_rear = b1_pos.z + (b1_size.z * 0.5f);

	float boxb_left = b2_pos.x - (b2_size.x * 0.5f);
	float boxb_right = b2_pos.x + (b2_size.x * 0.5f);
	float boxb_bottom = b2_pos.y - (b2_size.y * 0.5f);
	float boxb_top = b2_pos.y + (b2_size.y * 0.5f);
	float boxb_front = b2_pos.z - (b2_size.z * 0.5f);
	float boxb_rear = b2_pos.z + (b2_size.z * 0.5f);

	if (boxa_left < boxb_right &&
		boxa_right > boxb_left &&
		boxa_top > boxb_bottom &&
		boxa_bottom < boxb_top &&
		boxa_front < boxb_rear &&
		boxa_rear > boxb_front) {
		// 当たっていたらtrue
		return true;
	}
	//当たっていなかったらfalse
	return false;
}


//球
bool CheckHitBS(const D3DXVECTOR3& s1_pos, const float& s1_r, 
				const D3DXVECTOR3& s2_pos, const float& s2_r) {
	//s1とs2の距離を取得する
	D3DXVECTOR3 dist = s1_pos - s2_pos;
	float length = D3DXVec3Length(&dist);

	//s1のrとs2のrを足す
	float r = s1_r + s2_r;

	//この二つを比較して半径が大きいなら当たっている
	if (length < r) {
		return true;
	}

	return false;
}
//平方根にしないので少し早い
bool CheckHitBSSq(const D3DXVECTOR3& s1_pos, const float& s1_r, 
				const D3DXVECTOR3& s2_pos, const float& s2_r) {
	//s1とs2の距離を取得する
	D3DXVECTOR3 dist = s1_pos - s2_pos;
	float length = D3DXVec3LengthSq(&dist);

	//s1のrとs2のrを足す
	float r = (s1_r + s2_r) * (s1_r + s2_r);

	//この二つを比較して半径が大きいなら当たっている
	if (length < r) {
		return true;
	}

	return false;
}