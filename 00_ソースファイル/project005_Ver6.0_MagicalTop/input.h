//============================================================
//
//	���̓w�b�_�[ [input.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
// �L�[�{�[�h
#define MAX_NUMKEY	(256)	// �L�[�̍ő吔

// �p�b�h�p
#define MAX_PLAYER	(4)		// �v���C���[�̍ő吔

//************************************************************
//	�N���X��`
//************************************************************
// ���̓N���X
class CInput
{
public:
	// �R���X�g���N�^
	CInput();

	// �f�X�g���N�^
	virtual ~CInput();

	// ���z�֐�
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ������
	virtual void Uninit(void);		// �I��

	// �������z�֐�
	virtual void Update(void) = 0;	// �X�V

protected:
	// �����o�ϐ�
	static int m_nNumAll;	// ���̓f�o�C�X�̑���
	static LPDIRECTINPUT8 m_pInput;		// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8  m_pDevice;	// DirectInput�f�o�C�X
};

// �L�[�{�[�h�N���X
class CInputKeyboard : public CInput
{
public:
	// �R���X�g���N�^
	CInputKeyboard();

	// �f�X�g���N�^
	~CInputKeyboard();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);	// ����
	static HRESULT Release(CInputKeyboard *&pKeyboard);				// �j��

	// �����o�֐�
	bool GetPress(int nKey);	// �v���X�擾
	bool GetTrigger(int nKey);	// �g���K�[�擾
	bool GetRelease(int nKey);	// �����[�X�擾

private:
	// �����o�ϐ�
	BYTE m_aKeyStatePress[MAX_NUMKEY];		// �v���X���
	BYTE m_aKeyStateTrigger[MAX_NUMKEY];	// �g���K�[���
	BYTE m_aKeyStateRelease[MAX_NUMKEY];	// �����[�X���
};

// �}�E�X�N���X
class CInputMouse : public CInput
{
public:
	// �R���X�g���N�^
	CInputMouse();

	// �f�X�g���N�^
	~CInputMouse();

	// �}�E�X�L�[��
	typedef enum
	{
		KEY_LEFT = 0,	// �}�E�X�E�N���b�N
		KEY_RIGHT,		// �}�E�X���N���b�N
		KEY_WHEEL,		// �}�E�X�z�C�[����������
		KEY_MAX			// ���̗񋓌^�̑���
	}KEY;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);	// ����
	static HRESULT Release(CInputMouse *&pMouse);				// �j��

	// �����o�֐�
	bool GetPress(int nMouKey);		// �v���X�擾
	bool GetTrigger(int nMouKey);	// �g���K�[�擾
	bool GetRelease(int nMouKey);	// �����[�X�擾
	D3DXVECTOR3 GetMove(void);		// �ړ��ʎ擾

private:
	// �����o�ϐ�
	DIMOUSESTATE m_mouseStatePress;		// �}�E�X�̃v���X���
	DIMOUSESTATE m_mouseStateTrigger;	// �}�E�X�̃g���K�[���
	DIMOUSESTATE m_mouseStateRelease;	// �}�E�X�̃����[�X���
};

// �p�b�h�N���X
class CInputPad
{
public:
	// �R���X�g���N�^
	CInputPad();

	// �f�X�g���N�^
	~CInputPad();

	// �p�b�h�L�[��
	typedef enum
	{
		KEY_UP = 0,		// �����L�[ ��
		KEY_DOWN,		// �����L�[ ��
		KEY_LEFT,		// �����L�[ ��
		KEY_RIGHT,		// �����L�[ �E
		KEY_START,		// START�{�^��
		KEY_BACK,		// BACK�{�^��
		KEY_LSTICKPUSH,	// ���X�e�B�b�N��������
		KEY_RSTICKPUSH,	// �E�X�e�B�b�N��������
		KEY_L1,			// LB�{�^��
		KEY_R1,			// RB�{�^��
		KEY_L2,			// LT�{�^��
		KEY_R2,			// RT�{�^��
		KEY_A,			// A�{�^��
		KEY_B,			// B�{�^��
		KEY_X,			// X�{�^��
		KEY_Y,			// Y�{�^��
		KEY_LSTICK_X,	// ���X�e�B�b�N ���E
		KEY_LSTICK_Y,	// ���X�e�B�b�N �㉺
		KEY_RSTICK_X,	// �E�X�e�B�b�N ���E
		KEY_RSTICK_Y,	// �E�X�e�B�b�N �㉺
		KEY_MAX			// ���̗񋓌^�̑���
	}KEY;
	
	// �U����ޗ�
	typedef enum
	{
		TYPE_NONE = 0,		// �������Ă��Ȃ�
		TYPE_DAMAGE,		// �_���[�W
		TYPE_BIG_DAMAGE,	// ��_���[�W
		TYPE_DEATH,			// ���S
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �U���\����
	typedef struct
	{
		_XINPUT_VIBRATION vibration;	// �o�C�u���
		TYPE type;	// �o�C�u���
		int  nTime;	// �o�C�u����
	}VIBRATION;

	// �ÓI�����o�֐�
	static CInputPad *Create(void);				// ����
	static HRESULT Release(CInputPad *&pPad);	// �j��

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetVibration(TYPE type, int nPlayer = 0);	// �o�C�u�ݒ�
	bool GetPress(KEY nJoyKey, int nPlayer = 0);	// �v���X�擾 (�{�^��)
	SHORT GetPressLStickX(int nPlayer = 0);			// �v���X�擾 (L�X�e�B�b�NX)
	SHORT GetPressLStickY(int nPlayer = 0);			// �v���X�擾 (L�X�e�B�b�NY)
	SHORT GetPressRStickX(int nPlayer = 0);			// �v���X�擾 (R�X�e�B�b�NX)
	SHORT GetPressRStickY(int nPlayer = 0);			// �v���X�擾 (R�X�e�B�b�NY)
	float GetPressLStickRot(int nPlayer = 0);		// �v���X�擾 (L�X�e�B�b�N����)
	float GetPressRStickRot(int nPlayer = 0);		// �v���X�擾 (R�X�e�B�b�N����)
	bool GetTrigger(KEY nJoyKey, int nPlayer = 0);	// �g���K�[�擾 (�{�^��)
	bool GetRelease(KEY nJoyKey, int nPlayer = 0);	// �����[�X�擾 (�{�^��)

private:
	// �����o�֐�
	void UpdateVibration(VIBRATION *pVibration, int nID);	// �o�C�u���[�V�����X�V

	// �����o�ϐ�
	VIBRATION    m_aVibration[MAX_PLAYER];			// �p�b�h�̃o�C�u���
	XINPUT_STATE m_aJoyKeyStatePress[MAX_PLAYER];	// �p�b�h�̃v���X���
	XINPUT_STATE m_aJoyKeyStateTrigger[MAX_PLAYER];	// �p�b�h�̃g���K�[���
	XINPUT_STATE m_aJoyKeyStateRelease[MAX_PLAYER];	// �p�b�h�̃����[�X���
};

#endif	// _INPUT_H_
