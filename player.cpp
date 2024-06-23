//player.cpp	�v���C���[�\������
//20106_�c���@�@
//23_12_07
#include "main.h"
#include "renderer.h"
#include "inputx.h"
#include "texture.h"
#include "model.h"

#include "camera.h"
#include "player.h"
#include "bullet.h"
#include "shadow.h"

#include "fade.h"

#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float VALUE_MOVE = 5.0f;				// �ړ���
static const float PLAYER_RAD = 40.0f;				// ���a

static const int PLAYER_LIFE = 10;					//�ő僉�C�t
static const int PLAYER_FRASH_FRAME = 30;			//�_�Ŏ���
static const int PLAYER_NOHIT_FRAME = 120;			//���G����

static const int WALK_ANIM_ROT = 20;				//�����A�j���[�V�����̎葫�̊p�x
static const int ANIME_NUM = 5;

static const float		STICK_DEADZONE = 0.15f;
static const float		ROTATE_SPEED = 0.15f;

struct PLAYER{
	D3DXVECTOR3 pos;		//�|�W�V����
	D3DXVECTOR3 shot_pos;	//�e���o��ʒu
	D3DXVECTOR3 rot;		//�p�x
	D3DXVECTOR3 dir;		//�p�x
	D3DXVECTOR3 scl;		//�X�P�[��

	float		rad;		//�����蔻��p���a

	float		speed;		//�ړ����x
	float		front;		//�����Ă������

	int			frashframe;	//�_��
	int			nohitframe;	//���G����
	int			walkframe;	//�����Ă���Ƃ��̃t���[��

	int			life;		//���C�t
	bool		hit;		//�q�b�g��Ԃ�

	bool		isuse;		//�g���Ă����Ԃ�

	int			shadowID;	//�eID

	int			coin;		//�E�����R�C���̖���
	int			killedenemy;//�|�����G�̐�


	unsigned int second;	//�����Ă��鎞��
	unsigned int starttime;	//�X�^�[�g����
	unsigned int endtime;	//�I������


	int			state;
	D3DXMATRIX  mtxWorld;
};

struct PARTS{
	D3DXVECTOR3		pos;	//�ʒu���
	D3DXVECTOR3		scl;	//�傫�����
	D3DXVECTOR3		rot;	//��]���	
	D3DXMATRIX		mtxWorld;	//�}�g���b�N�X
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
struct KeyFrame {
	D3DXVECTOR3	key;	//��]�p�x
	float		frame;	//�t���[��
};
struct MotionSet {
	KeyFrame	LegLMotion[ANIME_NUM];
	KeyFrame	LegRMotion[ANIME_NUM];
	KeyFrame	ArmLMotion[ANIME_NUM];
	KeyFrame	ArmRMotion[ANIME_NUM];
	KeyFrame	HeadMotion[ANIME_NUM];
};

//�������[�V����
MotionSet WalkMotion = {
{//����
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.5f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{-0.5f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//�E��
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{-0.5f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.5f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//���r
	{{ 0.0f, -3.0f, -0.2f},   0.0f},
	{{ 0.5f, -3.0f, -0.2f},  60.0f},
	{{ 0.0f, -3.0f, -0.2f}, 120.0f},
	{{-0.5f, -3.0f, -0.2f}, 180.0f},
	{{ 0.0f, -3.0f, -0.2f}, 240.0f},
},
{//�E�r
	{{ 0.0f,  3.0f,  0.2f},   0.0f},
	{{-0.5f,  3.0f,  0.2f},  60.0f},
	{{ 0.0f,  3.0f,  0.2f}, 120.0f},
	{{ 0.5f,  3.0f,  0.2f}, 180.0f},
	{{ 0.0f,  3.0f,  0.2f}, 240.0f},
},
{//��
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
};

//�ҋ@���[�V����
MotionSet WaitMotion = {
{//����
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//�E��
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//���r
	{{ 0.0f, -3.0f, -0.0f},   0.0f},
	{{ 0.0f, -3.0f, -0.5f},  60.0f},
	{{ 0.0f, -3.0f, -0.0f}, 120.0f},
	{{ 0.0f, -3.0f, -0.5f}, 180.0f},
	{{ 0.0f, -3.0f, -0.0f}, 240.0f},
},
{//�E�r
	{{ 0.0f,  3.0f,  0.0f},   0.0f},
	{{ 0.0f,  3.0f,  0.5f},  60.0f},
	{{ 0.0f,  3.0f,  0.0f}, 120.0f},
	{{ 0.0f,  3.0f,  0.5f}, 180.0f},
	{{ 0.0f,  3.0f,  0.0f}, 240.0f},
},
{//��
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.2f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f, -0.2f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
};

void HitToEnemy(void);
D3DXVECTOR3 Anime(KeyFrame* keyframe, int size);
D3DXVECTOR3 StopAnime(KeyFrame* keyframe, int state);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//�T�E���h�p
enum SOUND_NUM {
	walk,
	hit,
	dead,

	sound_max,
};

static D3DXVECTOR3	g_rotate;

static DX11_MODEL	g_BodyModel;	// ���f���ǂݍ���	�e
static DX11_MODEL	g_HeadModel;	// ���f���ǂݍ���
static DX11_MODEL	g_ArmRModel;	// ���f���ǂݍ���
static DX11_MODEL	g_ArmLModel;	// ���f���ǂݍ���
static DX11_MODEL	g_LegRModel;	// ���f���ǂݍ���
static DX11_MODEL	g_LegLModel;	// ���f���ǂݍ���

static PLAYER		g_Body;		// �v���C���[�����		�e
static PARTS		g_Head;
static PARTS		g_ArmR;
static PARTS		g_ArmL;
static PARTS		g_LegR;
static PARTS		g_LegL;

static LIGHT		g_DirectionalLight;		//�f�B���N�V���i�����C�g
static LIGHT		g_PointLight[4];		//�|�C���g���C�g

static int			g_SoundNo[sound_max];	//�T�E���h

static float		g_frame = 0.0f;
static int			g_keyindex = 0;

MotionSet*			g_pMotion;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	//obj���f���̓ǂݍ���
	LoadModel((char*)"data/MODEL/frog_body.obj", &g_BodyModel);
	LoadModel((char*)"data/MODEL/frog_head.obj", &g_HeadModel);
	LoadModel((char*)"data/MODEL/frog_armR.obj", &g_ArmRModel);
	LoadModel((char*)"data/MODEL/frog_armL.obj", &g_ArmLModel);
	LoadModel((char*)"data/MODEL/frog_legR.obj", &g_LegRModel);
	LoadModel((char*)"data/MODEL/frog_legL.obj", &g_LegLModel);

	//�T�E���h�̓ǂݍ���
	g_SoundNo[walk] = LoadSound((char*)"data/sound/player_walk.wav");
	SetVolume(g_SoundNo[walk], 0.2f);
	g_SoundNo[hit] = LoadSound((char*)"data/sound/player_hit.wav");
	SetVolume(g_SoundNo[hit], 0.4f);
	g_SoundNo[dead] = LoadSound((char*)"data/sound/player_dead.wav");
	SetVolume(g_SoundNo[dead], 0.2f);


	// ���C�g��L����
	SetLightEnable(true);

	// ���s�����̐ݒ�i���E���Ƃ炷���j
	g_DirectionalLight.Type = LIGHT_TYPE_DIRECTIONAL;				// ���s����
	g_DirectionalLight.Enable = true;								// ���̃��C�g��ON
	g_DirectionalLight.Direction = D3DXVECTOR4(0.0f, -1.0f, 0.1f, 0.0f);// ���̌���
	g_DirectionalLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���̐F
	SetLight(0, &g_DirectionalLight);								// ����Őݒ肵�Ă���

	// �_�����̐ݒ�
	g_PointLight[0].Position = D3DXVECTOR4( 700.0f, 300.0f, -1000.0f, 0.0f);// ���̍��W
	g_PointLight[1].Position = D3DXVECTOR4(-700.0f, 300.0f, -1000.0f, 0.0f);// ���̍��W
	g_PointLight[2].Position = D3DXVECTOR4( 700.0f, 300.0f,  2000.0f, 0.0f);// ���̍��W
	g_PointLight[3].Position = D3DXVECTOR4(-700.0f, 300.0f,  2000.0f, 0.0f);// ���̍��W
	g_PointLight[0].Direction = D3DXVECTOR4(-0.5f, -1.0f, 0.0f, 0.5f);// ���̌���
	g_PointLight[1].Direction = D3DXVECTOR4(0.5f, -1.0f, 0.0f, 0.5f);// ���̌���
	g_PointLight[2].Direction = D3DXVECTOR4(-0.5f, -1.0f, 0.0f, -0.5f);// ���̌���
	g_PointLight[3].Direction = D3DXVECTOR4(0.5f, -1.0f, 0.0f, -0.5f);// ���̌���
	for (int i = 0; i < 4; i++) {
		g_PointLight[i].Type = LIGHT_TYPE_POINT;						// ���s����
		g_PointLight[i].Enable = true;									// ���̃��C�g��ON
		g_PointLight[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���̐F
		g_PointLight[i].Attenuation = 1000.0f;							// ��������
		SetLight(i + 1, &g_PointLight[i]);									// ����Őݒ肵�Ă���
	}

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Body.pos = D3DXVECTOR3(0.0f, -30.0f, -1700.0f);
	g_Body.shot_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Body.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Body.dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Body.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Body.rad = PLAYER_RAD;

	g_Body.speed = 0.0f;
	g_Body.front = 0.0f;

	g_Body.frashframe = 0;
	g_Body.nohitframe = 0;
	g_Body.walkframe = 0;

	g_Body.life = PLAYER_LIFE;
	g_Body.hit = false;

	g_Body.isuse = true;


	g_Body.coin = 0;
	g_Body.killedenemy = 0;

	g_Body.second = 0;
	//�J�n������ۑ�����
	g_Body.starttime = timeGetTime();
	g_Body.endtime = 0;

	//�e�̃Z�b�g
	g_Body.shadowID = SetShadow(g_Body.pos, 200.0f);


	g_Head.pos = {0.0f, 20.0f, 0.0f};	// �v���C���[�̈ʒu���
	g_Head.scl = {1.0f, 1.0f, 1.0f};	// �v���C���[�̑傫�����
	g_Head.rot = {0.0f, 0.0f, 0.0f};	// �v���C���[�̉�]���

	g_ArmR.pos = {-20.0f, 15.0f, 0.0f};	// �v���C���[�E�r�ʒu���
	g_ArmR.scl = {1.0f, 1.0f, 1.0f};	// �v���C���[�E�r�傫�����
	g_ArmR.rot = {0.0f, 0.0f, 0.0f};	// �v���C���[�E�r��]���

	g_ArmL.pos = {20.0f, 15.0f, 0.0f};	// �v���C���[���r�ʒu���
	g_ArmL.scl = {1.0f, 1.0f, 1.0f};	// �v���C���[���r�傫�����
	g_ArmL.rot = {0.0f, 0.0f, 0.0f};	// �v���C���[���r��]���

	g_LegR.pos = {-10.0f, -25.0f, -5.0f};	// �v���C���[�E���ʒu���
	g_LegR.scl = {1.0f, 1.0f, 1.0f};	// �v���C���[�E���傫�����
	g_LegR.rot = {0.0f, 0.0f, 0.0f};	// �v���C���[�E����]���

	g_LegL.pos = {10.0f, -25.0f, -5.0f};	// �v���C���[�����ʒu���
	g_LegL.scl = {1.0f, 1.0f, 1.0f};	// �v���C���[�����傫�����
	g_LegL.rot = {0.0f, 0.0f, 0.0f};	// �v���C���[������]���

	g_rotate = { 0.0f, 0.0f, 0.0f };

	g_pMotion = &WaitMotion;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	//�I�����Ԃ�����
	g_Body.endtime = g_Body.second;
	if (&g_BodyModel != NULL) {
		//obj���f���̉��
		UnloadModel(&g_BodyModel);
	}
	if (&g_HeadModel != NULL) {
		//obj���f���̉��
		UnloadModel(&g_HeadModel);
	}
	if (&g_ArmRModel != NULL) {
		//obj���f���̉��
		UnloadModel(&g_ArmRModel);
	}
	if (&g_ArmLModel != NULL) {
		//obj���f���̉��
		UnloadModel(&g_ArmLModel);
	}
	if (&g_LegRModel != NULL) {
		//obj���f���̉��
		UnloadModel(&g_LegRModel);
	}
	if (&g_LegLModel != NULL) {
		//obj���f���̉��
		UnloadModel(&g_LegLModel);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	if (!GetPlayerUse()) {
		//�̗͂��O�ɂȂ����烊�U���g�ɑJ��
		SceneTransition(SCENE_RESULT, 0.03f);
		return;
	}

	//���U���g�ɍs���܂łɌo�߂����b�����X�V����
	g_Body.second = (timeGetTime() - g_Body.starttime) / 1000;

	//�J�������󂯎��
	CAMERA *cam = GetCamera();

	//�J�����̒����_���Z�b�g
	SetCameraAT(g_Body.pos);

	//���x���Z�b�g
	g_Body.speed *= 0.9f;

	// �ړ�-----------------------------------------------------------------------------------
	//�X�e�B�b�N
	if (GetThumbLeftY(0) < -STICK_DEADZONE ||				//�E�X�e�B�b�N
		GetThumbLeftY(0) > STICK_DEADZONE ||
		GetThumbLeftX(0) < -STICK_DEADZONE ||				//�E�X�e�B�b�N
		GetThumbLeftX(0) > STICK_DEADZONE) {
		g_Body.speed = VALUE_MOVE;
		//�X�e�B�b�N�̓|��������������
		g_Body.rot.y = g_Body.front = atan2f(GetThumbLeftY(0), -GetThumbLeftX(0)) + (D3DX_PI * 1.5f) + cam->rot.y;
		if (g_Body.state != 1) {
			g_Body.state = 0;
		}
	}

	if (GetKeyboardPress(DIK_A)) {
		g_Body.speed = VALUE_MOVE;							//���x������
		if (GetKeyboardPress(DIK_W)) {						//����
			g_Body.rot.y = g_Body.front = D3DX_PI * 1.75f + cam->rot.y;
		}
		else if (GetKeyboardPress(DIK_S)) {					//����
			g_Body.rot.y = g_Body.front = D3DX_PI * 1.25f + cam->rot.y;
		}
		else {												//��
			g_Body.rot.y = g_Body.front = D3DX_PI * 1.5f + cam->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_D))	{
		g_Body.speed = VALUE_MOVE;							//���x������
		if (GetKeyboardPress(DIK_W)) {						//�E��
			g_Body.rot.y = g_Body.front = D3DX_PI * 0.25f + cam->rot.y;
		}
		else if (GetKeyboardPress(DIK_S)) {					//�E��
			g_Body.rot.y = g_Body.front = D3DX_PI * 0.75f + cam->rot.y;
		}
		else {												//�E
			g_Body.rot.y = g_Body.front = D3DX_PI * 0.5f + cam->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_W))	{
		g_Body.speed = VALUE_MOVE;							//���x������
		g_Body.rot.y = g_Body.front = D3DX_PI * 0.0f + cam->rot.y;		//��
	}
	else if (GetKeyboardPress(DIK_S))	{
		g_Body.speed = VALUE_MOVE;							//���x������
		g_Body.rot.y = g_Body.front = D3DX_PI * 1.0f + cam->rot.y;		//��
	}

	//�J�����������Ă�������𐳖ʂɂ���
	g_Body.front = cam->rot.y;


	//���W�X�V----------------------------------------------------------------------------------
	g_Body.pos.x += sinf(g_Body.rot.y) * g_Body.speed;
	g_Body.pos.z += cosf(g_Body.rot.y) * g_Body.speed;

	//�e���o��ʒu���L�����̖ڂ̑O�ɂ���
	g_Body.shot_pos.x = g_Body.pos.x + sinf(g_Body.rot.y) * 40.0f;
	g_Body.shot_pos.z = g_Body.pos.z + cosf(g_Body.rot.y) * 40.0f;



	//�e�̍��W�X�V
	D3DXVECTOR3 shadowpos = g_Body.pos;
	shadowpos.y = - 75.0f;
	SetPositionShadow(g_Body.shadowID, shadowpos);

	//�e������
	if (GetKeyboardTrigger(DIK_M) ||
		IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
		//�����Ă�������Ɍ���
		g_Body.front = g_Body.rot.y;
		SetBullet(g_Body.shot_pos);
	}

	//-----------------------------------------------------------------------------------
	//�A�j���[�V�����̏�Ԃ�ς���
	if (GetKeyboardPress(DIK_W) ||
		GetKeyboardPress(DIK_A) ||
		GetKeyboardPress(DIK_S) ||
		GetKeyboardPress(DIK_D)) {
		if (GetKeyboardTrigger(DIK_W)) {
			g_Body.state = 0;
		}
		if (GetKeyboardTrigger(DIK_A)) {
			g_Body.state = 0;
		}
		if (GetKeyboardTrigger(DIK_S)) {
			g_Body.state = 0;
		}
		if (GetKeyboardTrigger(DIK_D)) {
			g_Body.state = 0;
		}
	}
	else if (GetThumbLeftY(0) < -STICK_DEADZONE ||				//�E�X�e�B�b�N
		GetThumbLeftY(0) > STICK_DEADZONE ||
		GetThumbLeftX(0) < -STICK_DEADZONE ||				//�E�X�e�B�b�N
		GetThumbLeftX(0) > STICK_DEADZONE) {
		//�������[�V�������͉������Ȃ�
		if (g_Body.state != 1) {
			g_Body.state = 0;
		}
	}
	else {
		if (g_Body.state == 3) {
			g_Body.state = 3;
		}
		else if (g_Body.state != 2) {
			g_Body.state = 2;
		}
	}

	//�A�j���[�V����
	if (g_Body.state == 0) {
		//�葫�����낦�Ă�������ɍs��
		g_LegL.rot = StopAnime(g_pMotion->LegLMotion, 1);
		g_LegR.rot = StopAnime(g_pMotion->LegRMotion, 1);
		g_ArmL.rot = StopAnime(g_pMotion->ArmLMotion, 1);
		g_ArmR.rot = StopAnime(g_pMotion->ArmRMotion, 1);
		g_Head.rot = StopAnime(g_pMotion->HeadMotion, 1);
	}
	//����
	if (g_Body.state == 1) {
		g_pMotion = &WalkMotion;
	}
	if (g_Body.state == 2) {
		//��������葫�����낦��
		g_LegL.rot = StopAnime(g_pMotion->LegLMotion, 3);
		g_LegR.rot = StopAnime(g_pMotion->LegRMotion, 3);
		g_ArmL.rot = StopAnime(g_pMotion->ArmLMotion, 3);
		g_ArmR.rot = StopAnime(g_pMotion->ArmRMotion, 3);
		g_Head.rot = StopAnime(g_pMotion->HeadMotion, 3);
	}
	//�ҋ@���[�V����
	if (g_Body.state == 3) {
		g_pMotion = &WaitMotion;
	}

	g_LegL.rot = Anime(g_pMotion->LegLMotion, sizeof(g_pMotion->LegLMotion) / sizeof(KeyFrame));
	g_LegR.rot = Anime(g_pMotion->LegRMotion, sizeof(g_pMotion->LegRMotion) / sizeof(KeyFrame));
	g_ArmL.rot = Anime(g_pMotion->ArmLMotion, sizeof(g_pMotion->ArmLMotion) / sizeof(KeyFrame));
	g_ArmR.rot = Anime(g_pMotion->ArmRMotion, sizeof(g_pMotion->ArmRMotion) / sizeof(KeyFrame));
	g_Head.rot = Anime(g_pMotion->HeadMotion, sizeof(g_pMotion->HeadMotion) / sizeof(KeyFrame));




	//���������̉��Đ�-------------------------------------------------------------------
	if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_W) ||
		GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_A) ||
		GetThumbLeftY(0) < -STICK_DEADZONE ||				//�E�X�e�B�b�N
		GetThumbLeftY(0) > STICK_DEADZONE ||
		GetThumbLeftX(0) < -STICK_DEADZONE ||				//�E�X�e�B�b�N
		GetThumbLeftX(0) > STICK_DEADZONE) {
		g_Body.walkframe++;
		if (g_Body.walkframe > 25) {
			//�������Đ�
			PlaySound(g_SoundNo[walk], 0);
			g_Body.walkframe = 0;
		}
	}
	//�����Ă��Ȃ��Ƃ�
	else {
		g_Body.walkframe = 0;
		StopSound(g_SoundNo[walk]);
	}

	//�G�Ƃ̓����蔻��-------------------------------------------------------------------
	HitToEnemy();

	//���C�t���O�ȉ��ɂȂ����瓮���Ȃ�
	if (GetPlayerLife() <= 0) {
		//�_���[�W���󂯂��Ƃ��̉��͍Đ����Ȃ�
		StopSound(g_SoundNo[hit]);
		//�|���ꂽ���̉����Đ�
		PlaySound(g_SoundNo[dead], 0);
		SetPlayerUse(false);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	if (!GetPlayerUse()) {
		return;
	}
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	//�e�̃}�g���b�N�X
	D3DXMATRIX mtxParent;


	//�̃p�[�c�̕ϊ��ƕ`��---------------------------------------------------------------
	D3DXMatrixIdentity(&g_Body.mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_Body.scl.x, g_Body.scl.y, g_Body.scl.z);
	g_Body.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Body.rot.y, g_Body.rot.x, g_Body.rot.z);
	g_Body.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_Body.pos.x, g_Body.pos.y, g_Body.pos.z);
	g_Body.mtxWorld *= mtxTranslate;

	//�e�̃}�g���b�N�X�̈ړ��Ɖ�]�������X�V
	mtxParent = mtxRot;
	mtxParent *= mtxTranslate;

	SetWorldMatrix(&g_Body.mtxWorld);

	if (g_Body.frashframe == 0) {
		//obj���f���̕`��
		DrawModel(&g_BodyModel);
	}



	//���p�[�c�̕ϊ��ƕ`��---------------------------------------------------------------
	D3DXMatrixIdentity(&g_Head.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_Head.scl.x, g_Head.scl.y, g_Head.scl.z);
	g_Head.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Head.rot.y, g_Head.rot.x, g_Head.rot.z);
	g_Head.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_Head.pos.x, g_Head.pos.y, g_Head.pos.z);
	g_Head.mtxWorld *= mtxTranslate;

	//�q�̃��[���h�}�g���b�N�X�ɐe�̃}�g���b�N�X�𔽉f������
	g_Head.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_Head.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_HeadModel);
	}


	//�E�r�p�[�c�̕ϊ��ƕ`��-------------------------------------------------------------
	D3DXMatrixIdentity(&g_ArmR.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_ArmR.scl.x, g_ArmR.scl.y, g_ArmR.scl.z);
	g_ArmR.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ArmR.rot.y, g_ArmR.rot.x, g_ArmR.rot.z);
	g_ArmR.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_ArmR.pos.x, g_ArmR.pos.y, g_ArmR.pos.z);
	g_ArmR.mtxWorld *= mtxTranslate;

	g_ArmR.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_ArmR.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_ArmRModel);
	}



	//���r�p�[�c�̕ϊ��ƕ`��-------------------------------------------------------------
	D3DXMatrixIdentity(&g_ArmL.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_ArmL.scl.x, g_ArmL.scl.y, g_ArmL.scl.z);
	g_ArmL.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ArmL.rot.y, g_ArmL.rot.x, g_ArmL.rot.z);
	g_ArmL.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_ArmL.pos.x, g_ArmL.pos.y, g_ArmL.pos.z);
	g_ArmL.mtxWorld *= mtxTranslate;

	g_ArmL.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_ArmL.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_ArmLModel);
	}


	//�E�r�p�[�c�̕ϊ��ƕ`��-------------------------------------------------------------
	D3DXMatrixIdentity(&g_LegR.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_LegR.scl.x, g_LegR.scl.y, g_LegR.scl.z);
	g_LegR.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LegR.rot.y, g_LegR.rot.x, g_LegR.rot.z);
	g_LegR.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_LegR.pos.x, g_LegR.pos.y, g_LegR.pos.z);
	g_LegR.mtxWorld *= mtxTranslate;

	g_LegR.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_LegR.mtxWorld);


	if (g_Body.frashframe == 0) {
		DrawModel(&g_LegRModel);
	}



	//���r�p�[�c�̕ϊ��ƕ`��-------------------------------------------------------------
	D3DXMatrixIdentity(&g_LegL.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_LegL.scl.x, g_LegL.scl.y, g_LegL.scl.z);
	g_LegL.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LegL.rot.y, g_LegL.rot.x, g_LegL.rot.z);
	g_LegL.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_LegL.pos.x, g_LegL.pos.y, g_LegL.pos.z);
	g_LegL.mtxWorld *= mtxTranslate;

	g_LegL.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_LegL.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_LegLModel);
	}
}

//�g���Ă����Ԃ���Ԃ�
bool GetPlayerUse(void) {
	return g_Body.isuse;
}
//�g���Ă����Ԃ�ς���
void SetPlayerUse(const bool& use) {
	g_Body.isuse = use;
}

//�|�W�V������Ԃ�
D3DXVECTOR3 GetPlayerPos() {
	return g_Body.pos;
}

//���a��Ԃ�
float GetPlayerRad() {
	return g_Body.rad;
}

//�����Ă��������Ԃ�
float GetFront(void) {
	return g_Body.front;
}

//�p�x��Ԃ�
D3DXVECTOR3 GetRot(void) {
	return g_Body.rot;
}

//����������������
void SetPlayerHit(const bool& hit) {
	g_Body.hit = hit;
}
//������������Ԃ�
bool GetPlayerHit(void) {
	return g_Body.hit;
}

//���C�t�����炷
void PlayerLifeDim(void) {
	//�_���[�W���󂯂��Ƃ��̉����Đ�
	PlaySound(g_SoundNo[hit], 0);
	g_Body.life--;
}
//���C�t��Ԃ�
int GetPlayerLife(void) {
	return g_Body.life;
}

//�����Ă���R�C���𑝂₷
void AddPlayerCoinNum(void) {
	g_Body.coin++;
}
//�����Ă���R�C����Ԃ�
int GetPlayerCoinNum(void) {
	return g_Body.coin;
}

//�|�����G�̐��𑝂₷
void AddPlayerkilledNum(void) {
	g_Body.killedenemy++;
}
//�|�����G�̐���Ԃ�
int GetPlayerkilledNum(void) {
	return g_Body.killedenemy;
}

//�I�����_�ł̎��Ԃ�Ԃ�
unsigned int GetEndTime(void) {
	return g_Body.endtime;
}

void HitToEnemy(void) {
	if (GetPlayerHit()) {
		//���G���Ԃ��J�E���g���n�߂�
		g_Body.nohitframe++;
		g_Body.frashframe++;
	}
	//120���z�����疳�G���ԏI��
	if (g_Body.nohitframe > PLAYER_NOHIT_FRAME) {
		SetPlayerHit(false);
		g_Body.nohitframe = 0;
		g_Body.frashframe = 0;
	}
	//300���z������_�ŏI��
	if (g_Body.frashframe > PLAYER_FRASH_FRAME) {
		g_Body.frashframe = 0;
	}
}

D3DXVECTOR3 Anime(KeyFrame* keyframe, int size) {
	for (int i = 0; i < size; i++) {
		if (g_frame < keyframe[i].frame) {
			g_keyindex = i - 1;
			break;
		}
	}
	//	  t = (���݂̃t���[�� - �X�^�[�g�L�[�t���[��)    / (�G���h�L�[�t���[�� - �X�^�[�g�L�[�t���[��)
	float t = (g_frame - keyframe[g_keyindex].frame) / (keyframe[g_keyindex + 1].frame - keyframe[g_keyindex].frame);

	D3DXVECTOR3 move;
	D3DXVec3Lerp(&move, &keyframe[g_keyindex].key, &keyframe[g_keyindex + 1].key, t);

	g_frame += 1.0f;
	if (g_frame >= keyframe[size - 1].frame) {
		g_frame = 0.0f;
		g_keyindex++;
	}
	if (g_keyindex >= size - 1) {
		g_keyindex = 0;
	}
	return move;
}

D3DXVECTOR3 StopAnime(KeyFrame* keyframe, int state) {
	float t = g_frame / 25.0f;

	D3DXVECTOR3 move;
	D3DXVec3Lerp(&move, &move, &keyframe[0].key, t);

	g_frame += 1.0f;
	if (g_frame >= 25.0f) {
		g_Body.state = state;
		g_frame = 0.0f;
		g_keyindex++;
	}
	if (g_keyindex >= ANIME_NUM - 1) {
		g_keyindex = 0;
	}
	return move;
}