//trout_end.cpp	�I���̃}�X
//20106_�c���@�@
//23_12_14
#include "trout_end.h"
#include "texture.h"
#include "renderer.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// ����������
//=============================================================================
void TROUT_END::Init(void){
	//�����ǂݍ���
	SetSound(LoadSound((char*)"data/sound/clear.wav"));
	SetVolume(GetSoundNo(), 0.5f);

	TROUT::Init();
}

void TROUT_END::Event(){
	//�����Đ�
	PlaySound(GetSoundNo(), 0);
	
	//���U���g��ʂɈڍs����
	if (GetFadeState() == FADE_NONE) {
		SceneTransition(SCENE_RESULT, 0.01f);
	}
}

