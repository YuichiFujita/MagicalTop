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

//************************************************************
//	�N���X��`
//************************************************************
// �E�F�[�u�}�l�[�W���[�N���X
class CWaveManager
{
public:
	// ��ԗ�
	typedef enum
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_SEASON_START,	// �G�߂̊J�n���
		STATE_WAVE_START,	// �E�F�[�u�J�n���
		STATE_PROGRESSION,	// �G�߂̐i�s���
		STATE_SEASON_END,	// �G�߂̏I�����
		STATE_WAIT,			// ���G�߂̊J�n�ҋ@���
		STATE_MAX			// ���̗񋓌^�̑���
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

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CWaveManager *Create(void);	// ����
	static HRESULT Release(CWaveManager *&prWaveManager);	// �j��

private:
	// �����o�ϐ�
	STATE m_state;				// ���
	SEASON m_nSeasonCounter;	// �G�߃J�E���^�[
	int m_nWaveCounter;			// �E�F�[�u�J�E���^�[
};

#endif	// _WAVEMANAGER_H_
