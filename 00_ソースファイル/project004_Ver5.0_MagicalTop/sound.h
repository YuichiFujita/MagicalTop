//============================================================
//
//	�T�E���h�w�b�_�[ [sound.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �T�E���h�N���X
class CSound
{
public:
	// �R���X�g���N�^
	CSound();

	// �f�X�g���N�^
	~CSound();

	// �T�E���h���x����
	typedef enum
	{
		LABEL_BGM_000 = 0,	// BGM (��)
		LABEL_SE_SHOT,		// SE (��)
		LABEL_MAX,			// ���̗񋓌^�̑���
	}LABEL;

	// �T�E���h���\����
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int   nCntLoop;		// ���[�v�J�E���g
	}SOUNDINFO;

	// �����o�֐�
	HRESULT Init(HWND hWnd);	// ������
	void Uninit(void);			// �I��
	HRESULT Play(LABEL label);	// �Đ�
	void Stop(LABEL label);		// ��~
	void Stop(void);			// �S��~

	// �ÓI�����o�֐�
	static CSound *Create(HWND hWnd);			// ����
	static HRESULT Release(CSound *&pSound);	// �j��

private:
	// �����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// �`�����N�`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// �`�����N�f�[�^�Ǎ�

	// �����o�ϐ�
	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice    *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	static const SOUNDINFO mc_aSoundInfo[LABEL_MAX];	// �T�E���h���萔
};

#endif	// _SOUND_H_
