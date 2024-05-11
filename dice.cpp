//dice.cpp		�T�C�R������
//20106_�c���@�@ 
//23_12_31
#include "main.h"
#include "dice.h"
#include "texture.h"
#include "collision.h"
#include "bullet.h"
#include "piece.h"
#include "sound.h"

#include <ctime>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float VALUE_MOVE = 5.0f;							// �ړ���

static const float SIZE_X = 50.0f;								//�T�C�Y(X����)
static const float SIZE_Y = 50.0f;								//�T�C�Y(Y����)
static const float SIZE_Z = 50.0f;								//�T�C�Y(Z����)

static const float ROOL_FRAME = 30.0f;							//�ڂ��ς��t���[����

static const int   SIDE_NUM = 6;								//�T�C�R���̖ʂ̐�
static const int   VERTEX_NUM = 4;								//���_��
static const int   SIZE_VERTEX_NUM = SIDE_NUM * VERTEX_NUM;		//�T�C�R���̒��_��


static const int   INDEX_NUM = 36;								//�C���f�b�N�X�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexDice(void);
void HitToBullet(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//�T�E���h�p
enum SOUND_NUM {
	rool,
	hit,

	sound_max,
};
static ID3D11Buffer					*g_VertexBuffer = NULL;	//���_���
static ID3D11Buffer					*g_IndexBuffer = NULL;	//�C���f�b�N�X���

static DICE							g_Dice;					//�|���S���f�[�^
static int							g_TexNo;				//�e�N�X�`���ԍ�
static int							g_SoundNo[sound_max];	//�T�E���h�ԍ�


// ���_�z��
static VERTEX_3D g_VertexArray[SIZE_VERTEX_NUM] = {
	//1�̖� ��
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 1.0f)},
	
	//2�̖ʁ@��O
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 1.0f)},
	
	//3�̖ʁ@�E
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 1.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 1.0f)},
	
	//4�̖ʁ@��
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 1.0f)},
	
	//5�̖ʁ@��
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 1.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 1.0f)},
	
	//6�̖ʁ@��
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f)},
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDice(void)
{
	// ���_�o�b�t�@�̍쐬
	MakeVertexDice();

	// �e�N�X�`������
	g_TexNo = LoadTexture((char*)"data/TEXTURE/dice.png");

	//�T�E���h�̓ǂݍ���
	g_SoundNo[rool] = LoadSound((char*)"data/sound/dice_rool.wav");
	SetVolume(g_SoundNo[rool], 0.3f);
	g_SoundNo[hit] = LoadSound((char*)"data/sound/dice_hit.wav");
	SetVolume(g_SoundNo[hit], 0.4f);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Dice.pos = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	g_Dice.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Dice.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Dice.frame = 0.0f;
	g_Dice.deme = 0;
	g_Dice.stop = false;
	g_Dice.rad = 40.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDice(void)
{
	// �C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer != NULL) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}
	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDice(void)
{
	if (g_Dice.stop) {
		return;
	}
	//�e�Ƃ̓����蔻�菈��
	HitToBullet();

	g_Dice.frame += 1.0f;

	if (g_Dice.frame > ROOL_FRAME) {
		PlaySound(g_SoundNo[rool], 0);
		RooltheDice();
		g_Dice.frame = 0.0f;
	}

	if (GetDeme() == 1) {
		g_Dice.rot.z = D3DX_PI;
		g_Dice.rot.x = D3DX_PI * 1.0f;
	}
	else if (GetDeme() == 2) {
		g_Dice.rot.z = D3DX_PI;
		g_Dice.rot.x = D3DX_PI * 0.5f;
	}
	else if (GetDeme() == 3) {
		g_Dice.rot.z = D3DX_PI * -0.5f;
		g_Dice.rot.x = D3DX_PI;
	}
	else if (GetDeme() == 4) {
		g_Dice.rot.z = D3DX_PI * 0.5f;
		g_Dice.rot.x = D3DX_PI;
	}
	else if (GetDeme() == 5) {
		g_Dice.rot.z = D3DX_PI;
		g_Dice.rot.x = D3DX_PI * -0.5f;
	}
	else if (GetDeme() == 6) {
		g_Dice.rot.z = D3DX_PI;
		g_Dice.rot.x = D3DX_PI * 0.0f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDice(void)
{
	SetLightEnable(false);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(g_TexNo));


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	//�s��A4x4
	D3DXMatrixIdentity(&g_Dice.mtxWorld);	//�P�ʍs������֐�

		// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_Dice.scl.x, g_Dice.scl.y, g_Dice.scl.z);
		//�s��̊|���Z�����Ă����֐��@�����@�������A�@���@�����B
	D3DXMatrixMultiply(&g_Dice.mtxWorld, &g_Dice.mtxWorld, &mtxScl);

	// ��]�𔽉f										y���[			x�s�b�`			z���[��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Dice.rot.y, g_Dice.rot.x, g_Dice.rot.z);
	//�g��k������������Ԃ�
	//�s��̊|���Z�����Ă����֐��@�����@�������A�@���@�����B
	D3DXMatrixMultiply(&g_Dice.mtxWorld, &g_Dice.mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_Dice.pos.x, g_Dice.pos.y, g_Dice.pos.z);
	D3DXMatrixMultiply(&g_Dice.mtxWorld, &g_Dice.mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&g_Dice.mtxWorld);

	// �|���S���`��
	//GetDeviceContext()->Draw(SIZE_VERTEX_NUM, 0);


	//�}�e���A��������
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	//�C���f�b�N�X�t���|���S���`��
	GetDeviceContext()->DrawIndexed(INDEX_NUM, 0, 0);

	SetLightEnable(true);
}

HRESULT MakeVertexDice(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;;
	bd.ByteWidth = sizeof(VERTEX_3D) * SIZE_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// �O���[�o���̒��_�z�񂩂璸�_�o�b�t�@������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = g_VertexArray;

	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);


	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned short) * INDEX_NUM;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//�o�b�t�@�������ɂ��炩���ߖ��߂�f�[�^��ݒ�ł���i�Ȃ��Ă�OK�j
	ZeroMemory(&sd, sizeof(sd));
	//�C���f�b�N�X�f�[�^�̔z������
	unsigned short indexArray[] =
	{
		//��
		0,1,2,
		1,3,2,
		//��O
		4,5,6,
		5,7,6,
		//�E
		8,9,10,
		9,11,10,		
		//��
		12,13,14,
		13,15,14,
		//��
		16,17,18,
		17,19,18,
		//��
		20,21,22,
		21,23,22,

		//���_�ɖ@���̏��AUV�l�̏�񂪓����Ă���̂Ō��E�ł���
	};

	sd.pSysMem = indexArray;
	//�C���f�b�N�X�o�b�t�@�̍쐬
	GetDevice()->CreateBuffer(&bd, &sd, &g_IndexBuffer);


	return S_OK;
}

void RooltheDice(void) {
	// ����������̏�����
	srand(static_cast<unsigned int>(time(nullptr)));

	//�T�C�R����U��
	g_Dice.deme = rand() % 6 + 1;
}

//�o�ڂ�n��
int GetDeme(void) {
	return g_Dice.deme;
}

//�~�܂�����Ԃɂ���
void DiceSetStop(const bool& stop) {
	g_Dice.stop = stop;
}

//�e�Ƃ̓����蔻��
void HitToBullet(void) {
	//�e�Ƃ̓����蔻��
	for (int b = 0; b < NUM_BULLET; b++) {
		if (!GetBulletUse(b)) {
			continue;
		}
		if (CheckHitBS(g_Dice.pos, g_Dice.rad,
			GetBulletPos(b), GetBulletRad())) {
			//�e������
			EraseBullet(b);
			//�ڂ��o��
			DiceSetStop(true);
			//�o�ڂ̕��i�߂�悤�ɂ���
			SetMovenum(GetDeme());
			SetCanMove(true);
			//�ڂ��o���Ƃ��̉����Đ�
			PlaySound(g_SoundNo[hit], 0);
		}
	}
}