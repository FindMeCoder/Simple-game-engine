
#include "EGraphics.h"
#include "EUtil.h"

namespace E3D
{
	

	EBitmap::EBitmap(const EString &filename) : name(filename), pixels(NULL),
		pitch(0), width(0), height(0), valid(false)
	{


		// ���ʽű�, ���Ȼ�ȡĬ��·��
		hBitmap = (HBITMAP)::LoadImage(GHInstance, GetPath(filename).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hBitmap != NULL)
		{
			bitmapHDC = ::CreateCompatibleDC(NULL);
			::SelectObject(bitmapHDC, (HGDIOBJ)hBitmap);
			::GetObject(hBitmap,sizeof(BITMAP),&bitmap);

			width	= bitmap.bmWidth;
			height  = bitmap.bmHeight;
			pitch	= bitmap.bmHeight;
			valid	= true;

			pixels = new EColor[width * height];
			for (EInt i = 0; i < getHeight(); i++)
			{
				for (EInt j = 0; j < getWidth(); j++)
				{
					COLORREF color = ::GetPixel(bitmapHDC, i, j);
					pixels[j * width + i] = EColor(GetRValue(color), GetGValue(color), GetBValue(color));
				}
			}
		}

	}

	

	EBitmap::~EBitmap()
	{

		DeleteObject(hBitmap);
		DeleteDC(bitmapHDC);
		SafeDeleteArray(pixels);

	}

	

	EColor EBitmap::getPixel(EInt x, EInt y)
	{
		return pixels[y * pitch + x];

	}

	

	HINSTANCE GHInstance;


	HBITMAP							EGraphics::GBufferedHandle;
	HDC								EGraphics::GBufferedHDC;
	HBRUSH							EGraphics::GBgBrush;
	HPEN							EGraphics::GPen;
	HINSTANCE						EGraphics::GInstance;
	RECT							EGraphics::GBufferSize;
	DIBSECTION						EGraphics::GDIBSection;
	BYTE							*EGraphics::GDatas;
	EInt							EGraphics::GPitch;


	EFloat							*EGraphics::GZBuffer;

	

	// ��ʼ����ͼϵͳ
	bool EGraphics::initGraphics(HINSTANCE hinstance)
	{

		GBufferedHDC = ::CreateCompatibleDC(NULL);

		BITMAPINFO info					= {0};    
		info.bmiHeader.biSize			= sizeof(info.bmiHeader);    
		info.bmiHeader.biWidth			= SCREEN_WIDTH;
		info.bmiHeader.biHeight			= -SCREEN_HEIGHT;    
		info.bmiHeader.biPlanes			= 1;    
		info.bmiHeader.biBitCount		= 32;    
		info.bmiHeader.biCompression	= BI_RGB;    
		info.bmiHeader.biSizeImage		= SCREEN_WIDTH * SCREEN_HEIGHT * 32 / 8;  

		// ����һ���ڴ�������ȡ������ָ��
		void* pBits = NULL;
		GBufferedHandle = ::CreateDIBSection(GBufferedHDC, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
		::SelectObject(GBufferedHDC, GBufferedHandle);

		//here: "dib.dsBm.bmBits" will points to the pixels of hdib.
		::GetObject(GBufferedHandle , sizeof(DIBSECTION), &GDIBSection);
		GDatas = (BYTE*)GDIBSection.dsBm.bmBits;
		GPitch = GDIBSection.dsBm.bmWidthBytes;

		// ����ˢ�������С
		::SetRect(&GBufferSize, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		//GPen = (HPEN)::GetStockObject(WHITE_PEN); 
		GPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); 
		::SelectObject(GBufferedHDC, GPen);

		GBgBrush= ::CreateSolidBrush(RGB(0, 0, 0));
		::SelectObject (GBufferedHDC, GBgBrush) ;

		// ��������
		HFONT hfnt = (HFONT)::GetStockObject(OEM_FIXED_FONT); 
		::SelectObject(GBufferedHDC, hfnt);
		// �������ֱ���Ϊ͸��ɫ
		::SetBkMode(GBufferedHDC, TRANSPARENT); 


		GZBuffer = new EFloat[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(GZBuffer, 0, sizeof(EFloat) * SCREEN_WIDTH * SCREEN_HEIGHT);

		return true;
	}

	

	// �رջ�ͼϵͳ
	void EGraphics::shutdownGraphics()
	{

		::DeleteObject(GPen);
		::DeleteObject(GBgBrush);
		::DeleteObject(GBufferedHandle);
		::DeleteDC(GBufferedHDC);

	}

	

	// ��յ�ǰ������, ��������ɫ����Ϊ��ɫ
	void EGraphics::clearBuffer(const EColor &c)
	{

		::FillRect(GBufferedHDC, &GBufferSize, GBgBrush);
		// ������Ȼ���
		// ע : ����memsetֻ�ܶ�Int���͵�������г�ʼ��, ��������ֱ��ʹ����Int����
		// ��û��ʹ��float����, Ӧ��ʹ��float
		::memset(GZBuffer, 0, sizeof(EFloat) * SCREEN_WIDTH * SCREEN_HEIGHT);
		
	}

	

	// �ڵ�ǰ�������ڻ���һ����
	void EGraphics::drawLine(EInt x0, EInt y0, EInt x1, EInt y1, const EColor &c)
	{
		::SelectObject(GBufferedHDC,GetStockObject(DC_PEN));
		::SetDCPenColor(GBufferedHDC, RGB(c.r, c.g, c.b));
		
		::MoveToEx(GBufferedHDC, x0, y0, NULL);
		::LineTo(GBufferedHDC, x1, y1);
		
		//::DeleteObject(hPen);

	}

	void EGraphics::drawString(const EString &str, EInt x, EInt y, const EColor &c)
	{

		::SetTextColor(GBufferedHDC, RGB(c.r, c.g, c.b));
		::TextOut(GBufferedHDC, x, y, str.c_str(), str.length()); 

	}

	void EGraphics::fillTriangle(EInt x0, EInt y0, EInt x1, EInt y1, EInt x2, EInt y2, const EColor &c)
	{

	}

	void EGraphics::enableSmoothingMode(EBool enable)
	{

	}

	

	EBool EGraphics::checkZ(EInt x, EInt y, EFloat z)
	{
		// ����ZӦ��ʹ��float�������洢, ���ʹ��int����, ��ô�ᵼ�¾��ȶ�ʧ
		// �������������
		EInt index = y * SCREEN_WIDTH + x;
		EFloat divZ = 1.0f / z;
		// �����ǻ���1/z���ıȽ�
		if (GZBuffer[index] > divZ)
			return false;

		GZBuffer[index] = divZ;
		return true;
	}

	

	void EGraphics::setPixel(EInt x, EInt y, /*EFloat z, */const EColor &c)
	{
		// ���ﱾ��Ӧ�ü���zֵ,����Ϊ�˱����Image���صĶ�ȡ, �ҽ�z�������˳���
		// ����, �ڵ���setPixel֮ǰӦ���ȼ��checkZ, ����true�ڵ���setPixel
		
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		// blue green red alpha
		pSrcPix[0] = c.b;
		pSrcPix[1] = c.g;
		pSrcPix[2] = c.r;
		pSrcPix[3] = c.a;

	}

	

	EColor EGraphics::getPixel(EInt x, EInt y)
	{

		//int pitch = GDIBSection.dsBm.bmWidthBytes;
		//pointer that will advance in memory one pixel at a time...
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		return EColor(pSrcPix[2], pSrcPix[1], pSrcPix[0]);

	}

	

	// ���Ѿ����ƺõĻ������ݽ���Graphics����Ļ�ϻ���, ������ǰ����������Ϊ��һ��������
	void EGraphics::fillBuffer(HDC hdc)
	{	

		::BitBlt(hdc,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GBufferedHDC,0,0,SRCCOPY);

	}

	
}