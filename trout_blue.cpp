//trout_blue.cpp	�R�C�����o��}�X
//20106_�c���@�@
//23_12_14
#include "trout_blue.h"
#include "texture.h"
#include "renderer.h"
#include "coin.h"
#include "sound.h"

//=============================================================================
// ����������
//=============================================================================
void TROUT_BLUE::Init(void){
	//�����ǂݍ���
	SetSound(LoadSound((char*)"data/sound/coin_pop.wav"));
	SetVolume(GetSoundNo(), 0.3f);

	TROUT::Init();

	_frame = 0.0f;
}

//=============================================================================
// �X�V����
//=============================================================================
void TROUT_BLUE::Update(void){
	//�J�E���g�A�b�v����
	_frame += 1.0f;
	//������
	if (_frame > END_FRAME) {
		EraseCoinAll();
		_frame = 0.0f;
		_eventend = true;
	}
}

//�C�x���g
void TROUT_BLUE::Event(){
	//�����Đ�
	PlaySound(GetSoundNo(), 0);

	//�C�x���g�I����Ԃɂ���
	_eventend = false;
	//�R�C�����o��������
	SetCoin();
}
