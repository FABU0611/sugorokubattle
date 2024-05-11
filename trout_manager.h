//trout_manager.h	マスマネージャー
//20106_田中　蓮
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

	//更新
	void UpdateTroutManager();
	void DrawTroutManager() {
		for (auto& t : _trouts) {
			t->Draw();
		}
	}

	void Add(TROUT* trout) {
		_trouts.emplace_back(trout);
	}

	//次に進むマスの位置を渡す
	void SetNextPosPiece();

	//止まったマスのイベントを実行
	void TroutEvent();

	bool TroutEndEvent();
};