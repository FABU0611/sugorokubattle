//life.cpp		���C�t�\������
//20106_�c���@�@
//23_07_06

#include "life.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float LIFE_WIDTH = 50.0f;				//���̉���
static const float LIFE_HEIGHT = 100.0f;			//���̏c��

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
static int			g_TexNo[2];		//�e�N�X�`���ԍ�
static int			g_Life = 0;			//�I�����̃��C�t

//=============================================================================
// ����������
//=============================================================================
void InitLife(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo[0] = LoadTexture((char*)"data/TEXTURE/number001.png");
	g_TexNo[1] = LoadTexture((char*)"data/TEXTURE/life.png");

	//�I�����̃��C�t�����炤
	g_Life = GetPlayerLife();
}

//=============================================================================
// �I������
//=============================================================================
void UninitLife(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife(void)
{
	//�����X�V
	g_Life = GetPlayerLife();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife(void)
{
	int life = g_Life;
	//�\��
	for (int i = 0; i < 2; i++)	{
		//�X�R�A�̕`��
		DrawSprite(g_TexNo[0],
			200.0f - (LIFE_WIDTH * i), 70.0f,
			LIFE_WIDTH, LIFE_HEIGHT,
			ANIME_PTN_U * (life % 10), 0.0f,//UV�l�̎n�_
			ANIME_PTN_U, ANIME_PTN_V);

		life = life / 10;
	}
	DrawSprite(g_TexNo[1],
		80.0f, 70.0f,
		100.0f, LIFE_HEIGHT,
		0.0f, 0.0f,//UV�l�̎n�_
		1.0f, 1.0f);
}
