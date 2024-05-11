//collision.cpp			�����蔻�菈��
//20106_�c���@�@
//23_11_21
#include "collision.h"

//�l�p
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
		// �������Ă�����true
		return true;
	}
	//�������Ă��Ȃ�������false
	return false;
}


//��
bool CheckHitBS(const D3DXVECTOR3& s1_pos, const float& s1_r, 
				const D3DXVECTOR3& s2_pos, const float& s2_r) {
	//s1��s2�̋������擾����
	D3DXVECTOR3 dist = s1_pos - s2_pos;
	float length = D3DXVec3Length(&dist);

	//s1��r��s2��r�𑫂�
	float r = s1_r + s2_r;

	//���̓���r���Ĕ��a���傫���Ȃ瓖�����Ă���
	if (length < r) {
		return true;
	}

	return false;
}
//�������ɂ��Ȃ��̂ŏ�������
bool CheckHitBSSq(const D3DXVECTOR3& s1_pos, const float& s1_r, 
				const D3DXVECTOR3& s2_pos, const float& s2_r) {
	//s1��s2�̋������擾����
	D3DXVECTOR3 dist = s1_pos - s2_pos;
	float length = D3DXVec3LengthSq(&dist);

	//s1��r��s2��r�𑫂�
	float r = (s1_r + s2_r) * (s1_r + s2_r);

	//���̓���r���Ĕ��a���傫���Ȃ瓖�����Ă���
	if (length < r) {
		return true;
	}

	return false;
}