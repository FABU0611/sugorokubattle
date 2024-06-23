//trout_red.h	�G���o��}�X
//20106_�c���@�@
//23_12_12
#pragma once

#include "trout.h"
#include "enemy_manager.h"

class TROUT_RED : public TROUT{
	ENEMY_MANAGER*			_em;

public:
	TROUT_RED() = default;
	TROUT_RED(D3DXVECTOR3 pos) :
		TROUT(pos, TYPE_RED, 0) {}
	~TROUT_RED() { delete _em; }

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Event()override;
};