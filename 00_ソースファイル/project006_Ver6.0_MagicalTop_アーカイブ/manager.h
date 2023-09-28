//============================================================
//
//	�}�l�[�W���[�w�b�_�[ [manager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CRenderer;		// �����_���[�N���X
class CInputKeyboard;	// �L�[�{�[�h�N���X
class CInputMouse;		// �}�E�X�N���X
class CInputPad;		// �p�b�h�N���X
class CSound;			// �T�E���h�N���X
class CCamera;			// �J�����N���X
class CLight;			// ���C�g�N���X
class CTexture;			// �e�N�X�`���N���X
class CModel;			// ���f���N���X
class CFade;			// �t�F�[�h�N���X
class CGameManager;		// �Q�[���}�l�[�W���[�N���X
class CDebugProc;		// �f�o�b�O�\���N���X

#ifdef _DEBUG	// �f�o�b�O����

class CDebug;	// �f�o�b�O�N���X

#endif	// _DEBUG

//************************************************************
//	�N���X��`
//************************************************************
// �}�l�[�W���[�N���X
class CManager
{
public:
	// �R���X�g���N�^
	CManager();

	// �f�X�g���N�^
	~CManager();

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	// �ÓI�����o�֐�
	static CManager *Create(HINSTANCE hInstance, HWND hWnd);			// ����
	static HRESULT Release(CManager *&prManager);						// �j��
	static void SetScene(const CScene::MODE mode, const int nWait = 0);	// �V�[���ݒ�
	static HRESULT SetMode(const CScene::MODE mode);					// ���[�h�ݒ�
	static CScene::MODE GetMode(void);									// ���[�h�擾

	static CRenderer		*GetRenderer(void);		// �����_���[�擾
	static CInputKeyboard	*GetKeyboard(void);		// �L�[�{�[�h�擾
	static CInputMouse		*GetMouse(void);		// �}�E�X�擾
	static CInputPad		*GetPad(void);			// �p�b�h�擾
	static CSound			*GetSound(void);		// �T�E���h�擾
	static CCamera			*GetCamera(void);		// �J�����擾
	static CLight			*GetLight(void);		// ���C�g�擾
	static CTexture			*GetTexture(void);		// �e�N�X�`���擾
	static CModel			*GetModel(void);		// ���f���擾
	static CFade			*GetFade(void);			// �t�F�[�h�擾
	static CScene			*GetScene(void);		// �V�[���擾
	static CGameManager		*GetGameManager(void);	// �Q�[���}�l�[�W���[�擾
	static CDebugProc		*GetDebugProc(void);	// �f�o�b�O�\���擾

#ifdef _DEBUG	// �f�o�b�O����

	// �ÓI�����o�֐�
	static CDebug *GetDebug(void);	// �f�o�b�O�擾

#endif	// _DEBUG

private:
	// �ÓI�����o�ϐ�
	static CRenderer		*m_pRenderer;		// �����_���[�I�u�W�F�N�g
	static CInputKeyboard	*m_pKeyboard;		// �L�[�{�[�h�I�u�W�F�N�g
	static CInputMouse		*m_pMouse;			// �}�E�X�I�u�W�F�N�g
	static CInputPad		*m_pPad;			// �p�b�h�I�u�W�F�N�g
	static CSound			*m_pSound;			// �T�E���h�I�u�W�F�N�g
	static CCamera			*m_pCamera;			// �J�����I�u�W�F�N�g
	static CLight			*m_pLight;			// ���C�g�I�u�W�F�N�g
	static CTexture			*m_pTexture;		// �e�N�X�`���I�u�W�F�N�g
	static CModel			*m_pModel;			// ���f���I�u�W�F�N�g
	static CFade			*m_pFade;			// �t�F�[�h�I�u�W�F�N�g
	static CScene			*m_pScene;			// �V�[���I�u�W�F�N�g
	static CGameManager		*m_pGameManager;	// �Q�[���}�l�[�W���[
	static CDebugProc		*m_pDebugProc;		// �f�o�b�O�\���I�u�W�F�N�g

#ifdef _DEBUG	// �f�o�b�O����

	// �ÓI�����o�ϐ�
	static CDebug *m_pDebug;	// �f�o�b�O�I�u�W�F�N�g

#endif	// _DEBUG

};

#endif	// _MANAGER_H_
