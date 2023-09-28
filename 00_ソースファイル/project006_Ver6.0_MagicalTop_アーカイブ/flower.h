//============================================================
//
//	�}�i�t�����[�w�b�_�[ [flower.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FLOWER_H_
#define _FLOWER_H_

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
// �}�i�t�����[�N���X
class CFlower : public CObject3D
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_SPRING = 0,	// �t�}�i�t�����[�e�N�X�`��
		TYPE_SUMMER,		// �ă}�i�t�����[�e�N�X�`��
		TYPE_AUTUMN,		// �H�}�i�t�����[�e�N�X�`��
		TYPE_WINTER,		// �~�}�i�t�����[�e�N�X�`��
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CFlower();

	// �f�X�g���N�^
	~CFlower();

	// �X�e�[�^�X�\����
	typedef struct
	{
		D3DXVECTOR3 size;	// �傫��
		int nLife;			// ����
	}StatusInfo;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CFlower *Create	// ����
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rSize,	// �傫��
		const int nLife				// �̗�
	);
	static void RandomSpawn	// �����_������
	( // ����
		const int nNum,	// ������
		const TYPE type	// ���
	);
	static void SetSeason(const CWaveManager::SEASON season);	// �G�ߐݒ�
	static int GetNumAll(void);		// �����擾

	// �����o�֐�
	void SetType(const int nType);	// ��ސݒ�
	void SetLife(const int nLife);	// �̗͐ݒ�

private:
	// �����o�֐�
	bool CollisionPlayer(const D3DXVECTOR3& rPos);	// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];		// �e�N�X�`���萔
	static StatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���
	static int m_nNumAll;						// �}�i�t�����[�̑���

	// �����o�ϐ�
	CShadow *m_pShadow;		// �e�̏��
	STATE m_state;			// ���
	TYPE m_type;			// ���
	int m_nLife;			// �̗�
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
};

#endif	// _FLOWER_H_
