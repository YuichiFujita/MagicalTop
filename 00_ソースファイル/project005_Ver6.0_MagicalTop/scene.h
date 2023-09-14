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
//	�O���錾
//************************************************************
class CField;	// �n�ʃN���X

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
	CScene(const MODE mode);

	// �f�X�g���N�^
	~CScene();

	// �������z�֐�
	virtual void Draw(void)	= 0;	// �`��

	// ���z�֐�
	virtual HRESULT Init(void);		// ������
	virtual HRESULT Uninit(void);	// �I��
	virtual void Update(void);		// �X�V

	// �ÓI�����o�֐�
	static CScene *Create(MODE mode);			// ����
	static HRESULT Release(CScene *&prScene);	// �j��

	static HRESULT CreateField(MODE mode);	// �n�ʐ���
	static CField *GetField(void);			// �n�ʎ擾

	// �����o�֐�
	void SetMode(const MODE mode);	// ���[�h�ݒ�
	MODE GetMode(void) const;		// ���[�h�擾

private:
	// �ÓI�����o�ϐ�
	static CField *m_pField;	// �n�ʃI�u�W�F�N�g

	// �����o�ϐ�
	MODE m_mode;	// ���[�h
};

#endif	// _SCENE_H_
