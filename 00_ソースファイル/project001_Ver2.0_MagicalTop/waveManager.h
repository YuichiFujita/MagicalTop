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

//************************************************************
//	�O���錾
//************************************************************
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
		STATE_NONE = 0,			// �������Ȃ����
		STATE_SEASON_START,		// �G�߂̊J�n���
		STATE_WAVE_START_INIT,	// �E�F�[�u�J�n���������
		STATE_WAVE_START,		// �E�F�[�u�J�n���
		STATE_PROGRESSION,		// �E�F�[�u�i�s���
		STATE_SEASON_END,		// �G�߂̏I�����
		STATE_WAIT,				// ���G�߂̊J�n�ҋ@���
		STATE_END,				// �I�����
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

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
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static void LoadSetup(void);		// �Z�b�g�A�b�v
	static CWaveManager *Create(void);	// ����
	static HRESULT Release(CWaveManager *&prWaveManager);	// �j��

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
	static Season m_aWaveInfo[SEASON_MAX];	// �E�F�[�u���

	// �����o�ϐ�
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
