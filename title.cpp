//title.cpp	�^�C�g����ʊǗ�
//20106_�c���@
//23_06_30
#include "title.h"
#include "renderer.h"
#include "inputx.h"
#include "sprite.h"
#include "texture.h"

#include "logo.h"
#include "pushspace.h"
#include "star.h"

#include "scene.h"
#include "fade.h"

#include "sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
enum TITLE_PHASE {
	TP_INIT,		//������
	TP_BG,			//�w�i�\��
	TP_LOGO,		//�^�C�g�����S�\��
	TP_PUSH,		//PushStart �\��
	TP_LOOP,		 //�^�C�g�����[�v
	TP_INFO,		//�������
	TP_SCENECHANGE, //�V�[���ڍs


	TP_NUM,
};

struct TITLE_IMAGE {
	D3DXVECTOR3	pos;		//�ʒu
	D3DXCOLOR	color;		//�F
	float		flame;		//�t���[��
	int			tex_no;		//�e�N�X�`���ԍ�
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int			g_SoundNo = 0;		//�T�E���h�ԍ�

static TITLE_PHASE	g_Phase = TP_INIT;	//�t�F�[�Y

static TITLE_IMAGE	g_BG;				//�w�i
static TITLE_IMAGE	g_INFO;				//�������
//=============================================================================
// ����������
//=============================================================================
void InitTitle(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_BG.tex_no = LoadTexture((char*)"data/TEXTURE/title00.png");
	g_INFO.tex_no = LoadTexture((char*)"data/TEXTURE/info.png");

	//�T�E���h�̓ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/bgm_title.wav");

	//�T�E���h�Đ�
	SetVolume(g_SoundNo, 0.3f);
	PlaySound(g_SoundNo, -1);

	//�������t�F�[�Y����X�^�[�g����
	g_Phase = TP_INIT;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	//�������ׂĎ~�߂�
	StopSoundAll();
	UninitStar();
	UninitPushspace();
	UninitLogo();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	switch (g_Phase)	{
		//����������
	case TP_INIT:
		g_BG.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BG.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_BG.flame = 0;

		InitLogo();
		InitPushspace();
		InitStar();

		g_Phase = TP_BG;
		break;
		//�w�i�����\��
	case TP_BG:
		//�t�F�[�h����������������
		if (GetFadeState() == FADE_NONE) {
			g_Phase = TP_LOGO;
		}
		if (GetKeyboardTrigger(DIK_RETURN) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_LOOP;
		}
		break;
		//���S��\��
	case TP_LOGO:
		LerpLogo();
		if(GetEndLerpLogo()){
			g_Phase = TP_PUSH;
		}
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_LOOP;
		}
		break;
		//�v�b�V���X�y�[�X��\��
	case TP_PUSH:
		LerpPush();
		//������オ�肫�����烍�[�v�Ɉڍs
		if (GetEndLerp()) {
			g_Phase = TP_LOOP;
		}
		//�X�y�[�X�L�[�������ꂽ���΂�
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_LOOP;
		}
		break;
		//���[�v������
	case TP_LOOP:
		//�L�[�������ꂽ��A�j���[�V�������I������Ƃ̉�ʂɂ���
		UpdateLogo();
		UpdatePushspace();
		UpdateStar();

		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_INFO;
		}
		break;
		//������ʕ\��
	case TP_INFO:
		UpdatePushspace();
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_SCENECHANGE;
		}
		break;
		//�Q�[���V�[���Ɉڍs
	case TP_SCENECHANGE:
		if (GetFadeState() == FADE_NONE) {
			SetFadeColor(0.0f, 0.0f, 0.0f);
			SceneTransition(SCENE_GAME, 0.03f);
		}
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	DrawSpriteLeftTop(g_BG.tex_no, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	DrawPushspace();
	if (g_Phase == TP_INFO) {
		DrawSpriteLeftTop(g_INFO.tex_no, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	}
	else {
		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		DrawStar();
		DrawLogo();
	}
}
