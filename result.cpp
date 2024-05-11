//result.cpp	���U���g��ʊǗ�
//20106_�c���@
//23_12_05
#include "result.h"
#include "input.h"
#include "renderer.h"
#include "sprite.h"
#include "texture.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#include "coin_screen.h"
#include "second_screen.h"
#include "clearrunk.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int			g_TexNo0 = 0;	//�e�N�X�`���ԍ�
static int			g_TexNo1 = 0;	//�e�N�X�`���ԍ�
static int			g_SoundNo = 0;	//�T�E���h�ԍ�

//=============================================================================
// ����������
//=============================================================================
void InitResult(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_TexNo0 = LoadTexture((char*)"data/TEXTURE/result00.png");	//�w�i
	g_TexNo1 = LoadTexture((char*)"data/TEXTURE/result01.png");	//����

	//�T�E���h�ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/result_score.wav");

	InitCoinscreen();
	InitSecondscreen();
	InitClearrunk();

	SetVolume(g_SoundNo, 0.3f);
	//�T�E���h�Đ�
	PlaySound(g_SoundNo, 0);
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	//���ׂẲ����~�߂�
	StopSoundAll();
	//�\������Ă�����̂����̏I������
	UninitClearrunk();
	UninitSecondscreen();
	UninitCoinscreen();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	//�\������Ă�����̂����̍X�V����
	UpdateSecondscreen();
	UpdateCoinscreen();
	UpdateClearrunk();

	//space�L�[�Ń^�C�g���ɖ߂�
	if (GetKeyboardTrigger(DIK_SPACE) && GetFadeState() == FADE_NONE){
		SetFadeColor(0.0f, 0.0f, 0.0f);
		SceneFadeIn(SCENE_TITLE, 0.03f);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	//�`��
	//�w�i
	DrawSprite(g_TexNo0, SCREEN_CENTER_X, SCREEN_CENTER_Y,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);
	//����
	DrawSprite(g_TexNo1, SCREEN_CENTER_X, SCREEN_CENTER_Y,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);

	//�\������Ă�����̂���
	DrawSecondscreen();
	DrawCoinscreen();
	DrawClearrunk();
}
