//pushspace.h	�v�b�V���X�y�[�X�\���Ǘ�
//20106_�c���@
//23_09_22
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPushspace(void);
void UninitPushspace(void);
void UpdatePushspace(void);
void DrawPushspace(void);

void LerpPush(void);
bool GetEndLerp(void);
