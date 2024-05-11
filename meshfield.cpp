//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "texture.h"
#include "meshfield.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct FIELD
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	D3DXMATRIX  mtxWorld;
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11Buffer					*g_IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@

static FIELD	g_Field;	// �t�B�[���h���
static int		g_TexNo;	// �e�N�X�`���ԍ�

static int		g_VertexNum = 0;//���_��
static int		g_IndexNum = 0;	//�C���f�b�N�X��


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int numBlockX, int numBlockZ, float sizeX, float sizeZ)
{
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Field.pos = pos;
	g_Field.rot = rot;
	g_Field.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`������
	g_TexNo = LoadTexture((char*)"data/TEXTURE/field000.jpg");


	int yoko = numBlockX;
	int tate = numBlockZ;

	//�K�v�Ȓ��_���ƃC���f�b�N�X�����v�Z����
	g_VertexNum = (yoko + 1) * (tate + 1);
	g_IndexNum = (yoko + 1) * 2 * tate + 2 * (tate - 1);//�k�ރ|���S���ɕK�v�Ȑ��𑫂�

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * g_VertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * g_IndexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);


	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		//�ŏ��ɕ`�悳���ʒu
		float ofsetX = -(sizeX * (numBlockX * 0.5f));
		float ofsetZ = (sizeZ * (numBlockZ * 0.5f));

		for (int y = 0; y < (tate + 1); y++) {
			for (int x = 0; x < (yoko + 1); x++) {
				float height = frand() * 100;
				// ���_���W�̐ݒ�
				pVtx[y * (yoko + 1) + x].Position = D3DXVECTOR3(ofsetX + (x * sizeX), 120.0f, ofsetZ - (y * sizeZ));
				// UV�l�̐ݒ�
				pVtx[y * (yoko + 1) + x].TexCoord = D3DXVECTOR2(x, y);
				// �@���̐ݒ�
				pVtx[y * (yoko + 1) + x].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// ���_�J���[�̐ݒ�
				pVtx[y * (yoko + 1) + x].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;
		int idx = 0;

		//4, 0, 5, 1, 6, 2, 7, 3
		//8, 4, 9, 5,10, 6,11, 7

		int index_count = 0;
		for (int y = 0; y < tate; y++) {
			for (int x = 0; x < (yoko + 1); x++) {
				pIdx[index_count] = (yoko + 1) + x + (yoko + 1) * y;
				index_count++;
				pIdx[index_count] = x + (yoko + 1) * y;
				index_count++;
			}
			//�k�ރ|���S����
			if (y  != (tate - 1)) {
				pIdx[index_count] = yoko + (yoko + 1) * y;
				index_count++;
				pIdx[index_count] = (yoko + 1) * 2 + (yoko + 1) * y;
				index_count++;
			}
		}

 		GetDeviceContext()->Unmap(g_IndexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer != NULL) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	SetCullingMode(CULL_MODE_BACK);
	
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(g_TexNo));


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Field.mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&g_Field.mtxWorld);

	// �|���S���̕`��
	GetDeviceContext()->DrawIndexed(g_IndexNum, 0, 0);
}

