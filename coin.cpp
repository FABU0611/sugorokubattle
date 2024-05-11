//coin.cpp	コイン処理
//20106_田中　蓮
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
// マクロ定義
//*****************************************************************************
static const float	SIZE_X = 30.0f;					//コインのサイズ(X方向)
static const float	SIZE_Y = 30.0f;					//コインのサイズ(Y方向)

static const int	ERASE_FRAME = 300;				//消えるまでのフレーム数
static const float	COIN_TEX_SIZE = 80.0f;			//テクスチャサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void LerpCoin(void);
void PickupCoin(const int& idx);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点情報

static COIN							g_Coin[NUM_COIN];		// ポリゴンデータ
static int							g_TexNo;				// テクスチャ番号
static int							g_SoundNo;				// テクスチャ番号

static MATERIAL						g_Material;				//マテリアル

//コインが出現する場所
static D3DXVECTOR3 g_CoinPos[NUM_COIN]{
	{ 300.0f, 300.0f, 0.0f},
	{ 400.0f, 300.0f, 300.0f},
	{-200.0f, 300.0f, -100.0f},
	{-300.0f, 300.0f, 400.0f},
	{  50.0f, 300.0f, -200.0f},
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCoin(void)
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
		vertex[0].Position = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
		vertex[1].Position = D3DXVECTOR3(SIZE_X, -SIZE_Y, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-SIZE_X, SIZE_Y, 0.0f);
		vertex[3].Position = D3DXVECTOR3(-SIZE_X, -SIZE_Y, 0.0f);

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
	g_TexNo = LoadTexture((char*)"data/TEXTURE/coin.png");

	//音声読み込み
	g_SoundNo = LoadSound((char*)"data/sound/coin_hit.wav");
	SetVolume(g_SoundNo, 0.3f);

	// マテリアル初期化
	ZeroMemory(&g_Material, sizeof(g_Material));
	g_Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 初期化
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
// 終了処理
//=============================================================================
void UninitCoin(void)
{
	//ポリラインを消す
	for (int i = 0; i < NUM_COIN; i++) {
		ReleasePolyLine(g_Coin[i].polylineID);
	}
	// 頂点バッファの解放
	if (g_VertexBuffer != NULL) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCoin(void)
{
	for (int i = 0; i < NUM_COIN; i++) {
		//使っていないものは更新しない
		if (!GetCoinUse(i)) {
			continue;
		}
		//落ちてくる
		LerpCoin();

		//拾われる
		PickupCoin(i);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCoin(void)
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

	for (int i = 0; i < NUM_COIN; i++)	{
		//使っていないものは描画しない
		if (!GetCoinUse(i)) {
			continue;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Coin[i].mtxWorld);

		CAMERA* cam = GetCamera();

		// ポリゴンを正面に向ける
#if 0
			// ビューマトリックスの逆行列を取得
		g_Coin[i].mtxWorld = cam->mtxInvView;
		g_Coin[i].mtxWorld._41 = 0.0f;
		g_Coin[i].mtxWorld._42 = 0.0f;
		g_Coin[i].mtxWorld._43 = 0.0f;
#else
			// ビューマトリックスを取得
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


		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Coin[i].pos.x,
			g_Coin[i].pos.y,
			g_Coin[i].pos.z);
		D3DXMatrixMultiply(&g_Coin[i].mtxWorld, &g_Coin[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&g_Coin[i].mtxWorld);

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
void SetCoin(void) {
	for (int i = 0; i < NUM_COIN; i++) {
		if (g_Coin[i].isUse) {
			continue;
		}
		//初期位置は出現する位置
		g_Coin[i].pos = g_CoinPos[i];

		//線形補間に使う位置を入れる
		g_Coin[i].spos = g_Coin[i].pos;
		g_Coin[i].epos = g_Coin[i].pos;
		g_Coin[i].epos.y = -20.0f;

		//ポリラインの位置を入れる
		g_Coin[i].polylinepos = g_Coin[i].epos;
		g_Coin[i].polylinepos.y = g_Coin[i].epos.y + 70.0f;

		//使用している状態にする
		g_Coin[i].isUse = true;
	}
}

//-----------------------------------------------------------------------------
//ゲッター
//ポジションを返す
D3DXVECTOR3 GetCoinPos(const int& idx){
	return g_Coin[idx].pos;
}
//半径を返す
float GetCoinRad(void){
	return SIZE_X * 0.5f;
}
//使っているかを返す
bool GetCoinUse(const int& idx){
	return g_Coin[idx].isUse;
}


//必要なものだけ消す
void EraseCoin(const int& idx) {
	if (!g_Coin[idx].isUse) {
		return;
	}
	g_Coin[idx].isUse = false;
	ReleasePolyLine(g_Coin[idx].polylineID);
}
//全部消す
void EraseCoinAll(void) {
	for (int i = 0; i < NUM_COIN; i++) {
		g_Coin[i].lerp_end = false;
		g_Coin[i].isUse = false;
		ReleasePolyLine(g_Coin[i].polylineID);
	}
}

//コインが落ちてくる
void LerpCoin(void) {
	for (int i = 0; i < NUM_COIN; i++) {
		if (g_Coin[i].lerp_end) {
			continue;
		}
		float		t = g_Coin[i].frame / 120.0f;	//ある程度の数で割る
		g_Coin[i].frame += 1.0f;

		D3DXVec3Lerp(&g_Coin[i].pos, &g_Coin[i].spos, &g_Coin[i].epos, easeOutBounce(t));

		if (t >= 1.0f) {
			g_Coin[i].frame = 0.0f;
			g_Coin[i].pos = g_Coin[i].epos;
			g_Coin[i].lerp_end = true;


			//ポリラインのセット
			g_Coin[i].polylineID = SetPolyLine(g_Coin[i].pos, g_Coin[i].polylinepos, 30.0f);
			SetColorPolyLine(g_Coin[i].polylineID, {1.0f, 1.0f, 0.0f, 1.0f});
		}
	}
}

//コインをプレーヤーが拾う
void PickupCoin(const int& idx) {
	if (CheckHitBS(g_Coin[idx].pos, SIZE_X,
		GetPlayerPos(), GetPlayerRad())) {
		AddPlayerCoinNum();
		EraseCoin(idx);
		PlaySound(g_SoundNo, 0);
	}
}