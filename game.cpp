//game.cpp		�Q�[���Ǘ�
//20106_�c���@�@
//23_11_30
#include "main.h"
#include "renderer.h"
#include "Texture.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

#include "inputx.h"
#include "camera.h"
#include "player.h"
#include "life.h"
#include "piece.h"
#include "bullet.h"
#include "dice.h"

#include "enemy_manager.h"

#include "coin.h"

#include "trout_manager.h"
#include "trout_red.h"
#include "trout_blue.h"
#include "trout_white.h"
#include "trout_end.h"

#include "meshfield.h"
#include "meshsky.h"
#include "meshmountain.h"

#include "shadow.h"
#include "polyline.h"


static int g_TexNoK = 0;
static int g_TexNoP = 0;
static int g_SoundNo = 0;
TROUT_MANAGER* tm;
bool	g_infoK;
bool	g_infoP;

//=============================================================================
// ����������
//=============================================================================
void InitGame(){
	//�J�����̏�����
	InitCamera();

	//�e�̏�����(�v���C���[�̏������ȂǂŃZ�b�g�����O�ɏ���������)
	InitShadow();

	//���̏�����
	InitPolyLine();

	// �n�ʂ̏�����
	InitMeshField(D3DXVECTOR3(0.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, 100.0f, 100.0f);

	// ��̏�����
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3000.0f, 16, 8);

	// �R�̏�����
	InitMeshMountain(D3DXVECTOR3(0.0f, -70.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 1, 2500.0f, 400.0f);

	//�}�X�}�l�[�W���[�쐬
	tm = new TROUT_MANAGER();

	//�}�X�z�u
	tm->Add(new TROUT_WHITE	({   0.0f,  -200.0f, -1000.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,  -900.0f }));
	tm->Add(new TROUT_BLUE	({  600.0f, -200.0f,  -700.0f }));
	tm->Add(new TROUT_RED	({  300.0f, -200.0f,  -400.0f }));
	tm->Add(new TROUT_WHITE	({    0.0f, -200.0f,  -500.0f }));
	tm->Add(new TROUT_WHITE	({ -300.0f, -200.0f,  -600.0f }));
	tm->Add(new TROUT_BLUE	({ -600.0f, -200.0f,  -400.0f }));
	tm->Add(new TROUT_WHITE	({ -300.0f, -200.0f,  -200.0f }));
	tm->Add(new TROUT_RED	({    0.0f, -200.0f,     0.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,   200.0f }));
	tm->Add(new TROUT_WHITE	({  600.0f, -200.0f,   400.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,   600.0f }));
	tm->Add(new TROUT_BLUE	({    0.0f, -200.0f,   500.0f }));
	tm->Add(new TROUT_RED	({ -300.0f, -200.0f,   400.0f }));
	tm->Add(new TROUT_WHITE	({ -600.0f, -200.0f,   600.0f }));
	tm->Add(new TROUT_RED	({ -300.0f, -200.0f,   800.0f }));
	tm->Add(new TROUT_BLUE	({    0.0f, -200.0f,  1000.0f }));
	tm->Add(new TROUT_WHITE	({  300.0f, -200.0f,  1200.0f }));
	tm->Add(new TROUT_RED	({  600.0f, -200.0f,  1400.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,  1600.0f }));
	tm->Add(new TROUT_END	({    0.0f, -200.0f,  2000.0f }));

	tm->InitTroutManager();

	// �|���S���̏�����
	InitPlayer();
	InitLife();
	InitPiece();
	InitBullet();
	InitDice();
	InitCoin();

	//UI�e�N�X�`���ǂݍ���
	g_TexNoK = LoadTexture((char*)"data/TEXTURE/gameinfo_key.png");
	g_TexNoP = LoadTexture((char*)"data/TEXTURE/gameinfo_pad.png");
	//�T�E���h�ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/bgm_game.wav");
	SetVolume(g_SoundNo, 0.3f);
	PlaySound(g_SoundNo, -1);

	g_infoK = false;
	g_infoP = false;
}


//=============================================================================
// �I������
//=============================================================================
void UninitGame(){
	StopSoundAll();
	// �|���S���̏I������
	UninitCoin();
	UninitDice();
	UninitBullet();
	UninitPiece();
	UninitLife();
	UninitPlayer();

	//�}�X�����̏I������
	tm->UninitTroutManager();

	//�}�X�}�l�[�W���[���
	delete tm;

	UninitMeshMountain();
	UninitMeshSky();
	UninitMeshField();

	//���̏I������
	UninitPolyLine();

	//�e�̏I������
	UninitShadow();

	// �J�����̏I������
	UninitCamera();
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(){
	if (GetKeyboardTrigger(DIK_TAB)) {
		g_infoK = !g_infoK;
	}
	if (IsButtonTriggered(0, XINPUT_GAMEPAD_START)) {
		g_infoP = !g_infoP;
	}
	if (g_infoK || g_infoP) {
		return;
	}
	// �J�����X�V
	UpdateCamera();

	// �n�ʂ̍X�V����
	UpdateMeshField();
	UpdateMeshSky();
	UpdateMeshMountain();

	// �|���S���̍X�V����
	UpdatePlayer();
	UpdateLife();
	UpdatePiece();
	UpdateBullet();

	//�}�X�̍X�V����
	tm->UpdateTroutManager();

	if (tm->TroutEndEvent()) {
		//�~�܂����}�X�̃C�x���g���I����Ă�Ȃ炳���������
		DiceSetStop(false);
		UpdateDice();
	}
	else {
		DiceSetStop(true);
	}

	UpdateCoin();
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(){

	SetCamera();

	// �n�ʂ̕`�揈��
	DrawMeshSky();
	DrawMeshField();
	DrawMeshMountain();

	//�}�X�����̕`�揈��
	tm->DrawTroutManager();

	//�e�̕`��iZ��r�����̂��ߕ`�悷�鏇�Ԃɒ��ӂ���j
	DrawShadow();

	//���̕`��
	DrawPolyLine();

	// �|���S���̕`�揈��
	DrawPlayer();
	DrawPiece();
	DrawBullet();
	DrawDice();
	DrawCoin();

	//2D�ɂȂ�(���s���e)
	SetWorldViewProjection2D();

	if (g_infoK) {
		//����{�^���\��
		DrawSpriteLeftTop(g_TexNoK, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		return;
	}
	if (g_infoP) {
		//����{�^���\��
		DrawSpriteLeftTop(g_TexNoP, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		return;
	}

	//�v���C���[�̗̑͂�\��
	DrawLife();
}