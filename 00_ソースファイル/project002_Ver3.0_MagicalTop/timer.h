//============================================================
//
//	�^�C�}�[�w�b�_�[ [timer.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

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
// �^�C�}�[�N���X
class CTimer : public CObject
{
public:
	// �R���X�g���N�^
	CTimer();

	// �f�X�g���N�^
	~CTimer();

	// �v����
	typedef enum
	{
		STATE_NONE = 0,	// �����Ȃ�
		STATE_MEASURE,	// �v����
		STATE_END,		// �v���I�� 
		STATE_MAX		// ���̗񋓌^�̑���
	}STATE;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CTimer *Create(void);	// ����

	// �����o�֐�
	void Start(void);	// �v���J�n
	void End(void);		// �v���I��
	void EnableStop(const bool bStop);	// �v����~�ݒ�

#if 0
	void AddMSec(const int nMSec);	// �~���b���Z
	void AddSec(const int nSec);	// �b���Z
	void AddMin(const int nMin);	// �����Z
#endif

	int GetMSec(void);	// �~���b�擾
	int GetSec(void);	// �b�擾
	int GetMin(void);	// ���擾

private:
	// �����o�֐�
	void SetTexNum(void);	// �����̃e�N�X�`�����W�ݒ�

	// �����o�ϐ�
	CValue *m_apValue[MAX_TIMER];	// ���l�̏��
	DWORD m_dwStartTime;	// �J�n����
	DWORD m_dwTime;			// �o�ߎ���
	DWORD m_dwStopTime;		// ��~����
	STATE m_state;			// �v�����
	bool  m_bStop;			// �v����~��
};

#endif	// _TIMER_H_

// TODO�F�^�C�}�[�����Z�ł���悤�ɂ���
