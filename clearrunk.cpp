//clearrunk.cpp		�N���A�����N�\������
//20106_�c���@
//23_09_21
#include "clearrunk.h"
#include "player.h"
#include "sprite.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float TEX_WIDTH	= 235.0f;	//�e�N�X�`���̉���
static const float TEX_HEIGHT	= 250.0f;	//�e�N�X�`���̏c��

struct SCORE {
	int coin;
	int killed;
	int life;

	int score;
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int g_TexNo;		//�e�N�X�`���ԍ�
static float g_U;		//U�l�̉���
static float g_V;		//V�l�̍���

SCORE g_Score;
//=============================================================================
// ����������
//=============================================================================
void InitClearrunk(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/clearrunk.png");

	g_U = 0.0f;
	g_V = 0.0f;

	//�Q�[���I�����̃v���C���[�������炤
	g_Score.coin = GetPlayerCoinNum();
	g_Score.killed = GetPlayerkilledNum();
	g_Score.life = GetPlayerLife();

	g_Score.score = 0;
}

//=============================================================================
// �I������
//=============================================================================
void UninitClearrunk(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateClearrunk(void)
{
	//��񂩂�X�R�A�v�Z
	g_Score.score = (g_Score.coin) + (g_Score.killed * 0.5f) + (g_Score.life);

	//�X�R�A����\�����郉���N��ݒ�
	//S
	if (g_Score.score > 20) {
		g_U = (1.0f / 3) * 0;
	}
	//A
	else if (g_Score.score > 10) {
		g_U = (1.0f / 3) * 1;
	}
	//B
	else {
		g_U = (1.0f / 3) * 2;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawClearrunk(void)
{
	//�����Ȃ���
	SetSamplerState(FILTER_MODE_POINT, ADDRESS_MODE_WRAP);
	//�w�i�̕`��
	DrawSprite(g_TexNo,
		1300.0f, 825.0f,
		TEX_WIDTH, TEX_HEIGHT,
		g_U, g_V,
		(1.0f / 3), 1.0f);
}