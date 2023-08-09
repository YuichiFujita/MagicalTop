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
class CWaveManager;	// �E�F�[�u�}�l�[�W���[�N���X
class CPause;		// �|�[�Y�N���X
class CStage;		// �X�e�[�W�N���X
class CPlayer;		// �v���C���[�N���X
class CTarget;		// �^�[�Q�b�g�N���X
class CField;		// �n�ʃN���X
class CScore;		// �X�R�A�N���X
class CTimer;		// �^�C�}�[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[����ʃN���X
class CSceneGame : public CScene
{
public:
	// �R���X�g���N�^
	CSceneGame();

	// �f�X�g���N�^
	~CSceneGame();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CWaveManager	*GetWaveManager(void);	// �E�F�[�u�}�l�[�W���[�擾
	static CPause		*GetPause(void);		// �|�[�Y�擾
	static CStage		*GetStage(void);		// �X�e�[�W�擾
	static CPlayer		*GetPlayer(void);		// �v���C���[�擾
	static CTarget		*GetTarget(void);		// �^�[�Q�b�g�擾
	static CField		*GetField(void);		// �n�ʎ擾
	static CScore		*GetScore(void);		// �X�R�A�擾
	static CTimer		*GetTimer(void);		// �^�C�}�[�擾

private:
	// �ÓI�����o�ϐ�
	static CWaveManager	*m_pWaveManager;	// �E�F�[�u�}�l�[�W���[
	static CPause		*m_pPause;			// �|�[�Y
	static CStage		*m_pStage;			// �X�e�[�W
	static CPlayer		*m_pPlayer;			// �v���C���[�I�u�W�F�N�g
	static CTarget		*m_pTarget;			// �^�[�Q�b�g�I�u�W�F�N�g
	static CField		*m_pField;			// �n�ʃI�u�W�F�N�g
	static CScore		*m_pScore;			// �X�R�A�I�u�W�F�N�g
	static CTimer		*m_pTimer;			// �^�C�}�[�I�u�W�F�N�g
};

#endif	// _SCENE_GAME_H_
