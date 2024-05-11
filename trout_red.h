//trout_red.h	敵が出るマス
//20106_田中　蓮
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