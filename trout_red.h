//trout_red.h	�G���o��}�X
//20106_�c���@�@
//23_12_12
#pragma once

#include "trout.h"

class TROUT_RED : public TROUT{
public:
	TROUT_RED() = default;
	TROUT_RED(D3DXVECTOR3 pos) :
		TROUT(pos, TYPE_RED) {}
	~TROUT_RED() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Event()override;
};