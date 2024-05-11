//player.h	�v���C���[����
//20106_�c���@�@
//23_12_07
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);


bool GetPlayerUse(void);
void SetPlayerUse(const bool& use);
D3DXVECTOR3 GetPlayerPos();
float GetPlayerRad();
float GetFront(void);
D3DXVECTOR3 GetRot(void);
void SetPlayerHit(const bool& hit);
bool GetPlayerHit(void);
void PlayerLifeDim(void);
int GetPlayerLife(void);
void AddPlayerCoinNum(void);
int GetPlayerCoinNum(void);
void AddPlayerkilledNum(void);
int GetPlayerkilledNum(void);
unsigned int GetEndTime(void);