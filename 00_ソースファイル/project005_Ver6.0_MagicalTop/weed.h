//============================================================
//
//	���w�b�_�[ [weed.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WEED_H_
#define _WEED_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object3D.h"
#include "waveManager.h"

//************************************************************
//	�O���錾
//************************************************************
class CShadow;	// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CWeed : public CObject3D
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_SPRING = 0,	// �t���e�N�X�`��
		TYPE_SUMMER,		// �đ��e�N�X�`��
		TYPE_AUTUMN,		// �H���e�N�X�`��
		TYPE_WINTER,		// �~���e�N�X�`��
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CWeed();

	// �f�X�g���N�^
	~CWeed();

	// �X�e�[�^�X�\����
	typedef struct
	{
		D3DXVECTOR3 size;	// �傫��
	}StatusInfo;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CWeed *Create	// ����
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rSize	// �傫��
	);
	static void RandomSpawn	// �����_������
	( // ����
		const int nNum,	// ������
		const TYPE type	// ���
	);
	static void SetSeason(const CWaveManager::SEASON season);	// �G�ߐݒ�

	// �����o�֐�
	void SetType(const int nType);	// ��ސݒ�

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];		// �e�N�X�`���萔
	static StatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���

	// �����o�ϐ�
	CShadow *m_pShadow;	// �e�̏��
	TYPE m_type;		// ���
	float m_fSinRot;	// �Ȃт�����
};

#endif	// _WEED_H_
