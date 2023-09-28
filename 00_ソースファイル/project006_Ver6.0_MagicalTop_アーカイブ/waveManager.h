//============================================================
//
//	�E�F�[�u�}�l�[�W���[�w�b�_�[ [waveManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WAVEMANAGER_H_
#define _WAVEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "enemy.h"
#include "gameManager.h"

//************************************************************
//	�}�N����`
//************************************************************
#define NUM_MODEL_UI	(2)	// ���f��UI�̎g�p��

//************************************************************
//	�O���錾
//************************************************************
class CModelUI;		// ���f��UI�N���X
class CObject2D;	// �I�u�W�F�N�g2D�N���X
class CValue;		// �����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �E�F�[�u�}�l�[�W���[�N���X
class CWaveManager
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// �E�F�[�u
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// ��ԗ�
	typedef enum
	{
		STATE_NONE = 0,				// �������Ȃ����
		STATE_SEASON_START_INIT,	// �G�߂̊J�n���������
		STATE_SEASON_START,			// �G�߂̊J�n���
		STATE_WAVE_START_INIT,		// �E�F�[�u�J�n���������
		STATE_WAVE_START,			// �E�F�[�u�J�n���
		STATE_PROGRESSION,			// �E�F�[�u�i�s���
		STATE_SEASON_END_INIT,		// �G�߂̏I�����������
		STATE_SEASON_END,			// �G�߂̏I�����
		STATE_WAIT,					// ���G�߂̊J�n�ҋ@���
		STATE_END,					// �I�����
		STATE_MAX					// ���̗񋓌^�̑���
	}STATE;

	// ���f����
	typedef enum
	{
		MODEL_SEASON = 0,	// SEASON�t�H���g
		MODEL_END,			// END�t�H���g
		MODEL_MAX,			// ���̗񋓌^�̑���
	}MODEL;

	// �G�ߗ�
	typedef enum
	{
		SEASON_SPRING = 0,	// �t
		SEASON_SUMMER,		// ��
		SEASON_AUTUMN,		// �H
		SEASON_WINTER,		// �~
		SEASON_MAX			// ���̗񋓌^�̑���
	}SEASON;

	// �R���X�g���N�^
	CWaveManager();

	// �f�X�g���N�^
	~CWaveManager();

	// �o���\����
	typedef struct
	{
		int aNumSpawn[CEnemy::TYPE_MAX];	// ������
		int nFrame;		// �P�\�t���[��
	}Point;

	// �E�F�[�u�\����
	typedef struct
	{
		Point *pPoint;	// �o�����
		int nNumPoint;	// �o����
	}Wave;

	// �G�ߍ\����
	typedef struct
	{
		Wave *pWave;	// �E�F�[�u���
		int nNumWave;	// �E�F�[�u��
	}Season;

	// �����o�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void NextSeason(void);	// �G�߈ڍs
	STATE GetState(void);	// ��Ԏ擾

	// �ÓI�����o�֐�
	static void LoadSetup(void);		// �Z�b�g�A�b�v
	static CWaveManager *Create(void);	// ����
	static HRESULT Release(CWaveManager *&prWaveManager);	// �j��

private:
	// �����o�֐�
	void ResultTransition(const CGameManager::RESULT result, const int nWait = 0);	// ���U���g�J��
	void SetSeason(const SEASON season);	// �G�ߐݒ�
	void InitSeasonStart(void);		// �G�߂̊J�n������
	void UpdateSeasonStart(void);	// �G�߂̊J�n
	void InitWaveStart(void);		// �E�F�[�u�J�n������
	void UpdateWaveStart(void);		// �E�F�[�u�J�n
	void UpdateProgression(void);	// �E�F�[�u�i�s
	void InitWaveEnd(void);			// �G�߂̏I��������
	void UpdateWaveEnd(void);		// �G�߂̏I��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];		// �e�N�X�`���萔
	static const char *mc_apModelFile[];		// ���f���萔
	static const char *mc_apSeasonModelFile[];	// �V�[�Y���\�L���f���萔
	static Season m_aWaveInfo[SEASON_MAX];		// �E�F�[�u���

	// �����o�ϐ�
	CModelUI *m_apModelUI[NUM_MODEL_UI];	// �V�[�Y���\���̏��
	CObject2D *m_pObject2D;	// �E�F�[�u�\���̏��
	CValue *m_pValue;		// �E�F�[�u���\���̏��
	STATE m_state;			// ���
	int m_nSeason;			// �G�ߊǗ��J�E���^�[
	int m_nWave;			// �E�F�[�u�Ǘ��J�E���^�[
	int m_nPoint;			// �o���Ǘ��J�E���^�[
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nCounterFrame;	// �E�F�[�u�]�C�Ǘ��J�E���^�[
};

#endif	// _WAVEMANAGER_H_
