//============================================================
//
//	�V�[�Y���w�b�_�[ [season.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SEASON_H_
#define _SEASON_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �V�[�Y���N���X
class CSeason : public CObjectModel
{
public:
	// ��ޗ�
	typedef enum
	{
		MODEL_NORMAL = 0,	// �ʏ�
		MODEL_MAX			// ���̗񋓌^�̑���
	}MODEL;

	// �R���X�g���N�^
	CSeason();

	// �f�X�g���N�^
	~CSeason();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CSeason *Create(const MODEL model);	// ����

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔
};

#endif	// _SEASON_H_
