#include "Essentials.h"

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		// �E�B���h�E���j�����ꂽ
		case WM_DESTROY:
			// OS�ɑ΂��āA�A�v���̏I����`����
			PostQuitMessage(0);
			return 0;
	}

	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}