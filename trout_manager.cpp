//trout_manager.cpp	マスマネージャー処理
//20106_田中　蓮
//23_12_12
#include "trout_manager.h"
#include "piece.h"

//更新処理
void TROUT_MANAGER::UpdateTroutManager() {
	TroutEvent();
	for (auto t : _trouts) {
		if (!t->GetEvent() || t->GetEventEnd()) {
			continue;
		}
		t->Update();
	}
	SetNextPosPiece();
}

//次に進むマスを入れる
void TROUT_MANAGER::SetNextPosPiece() {
	//通ってないマスを探す
	auto it = std::find_if(_trouts.begin(), _trouts.end(),
		[](const TROUT* t) {
			return !t->GetStep();
		}
	);
	//見つからなかったら何もしない
	if (it == _trouts.end()) {
		return;
	}
	//最後のマスならそれ以上進めないようにする
	if ((*it)->GetType() == TYPE_END) {
		SetMovenum(1);
	}

	//止まったことがあるなら次に進む
	if ((*it)->GetStep()) {
		it++;
	}
	else {
		//進む先の位置を入れる
		SetNextPos((*it)->GetPos());
	}
	//止まったことあるマスにする
	if (GetNowPos() == (*it)->GetPos()) {
		(*it)->SetStep(true);
	}
}

//コマが止まっているマスのイベントを実行
void TROUT_MANAGER::TroutEvent() {
	//コマが動ける状態なら何もしない
	if (GetCanMove()) {
		return;
	}

	//コマの位置をもらう
	D3DXVECTOR3 pos = GetNowPos();

	//イベントを発生させていないマスを見つける
	auto it = std::find_if(_trouts.begin(), _trouts.end(),
		[pos](const TROUT* t) {
			return !t->GetEvent() && t->GetPos().x == pos.x && t->GetPos().z == pos.z;
		}
	);
	//見つからなかったら何もしない
	if (it == _trouts.end()) {
		return;
	}

	(*it)->SetEvent(true);
	(*it)->Event();
}

//コマが止まっているマスのイベントが終わっているか
bool TROUT_MANAGER::TroutEndEvent(){
	//コマの位置をもらう
	D3DXVECTOR3 pos = GetNowPos();

	//イベントを終わったマスを見つける
	auto it = std::find_if(_trouts.begin(), _trouts.end(),
		[pos](const TROUT* t) {
			return t->GetEventEnd() && t->GetPos().x == pos.x && t->GetPos().z == pos.z;
		}
	);
	//見つからなかったら何もしない
	if (it == _trouts.end()) {
		return false;
	}

	//見つかったマスのイベントが終わったかを返す
	return (*it)->GetEventEnd();
}
