//trout_blue.cpp	�R�C�����o��}�X
//20106_�c���@�@
//23_12_14
#include "trout_blue.h"
#include "texture.h"
#include "renderer.h"
#include "coin.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ����������
//=============================================================================
void TROUT_BLUE::Init(void)
{
	SetPos({ GetPos().x, 0.0f, GetPos().z });
	SetStep(false);
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	SetRot({ 0.0f, 0.0f, 0.0f });
	SetScl({ 1.0f, 1.0f, 1.0f });

	// �e�N�X�`������
	SetTex(LoadTexture((char*)"data/TEXTURE/trout.png"));

	//�����ǂݍ���
	SetSound(LoadSound((char*)"data/sound/coin_pop.wav"));
	SetVolume(GetSoundNo(), 0.3f);

	int yoko = 1;
	int tate = 1;

	//�K�v�Ȓ��_���ƃC���f�b�N�X�����v�Z����

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * GetVertexNum();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, GetVerBufp());


	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * GetIndexNum();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, GetIndBufp());


	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(GetVerBuf(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		//�ŏ��ɕ`�悳���ʒu
		float ofsetX = -(100.0f * (1 * 0.5f));
		float ofsetZ = (100.0f * (1 * 0.5f));

		for (int y = 0; y < (tate + 1); y++) {
			for (int x = 0; x < (yoko + 1); x++) {
				float height = frand() * 100;
				// ���_���W�̐ݒ�
				pVtx[y * (yoko + 1) + x].Position = D3DXVECTOR3(ofsetX + (x * 100.0f), 120.0f, ofsetZ - (y * 100.0f));
				// �@���̐ݒ�
				pVtx[y * (yoko + 1) + x].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// ���_�J���[�̐ݒ�
				pVtx[y * (yoko + 1) + x].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		// UV�l�̐ݒ�
		pVtx[0].TexCoord = D3DXVECTOR2((1.0f / 6) * 1, 0.0f);
		pVtx[1].TexCoord = D3DXVECTOR2((1.0f / 6) * 1, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2((1.0f / 6) * 2, 0.0f);
		pVtx[3].TexCoord = D3DXVECTOR2((1.0f / 6) * 2, 1.0f);


		GetDeviceContext()->Unmap(GetVerBuf(), 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(GetIndBuf(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;
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
		}

		GetDeviceContext()->Unmap(GetIndBuf(), 0);
	}
	_frame = 0.0f;
}

//=============================================================================
// �I������
//=============================================================================
void TROUT_BLUE::Uninit(void)
{
	ReleaseIndBuf();

	ReleaseVerBuf();
}

//=============================================================================
// �X�V����
//=============================================================================
void TROUT_BLUE::Update(void)
{
	//�J�E���g�A�b�v����
	_frame += 1.0f;
	//������
	if (_frame > END_FRAME) {
		EraseCoinAll();
		_frame = 0.0f;
		_eventend = true;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void TROUT_BLUE::Draw(void)
{
	SetCullingMode(CULL_MODE_BACK);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, GetVerBufp(), &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	GetDeviceContext()->IASetIndexBuffer(GetIndBuf(), DXGI_FORMAT_R16_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(GetTexNo()));


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(GetMtxWorld());

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, GetScl().x, GetScl().y, GetScl().z);
	D3DXMatrixMultiply(GetMtxWorld(), GetMtxWorld(), &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(GetMtxWorld(), GetMtxWorld(), &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, GetPos().x, -199.0f, GetPos().z);
	D3DXMatrixMultiply(GetMtxWorld(), GetMtxWorld(), &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(GetMtxWorld());

	// �|���S���̕`��
	GetDeviceContext()->DrawIndexed(GetIndexNum(), 0, 0);
}

//�C�x���g
void TROUT_BLUE::Event(){
	//�����Đ�
	PlaySound(GetSoundNo(), 0);

	//�C�x���g�I����Ԃɂ���
	_eventend = false;
	//�R�C�����o��������
	SetCoin();
}
