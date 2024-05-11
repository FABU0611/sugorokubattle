//bullet.h	�e�\������
//20106_�c���@�@
//23_12_07
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const int NUM_BULLET = 5;		//�ő�o���b�g��

struct BULLET
{
	D3DXVECTOR3		pos;		// �|���S���̈ʒu
	D3DXVECTOR3		rot;		// �|���S���̌���(��])
	D3DXVECTOR3		scl;		// �|���S���̑傫��(�X�P�[��)

	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X

	bool			isUse;		//�g���Ă��邩
	bool			isShot;		//�������
	int				frame;

	int				shadowID;	//�eID

	float			speed;	//�ړ����x
	float			front;	//�����Ă������
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);


void SetBullet(const D3DXVECTOR3& pos);

D3DXVECTOR3 GetBulletPos(const int& idx);
float GetBulletRad(void);
bool GetBulletUse(const int& idx);
void EraseBullet(const int& idx);