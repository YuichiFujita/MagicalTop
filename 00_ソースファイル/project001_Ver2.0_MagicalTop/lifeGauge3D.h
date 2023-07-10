//============================================================
//
//	�̗̓Q�[�W3D�w�b�_�[ [lifeGauge3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIFEGAUGE3D_H_
#define _LIFEGAUGE3D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObjectBillboard;	// �I�u�W�F�N�g�r���{�[�h�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �̗̓Q�[�W3D�N���X
class CLifeGauge3D : public CObject
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_BG = 0,		// �e�N�X�`���Ȃ�
		TEXTURE_GAUGE,		// �e�N�X�`���Ȃ�
		TEXTURE_FRAME,		// �̗̓Q�[�W�t���[��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// �̗͕ϓ���
	typedef enum
	{
		STATE_NONE = 0,	// �ʏ���
		STATE_CHANGE,	// �̗͕ϓ����
		STATE_MAX		// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CLifeGauge3D(const int nMax, const int nFrame);

	// �f�X�g���N�^
	~CLifeGauge3D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CLifeGauge3D *Create		// ����
	( // ����
		const int nLife,	// �̗�
		const int nMax,		// �ő�̗�
		const int nFrame,	// �̗͕ϓ��t���[��
		const float fPosUp,	// �̗͕\����Y�ʒu���Z��
		CObject *pObject	// �̗̓Q�[�W�\���I�u�W�F�N�g
	);

	// �����o�֐�
	void AddLife(const int nAdd);				// �̗͉��Z
	void SetLifeObject(CObject *pObject);		// �̗͕\���ݒ�
	void SetLife(const int nLife);				// �̗͐ݒ�
	void SetPositionUp(const float fUp);		// Y�ʒu���Z�ʐݒ�
	const CObject *GetLifeObject(void) const;	// �̗͕\���擾
	int GetLife(void) const;					// �̗͎擾
	float GetPositionUp(void) const;			// Y�ʒu���Z�ʎ擾

private:
	// �����o�ϐ�
	CObjectBillboard *m_apBilboard[TEXTURE_MAX];	// �r���{�[�h�̏��
	CObject *m_pLife;		// �̗̓Q�[�W3D�\���I�u�W�F�N�g�̏��
	STATE m_state;			// ���
	float m_fChange;		// �̗͕ϓ���
	float m_fCurrentLife;	// ���ݑ̗�
	float m_fPosUp;			// �̗͕\����Y�ʒu���Z��
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nLife;			// �̗�
	const int m_nMaxLife;	// �ő�̗͒萔
	const int m_nFrame;		// �̗͕ϓ��t���[���萔

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
};

#endif	// _LIFEGAUGE3D_H_
