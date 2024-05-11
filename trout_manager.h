//trout_manager.h	�}�X�}�l�[�W���[
//20106_�c���@�@
//23_12_12
#pragma once

#include <list>
#include <algorithm>
#include "trout.h"

class TROUT_MANAGER {
	std::list<TROUT*>	_trouts;

public:
	TROUT_MANAGER() {}
	~TROUT_MANAGER() {
		for (auto& t : _trouts) {
			delete t;
		}
	}

	void InitTroutManager() {
		for (auto& t : _trouts) {
			t->Init();
		}
	}

	void UninitTroutManager() {
		for (auto& t : _trouts) {
			t->Uninit();
		}
	}

	//�X�V
	void UpdateTroutManager();
	void DrawTroutManager() {
		for (auto& t : _trouts) {
			t->Draw();
		}
	}

	void Add(TROUT* trout) {
		_trouts.emplace_back(trout);
	}

	//���ɐi�ރ}�X�̈ʒu��n��
	void SetNextPosPiece();

	//�~�܂����}�X�̃C�x���g�����s
	void TroutEvent();

	bool TroutEndEvent();
};