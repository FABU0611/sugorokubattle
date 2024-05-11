//trout_white.h	�����N����Ȃ��}�X
//20106_�c���@�@
//23_12_14
#pragma once

#include "trout.h"

class TROUT_WHITE : public TROUT{
public:
	TROUT_WHITE() = default;
	TROUT_WHITE(D3DXVECTOR3 pos) :
		TROUT(pos, TYPE_WHITE) {}
	~TROUT_WHITE() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Event()override;
};