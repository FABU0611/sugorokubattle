//lcoin_screen.cpp		�R�C�������\������
//20106_�c���@�@
//23_07_06

#include "coin_screen.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float NUM_WIDTH = 100.0f;
static const float NUM_HEIGHT = 150.0f;

const static int ANIME_PTN_YOKO = 10;
const static int ANIME_PTN_TATE = 1;
const static int ANIME_PTN_WAIT = 5;

const static float ANIME_PTN = ANIME_PTN_YOKO * ANIME_PTN_TATE;
const static float ANIME_PTN_U = 1.0f / ANIME_PTN_YOKO;
const static float ANIME_PTN_V = 1.0f / ANIME_PTN_TATE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int			g_TexNo = 0;		//�e�N�X�`���ԍ�
static int			g_coinnum = 0;		//�E�����R�C���̖���

//=============================================================================
// ����������
//=============================================================================
void InitCoinscreen(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/number001.png");

	//�E�����R�C���̖��������炤
	g_coinnum =	GetPlayerCoinNum();
}

//=============================================================================
// �I������
//=============================================================================
void UninitCoinscreen(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCoinscreen(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCoinscreen(void)
{
	int coinnum = g_coinnum;
	//�\��
	for (int i = 0; i < 2; i++)	{
		//�X�R�A�̕`��
		DrawSprite(g_TexNo,
			1500.0f - (NUM_WIDTH * i), 550.0f,
			NUM_WIDTH, NUM_HEIGHT,
			ANIME_PTN_U * (coinnum % 10), 0.0f,//UV�l�̎n�_
			ANIME_PTN_U, ANIME_PTN_V);

		coinnum = coinnum / 10;
	}
}
