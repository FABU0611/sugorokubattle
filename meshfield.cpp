//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "texture.h"
#include "meshfield.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct FIELD
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	D3DXMATRIX  mtxWorld;
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11Buffer					*g_IndexBuffer = NULL;	// インデックスバッファ

static FIELD	g_Field;	// フィールド情報
static int		g_TexNo;	// テクスチャ番号

static int		g_VertexNum = 0;//頂点数
static int		g_IndexNum = 0;	//インデックス数


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int numBlockX, int numBlockZ, float sizeX, float sizeZ)
{
	// 位置・回転・スケールの初期設定
	g_Field.pos = pos;
	g_Field.rot = rot;
	g_Field.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャ生成
	g_TexNo = LoadTexture((char*)"data/TEXTURE/field000.jpg");


	int yoko = numBlockX;
	int tate = numBlockZ;

	//必要な頂点数とインデックス数を計算する
	g_VertexNum = (yoko + 1) * (tate + 1);
	g_IndexNum = (yoko + 1) * 2 * tate + 2 * (tate - 1);//縮退ポリゴンに必要な数を足す

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * g_VertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * g_IndexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);


	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		//最初に描画される位置
		float ofsetX = -(sizeX * (numBlockX * 0.5f));
		float ofsetZ = (sizeZ * (numBlockZ * 0.5f));

		for (int y = 0; y < (tate + 1); y++) {
			for (int x = 0; x < (yoko + 1); x++) {
				float height = frand() * 100;
				// 頂点座標の設定
				pVtx[y * (yoko + 1) + x].Position = D3DXVECTOR3(ofsetX + (x * sizeX), 120.0f, ofsetZ - (y * sizeZ));
				// UV値の設定
				pVtx[y * (yoko + 1) + x].TexCoord = D3DXVECTOR2(x, y);
				// 法線の設定
				pVtx[y * (yoko + 1) + x].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// 頂点カラーの設定
				pVtx[y * (yoko + 1) + x].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
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
			//縮退ポリゴン分
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
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	//インデックスバッファの解放
	if (g_IndexBuffer != NULL) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	//頂点バッファの解放
	if (g_VertexBuffer != NULL) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	SetCullingMode(CULL_MODE_BACK);
	
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(g_TexNo));


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Field.mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&g_Field.mtxWorld);

	// ポリゴンの描画
	GetDeviceContext()->DrawIndexed(g_IndexNum, 0, 0);
}

