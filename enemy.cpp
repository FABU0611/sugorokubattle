//enemy.cpp		�G�\������
//20106_�c���@�@
//23_12_18
#include "main.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "bullet.h"
#include "collision.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)			// ��]��

const float ENEMY::SIZE = 5.0f;						//�T�C�Y
const float ENEMY::SPEED = 2.5f;					//�X�s�[�h


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// ����������
//=============================================================================
void ENEMY::Init(void)
{
	//obj���f���̓ǂݍ���
	LoadModel((char*)"data/MODEL/sword.obj", GetEnemyModel());

	//�T�E���h�ǂݍ���
	_sound_no = LoadSound((char*)"data/sound/enemy_dead.wav");
	SetVolume(_sound_no, 0.3f);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	SetEnemyShadowPos({ GetEnemyPos().x ,GetEnemyPos().y - 75.0f, GetEnemyPos().z });
	SetEnmeyRot({ 0.0f, 0.0f, 0.0f });
	SetEnmeyScl({ SIZE, SIZE, SIZE });

	SetEnemySpeed(SPEED);
	SetEnemyFlont(0.0f);
	SetEnemyUse(true);

	SetEnemyShadowID(_shadowpos, 150.0f);
}

//=============================================================================
// �I������
//=============================================================================
void ENEMY::Uninit(void)
{
	//obj���f���̉��
	UnloadModel(GetEnemyModel());
}

//=============================================================================
// �X�V����
//=============================================================================
void ENEMY::Update(void)
{
	if (!_isUse) {
		//�g���Ă��Ȃ����͍̂X�V���Ȃ�
		return;
	}
	//�v���C���[�̕����ɐi��-------------------------------------------------------------
	//�p�x���o��
	SetEnemyFlont(atan2f(GetEnemyPos().z - GetPlayerPos().z, GetEnemyPos().x - GetPlayerPos().x));
	//�p�x������
	SetEnmeyDir({ -cosf(GetEnemyFront()), 0.0f, -sinf(GetEnemyFront()) });
	//�i�ޕ����Ɍ�������
	SetEnmeyRot({ 0.0f, atan2f(-GetEnemyDir().z, GetEnemyDir().x) + (D3DX_PI / 2), 0.0f });


	//���W�X�V
	_pos.x += sinf(GetEnemyRot().y) * GetEnemySpeed();
	_pos.z += cosf(GetEnemyRot().y) * GetEnemySpeed();

	//�e�̈ʒu���X�V
	SetEnemyShadowPos({ GetEnemyPos().x ,GetEnemyPos().y - 75.0f, GetEnemyPos().z });
	//�ʒu�𔽉f������
	SetPositionShadow(GetEnemyShadow(), GetEnemyShadowPos());

	//�e�Ƃ̓����蔻��
	for (int b = 0; b < NUM_BULLET; b++) {
		if (!GetBulletUse(b)) {
			continue;
		}
		if (CheckHitBS(GetEnemyPos(), GetEnemyRad(),
			GetBulletPos(b), GetBulletRad())) {
			EraseBullet(b);
			DeadEnemy();
		}
	}

	//�v���C���[�Ƃ̓����蔻��
	{
		//�����Ă���Ԃ������肷��
		if (!GetPlayerUse()) {
			SetEnemyUse(false);
			return;
		}
		//�������ĂȂ���ԂŔ���
		if (!GetPlayerHit() &&
			CheckHitBS(GetEnemyPos(), GetEnemyRad(),
			GetPlayerPos(), GetPlayerRad())) {
			PlayerLifeDim();
			AddPlayerkilledNum();
			SetPlayerHit(true);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void ENEMY::Draw(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	if (!_isUse) {
		//�g���Ă��Ȃ����͕̂`�悵�Ȃ�
		return;
	}
	// ���[���h�}�g���b�N�X�̏������i�P�ʍs������֐��j
	D3DXMatrixIdentity(GetEnemymtxworld());

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, GetEnemyScl().x, GetEnemyScl().y, GetEnemyScl().z);

	//��1���� ��          ��2����      ��    ��3����
	D3DXMatrixMultiply(GetEnemymtxworld(), GetEnemymtxworld(), &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetEnemyRot().y, GetEnemyRot().x, GetEnemyRot().z);
	D3DXMatrixMultiply(GetEnemymtxworld(), GetEnemymtxworld(), &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, GetEnemyPos().x, GetEnemyPos().y, GetEnemyPos().z);
	D3DXMatrixMultiply(GetEnemymtxworld(), GetEnemymtxworld(), &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(GetEnemymtxworld());

	//obj���f���̕`��
	DrawModel(GetEnemyModel());
}

//�G���|�����
void ENEMY::DeadEnemy(){
	//�|���ꂽ���̉����Đ�
	PlaySound(_sound_no, 0);
	//�g���Ă��Ȃ���Ԃɂ���
	SetEnemyUse(false);
}
