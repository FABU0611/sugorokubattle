//trout_manager.cpp	�}�X�}�l�[�W���[����
//20106_�c���@�@
//23_12_12
#include "trout_manager.h"
#include "piece.h"

//�X�V����
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

//���ɐi�ރ}�X������
void TROUT_MANAGER::SetNextPosPiece() {
	//�ʂ��ĂȂ��}�X��T��
	auto it = std::find_if(_trouts.begin(), _trouts.end(),
		[](const TROUT* t) {
			return !t->GetStep();
		}
	);
	//������Ȃ������牽�����Ȃ�
	if (it == _trouts.end()) {
		return;
	}
	//�Ō�̃}�X�Ȃ炻��ȏ�i�߂Ȃ��悤�ɂ���
	if ((*it)->GetType() == TYPE_END) {
		SetMovenum(1);
	}

	//�~�܂������Ƃ�����Ȃ玟�ɐi��
	if ((*it)->GetStep()) {
		it++;
	}
	else {
		//�i�ސ�̈ʒu������
		SetNextPos((*it)->GetPos());
	}
	//�~�܂������Ƃ���}�X�ɂ���
	if (GetNowPos() == (*it)->GetPos()) {
		(*it)->SetStep(true);
	}
}

//�R�}���~�܂��Ă���}�X�̃C�x���g�����s
void TROUT_MANAGER::TroutEvent() {
	//�R�}���������ԂȂ牽�����Ȃ�
	if (GetCanMove()) {
		return;
	}

	//�R�}�̈ʒu�����炤
	D3DXVECTOR3 pos = GetNowPos();

	//�C�x���g�𔭐������Ă��Ȃ��}�X��������
	auto it = std::find_if(_trouts.begin(), _trouts.end(),
		[pos](const TROUT* t) {
			return !t->GetEvent() && t->GetPos().x == pos.x && t->GetPos().z == pos.z;
		}
	);
	//������Ȃ������牽�����Ȃ�
	if (it == _trouts.end()) {
		return;
	}

	(*it)->SetEvent(true);
	(*it)->Event();
}

//�R�}���~�܂��Ă���}�X�̃C�x���g���I����Ă��邩
bool TROUT_MANAGER::TroutEndEvent(){
	//�R�}�̈ʒu�����炤
	D3DXVECTOR3 pos = GetNowPos();

	//�C�x���g���I������}�X��������
	auto it = std::find_if(_trouts.begin(), _trouts.end(),
		[pos](const TROUT* t) {
			return t->GetEventEnd() && t->GetPos().x == pos.x && t->GetPos().z == pos.z;
		}
	);
	//������Ȃ������牽�����Ȃ�
	if (it == _trouts.end()) {
		return false;
	}

	//���������}�X�̃C�x���g���I���������Ԃ�
	return (*it)->GetEventEnd();
}
