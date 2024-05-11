//coin.cpp	�R�C������
//20106_�c���@�@
//23_12_27
#include "main.h"
#include "camera.h"
#include "coin.h"
#include "player.h"
#include "collision.h"
#include "texture.h"
#include "polyline.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float	SIZE_X = 30.0f;					//�R�C���̃T�C�Y(X����)
static const float	SIZE_Y = 30.0f;					//�R�C���̃T�C�Y(Y����)

static const int	ERASE_FRAME = 300;				//������܂ł̃t���[����
static const float	COIN_TEX_SIZE = 80.0f;			//�e�N�X�`���T�C�Y

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void LerpCoin(void);
void PickupCoin(const int& idx);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���

static COIN							g_Coin[NUM_COIN];		// �|���S���f�[�^
static int							g_TexNo;				// �e�N�X�`���ԍ�
static int							g_SoundNo;				// �e�N�X�`���ԍ�

static MATERIAL						g_Material;				//�}�e���A��

//�R�C�����o������ꏊ
static D3DXVECTOR3 g_CoinPos[NUM_COIN]{
	{ 300.0f, 300.0f, 0.0f},
	{ 400.0f, 300.0f, 300.0f},
	{-200.0f, 300.0f, -100.0f},
	{-300.0f, 300.0f, 400.0f},
	{  50.0f, 300.0f, -200.0f},
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCoin(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// ���_�o�b�t�@�ɒl���Z�b�g����
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float width = 50.0f;
		float height = 50.0f;

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
		vertex[1].Position = D3DXVECTOR3(SIZE_X, -SIZE_Y, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-SIZE_X, SIZE_Y, 0.0f);
		vertex[3].Position = D3DXVECTOR3(-SIZE_X, -SIZE_Y, 0.0f);

		// ���_�J���[�̐ݒ�
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	// �e�N�X�`������
	g_TexNo = LoadTexture((char*)"data/TEXTURE/coin.png");

	//�����ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/coin_hit.wav");
	SetVolume(g_SoundNo, 0.3f);

	// �}�e���A��������
	ZeroMemory(&g_Material, sizeof(g_Material));
	g_Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ������
	for (int i = 0; i < NUM_COIN; i++) {
		g_Coin[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Coin[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Coin[i].isUse = false;
		g_Coin[i].frame = 0.0f;
		g_Coin[i].lerp_end = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCoin(void)
{
	//�|�����C��������
	for (int i = 0; i < NUM_COIN; i++) {
		ReleasePolyLine(g_Coin[i].polylineID);
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
void UpdateCoin(void)
{
	for (int i = 0; i < NUM_COIN; i++) {
		//�g���Ă��Ȃ����͍̂X�V���Ȃ�
		if (!GetCoinUse(i)) {
			continue;
		}
		//�����Ă���
		LerpCoin();

		//�E����
		PickupCoin(i);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCoin(void)
{
	// ���e�X�g��L����
	SetAlphaTestEnable(true);

	// ���C�e�B���O�𖳌�
	SetLightEnable(false);

	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for (int i = 0; i < NUM_COIN; i++)	{
		//�g���Ă��Ȃ����͕̂`�悵�Ȃ�
		if (!GetCoinUse(i)) {
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Coin[i].mtxWorld);

		CAMERA* cam = GetCamera();

		// �|���S���𐳖ʂɌ�����
#if 0
			// �r���[�}�g���b�N�X�̋t�s����擾
		g_Coin[i].mtxWorld = cam->mtxInvView;
		g_Coin[i].mtxWorld._41 = 0.0f;
		g_Coin[i].mtxWorld._42 = 0.0f;
		g_Coin[i].mtxWorld._43 = 0.0f;
#else
			// �r���[�}�g���b�N�X���擾
		D3DXMATRIX mtxView = cam->mtxView;

		g_Coin[i].mtxWorld._11 = mtxView._11;
		g_Coin[i].mtxWorld._12 = mtxView._21;
		g_Coin[i].mtxWorld._13 = mtxView._31;
		g_Coin[i].mtxWorld._21 = mtxView._12;
		g_Coin[i].mtxWorld._22 = mtxView._22;
		g_Coin[i].mtxWorld._23 = mtxView._32;
		g_Coin[i].mtxWorld._31 = mtxView._13;
		g_Coin[i].mtxWorld._32 = mtxView._23;
		g_Coin[i].mtxWorld._33 = mtxView._33;
#endif


		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Coin[i].pos.x,
			g_Coin[i].pos.y,
			g_Coin[i].pos.z);
		D3DXMatrixMultiply(&g_Coin[i].mtxWorld, &g_Coin[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Coin[i].mtxWorld);

		// �}�e���A���ݒ�
		SetMaterial(g_Material);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(g_TexNo));

		// �|���S���̕`��
		GetDeviceContext()->Draw(4, 0);

	}

	// ���C�e�B���O��L����
	SetLightEnable(true);

	// ���e�X�g�𖳌���
	SetAlphaTestEnable(false);
}

//-----------------------------------------------------------------------------
//�Z�b�^�[
void SetCoin(void) {
	for (int i = 0; i < NUM_COIN; i++) {
		if (g_Coin[i].isUse) {
			continue;
		}
		//�����ʒu�͏o������ʒu
		g_Coin[i].pos = g_CoinPos[i];

		//���`��ԂɎg���ʒu������
		g_Coin[i].spos = g_Coin[i].pos;
		g_Coin[i].epos = g_Coin[i].pos;
		g_Coin[i].epos.y = -20.0f;

		//�|�����C���̈ʒu������
		g_Coin[i].polylinepos = g_Coin[i].epos;
		g_Coin[i].polylinepos.y = g_Coin[i].epos.y + 70.0f;

		//�g�p���Ă����Ԃɂ���
		g_Coin[i].isUse = true;
	}
}

//-----------------------------------------------------------------------------
//�Q�b�^�[
//�|�W�V������Ԃ�
D3DXVECTOR3 GetCoinPos(const int& idx){
	return g_Coin[idx].pos;
}
//���a��Ԃ�
float GetCoinRad(void){
	return SIZE_X * 0.5f;
}
//�g���Ă��邩��Ԃ�
bool GetCoinUse(const int& idx){
	return g_Coin[idx].isUse;
}


//�K�v�Ȃ��̂�������
void EraseCoin(const int& idx) {
	if (!g_Coin[idx].isUse) {
		return;
	}
	g_Coin[idx].isUse = false;
	ReleasePolyLine(g_Coin[idx].polylineID);
}
//�S������
void EraseCoinAll(void) {
	for (int i = 0; i < NUM_COIN; i++) {
		g_Coin[i].lerp_end = false;
		g_Coin[i].isUse = false;
		ReleasePolyLine(g_Coin[i].polylineID);
	}
}

//�R�C���������Ă���
void LerpCoin(void) {
	for (int i = 0; i < NUM_COIN; i++) {
		if (g_Coin[i].lerp_end) {
			continue;
		}
		float		t = g_Coin[i].frame / 120.0f;	//������x�̐��Ŋ���
		g_Coin[i].frame += 1.0f;

		D3DXVec3Lerp(&g_Coin[i].pos, &g_Coin[i].spos, &g_Coin[i].epos, easeOutBounce(t));

		if (t >= 1.0f) {
			g_Coin[i].frame = 0.0f;
			g_Coin[i].pos = g_Coin[i].epos;
			g_Coin[i].lerp_end = true;


			//�|�����C���̃Z�b�g
			g_Coin[i].polylineID = SetPolyLine(g_Coin[i].pos, g_Coin[i].polylinepos, 30.0f);
			SetColorPolyLine(g_Coin[i].polylineID, {1.0f, 1.0f, 0.0f, 1.0f});
		}
	}
}

//�R�C�����v���[���[���E��
void PickupCoin(const int& idx) {
	if (CheckHitBS(g_Coin[idx].pos, SIZE_X,
		GetPlayerPos(), GetPlayerRad())) {
		AddPlayerCoinNum();
		EraseCoin(idx);
		PlaySound(g_SoundNo, 0);
	}
}