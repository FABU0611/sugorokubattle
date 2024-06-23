//trout_end.cpp	終わりのマス
//20106_田中　蓮
//23_12_14
#include "trout_end.h"
#include "texture.h"
#include "renderer.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// 初期化処理
//=============================================================================
void TROUT_END::Init(void){
	//音声読み込み
	SetSound(LoadSound((char*)"data/sound/clear.wav"));
	SetVolume(GetSoundNo(), 0.5f);

	TROUT::Init();
}

void TROUT_END::Event(){
	//音声再生
	PlaySound(GetSoundNo(), 0);
	
	//リザルト画面に移行する
	if (GetFadeState() == FADE_NONE) {
		SceneTransition(SCENE_RESULT, 0.01f);
	}
}

