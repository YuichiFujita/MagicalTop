//============================================================
//
//	�V�[���w�b�_�[ [scene.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �V�[���N���X
class CScene
{
public:
	// ���[�h��
	enum MODE
	{
		MODE_TITLE = 0,	// �^�C�g�����
		MODE_GAME,		// �Q�[�����
		MODE_RESULT,	// ���U���g���
		MODE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CScene();

	// �f�X�g���N�^
	~CScene();

	// �������z�֐�
	virtual HRESULT Init(void)		= 0;	// ������
	virtual HRESULT Uninit(void)	= 0;	// �I��
	virtual void Draw(void)			= 0;	// �`��

	// ���z�֐�
	virtual void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CScene *Create(MODE mode);			// ����
	static HRESULT Release(CScene *&prScene);	// �j��

	// �����o�֐�
	void SetMode(const MODE mode);	// ���[�h�ݒ�
	MODE GetMode(void) const;		// ���[�h�擾

private:
	// �����o�ϐ�
	MODE m_mode;	// ���[�h
};

#endif	// _SCENE_H_
