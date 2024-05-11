//trout_blue.h	�R�C�����o��}�X
//20106_�c���@�@
//23_12_14
#pragma once

#include "trout.h"

class TROUT_BLUE : public TROUT{
	static const int	END_FRAME = 300;		//�R�C����������܂ł̎���
	float	_frame = 0.0f;						//�t���[��

public:
	TROUT_BLUE() = default;
	TROUT_BLUE(D3DXVECTOR3 pos) :
		TROUT(pos, TYPE_BLUE) {}
	~TROUT_BLUE() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Event()override;
};