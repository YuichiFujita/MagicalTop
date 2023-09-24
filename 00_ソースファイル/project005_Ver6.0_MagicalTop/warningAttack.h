//============================================================
//
//	�U���x���\���w�b�_�[ [warningAttack.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WARNING_ATTACK_H_
#define _WARNING_ATTACK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �U���x���\���N���X
class CWarningAttack : public CObject2D
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_WARNING = 0,	// �x���\��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	}TEXTURE;

	// ��ԗ�
	typedef enum
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_FADEOUT,	// �t�F�[�h�A�E�g���
		STATE_NORMAL,	// �ʏ���
		STATE_FADEIN,	// �t�F�[�h�C�����
		STATE_MAX		// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CWarningAttack();

	// �f�X�g���N�^
	~CWarningAttack();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CWarningAttack *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize	// �傫��
	);

	// �����o�֐�
	void SetDisp(void);	// �\���ݒ�
	void SetScalingOrigin(const D3DXVECTOR3& rSize);	// ���̑傫���ݒ�

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	D3DXVECTOR3 m_size;		// ���̑傫��
	STATE m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	float m_fScale;			// �g�嗦
};

#endif	// _WARNING_ATTACK_H_
