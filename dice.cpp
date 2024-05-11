//dice.cpp		サイコロ処理
//20106_田中　蓮 
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
// マクロ定義
//*****************************************************************************
static const float VALUE_MOVE = 5.0f;							// 移動量

static const float SIZE_X = 50.0f;								//サイズ(X方向)
static const float SIZE_Y = 50.0f;								//サイズ(Y方向)
static const float SIZE_Z = 50.0f;								//サイズ(Z方向)

static const float ROOL_FRAME = 30.0f;							//目が変わるフレーム数

static const int   SIDE_NUM = 6;								//サイコロの面の数
static const int   VERTEX_NUM = 4;								//頂点数
static const int   SIZE_VERTEX_NUM = SIDE_NUM * VERTEX_NUM;		//サイコロの頂点数


static const int   INDEX_NUM = 36;								//インデックスの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexDice(void);
void HitToBullet(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
//サウンド用
enum SOUND_NUM {
	rool,
	hit,

	sound_max,
};
static ID3D11Buffer					*g_VertexBuffer = NULL;	//頂点情報
static ID3D11Buffer					*g_IndexBuffer = NULL;	//インデックス情報

static DICE							g_Dice;					//ポリゴンデータ
static int							g_TexNo;				//テクスチャ番号
static int							g_SoundNo[sound_max];	//サウンド番号


// 頂点配列
static VERTEX_3D g_VertexArray[SIZE_VERTEX_NUM] = {
	//1の面 上
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 1.0f)},
	
	//2の面　手前
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.166f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 1.0f)},
	
	//3の面　右
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 0.0f)},
	{D3DXVECTOR3( SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.333f, 1.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 1.0f)},
	
	//4の面　左
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y,  SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, -SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 1.0f)},
	
	//5の面　奥
	{D3DXVECTOR3( SIZE_X,  SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X,  SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.666f, 1.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 0.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, SIZE_Z), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 1.0f)},
	
	//6の面　下
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y, -SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.833f, 1.0f)},
	{D3DXVECTOR3(-SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f)},
	{D3DXVECTOR3( SIZE_X, -SIZE_Y,  SIZE_Z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f)},
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDice(void)
{
	// 頂点バッファの作成
	MakeVertexDice();

	// テクスチャ生成
	g_TexNo = LoadTexture((char*)"data/TEXTURE/dice.png");

	//サウンドの読み込み
	g_SoundNo[rool] = LoadSound((char*)"data/sound/dice_rool.wav");
	SetVolume(g_SoundNo[rool], 0.3f);
	g_SoundNo[hit] = LoadSound((char*)"data/sound/dice_hit.wav");
	SetVolume(g_SoundNo[hit], 0.4f);

	// 位置・回転・スケールの初期設定
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
// 終了処理
//=============================================================================
void UninitDice(void)
{
	// インデックスバッファの解放
	if (g_IndexBuffer != NULL) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
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
void UpdateDice(void)
{
	if (g_Dice.stop) {
		return;
	}
	//弾との当たり判定処理
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
// 描画処理
//=============================================================================
void DrawDice(void)
{
	SetLightEnable(false);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(g_TexNo));


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	//行列、4x4
	D3DXMatrixIdentity(&g_Dice.mtxWorld);	//単位行列を作る関数

		// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_Dice.scl.x, g_Dice.scl.y, g_Dice.scl.z);
		//行列の掛け算をしてくれる関数　引数①＝引数②　＊　引数③
	D3DXMatrixMultiply(&g_Dice.mtxWorld, &g_Dice.mtxWorld, &mtxScl);

	// 回転を反映										yヨー			xピッチ			zロール
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Dice.rot.y, g_Dice.rot.x, g_Dice.rot.z);
	//拡大縮小が入った状態で
	//行列の掛け算をしてくれる関数　引数①＝引数②　＊　引数③
	D3DXMatrixMultiply(&g_Dice.mtxWorld, &g_Dice.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_Dice.pos.x, g_Dice.pos.y, g_Dice.pos.z);
	D3DXMatrixMultiply(&g_Dice.mtxWorld, &g_Dice.mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&g_Dice.mtxWorld);

	// ポリゴン描画
	//GetDeviceContext()->Draw(SIZE_VERTEX_NUM, 0);


	//マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	//インデックス付きポリゴン描画
	GetDeviceContext()->DrawIndexed(INDEX_NUM, 0, 0);

	SetLightEnable(true);
}

HRESULT MakeVertexDice(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;;
	bd.ByteWidth = sizeof(VERTEX_3D) * SIZE_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// グローバルの頂点配列から頂点バッファをつくる
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = g_VertexArray;

	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);


	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned short) * INDEX_NUM;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//バッファ生成時にあらかじめ埋めるデータを設定できる（なくてもOK）
	ZeroMemory(&sd, sizeof(sd));
	//インデックスデータの配列を作る
	unsigned short indexArray[] =
	{
		//上
		0,1,2,
		1,3,2,
		//手前
		4,5,6,
		5,7,6,
		//右
		8,9,10,
		9,11,10,		
		//左
		12,13,14,
		13,15,14,
		//奥
		16,17,18,
		17,19,18,
		//下
		20,21,22,
		21,23,22,

		//頂点に法線の情報、UV値の情報が入っているので限界でこれ
	};

	sd.pSysMem = indexArray;
	//インデックスバッファの作成
	GetDevice()->CreateBuffer(&bd, &sd, &g_IndexBuffer);


	return S_OK;
}

void RooltheDice(void) {
	// 乱数生成器の初期化
	srand(static_cast<unsigned int>(time(nullptr)));

	//サイコロを振る
	g_Dice.deme = rand() % 6 + 1;
}

//出目を渡す
int GetDeme(void) {
	return g_Dice.deme;
}

//止まった状態にする
void DiceSetStop(const bool& stop) {
	g_Dice.stop = stop;
}

//弾との当たり判定
void HitToBullet(void) {
	//弾との当たり判定
	for (int b = 0; b < NUM_BULLET; b++) {
		if (!GetBulletUse(b)) {
			continue;
		}
		if (CheckHitBS(g_Dice.pos, g_Dice.rad,
			GetBulletPos(b), GetBulletRad())) {
			//弾を消す
			EraseBullet(b);
			//目を出す
			DiceSetStop(true);
			//出目の分進めるようにする
			SetMovenum(GetDeme());
			SetCanMove(true);
			//目が出たときの音を再生
			PlaySound(g_SoundNo[hit], 0);
		}
	}
}