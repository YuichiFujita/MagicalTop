//============================================================
//
//	�Q�[����ʃw�b�_�[ [sceneGame.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CWaveManager;		// �E�F�[�u�}�l�[�W���[�N���X
class CTimerManager;	// �^�C�}�[�}�l�[�W���[�N���X
class CPause;			// �|�[�Y�N���X
class CTarget;			// �^�[�Q�b�g�N���X
class CScore;			// �X�R�A�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[����ʃN���X
class CSceneGame : public CScene
{
public:
	// �R���X�g���N�^
	CSceneGame(const MODE mode);

	// �f�X�g���N�^
	~CSceneGame();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CWaveManager		*GetWaveManager(void);	// �E�F�[�u�}�l�[�W���[�擾
	static CTimerManager	*GetTimerManager(void);	// �^�C�}�[�}�l�[�W���[�擾
	static CPause			*GetPause(void);		// �|�[�Y�擾
	static CTarget			*GetTarget(void);		// �^�[�Q�b�g�擾
	static CScore			*GetScore(void);		// �X�R�A�擾

private:
	// �ÓI�����o�ϐ�
	static CWaveManager		*m_pWaveManager;	// �E�F�[�u�}�l�[�W���[
	static CTimerManager	*m_pTimerManager;	// �^�C�}�[�}�l�[�W���[
	static CPause			*m_pPause;			// �|�[�Y
	static CTarget			*m_pTarget;			// �^�[�Q�b�g�I�u�W�F�N�g
	static CScore			*m_pScore;			// �X�R�A�I�u�W�F�N�g
};

#endif	// _SCENE_GAME_H_
