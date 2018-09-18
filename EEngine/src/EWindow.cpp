
#include "EWindow.h"
#include "Windowsx.h"
#include "EGraphics.h"
#include "EInputListener.h"
#include "ECanvas.h"
#include <ctime>

namespace E3D
{
	

	EGameWindow	*EGameWindow::GWindow;

	

	LRESULT EGameWindow::WinProc(HWND hWnd, UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		if (!EGameWindow::GWindow)
			return DefWindowProc(hWnd, Msg, wParam, lParam);

		switch(Msg)
		{
			// 绘制
			case WM_MOVING:
			case WM_PAINT:
			{
				PAINTSTRUCT  ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				EGameWindow::GWindow->onPaint(hdc);
				EndPaint(hWnd, &ps);
				break;
			}
			case WM_TIMER:
			{
				EGameWindow::GWindow->updateWindow();
				break;
			}
			// 键盘按下
			case WM_KEYDOWN:
			{
				EGameWindow::GWindow->onKeyDown(wParam);
				break;
			}
			// 键盘释放
			case WM_KEYUP:
			{
				EGameWindow::GWindow->onKeyRelease(wParam);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				EShort delta = GET_WHEEL_DELTA_WPARAM(wParam); // wheel rotation
				EGameWindow::GWindow->onMouseWheel(delta * 0.5f / PI);
			}

			// 鼠标移动
			case WM_MOUSEMOVE:
			{
				EInt x = GET_X_LPARAM(lParam); 
				EInt y = GET_Y_LPARAM(lParam); 
				EGameWindow::GWindow->onMouseMove(x, y);
				break;
			}
			// 关闭窗口
			case WM_CLOSE:
			{
				DestroyWindow(hWnd);
				break;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
			default:
				return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
		return 0;
	}

	

	EGameWindow::EGameWindow(const EString& windowTitle, HINSTANCE hInstance) : 
	mWindowTitle(windowTitle), mHinstance(hInstance), mCanvasListener(NULL), mQuite(false)
	{
		if (!(registerWindow() && createWindow()))
		{
			throw "Init Window Failed!";
		}				
	}

	

	EGameWindow::~EGameWindow()
	{
		DestroyWindow(mHwnd);
		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
		{
			SafeDelete(*itr);
		}

		SafeDelete(mCanvasListener);
	}

	

	// 显示窗口
	void EGameWindow::showWindow(EBool show)
	{
		::ShowWindow(mHwnd, SW_SHOW);
		::UpdateWindow(mHwnd);
	}

	

	void EGameWindow::updateWindow()
	{
		// 刷心窗口
		::InvalidateRect (mHwnd, NULL, FALSE) ;
		::UpdateWindow(mHwnd);
	}

	

	void EGameWindow::setCanvasListener(ECanvas *canvasListener)
	{
		mCanvasListener = canvasListener;
	}

	

	void EGameWindow::onUpdate()
	{

	}

	// 窗口绘制函数
	void EGameWindow::onPaint(HDC hdc)
	{
		if (mCanvasListener)
		{
			EGraphics::clearBuffer(EColor(0, 0, 0));
			mCanvasListener->update();
			mCanvasListener->onPaint();
			EGraphics::fillBuffer(hdc);
		}
	}

	

	void EGameWindow::addInputListener(EInputListener* listener)
	{
		mInputListeners.push_back(listener);
	}

	

	void EGameWindow::removeInputListener(EInputListener* listener)
	{
		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end();)
		{
			if (*itr == listener)
			{
				mInputListeners.erase(itr++);
			}
			else
				++itr;
		}
	}

	

	void EGameWindow::onKeyDown(EInt msg)
	{
		//::MessageBox(0, "Window onKeyDown", 0, 0);
		for(InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->keyPress(msg);
	}

	

	void EGameWindow::onKeyRelease(EInt msg)
	{
		
		for(InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->keyRlease(msg);
	}

	

	void EGameWindow::onMousePress(bool rightPress)
	{
		if (rightPress)
			::MessageBox(0, "Window R onMousePress", 0, 0);
		else
			::MessageBox(0, "Window L onMousePress", 0, 0);
	}

	

	void EGameWindow::onMouseMove(EInt x, EInt y)
	{
		
		for(InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->mouseMove(x, y);
	}

	

	void EGameWindow::onMouseWheel(EInt delta)
	{
		for(InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->mouseWheel(delta);
	}

	void EGameWindow::quiteApplication()
	{
		mQuite = true;
	}

	

	void EGameWindow::startLoop()
	{
		
		MSG msg;
		while (!mQuite)
		{        
			if (::PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))        
			{        
				if (msg.message == WM_QUIT)        
					break ;        
				::TranslateMessage (&msg) ;        
				::DispatchMessage (&msg) ;        

				onUpdate();
			}
			else
				::Sleep(10);
		}     
	}

	

	bool EGameWindow::createWindow()
	{
		mHwnd = ::CreateWindow(	"E3D", mWindowTitle.c_str(), 
			// 窗口不可更改大小
			WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX)& (~WS_THICKFRAME),
			CW_USEDEFAULT, CW_USEDEFAULT, 
			SCREEN_WIDTH, SCREEN_HEIGHT, 
			0, 0, mHinstance, 0);

		if (!mHwnd)
		{
			::MessageBox(0, "Create Window Failed", 0, 0);
			return false;
		}


		::SetTimer(mHwnd, WM_TIMER, 35,NULL);

		// 手动播种
		srand(time(NULL));

		return true;
	}

	

	bool EGameWindow::registerWindow()
	{
		WNDCLASS wind;
		{
			wind.style			= CS_HREDRAW | CS_VREDRAW;
			wind.lpfnWndProc	= (WNDPROC)EGameWindow::WinProc;
			wind.cbClsExtra		= 0;
			wind.cbWndExtra		= 0;
			wind.hInstance		= mHinstance;
			wind.hIcon			= LoadIcon(0, IDI_APPLICATION);
			wind.hCursor		= LoadCursor(0, IDC_ARROW);
			wind.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
			wind.lpszMenuName	= 0;
			wind.lpszClassName	= "E3D";
		}

		if (!RegisterClass(&wind))
		{
			::MessageBox(0, "Register Class Failed", 0, 0);
			return false;
		}

		return true;
	}

	
}