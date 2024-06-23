//trout_red.cpp	�G���o��}�X
//20106_�c���@�@
//23_12_08
#include "trout_red.h"
#include "texture.h"
#include "renderer.h"
#include "sound.h"
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// ����������
//=============================================================================
void TROUT_RED::Init(void){
	//�����ǂݍ���
	SetSound(LoadSound((char*)"data/sound/enemy_pop.wav"));
	SetVolume(GetSoundNo(), 0.3f);

	TROUT::Init();

	_em = new ENEMY_MANAGER();
}

//=============================================================================
// �I������
//=============================================================================
void TROUT_RED::Uninit(void)
{
	if (_em != nullptr) {
		_em->UninitEnemyManager();
	}
	TROUT::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void TROUT_RED::Update(void)
{
	_em->UpdateEnemyManager();

	if (_em->GetSize() == 0) {
		_eventend = true;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void TROUT_RED::Draw(void)
{
	TROUT::Draw();

	_em->DrawEnemyManager();
}

//�C�x���g
void TROUT_RED::Event(){
	//�����Đ�
	PlaySound(GetSoundNo(), 0);
	
	//�C�x���g�I����Ԃɂ���
	_eventend = false;

	//�G���o��������
	_em->Add(new ENEMY({ 0.0f, 0.0f, 300.0f }, true));
	_em->Add(new ENEMY({ 0.0f, 0.0f, -300.0f }, true));
	_em->Add(new ENEMY({ 300.0f, 0.0f, 0.0f }, true));
	_em->InitEnemyManager();
}

