// --- ������������ ���������� ����
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <string>

#include "resource.h"

// --- �������� ������� �������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- ���������� ����������
HINSTANCE hInst; 		// ���������� ���������� ����������
WCHAR ClassName[] = L"Window"; 		// �������� ������ ����
WCHAR AppTitle[] = L"Application Win32"; 	// ��������� �������� ����
UINT IdTimer = 0;

int WINAPI WinMain(HINSTANCE hInstance,
	// ���������� ���������� ����������
	HINSTANCE hPrevInstance, // � Win32 ������ ����� NULL
	LPSTR lpCmdLine,
	// ��������� �� ��������� ������. �� // ���������
	// ���������� �������� ������ �� ��������� ������.
	int nCmdShow
	// ����������, ��� ���������� ������������� 
	// ������������ �� �������: ������������
	// (nCmdShow = SW_SHOWMINNOACTIVE) 
	// ��� � ���� ��������� ���� 					//(nCmdShow = SW_SHOWNORMAL).
)
{
	WNDCLASS wc; 	// ��������� ��� ���������� � ������ ����
	HWND hWnd; 	// ���������� �������� ���� ����������
	MSG msg; 	// ��������� ��� �������� ���������
// ��������� ���������� ���������� ���������� � ����������
// ����������, ����� ��� ������������� ��������������� �� �
// ������� ����.
	hInst = hInstance;

	// --- ���������, ���� �� ���������� �������� �����.
	// ������������� �������� FindWindow, ������� ���������
	// ����� ���� �������� 
	// ������ �� ����� ������ ��� �� ��������� ����:
	// HWND FindWindow(LPCTSTR lpClassName,
	// LPCTSTRlpWindowName);
	// ����� �������� lpClassName ���������� ��������� ��
	// ��������� ������, � ������� ���������� �������� ���
	// ������ �������� ����. �� ���� ������ � ���� �� ������ 
	// ����� ������� ��������� ����. ���� ���������� ����� 
	// ���� � �������� ����������, �� ��� ��������� �������
	// �������� ����� �������� lpWindowName. ���� �� �������� 
	// ����� ����, �� �������� lpWindowName ����� �����
	// �������� NULL.
	if ((hWnd = FindWindow(ClassName, NULL)) != NULL)
	{
		// ������������ ����� �� �������, ����� ���������� ���
		// ��������, � ����� ���. ����� �� ��������� ����������, 
		// �� �������, ��� �� ������ �������� ��� ������� ����.
		// �������, ���� ���������� ���� �������� �����,
		// ������������� �������������� � ��������� �� ��������
		// ���� ��� ������� ����. ��� ������ ��, � ���� ������������
		// ������������.
		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);

		// ������� ���������� ����� - ������ ����� �����
		// ������������.
		return FALSE;
	}

	// --- ���������� ����� �� ������� - ������� WinMain
	// ���������� � �������������. ���������� ���������
	// WNDCLASS ��� ����������� ������ ����.
	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;		// ��� ������ ����
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// ����� ������� �������
	wc.style = CS_HREDRAW | CS_VREDRAW;	// ����� ������ 
	wc.hInstance = hInstance;		// ��������� ����������
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_POINTER));
	// ����������� ��� ����
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_POINTER));
	// ������ ���� ��� ����
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// ����� ��� ����
	wc.lpszMenuName = NULL;			// ������ ���� ����
	wc.cbClsExtra = 0;			// �������������� ������
	wc.cbWndExtra = 0;			// �������������� ������
	// P���������� ������ ����.
	RegisterClass(&wc);

	// ������� ������� ���� ����������.
	hWnd = CreateWindow(
		ClassName, 			// ��� ������ ����
		AppTitle,			// ��������� ���� 
		WS_OVERLAPPEDWINDOW, 		// ����� ����
		CW_USEDEFAULT,			// X-���������� 
		CW_USEDEFAULT,			// Y-���������� 
		CW_USEDEFAULT,			// ������ ����
		CW_USEDEFAULT,			// ������ ����
		NULL,			// ���������� ����-��������
		NULL,			// ���������� ���� ����
		hInst,		// ���������� ���������� ����������
		NULL);		// �������������� ����������
	if (!hWnd)
	{
		// ���� �� �������, ������ ��������������.
		MessageBox(NULL,
			L"Create: error", AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// ���������� ����.
	ShowWindow(hWnd, nCmdShow);

	// ��������� ���������� ���������� ������� ����.
	UpdateWindow(hWnd);

	SetTimer(hWnd,             // handle to main window 
		IdTimer,                   // timer identifier 
		5000,                           // 10-second interval 
		(TIMERPROC)NULL);               // no timer callback 

	HACCEL haccel = LoadAccelerators(hInst, MAKEINTRESOURCE(ID_ACC_KEYS));
	
	// ��������� ���� ��������� ������� ���������.
	// ������� GetMessage �������� ��������� �� �������, 
	// ������ false ��� ������� �� ������� ��������� WM_QUIT
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!haccel || !TranslateAccelerator(hWnd, haccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


void invalidateRect(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	RECT r;
	r.left = 0;
	r.right = rect.right;
	r.top = 0;
	r.bottom = 100;
	InvalidateRect(hWnd, &r, true);
}

std::wstring strWmKeydown = L"������ �������: ";
std::wstring strWmChar = L"������ ������� ����� WM_CHAR: ";
std::wstring strTimer = L"������ ������� ��������: 10 ���";
std::wstring strMousePosition = L"������� ����: ";

POINT ptOld;
RECT rc;
int timeToTimer = 10000;

// --- ������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
		{
			strTimer = L"������ ������� ��������: " + std::to_wstring(timeToTimer / 1000) + L" ���";
			HDC hDC;
			PAINTSTRUCT ps;
			RECT r;
			GetClientRect(hWnd, &r); 
			hDC = BeginPaint(hWnd, &ps);
			TextOut(hDC, 20, 20, &strWmChar[0], strWmChar.length());
			TextOut(hDC, 20, 40, &strMousePosition[0], strMousePosition.length());
			TextOut(hDC, 20, 60, &strTimer[0], strTimer.length());
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_MOUSEMOVE:
		{
			UINT fwKeys = wParam;
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);

			if (fwKeys & MK_LBUTTON)
			{
				HDC hDC = GetDC(hWnd);
				SetPixel(hDC, xPos, yPos, 0l);
				ReleaseDC(hWnd, hDC);
			}
			else if(fwKeys & MK_MBUTTON)
			{
				HDC hDC = GetDC(hWnd);
				SetPixel(hDC, xPos, yPos, RGB(0, 255, 0));
				ReleaseDC(hWnd, hDC);
			}
			else if (fwKeys & MK_SHIFT)
			{
				HDC hDC = GetDC(hWnd);
				SetPixel(hDC, xPos, yPos, RGB(0, 255, 255));
				ReleaseDC(hWnd, hDC);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			POINT p;
			p.x = GET_X_LPARAM(lParam);
			p.y = GET_Y_LPARAM(lParam);
			strWmKeydown = L"������ �������: ����� ������ ����";
			invalidateRect(hWnd);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			strWmKeydown = L"������ �������: ������ ������ ����";
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		case WM_KEYDOWN:
		{
			std::wstring str;
			str.resize(20);
			GetKeyNameText(lParam, &str[0], 20);
			strWmChar = L"������ ������� ����� WM_KEYDOWN: " + str;
			switch (wParam)
			{
			case VK_DOWN:
			{
				if (timeToTimer != 1000)
				{
					timeToTimer -= 1000;
					KillTimer(hWnd, IdTimer);
					SetTimer(hWnd, IdTimer, timeToTimer, NULL);
				}
				break;
			}
			case VK_UP:
			{
				if (timeToTimer != 40000)
				{
					timeToTimer += 1000;
					KillTimer(hWnd, IdTimer);
					SetTimer(hWnd, IdTimer, timeToTimer, NULL);
				}
				break;
			}
			default: break;
			}
			invalidateRect(hWnd);
			break;
		}
		case WM_CHAR:
		{
			std::wstring str;
			str.resize(20);
			switch (wParam)
			{
			case 0x08:
				str = L"BACKSPACE";
				break;
			case 0x0A:
				str = L"Shift + Enter (������� ������)";
				break;
			case 0x1B:
				str = L"Esc";
				break;
			case 0x09:
				str = L"TAB";
				break;
			case 0x0D:
				str = L"Enter (������� �������)";
				break;
			default:
				wsprintf(&str[0], L"%c", wParam);
				break;
			}
			strWmChar = L"������ ������� ����� WM_CHAR: " + str;
			invalidateRect(hWnd);
			break;
		}
		case WM_COMMAND:
		{
			std::wstring str;
			str.resize(20);
			switch (LOWORD(wParam))
			{
			case CM_SAVE:
				str = L"CTRL + S";
				strWmChar = L"������ ������� ����� WM_COMMAND: " + str;
				invalidateRect(hWnd);
				break;
			case CM_ALL:
				str = L"CTRL + A";
				strWmChar = L"������ ������� ����� WM_COMMAND: " + str;
				invalidateRect(hWnd);
				break;
			case CM_SHIFT:
				str = L"SHIFT + Q";
				strWmChar = L"������ ������� ����� WM_COMMAND: " + str;
				invalidateRect(hWnd);
				break;
			default:
				break;
			}
			break;
		}
		case WM_TIMER:
		{
			POINT pt;
			GetCursorPos(&pt);
			strMousePosition = L"������� ����: X = " + std::to_wstring(pt.x) + L" Y = " + std::to_wstring(pt.y);
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		case WM_DESTROY:
		{
			KillTimer(hWnd, IdTimer);
			PostQuitMessage(0);
			break;
		}
		default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}
