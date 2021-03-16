// --- Обязательный включаемый файл
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <string>

#include "resource.h"

// --- Описание функции главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- Глобальные переменные
HINSTANCE hInst; 		// Дескриптор экземпляра приложения
WCHAR ClassName[] = L"Window"; 		// Название класса окна
WCHAR AppTitle[] = L"Application Win32"; 	// Заголовок главного окна
UINT IdTimer = 0;

int WINAPI WinMain(HINSTANCE hInstance,
	// Дескриптор экземпляра приложения
	HINSTANCE hPrevInstance, // В Win32 всегда равен NULL
	LPSTR lpCmdLine,
	// Указатель на командную строку. Он // позволяет
	// приложению получать данные из командной строки.
	int nCmdShow
	// Определяет, как приложение первоначально 
	// отображается на дисплее: пиктограммой
	// (nCmdShow = SW_SHOWMINNOACTIVE) 
	// или в виде открытого окна 					//(nCmdShow = SW_SHOWNORMAL).
)
{
	WNDCLASS wc; 	// Структура для информации о классе окна
	HWND hWnd; 	// Дескриптор главного окна приложения
	MSG msg; 	// Структура для хранения сообщения
// Сохраняем дескриптор экземпляра приложения в глобальной
// переменной, чтобы при необходимости воспользоваться им в
// функции окна.
	hInst = hInstance;

	// --- Проверяем, было ли приложение запущено ранее.
	// Воспользуемся функцией FindWindow, которая позволяет
	// найти окно верхнего 
	// уровня по имени класса или по заголовку окна:
	// HWND FindWindow(LPCTSTR lpClassName,
	// LPCTSTRlpWindowName);
	// Через параметр lpClassName передается указатель на
	// текстовую строку, в которую необходимо записать имя
	// класса искомого окна. На базе одного и того же класса 
	// можно создать несколько окон. Если необходимо найти 
	// окно с заданным заголовком, то имя заголовка следует
	// передать через параметр lpWindowName. Если же подойдет 
	// любое окно, то параметр lpWindowName может иметь
	// значение NULL.
	if ((hWnd = FindWindow(ClassName, NULL)) != NULL)
	{
		// Пользователь может не помнить, какие приложения уже
		// запущены, а какие нет. Когда он запускает приложение, 
		// то ожидает, что на экране появится его главное окно.
		// Поэтому, если приложение было запущено ранее,
		// целесообразно активизировать и выдвинуть на передний
		// план его главное окно. Это именно то, к чему приготовился
		// пользователь.
		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);

		// Найдена работающая копия - работа новой копии
		// прекращается.
		return FALSE;
	}

	// --- Работающая копия не найдена - функция WinMain
	// приступает к инициализации. Заполнение структуры
	// WNDCLASS для регистрации класса окна.
	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;		// Имя класса окон
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// Адрес оконной функции
	wc.style = CS_HREDRAW | CS_VREDRAW;	// Стиль класса 
	wc.hInstance = hInstance;		// Экземпляр приложения
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_POINTER));
	// Пиктограмма для окон
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_POINTER));
	// Курсор мыши для окон
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// Кисть для окон
	wc.lpszMenuName = NULL;			// Ресурс меню окон
	wc.cbClsExtra = 0;			// Дополнительная память
	wc.cbWndExtra = 0;			// Дополнительная память
	// Pегистрация класса окна.
	RegisterClass(&wc);

	// Создаем главное окно приложения.
	hWnd = CreateWindow(
		ClassName, 			// Имя класса окон
		AppTitle,			// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна
		CW_USEDEFAULT,			// X-координаты 
		CW_USEDEFAULT,			// Y-координаты 
		CW_USEDEFAULT,			// Ширина окна
		CW_USEDEFAULT,			// Высота окна
		NULL,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInst,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация
	if (!hWnd)
	{
		// Окно не создано, выдаем предупреждение.
		MessageBox(NULL,
			L"Create: error", AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// Отображаем окно.
	ShowWindow(hWnd, nCmdShow);

	// Обновляем содержимое клиентской области окна.
	UpdateWindow(hWnd);

	SetTimer(hWnd,             // handle to main window 
		IdTimer,                   // timer identifier 
		5000,                           // 10-second interval 
		(TIMERPROC)NULL);               // no timer callback 

	HACCEL haccel = LoadAccelerators(hInst, MAKEINTRESOURCE(ID_ACC_KEYS));
	
	// Запускаем цикл обработки очереди сообщений.
	// Функция GetMessage получает сообщение из очереди, 
	// выдает false при выборке из очереди сообщения WM_QUIT
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

std::wstring strWmKeydown = L"Нажата клавиша: ";
std::wstring strWmChar = L"Нажата клавиша через WM_CHAR: ";
std::wstring strTimer = L"Период таймера очищения: 10 сек";
std::wstring strMousePosition = L"Позиция мыши: ";

POINT ptOld;
RECT rc;
int timeToTimer = 10000;

// --- Функция окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
		{
			strTimer = L"Период таймера очищения: " + std::to_wstring(timeToTimer / 1000) + L" сек";
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
			strWmKeydown = L"Нажата клавиша: Левая кнопка мыши";
			invalidateRect(hWnd);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			strWmKeydown = L"Нажата клавиша: Правая кнопка мыши";
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		case WM_KEYDOWN:
		{
			std::wstring str;
			str.resize(20);
			GetKeyNameText(lParam, &str[0], 20);
			strWmChar = L"Нажата клавиша через WM_KEYDOWN: " + str;
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
				str = L"Shift + Enter (перевод строки)";
				break;
			case 0x1B:
				str = L"Esc";
				break;
			case 0x09:
				str = L"TAB";
				break;
			case 0x0D:
				str = L"Enter (перевод каретки)";
				break;
			default:
				wsprintf(&str[0], L"%c", wParam);
				break;
			}
			strWmChar = L"Нажата клавиша через WM_CHAR: " + str;
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
				strWmChar = L"Нажата клавиша через WM_COMMAND: " + str;
				invalidateRect(hWnd);
				break;
			case CM_ALL:
				str = L"CTRL + A";
				strWmChar = L"Нажата клавиша через WM_COMMAND: " + str;
				invalidateRect(hWnd);
				break;
			case CM_SHIFT:
				str = L"SHIFT + Q";
				strWmChar = L"Нажата клавиша через WM_COMMAND: " + str;
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
			strMousePosition = L"Позиция мыши: X = " + std::to_wstring(pt.x) + L" Y = " + std::to_wstring(pt.y);
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
