//collision.cpp			“–‚½‚è”»’èˆ—
//20106_“c’†@˜@
//23_11_21
#include "collision.h"

//lŠp
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
		// “–‚½‚Á‚Ä‚¢‚½‚çtrue
		return true;
	}
	//“–‚½‚Á‚Ä‚¢‚È‚©‚Á‚½‚çfalse
	return false;
}


//‹…
bool CheckHitBS(const D3DXVECTOR3& s1_pos, const float& s1_r, 
				const D3DXVECTOR3& s2_pos, const float& s2_r) {
	//s1‚Æs2‚Ì‹——£‚ğæ“¾‚·‚é
	D3DXVECTOR3 dist = s1_pos - s2_pos;
	float length = D3DXVec3Length(&dist);

	//s1‚Ìr‚Æs2‚Ìr‚ğ‘«‚·
	float r = s1_r + s2_r;

	//‚±‚Ì“ñ‚Â‚ğ”äŠr‚µ‚Ä”¼Œa‚ª‘å‚«‚¢‚È‚ç“–‚½‚Á‚Ä‚¢‚é
	if (length < r) {
		return true;
	}

	return false;
}
//•½•ûª‚É‚µ‚È‚¢‚Ì‚Å­‚µ‘‚¢
bool CheckHitBSSq(const D3DXVECTOR3& s1_pos, const float& s1_r, 
				const D3DXVECTOR3& s2_pos, const float& s2_r) {
	//s1‚Æs2‚Ì‹——£‚ğæ“¾‚·‚é
	D3DXVECTOR3 dist = s1_pos - s2_pos;
	float length = D3DXVec3LengthSq(&dist);

	//s1‚Ìr‚Æs2‚Ìr‚ğ‘«‚·
	float r = (s1_r + s2_r) * (s1_r + s2_r);

	//‚±‚Ì“ñ‚Â‚ğ”äŠr‚µ‚Ä”¼Œa‚ª‘å‚«‚¢‚È‚ç“–‚½‚Á‚Ä‚¢‚é
	if (length < r) {
		return true;
	}

	return false;
}