//bullet.cpp	�e�\������
//20106_�c���@�@
//23_12_07
#include "main.h"
#include "camera.h"
#include "bullet.h"
#include "texture.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float	BULLET_APPED = 7.0f;					// �ړ���

static const float	BULLET_SIZE_X = 30.0f;					// �o���b�g�̃T�C�Y(X����)
static const float	BULLET_SIZE_Y = 30.0f;					// �o���b�g�̃T�C�Y(Y����)

static const int	ERASE_FRAME = 240;						//������܂ł̃t���[����
static const float	BULLET_TEX_SIZE = 80.0f;				//�e�N�X�`���T�C�Y

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���

static BULLET						g_Bullet[NUM_BULLET];	// �|���S���f�[�^
static int							g_TexNo;				// �e�N�X�`���ԍ�
static int							g_SoundNo;				// �e�N�X�`���ԍ�

static MATERIAL						g_Material;				//�}�e���A��
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
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
		vertex[0].Position = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
		vertex[1].Position = D3DXVECTOR3(BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
		vertex[3].Position = D3DXVECTOR3(-BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);

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
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet.png");

	//�T�E���h�ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/player_shot.wav");
	SetVolume(g_SoundNo, 0.2f);

	// �}�e���A��������
	ZeroMemory(&g_Material, sizeof(g_Material));
	g_Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ������
	for (int i = 0; i < NUM_BULLET; i++) {
		g_Bullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].isUse = false;
		g_Bullet[i].isShot = 0;
		g_Bullet[i].frame = 0;
		g_Bullet[i].speed = BULLET_APPED;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	if (g_VertexBuffer != NULL)
	{// ���_�o�b�t�@�̉��
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	for (int i = 0; i < NUM_BULLET; i++) {
		//�g���Ă��Ȃ��e�͍X�V���Ȃ�
		if (!GetBulletUse(i)) {
			continue;
		}
		CAMERA* camera = GetCamera();

		//�i�ݕ��������߂�
		if (!g_Bullet[i].isShot) {
			g_Bullet[i].rot.y = sinf(D3DX_PI) + GetFront();
			g_Bullet[i].isShot = true;
		}

		//���W�X�V
		g_Bullet[i].pos.x += sinf(g_Bullet[i].rot.y) * g_Bullet[i].speed;
		g_Bullet[i].pos.z += cosf(g_Bullet[i].rot.y) * g_Bullet[i].speed;

		//�e�̍��W�X�V
		D3DXVECTOR3 shadowpos = g_Bullet[i].pos;
		shadowpos.y = -75.0f;
		SetPositionShadow(g_Bullet[i].shadowID, shadowpos);
		SetColorShadow(g_Bullet[i].shadowID, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		//���Ԃŏ����鏈��
		g_Bullet[i].frame++;
		if (g_Bullet[i].frame > ERASE_FRAME) {
			EraseBullet(i);
			g_Bullet[i].frame = 0;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
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

	for (int i = 0; i < NUM_BULLET; i++)	{
		//�g���Ă��Ȃ����͕̂`�悵�Ȃ�
		if (!GetBulletUse(i)) {
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Bullet[i].mtxWorld);

		CAMERA* cam = GetCamera();

		// �|���S���𐳖ʂɌ�����
#if 0
			// �r���[�}�g���b�N�X�̋t�s����擾
		g_Bullet[i].mtxWorld = cam->mtxInvView;
		g_Bullet[i].mtxWorld._41 = 0.0f;
		g_Bullet[i].mtxWorld._42 = 0.0f;
		g_Bullet[i].mtxWorld._43 = 0.0f;
#else
			// �r���[�}�g���b�N�X���擾
		D3DXMATRIX mtxView = cam->mtxView;

		g_Bullet[i].mtxWorld._11 = mtxView._11;
		g_Bullet[i].mtxWorld._12 = mtxView._21;
		g_Bullet[i].mtxWorld._13 = mtxView._31;
		g_Bullet[i].mtxWorld._21 = mtxView._12;
		g_Bullet[i].mtxWorld._22 = mtxView._22;
		g_Bullet[i].mtxWorld._23 = mtxView._32;
		g_Bullet[i].mtxWorld._31 = mtxView._13;
		g_Bullet[i].mtxWorld._32 = mtxView._23;
		g_Bullet[i].mtxWorld._33 = mtxView._33;
#endif


		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].pos.x,
			g_Bullet[i].pos.y,
			g_Bullet[i].pos.z);
		D3DXMatrixMultiply(&g_Bullet[i].mtxWorld, &g_Bullet[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Bullet[i].mtxWorld);

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
void SetBullet(const D3DXVECTOR3& pos) {
	for (int i = 0; i < NUM_BULLET; i++) {
		if (g_Bullet[i].isUse) {
			continue;
		}
		//���������̉����Đ�
		PlaySound(g_SoundNo, 0);

		g_Bullet[i].pos = pos;
		g_Bullet[i].isUse = true;

		//�e�̃Z�b�g
		g_Bullet[i].shadowID = SetShadow(g_Bullet[i].pos, 100.0f);

		break;
	}
}

//-----------------------------------------------------------------------------
//�Q�b�^�[

//�|�W�V������Ԃ�
D3DXVECTOR3 GetBulletPos(const int& idx){
	return g_Bullet[idx].pos;
}
//���a��Ԃ�
float GetBulletRad(void){
	return BULLET_SIZE_X * 0.5f;
}
//�g���Ă��邩��Ԃ�
bool GetBulletUse(const int& idx){
	return g_Bullet[idx].isUse;
}


//����
void EraseBullet(const int& idx) {
	if (!g_Bullet[idx].isUse) {
		return;
	}
		g_Bullet[idx].isUse = false;
		g_Bullet[idx].isShot = false;
		g_Bullet[idx].rot.y = 0.0f;
		ReleaseShadow(g_Bullet[idx].shadowID);
}