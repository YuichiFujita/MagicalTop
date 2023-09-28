//============================================================
//
//	�Q�[���}�l�[�W���[�w�b�_�[ [gameManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���}�l�[�W���[�N���X
class CGameManager
{
public:
	// ���U���g��
	enum RESULT
	{
		RESULT_NONE,	// ����
		RESULT_FAILED,	// �N���A���s
		RESULT_CLEAR,	// �N���A����
		RESULT_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGameManager();

	// �f�X�g���N�^
	~CGameManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CGameManager *Create(void);	// ����
	static HRESULT Release(CGameManager *&prGameManager);	// �j��

	// �����o�֐�
	void SetResult(const RESULT result);	// �N���A�󋵐ݒ�
	void SetScore(const int nScore);		// �l���X�R�A�ݒ�
	void SetTime(const long nTime);			// �o�߃^�C���ݒ�
	RESULT GetResult(void) const;			// �N���A�󋵎擾
	int GetScore(void) const;				// �l���X�R�A�擾
	long GetTime(void) const;				// �o�߃^�C���擾

private:
	// �����o�ϐ�
	RESULT m_result;	// �N���A��
	int m_nScore;		// �l���X�R�A
	long m_nTime;		// �o�߃^�C��
};

#endif	// _GAMEMANAGER_H_
