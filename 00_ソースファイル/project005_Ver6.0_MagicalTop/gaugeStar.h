//============================================================
//
//	���Q�[�W�w�b�_�[ [gaugeStar.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAUGE_STAR_H_
#define _GAUGE_STAR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_STAR	(8)	// ���̍ő吔

//************************************************************
//	�O���錾
//************************************************************
class CObjectBillboard;	// �I�u�W�F�N�g�r���{�[�h�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���Q�[�W�N���X
class CGaugeStar : public CObject
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_STAR = 0,	// ���̃e�N�X�`��
		TEXTURE_FRAME,		// ���g�̃e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_USAGE,		// �Q�[�W�g�p���
		STATE_HEAL,			// �Q�[�W�񕜏��
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CGaugeStar(const int nHealNumGauge, const int nHealWait, const float fMaxRadius);

	// �f�X�g���N�^
	~CGaugeStar();

	// ���\����
	typedef struct
	{
		CObjectBillboard *pBillboard;	// �r���{�[�h���
		int nNumGauge;	// �Q�[�W��
		float fRadius;	// ���a
	}Star;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw);		// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CGaugeStar *Create		// ����
	( // ����
		const int nMaxNumGauge,		// �ő�Q�[�W��
		const float fMaxRadius,		// ���a
		const int nHealNumGauge,	// �Q�[�W�񕜗�
		const int nHealWait,		// �񕜑ҋ@�J�E���g
		CObject *pObject,			// �e�I�u�W�F�N�g
		const D3DXVECTOR3& rGap,	// �\���ʒu�̉��Z��
		const float fDistance,		// ���S����̋���
		const float fFlicker		// �h��߂���
	);

	// �����o�֐�
	bool UseGauge(void);		// �Q�[�W�g�p
	void HealNumGauge(void);	// �Q�[�W�S��
	void SetMaxNumGauge(const int nMax);	// �ő�Q�[�W��
	void SetStar(const int nNumGauge, const float fRadius);	// �����ݒ�
	void SetGapPosition(const D3DXVECTOR3& rGap);	// �\���ʒu�̉��Z�ʐݒ�
	void SetDistance(const float fDistance);		// ���S����̋����ݒ�
	void SetFlicker(const float fFlicker);			// �h��߂��ʐݒ�

private:
	// �����o�֐�
	HRESULT SetDrawInfo(void);	// �`����ݒ�
	void SetParentObject(CObject *pObject);	// �e�I�u�W�F�N�g�ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObjectBillboard *m_apStarFrame[MAX_STAR];	// ���̘g���
	CObjectBillboard *m_apStarBG[MAX_STAR];		// ���̔w�i���
	Star m_aStar[MAX_STAR];		// ���̏��
	STATE m_state;				// ���
	int m_nMaxNumGauge;			// �ő�Q�[�W��
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
	int m_nCurrentStar;			// ���ݏ���̐�
	bool m_bOverheat;			// �I�[�o�[�q�[�g��
	const int m_nHealNumGauge;	// �Q�[�W�񕜗ʒ萔
	const int m_nHealWait;		// �񕜑ҋ@�J�E���g�萔
	const float m_fMaxRadius;	// �ő唼�a�萔

	CObject *m_pParentObject;	// �e�I�u�W�F�N�g
	D3DXVECTOR3 m_posGap;		// �\���ʒu�̉��Z��
	float m_fDistance;			// ���S����̋���
	float m_fFlicker;			// �h��߂���
	float m_fSinRot;			// ����������
	float m_fAddRot;			// ����̐������ւ̌��݉��Z��
	float m_fDestAddRot;		// ����̐������ւ̖ڕW���Z��
};

#endif	// _GAUGE_STAR_H_
