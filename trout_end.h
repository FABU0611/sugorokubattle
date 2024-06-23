//trout_end.h	終わりのマス
//20106_田中　蓮
//23_12_14
#pragma once

#include "trout.h"

class TROUT_END : public TROUT{
public:
	TROUT_END() = default;
	TROUT_END(D3DXVECTOR3 pos) :
		TROUT(pos, TYPE_END, 4) {}
	~TROUT_END() {}

	void Init()override;
	void Uninit()override{ TROUT::Uninit();	}
	void Update()override {}
	void Draw()override{ TROUT::Draw(); }

	void Event()override;
};