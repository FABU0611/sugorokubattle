//enemy.h	�G����
//20106_�c���@�@
//23_12_18
#pragma once

#include <d3dx9.h>
#include "renderer.h"
#include "model.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class ENEMY {
	static const float SIZE;		//�T�C�Y
	static const float SPEED;		//�ړ����x

	D3DXVECTOR3 _pos;		//�|�W�V����
	D3DXVECTOR3 _shadowpos;	//�e�|�W�V����
	D3DXVECTOR3 _rot;		//�p�x
	D3DXVECTOR3 _dir;		//����
	D3DXVECTOR3 _scl;		//�X�P�[��
				
	float		_speed;		//�ړ����x
	float		_front;		//�����Ă������
				
	int			_shadowID;	//�eID
				
	D3DXMATRIX  _mtxWorld;

	bool		_isUse = false;	//�g���Ă��邩
	DX11_MODEL	_Model;		// ���f���ǂݍ���
	int			_sound_no;

public:
	ENEMY() {}
	ENEMY(D3DXVECTOR3 pos, bool isuse)
	:_pos(pos),_isUse(isuse)
	{}
	~ENEMY() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void DeadEnemy();

	D3DXVECTOR3 GetEnemyPos()const { return _pos; }
	D3DXVECTOR3 GetEnemyShadowPos()const { return _shadowpos; }
	D3DXVECTOR3 GetEnemyRot()const { return _rot; }
	D3DXVECTOR3 GetEnemyDir()const { return _dir; }
	D3DXVECTOR3 GetEnemyScl()const { return _scl; }
	float GetEnemySpeed()const { return _speed; }
	float GetEnemyFront()const { return _front; }
	int GetEnemyShadow()const { return _shadowID; }
	D3DXMATRIX* GetEnemymtxworld() { return &_mtxWorld; }
	bool GetEnemyUse()const { return _isUse; }
	DX11_MODEL* GetEnemyModel() { return &_Model; }
	float GetEnemyRad()const { return SIZE * 18.0f; }

	void SetEnemyPos(const D3DXVECTOR3& pos) { _pos = pos; }
	void SetEnemyShadowPos(const D3DXVECTOR3& shadowpos) { _shadowpos = shadowpos; }
	void SetEnmeyRot(const D3DXVECTOR3& rot) { _rot = rot; }
	void SetEnmeyDir(const D3DXVECTOR3& dir) { _dir = dir; }
	void SetEnmeyScl(const D3DXVECTOR3& scl) { _scl = scl; }
	void SetEnemySpeed(const float& speed) { _speed = speed; }
	void SetEnemyFlont(const float& flont) { _front = flont; }
	void SetEnemyUse(const bool& use) { _isUse = use; }
	void SetEnemyShadowID(const D3DXVECTOR3& pos, const float& size) { _shadowID = SetShadow(pos, size); }
};