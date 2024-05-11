//bullet.cpp	弾表示処理
//20106_田中　蓮
//23_12_07
#include "main.h"
#include "camera.h"
#include "bullet.h"
#include "texture.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float	BULLET_APPED = 7.0f;					// 移動量

static const float	BULLET_SIZE_X = 30.0f;					// バレットのサイズ(X方向)
static const float	BULLET_SIZE_Y = 30.0f;					// バレットのサイズ(Y方向)

static const int	ERASE_FRAME = 240;						//消えるまでのフレーム数
static const float	BULLET_TEX_SIZE = 80.0f;				//テクスチャサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報

static BULLET						g_Bullet[NUM_BULLET];	// ポリゴンデータ
static int							g_TexNo;				// テクスチャ番号
static int							g_SoundNo;				// テクスチャ番号

static MATERIAL						g_Material;				//マテリアル
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 頂点バッファに値をセットする
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float width = 50.0f;
		float height = 50.0f;

		// 頂点座標の設定
		vertex[0].Position = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
		vertex[1].Position = D3DXVECTOR3(BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
		vertex[3].Position = D3DXVECTOR3(-BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);

		// 頂点カラーの設定
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	// テクスチャ生成
	g_TexNo = LoadTexture((char*)"data/TEXTURE/bullet.png");

	//サウンド読み込み
	g_SoundNo = LoadSound((char*)"data/sound/player_shot.wav");
	SetVolume(g_SoundNo, 0.2f);

	// マテリアル初期化
	ZeroMemory(&g_Material, sizeof(g_Material));
	g_Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 初期化
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
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	if (g_VertexBuffer != NULL)
	{// 頂点バッファの解放
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	for (int i = 0; i < NUM_BULLET; i++) {
		//使っていない弾は更新しない
		if (!GetBulletUse(i)) {
			continue;
		}
		CAMERA* camera = GetCamera();

		//進み方向を決める
		if (!g_Bullet[i].isShot) {
			g_Bullet[i].rot.y = sinf(D3DX_PI) + GetFront();
			g_Bullet[i].isShot = true;
		}

		//座標更新
		g_Bullet[i].pos.x += sinf(g_Bullet[i].rot.y) * g_Bullet[i].speed;
		g_Bullet[i].pos.z += cosf(g_Bullet[i].rot.y) * g_Bullet[i].speed;

		//影の座標更新
		D3DXVECTOR3 shadowpos = g_Bullet[i].pos;
		shadowpos.y = -75.0f;
		SetPositionShadow(g_Bullet[i].shadowID, shadowpos);
		SetColorShadow(g_Bullet[i].shadowID, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		//時間で消える処理
		g_Bullet[i].frame++;
		if (g_Bullet[i].frame > ERASE_FRAME) {
			EraseBullet(i);
			g_Bullet[i].frame = 0;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	// αテストを有効に
	SetAlphaTestEnable(true);

	// ライティングを無効
	SetLightEnable(false);

	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for (int i = 0; i < NUM_BULLET; i++)	{
		//使っていないものは描画しない
		if (!GetBulletUse(i)) {
			continue;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Bullet[i].mtxWorld);

		CAMERA* cam = GetCamera();

		// ポリゴンを正面に向ける
#if 0
			// ビューマトリックスの逆行列を取得
		g_Bullet[i].mtxWorld = cam->mtxInvView;
		g_Bullet[i].mtxWorld._41 = 0.0f;
		g_Bullet[i].mtxWorld._42 = 0.0f;
		g_Bullet[i].mtxWorld._43 = 0.0f;
#else
			// ビューマトリックスを取得
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


		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].pos.x,
			g_Bullet[i].pos.y,
			g_Bullet[i].pos.z);
		D3DXMatrixMultiply(&g_Bullet[i].mtxWorld, &g_Bullet[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&g_Bullet[i].mtxWorld);

		// マテリアル設定
		SetMaterial(g_Material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(g_TexNo));

		// ポリゴンの描画
		GetDeviceContext()->Draw(4, 0);

	}

	// ライティングを有効に
	SetLightEnable(true);

	// αテストを無効に
	SetAlphaTestEnable(false);
}

//-----------------------------------------------------------------------------
//セッター
void SetBullet(const D3DXVECTOR3& pos) {
	for (int i = 0; i < NUM_BULLET; i++) {
		if (g_Bullet[i].isUse) {
			continue;
		}
		//撃った時の音を再生
		PlaySound(g_SoundNo, 0);

		g_Bullet[i].pos = pos;
		g_Bullet[i].isUse = true;

		//影のセット
		g_Bullet[i].shadowID = SetShadow(g_Bullet[i].pos, 100.0f);

		break;
	}
}

//-----------------------------------------------------------------------------
//ゲッター

//ポジションを返す
D3DXVECTOR3 GetBulletPos(const int& idx){
	return g_Bullet[idx].pos;
}
//半径を返す
float GetBulletRad(void){
	return BULLET_SIZE_X * 0.5f;
}
//使っているかを返す
bool GetBulletUse(const int& idx){
	return g_Bullet[idx].isUse;
}


//消す
void EraseBullet(const int& idx) {
	if (!g_Bullet[idx].isUse) {
		return;
	}
		g_Bullet[idx].isUse = false;
		g_Bullet[idx].isShot = false;
		g_Bullet[idx].rot.y = 0.0f;
		ReleaseShadow(g_Bullet[idx].shadowID);
}