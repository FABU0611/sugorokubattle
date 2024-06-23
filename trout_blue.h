//trout_blue.h	コインが出るマス
//20106_田中　蓮
//23_12_14
#pragma once

#include "trout.h"

class TROUT_BLUE : public TROUT{
	static const int	END_FRAME = 300;		//コインが消えるまでの時間
	float	_frame = 0.0f;						//フレーム

public:
	TROUT_BLUE() = default;
	TROUT_BLUE(D3DXVECTOR3 pos) :
		TROUT(pos, TYPE_BLUE, 1) {}
	~TROUT_BLUE() {}

	void Init()override;
	void Uninit()override { TROUT::Uninit(); }
	void Update()override;
	void Draw()override{ TROUT::Draw(); }

	void Event()override;
};