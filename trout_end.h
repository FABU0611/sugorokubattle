//trout_end.h	�I���̃}�X
//20106_�c���@�@
//23_12_14
#pragma once

#include "trout.h"

class TROUT_END : public TROUT{
public:
	TROUT_END() = default;
	TROUT_END(D3DXVECTOR3 pos) :
		TROUT(pos, TYPE_END) {}
	~TROUT_END() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Event()override;
};