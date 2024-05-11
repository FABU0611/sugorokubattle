//trout_blue.cpp	コインが出るマス
//20106_田中　蓮
//23_12_14
#include "trout_blue.h"
#include "texture.h"
#include "renderer.h"
#include "coin.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
void TROUT_BLUE::Init(void)
{
	SetPos({ GetPos().x, 0.0f, GetPos().z });
	SetStep(false);
	// 位置・回転・スケールの初期設定
	SetRot({ 0.0f, 0.0f, 0.0f });
	SetScl({ 1.0f, 1.0f, 1.0f });

	// テクスチャ生成
	SetTex(LoadTexture((char*)"data/TEXTURE/trout.png"));

	//音声読み込み
	SetSound(LoadSound((char*)"data/sound/coin_pop.wav"));
	SetVolume(GetSoundNo(), 0.3f);

	int yoko = 1;
	int tate = 1;

	//必要な頂点数とインデックス数を計算する

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * GetVertexNum();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, GetVerBufp());


	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * GetIndexNum();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, GetIndBufp());


	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(GetVerBuf(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		//最初に描画される位置
		float ofsetX = -(100.0f * (1 * 0.5f));
		float ofsetZ = (100.0f * (1 * 0.5f));

		for (int y = 0; y < (tate + 1); y++) {
			for (int x = 0; x < (yoko + 1); x++) {
				float height = frand() * 100;
				// 頂点座標の設定
				pVtx[y * (yoko + 1) + x].Position = D3DXVECTOR3(ofsetX + (x * 100.0f), 120.0f, ofsetZ - (y * 100.0f));
				// 法線の設定
				pVtx[y * (yoko + 1) + x].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// 頂点カラーの設定
				pVtx[y * (yoko + 1) + x].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		// UV値の設定
		pVtx[0].TexCoord = D3DXVECTOR2((1.0f / 6) * 1, 0.0f);
		pVtx[1].TexCoord = D3DXVECTOR2((1.0f / 6) * 1, 1.0f);
		pVtx[2].TexCoord = D3DXVECTOR2((1.0f / 6) * 2, 0.0f);
		pVtx[3].TexCoord = D3DXVECTOR2((1.0f / 6) * 2, 1.0f);


		GetDeviceContext()->Unmap(GetVerBuf(), 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
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
// 終了処理
//=============================================================================
void TROUT_BLUE::Uninit(void)
{
	ReleaseIndBuf();

	ReleaseVerBuf();
}

//=============================================================================
// 更新処理
//=============================================================================
void TROUT_BLUE::Update(void)
{
	//カウントアップして
	_frame += 1.0f;
	//消える
	if (_frame > END_FRAME) {
		EraseCoinAll();
		_frame = 0.0f;
		_eventend = true;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void TROUT_BLUE::Draw(void)
{
	SetCullingMode(CULL_MODE_BACK);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, GetVerBufp(), &stride, &offset);

	// インデックスバッファ設定
	GetDeviceContext()->IASetIndexBuffer(GetIndBuf(), DXGI_FORMAT_R16_UINT, 0);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(GetTexNo()));


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(GetMtxWorld());

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, GetScl().x, GetScl().y, GetScl().z);
	D3DXMatrixMultiply(GetMtxWorld(), GetMtxWorld(), &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(GetMtxWorld(), GetMtxWorld(), &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, GetPos().x, -199.0f, GetPos().z);
	D3DXMatrixMultiply(GetMtxWorld(), GetMtxWorld(), &mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(GetMtxWorld());

	// ポリゴンの描画
	GetDeviceContext()->DrawIndexed(GetIndexNum(), 0, 0);
}

//イベント
void TROUT_BLUE::Event(){
	//音声再生
	PlaySound(GetSoundNo(), 0);

	//イベント終了状態にする
	_eventend = false;
	//コインを出現させる
	SetCoin();
}
