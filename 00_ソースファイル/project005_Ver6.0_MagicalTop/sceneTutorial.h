//============================================================
//
//	�`���[�g���A����ʃw�b�_�[ [sceneTutorial.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_TUTORIAL_H_
#define _SCENE_TUTORIAL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CTarget;	// �^�[�Q�b�g�N���X
class CScore;	// �X�R�A�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �`���[�g���A����ʃN���X
class CSceneTutorial : public CScene
{
public:
	// �R���X�g���N�^
	CSceneTutorial(const MODE mode);

	// �f�X�g���N�^
	~CSceneTutorial();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CTarget *GetTarget(void);	// �^�[�Q�b�g�擾
	static CScore *GetScore(void);		// �X�R�A�擾

private:
	// �ÓI�����o�ϐ�
	static CTarget *m_pTarget;	// �^�[�Q�b�g�I�u�W�F�N�g
	static CScore *m_pScore;	// �X�R�A�I�u�W�F�N�g
};

#endif	// _SCENE_TUTORIAL_H_
