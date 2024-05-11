//lcoin_screen.cpp		�^�C���\������
//20106_�c���@�@
//23_07_06

#include "second_screen.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float TIME_WIDTH = 100.0f;		//����
static const float TIME_HEIGHT = 150.0f;	//�c��

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

unsigned int		g_second = 0;		//�N���A�^�C��

//=============================================================================
// ����������
//=============================================================================
void InitSecondscreen(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo = LoadTexture((char*)"data/TEXTURE/number001.png");

	//�N���A�^�C�������炤
	g_second = GetEndTime();
}

//=============================================================================
// �I������
//=============================================================================
void UninitSecondscreen(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSecondscreen(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSecondscreen(void)
{
	int second = g_second;
	//�\��
	for (int i = 0; i < 3; i++)	{
		//�X�R�A�̕`��
		DrawSprite(g_TexNo,
			1500.0f - (TIME_WIDTH * i), 400.0f,
			TIME_WIDTH, TIME_HEIGHT,
			ANIME_PTN_U * (second % 10), 0.0f,//UV�l�̎n�_
			ANIME_PTN_U, ANIME_PTN_V);

		second = second / 10;
	}
}
