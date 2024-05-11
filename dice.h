//dice.h		�T�C�R��
//20106_�c���@�@ 
//23_12_31
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct DICE
{
	D3DXVECTOR3		pos;		// �|���S���̈ʒu
	D3DXVECTOR3		rot;		// �|���S���̌���(��])
	D3DXVECTOR3		scl;		// �|���S���̑傫��(�X�P�[��)
	float			rad;

	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X

	float			frame;		//�t���[��
	int				deme;		//�o��
	bool			stop;		//����Ă��Ȃ���
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDice(void);
void UninitDice(void);
void UpdateDice(void);
void DrawDice(void);


void RooltheDice(void);
int GetDeme(void);
void DiceSetStop(const bool& stop);