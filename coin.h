//coin.h	�R�C��
//20106_�c���@�@
//23_12_27
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const int NUM_COIN = 5;		//�ő�R�C������

struct COIN
{
	D3DXVECTOR3		pos;		// �|���S���̈ʒu
	D3DXVECTOR3		spos;		// �o���̎n�܂�ʒu
	D3DXVECTOR3		epos;		// �o���̏I���ʒu
	D3DXVECTOR3		polylinepos;// �|�����C���̈ʒu
	D3DXVECTOR3		scl;		// �|���S���̑傫��(�X�P�[��)

	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X

	bool			isUse;		//�g���Ă��邩
	float			frame;		//�t���[��

	bool			lerp_end;	//���`��Ԃ��I�������

	int				polylineID;	//�|�����C��ID
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCoin(void);
void UninitCoin(void);
void UpdateCoin(void);
void DrawCoin(void);


void SetCoin(void);

D3DXVECTOR3 GetCoinPos(const int& idx);
float GetCoinRad(void);
bool GetCoinUse(const int& idx);
void EraseCoin(const int& idx);
void EraseCoinAll(void);