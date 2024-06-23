//trout_blue.cpp	コインが出るマス
//20106_田中　蓮
//23_12_14
#include "trout_blue.h"
#include "texture.h"
#include "renderer.h"
#include "coin.h"
#include "sound.h"

//=============================================================================
// 初期化処理
//=============================================================================
void TROUT_BLUE::Init(void){
	//音声読み込み
	SetSound(LoadSound((char*)"data/sound/coin_pop.wav"));
	SetVolume(GetSoundNo(), 0.3f);

	TROUT::Init();

	_frame = 0.0f;
}

//=============================================================================
// 更新処理
//=============================================================================
void TROUT_BLUE::Update(void){
	//カウントアップして
	_frame += 1.0f;
	//消える
	if (_frame > END_FRAME) {
		EraseCoinAll();
		_frame = 0.0f;
		_eventend = true;
	}
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
