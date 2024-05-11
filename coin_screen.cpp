//lcoin_screen.cpp		コイン枚数表示処理
//20106_田中　蓮
//23_07_06

#include "coin_screen.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float NUM_WIDTH = 100.0f;
static const float NUM_HEIGHT = 150.0f;

const static int ANIME_PTN_YOKO = 10;
const static int ANIME_PTN_TATE = 1;
const static int ANIME_PTN_WAIT = 5;

const static float ANIME_PTN = ANIME_PTN_YOKO * ANIME_PTN_TATE;
const static float ANIME_PTN_U = 1.0f / ANIME_PTN_YOKO;
const static float ANIME_PTN_V = 1.0f / ANIME_PTN_TATE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int			g_TexNo = 0;		//テクスチャ番号
static int			g_coinnum = 0;		//拾ったコインの枚数

//=============================================================================
// 初期化処理
//=============================================================================
void InitCoinscreen(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/number001.png");

	//拾ったコインの枚数をもらう
	g_coinnum =	GetPlayerCoinNum();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCoinscreen(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCoinscreen(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCoinscreen(void)
{
	int coinnum = g_coinnum;
	//表示
	for (int i = 0; i < 2; i++)	{
		//スコアの描画
		DrawSprite(g_TexNo,
			1500.0f - (NUM_WIDTH * i), 550.0f,
			NUM_WIDTH, NUM_HEIGHT,
			ANIME_PTN_U * (coinnum % 10), 0.0f,//UV値の始点
			ANIME_PTN_U, ANIME_PTN_V);

		coinnum = coinnum / 10;
	}
}
