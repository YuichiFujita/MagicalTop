//============================================================
//
//	�^�C�}�[�}�l�[�W���[�w�b�_�[ [timerManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TIMERMANAGER_H_
#define _TIMERMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_MIN		(2)	// �^�C�}�[�̕��̌���
#define MAX_SEC		(2)	// �^�C�}�[�̕b�̌���
#define MAX_MSEC	(3)	// �^�C�}�[�̃~���b�̌���

#define MAX_TIMER	(MAX_MIN + MAX_SEC + MAX_MSEC)	// �^�C�}�[�̌��� (���E�b�E�~���b)

//************************************************************
//	�O���錾
//************************************************************
class CValue;	// �����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�}�[�}�l�[�W���[�N���X
class CTimerManager
{
public:
	// �R���X�g���N�^
	CTimerManager();

	// �f�X�g���N�^
	~CTimerManager();

	// �v����
	typedef enum
	{
		STATE_NONE = 0,	// �����Ȃ�
		STATE_MEASURE,	// �v����
		STATE_END,		// �v���I�� 
		STATE_MAX		// ���̗񋓌^�̑���
	}STATE;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Start(void);	// �v���J�n
	void End(void);		// �v���I��

	void EnableStop(const bool bStop);	// �v����~�ݒ�
	void AddMSec(long nMSec);	// �~���b���Z
	void AddSec(long nSec);		// �b���Z
	void AddMin(long nMin);		// �����Z
	int GetMSec(void);			// �~���b�擾
	int GetSec(void);			// �b�擾
	int GetMin(void);			// ���擾

	// �ÓI�����o�֐�
	static CTimerManager *Create(void);	// ����
	static HRESULT Release(CTimerManager *&prTimerManager);	// �j��

private:
	// �����o�֐�
	void SetTexNum(void);	// �����̃e�N�X�`�����W�ݒ�

	// �����o�ϐ�
	CValue *m_apValue[MAX_TIMER];	// ���l�̏��
	DWORD m_dwStartTime;			// �J�n����
	DWORD m_dwTime;					// �o�ߎ���
	DWORD m_dwStopStartTime;		// ��~�J�n����
	DWORD m_dwStopTime;				// ��~����
	DWORD m_dwTempTime;				// �o�ߎ��Ԃ̌v�Z�p
	STATE m_state;					// �v�����
	bool  m_bStop;					// �v����~��
};

#endif	// _TIMERMANAGER_H_
