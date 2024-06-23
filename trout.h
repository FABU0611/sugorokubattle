//trout.h	�}�X���N���X
//20106_�c���@�@
//23_12_08
#pragma once

#include "main.h"
#include "sprite.h"

enum TYPE {
	TYPE_RED,	//�ԁA�G���o��
	TYPE_BLUE,	//�A�R�C�����o��
	TYPE_YELLOW,//���A�߂��̉��F�Ƀ��[�v
	TYPE_WHITE,	//���A�����N����Ȃ�
	TYPE_END,	//���A�I���

	TYPE_NUM,
};


class TROUT {
protected:
	D3DXVECTOR3 _pos;					//�|�W�V����
	D3DXVECTOR3 _rot;					//�p�x
	D3DXVECTOR3 _scl;					//�X�P�[��
	D3DXMATRIX  _mtxWorld;
	ID3D11Buffer* _VertexBuffer = NULL;	// ���_�o�b�t�@
	ID3D11Buffer* _IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@
	TYPE		_type;					//���
	bool		_step = false;			//�����������
	bool		_event = false;			//�C�x���g�𔭐���������
	bool		_eventend = false;		//�C�x���g���I�������
	int			_tex_no = 0;			//�e�N�X�`���ԍ�
	int			_sound_no = 0;			//�T�E���h�ԍ�
	int			_vertexnum = 4;			//���_��
	int			_indexnum = 4;			//�C���f�b�N�X��
	int			_uv = 0;				//UV�l

public:
	TROUT() = default;
	TROUT(D3DXVECTOR3 pos, TYPE type, const int& uv) :
		_pos(pos), _type(type), _event(false), _eventend(false), _uv(uv) {}
	virtual ~TROUT() {
		ReleaseVerBuf();
		ReleaseIndBuf();
	}

	virtual void Init();
	virtual void Uninit();
	virtual void Update() = 0 {}
	virtual void Draw();

	virtual void Event() = 0 {}


	D3DXVECTOR3 GetPos()const { return _pos; }
	D3DXVECTOR3 GetRot()const { return _rot; }
	D3DXVECTOR3 GetScl()const { return _scl; }
	TYPE GetType()const { return _type; }
	bool GetStep()const { return _step; }
	bool GetEvent()const { return _event; }
	bool GetEventEnd()const { return _eventend; }
	int GetTexNo()const { return _tex_no; }
	int GetSoundNo()const { return _sound_no; }
	int GetVertexNum()const { return _vertexnum; }
	int GetIndexNum()const { return _indexnum; }
	D3DXMATRIX* GetMtxWorld() { return &_mtxWorld; }
	ID3D11Buffer* GetVerBuf()const { return _VertexBuffer; }
	ID3D11Buffer** GetVerBufp() { return &_VertexBuffer; }
	ID3D11Buffer* GetIndBuf()const { return _IndexBuffer; }
	ID3D11Buffer** GetIndBufp() { return &_IndexBuffer; }

	void SetPos(const D3DXVECTOR3& pos) { _pos = pos; }
	void SetRot(const D3DXVECTOR3& rot) { _rot = rot; }
	void SetScl(const D3DXVECTOR3& scl) { _scl = scl; }
	void SetType(const TYPE& type) { _type = type; }
	void SetStep(const bool& step) { _step = step; }
	void SetEvent(const bool& event) { _event = event; }
	void SetEventEnd(const bool& eventend) { _eventend = eventend; }
	void SetTex(const int& texno) { _tex_no = texno; }
	void SetSound(const int& soundno) { _sound_no = soundno; }
	void SetUV(const int& uv) { _uv = uv; }

	void ReleaseVerBuf() {
		//���_�o�b�t�@�̉��
		if (_VertexBuffer != NULL) {
			_VertexBuffer->Release();
			_VertexBuffer = NULL;
		}
	}
	void ReleaseIndBuf() {
		//���_�o�b�t�@�̉��
		if (_IndexBuffer != NULL) {
			_IndexBuffer->Release();
			_IndexBuffer = NULL;
		}
	}
};

