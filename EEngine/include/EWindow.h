
#pragma once

#include "ECommon.h"
#include <Windows.h>

namespace E3D
{
	class EInputListener;
	class ECanvas;
	// ��Ϸ���ڹ�����
	class EGameWindow
	{
	public:
		// ȫ����ָ��, ������WinProc�лص�
		static EGameWindow *GWindow;
		
		// window�¼�����
		void startLoop();

		EGameWindow(const EString& windowTitle, HINSTANCE hInstance);
		~EGameWindow();

		// ��ʾ����
	    void showWindow(EBool show);
		void updateWindow();

		void quiteApplication();

		HWND getHWnd() const { return mHwnd; }
		HINSTANCE getHInstance() const { return mHinstance;}

		// ע��/�Ƴ�IO�¼�����
		void addInputListener(EInputListener* listener);
		void removeInputListener(EInputListener* listener);

		// ���û��Ƽ�����, ͬʱֻ����һ������Canvas
		void setCanvasListener(ECanvas *canvasListener);

	protected:

		// ȫ��Win�¼��ص�����
		static LRESULT WinProc(HWND hWnd, UINT Msg,WPARAM wParam,LPARAM lParam);

		// ���ڻ��ƺ���
		virtual void onPaint(HDC hdc);
		virtual void onUpdate();
		virtual void onKeyDown(EInt msg);
		virtual void onKeyRelease(EInt msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(EInt x, EInt y);
		virtual void onMouseWheel(EInt delta);

	protected:
		// ע�ᴰ��
		bool registerWindow();
		// ��������
		bool createWindow();

	protected:
		
		HWND							mHwnd;
		EString							mWindowTitle;
		HINSTANCE						mHinstance;
		std::vector<EInputListener*>	mInputListeners;
		ECanvas							*mCanvasListener;

		EBool							mQuite;

		typedef std::vector<EInputListener*>::iterator	InputItr;
	};
}