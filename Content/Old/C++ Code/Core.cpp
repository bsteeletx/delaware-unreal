// Common Includes (for Win32)
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <atlimage.h>
#include "AGK.h"
#include "resource.h"
#include "direct.h"
#include "Defines.h"

// App specific include for Core (so core can be distributed between apps without editing inside it)
#if (OS == IOS)
	#if (PLATFORM == TABLET)
		#define DEVICE_WIDTH 1024
		#define DEVICE_HEIGHT 768
	#else
		#define DEVICE_WIDTH 960
		#define DEVICE_HEIHT 640
	#endif
#elif (OS == ANDROID)
	#if (PLATFORM == TABLET)
		#define DEVICE_WIDTH 1280
		#define DEVICE_HEIGHT 768
	#elif (PLATFORM == MOBILE)
		#define DEVICE_WIDTH 960
		#define DEVICE_HEGHT 600
	#endif
#elif (OS == WINDOWS)
	#if (PLATFORM == PC)
		#define DEVICE_WIDTH 800
		#define DEVICE_HEIGHT 600
	#endif
#endif
#include "CoreForApp.h"

#define WM_SOUND_EVENT 0x8001

// comment out to revert to single mouse
//#define MULTI_MOUSE

// Namespace
using namespace AGK;

// Globals for core
HANDLE hDebugConsoleOut = NULL;
HWND g_hWnd = NULL;

//
// Windows Application STUB requires entry function, create window and message pump
//

#define AGK_MOUSE_LEFTDOWN		0x00000001
#define AGK_MOUSE_RIGHTDOWN		0x00000002
#define AGK_MOUSE_MIDDLEDOWN	0x00000004

class cMouseDevice
{
	protected:

	public:
		cMouseDevice() { ID = 0; hDevice = 0; x = 0; y = 0; pNext = 0; sprite = 0; m_bFlags = 0; }
		~cMouseDevice();

		UINT ID;
		HANDLE hDevice;
		int x;
		int y;
		UINT m_bFlags;
		cMouseDevice *pNext;
		UINT sprite;
};

cMouseDevice *pMouseList = 0;

void Output( const char *szMsg )
{
	if ( !hDebugConsoleOut ) 
	{
		AllocConsole ( );
		HWND hConsole = GetConsoleWindow();
		MoveWindow( hConsole, 32, 500, 800, 350, TRUE );
		hDebugConsoleOut = GetStdHandle ( STD_OUTPUT_HANDLE );
	}
	
	DWORD dwWritten = 0;
	WriteConsole ( hDebugConsoleOut, "> ", 2, &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, szMsg, (DWORD) strlen( szMsg ), &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, "\n", 1, &dwWritten, NULL );
}

unsigned int TranslateKey( unsigned int key )
{
	switch( key )
	{
		case 20: key = 0; break; // Caps lock
		case 120: key = 0; break; // F9
		case 121: key = 0; break; // F10
		case 122: key = 0; break; // F11
		case 123: key = 0; break; // F12
		case 18: key = 0; break; // Alt Gr
		case 91: key = 0; break; // Windows key (left)
		case 92: key = 0; break; // Windows key (right)

		// Num pad 0-9
		case 96:
		case 97: 
		case 98: 
		case 99: 
		case 100: 
		case 101: 
		case 102: 
		case 103: 
		case 104: 
		case 105: key -= 48; break;

		case 111: key = 192; break; // Num pad /
		case 109: key = 189; break; // Num pad -
		case 110: key = 190; break; // Num pad .

		case 144: key = 0; break;  // Num lock
		case 19: key = 0; break;  // pause/break
	}

	return key;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool bLeftDown = false;

	switch (message)
	{
#ifdef WM_TOUCH
		case WM_TOUCH:
		{
			// windows based touch device

			return DefWindowProc(hWnd, message, wParam, lParam);

			break;
		}

#endif

#ifdef MULTI_MOUSE
		case WM_INPUT:
		{			
			if ( !bCapturePointer ) break;
			// raw input from registered devices (in this case mice)
			UINT dwSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
			RAWINPUT* raw = (RAWINPUT*)lpb;

			// find mouse device
			int count = 0;
			cMouseDevice *pMouse = pMouseList;
			while ( pMouse )
			{
				count++;
				if ( pMouse->hDevice == raw->header.hDevice ) break;
				pMouse = pMouse->pNext;
			}

			// is this a new mouse device?
			if ( !pMouse )
			{
				pMouse = new cMouseDevice();
				pMouse->hDevice = raw->header.hDevice;
				pMouse->pNext = pMouseList;
				pMouse->ID = count+1;
				pMouse->x = lastX;
				pMouse->y = lastY;
				pMouse->sprite = agk::CreateSprite( 99 );
				agk::SetSpriteSize( pMouse->sprite, 4,-1 );
				agk::SetSpriteTransparency( pMouse->sprite, 2 );
				switch( count )
				{
					case 0: agk::SetSpriteColor( pMouse->sprite, 255,0,0,255 ); break;
					case 1: agk::SetSpriteColor( pMouse->sprite, 0,255,0,255 ); break;
					case 2: agk::SetSpriteColor( pMouse->sprite, 0,0,255,255 ); break;
					case 3: agk::SetSpriteColor( pMouse->sprite, 255,255,0,255 ); break;
				}
				pMouseList = pMouse;
			}

			// relative or absolute positioning?
			if ( raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE )
			{
				pMouse->x = raw->data.mouse.lLastX;
				pMouse->y = raw->data.mouse.lLastY;
			}
			else
			{
				pMouse->x += raw->data.mouse.lLastX;
				pMouse->y += raw->data.mouse.lLastY;
			}

			// check buttons
			if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN )
			{
				if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) == 0 )
				{
					agk::TouchPressed( pMouse->ID, pMouse->x, pMouse->y );
				}

				pMouse->m_bFlags |= AGK_MOUSE_LEFTDOWN;
			}

			if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP )
			{
				if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) != 0 )
				{
					agk::TouchReleased( pMouse->ID, pMouse->x, pMouse->y );
				}

				pMouse->m_bFlags &= ~AGK_MOUSE_LEFTDOWN;
			}

			// if holding left button send drag event
			if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) != 0 )
			{
				if ( raw->data.mouse.lLastX != 0 && raw->data.mouse.lLastY != 0 )
				{
					agk::TouchMoved( pMouse->ID, pMouse->x, pMouse->y );
				}
			}

			if ( pMouse->x > DEVICE_WIDTH ) pMouse->x = DEVICE_WIDTH;
			if ( pMouse->y > DEVICE_HEIGHT ) pMouse->y = DEVICE_HEIGHT;
			if ( pMouse->x < 0 ) pMouse->x = 0;
			if ( pMouse->y < 0 ) pMouse->y = 0;

			agk::SetSpritePosition( pMouse->sprite, agk::DeviceToScreenX(pMouse->x), agk::DeviceToScreenY(pMouse->y) );

			//char str[ 256 ];
			//sprintf( str, "X: %d, Y: %d", pMouse->x, pMouse->y );
			//Output( str );
			if ( bCapturePointer )
			{
				RECT rect;
				::GetWindowRect( g_hWnd, &rect );
				int centerX = (rect.left + rect.right) / 2;
				int centerY = (rect.top + rect.bottom) / 2;
				SetCursorPos( centerX,centerY );
			}
			break;
		}
#endif

		case WM_LBUTTONDOWN:
		{
			bLeftDown = true;
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			agk::MouseLeftButton( 0, 1 );

#ifdef MULTI_MOUSE
			if ( !bCapturePointer )
			{
				bCapturePointer = true;
				ShowCursor( FALSE );
				lastX = x;
				lastY = y;
			}
#else
			SetCapture(g_hWnd);
			agk::TouchPressed( 1, x,y );
#endif
			break;
		}

		case WM_LBUTTONUP:
		{
			bLeftDown = false;
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			agk::MouseLeftButton( 0, 0 );

#ifndef MULTI_MOUSE
			ReleaseCapture();
			agk::TouchReleased( 1, x,y );
#endif
			break;
		}

		case WM_RBUTTONDOWN:
		{
			agk::MouseRightButton( 0, 1 );

#ifdef MULTI_MOUSE
			if ( bCapturePointer ) // Alt Gr
			{
				bCapturePointer = false;
				ShowCursor( TRUE );
			}
#endif
			break;
		}

		case WM_RBUTTONUP:
		{
			agk::MouseRightButton( 0, 0 );
			break;
		}

		case WM_MOUSEMOVE:
		{
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			agk::MouseMove( 0, x, y );

			if ( bLeftDown )
			{
#ifndef MULTI_MOUSE
				agk::TouchMoved( 1, x,y );
#endif
			}
			break;
		}

		case WM_SIZE:
		{
			agk::UpdateDeviceSize();
			break;
		}

		case WM_KEYDOWN:
		{
#ifdef MULTI_MOUSE
			if ( wParam == 17 && bCapturePointer ) // Alt Gr
			{
				bCapturePointer = false;
				ShowCursor( TRUE );
			}
#endif
			unsigned int key = TranslateKey( wParam );
			if ( key > 0 )
			{
				agk::KeyDown( key );
			}
			
			/*
			uString sKey;
			sKey.Format( "Key: %d", wParam );
			agk::Warning( sKey );
			*/

			//if ( wParam == VK_ESCAPE ) PostQuitMessage(0);
			break;
		}

		case WM_KEYUP:
		{
			unsigned int key = TranslateKey( wParam );
			if ( key > 0 )
			{
				agk::KeyUp( key );
			}
			break;
		}

		case WM_CHAR:
		{
			agk::CharDown( wParam );
			break;
		}

		case WM_SOUND_EVENT:
		{
			agk::HandleMusicEvents( (UINT) lParam );
			break;
		}
			
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

HWND CreateWin32Window( HINSTANCE hInstance, int width, int height, uString &szTitle, int x, int y, bool fullscreen )
{
	// register the class
	WNDCLASSEX wcex;

	// hardcoded resource IDs for icons
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(50)); 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "OPENGLWINDOW";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassEx(&wcex);

	// check the given values against the desktop resolution and center if requested (x=-1 or y=-1)
	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);
	if ( width < 1 ) width = 1;
	if ( height < 1 ) height = 1;
	if ( width > rc.right-rc.left ) width = rc.right-rc.left;
	if ( height > rc.bottom-rc.top ) height = rc.bottom-rc.top;
	if ( x == -1 ) x = ((rc.right-rc.left)-width)/2 + rc.left;
	if ( y == -1 ) y = ((rc.bottom-rc.top)-height)/2 + rc.top;
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( x+width > rc.right ) x = rc.right-width;
	if ( y+height > rc.bottom ) y = rc.bottom-height;
	if ( fullscreen )												
    {
		width = rc.right-rc.left;
		height = rc.bottom-rc.top;
		x = 0;
		y = 0;

		/*
    	DEVMODE dmScreenSettings;								// Device Mode
    	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
    	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
    	dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
    	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    	// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
    	{
    		fullscreen = false;
    	}
		*/
    }

	DWORD dwExStyle;
	DWORD dwStyle;

	if ( fullscreen )											
    {
    	dwExStyle = WS_EX_APPWINDOW;							
    	dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	
    }
    else
    {
    	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
    	dwStyle = WS_OVERLAPPEDWINDOW & (~WS_SIZEBOX);	
    }


	// adjust the window and make it bigger for borders, window title, etc
	RECT WindowRect;							
	WindowRect.left=(long)0;					
	WindowRect.right=(long)width;				
	WindowRect.top=(long)0;						
	WindowRect.bottom=(long)height;				
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	// initialise window
	HWND hWnd = CreateWindow("OPENGLWINDOW", szTitle.GetStr(), dwStyle, x, y, 
			WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, NULL, NULL, hInstance, NULL);

	// terminate if failed to create window
	if (!hWnd) return FALSE;
	g_hWnd = hWnd;

	// show window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

#ifdef MULTI_MOUSE
	// Register that we want raw input from mice
	RAWINPUTDEVICE pDevices;
	pDevices.usUsage = 2;
	pDevices.usUsagePage = 1;
	pDevices.hwndTarget = hWnd;
	pDevices.dwFlags = 0;
	RegisterRawInputDevices( &pDevices, 1, sizeof(RAWINPUTDEVICE) );
#endif

	// success
	return hWnd;
}

void CreateInternalBitmapsForLIBIMAGES ( void )
{
	// use it
	if ( 0 )
	{
		// used to create byte data from images (used in builds)
		// interpreter graphics are stored internally, generated in hard code, right here (in debug mode)
		char cwd[MAX_PATH];
		_getcwd ( cwd, MAX_PATH );

		// The new lib images are uploaded and stored as PNG to try and keep the size down
		// although the backdrop file takes up 600KB of the 700KB libImages.h file.
		// We will read in the file and output it's raw data with the following code:
		for ( int n=0; n<5; n++ )
		{
			LPSTR pFileIn, pFileOut;
			if ( n==0 ) { pFileIn = "interpreter-backdrop.png"; pFileOut="interpreter-backdrop.txt"; }
			if ( n==1 ) { pFileIn = "interpreter-logo.png"; pFileOut="interpreter-logo.txt"; }
			if ( n==2 ) { pFileIn = "interpreter-spinner.png"; pFileOut="interpreter-spinner.txt"; }
			if ( n==3 ) { pFileIn = "interpreter-power.png"; pFileOut="interpreter-power.txt"; }
			if ( n==4 ) { pFileIn = "Avenir.png"; pFileOut="AvenirRaw.txt"; }
			cFile oFile, oFile2;
			oFile.OpenToRead( pFileIn );
			oFile2.OpenToWrite( pFileOut );
			int count = 0;
			int count2 = 0;
			char str[ 30 ];
			while ( !oFile.IsEOF() )
			{
				UINT i = oFile.ReadInteger();
				sprintf( str, "0x%x, ", i );
				oFile2.WriteData( str, strlen(str) );
				count++;
				count2++;
				if ( count > 30 ) 
				{
					oFile2.WriteData( "/\n", 2 );
					count = 0;
				}
			}
			sprintf( str, "\nsize = %d", count2 );
			oFile2.WriteData( str, strlen(str) );
			oFile2.Close();
			oFile.Close();
		}
		// The final line will be a size value
		// copy that into the "libImageInterpreterBackgroundPNGSize" parameter in libImages.h
		// copy everything else into the curley braces after "libImageInterpreterBackgroundPNG"
		// (or whatever image variable you are modifying). Everything else should be automatic.
		// output file is located at: C:\Users\you\Documents\AGK\yourapp\
		// i.e. C:\Users\Lee\Documents\AGK\c_TGCSHARED_GDKProWorkspace_GDKPro_Compiler_AGKFinalOuput_Projects_Basic_TestArea_SimpleTier1_SimpleTier1exe
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// Determine name of app
	if ( AppInitCode()==0 )
		return 0;

	// create a win32 window for the app
	uString szTitle( "Double Pinochle" );
	DWORD dwCenterWindowX = ( GetSystemMetrics ( SM_CXSCREEN ) - DEVICE_WIDTH ) / 2;
	DWORD dwCenterWindowY = ( GetSystemMetrics ( SM_CYSCREEN ) - DEVICE_HEIGHT ) / 2;
	HWND hWnd = CreateWin32Window( hInstance, DEVICE_WIDTH, DEVICE_HEIGHT, szTitle, dwCenterWindowX, dwCenterWindowY, false );

	// gather data
	AppGatherData ( (DWORD)hWnd, lpCmdLine );

	// initialise graphics API (win32 openGL) for app
	agk::InitGL( (void*) hWnd );

	// can call this to generate new internal images for LIBIMAGES (skipped in function if required)
	CreateInternalBitmapsForLIBIMAGES();

	/*****************APPUP CODE******************************/
/*#if (PLATFORM == PC)

	//Authorization code for Intel AppUp(TM) software
	Application *appupApp = NULL;

	try 
	{
		#ifdef _DEBUG
			appupApp = new Application(ApplicationId(ADP_DEBUG_APPLICATIONID));
		#else
			appupApp = new Application(ApplicationId(\* get application GUID));
		#endif
	} 
	catch (AdpException& e) 
	{
		agk::Message(e.message());

		if (appupApp != NULL)
			delete appupApp;
			
		return 0;
	}


	
#endif
/*/////////////////////////END APPUP CODE/////////////////////////*/
	// call app begin
	try
	{
		App.Begin();
	}
	catch(...)
	{
		uString err = agk::GetLastError();
		err.Prepend( "Uncaught exception: \n\n" );
		MessageBox( NULL, err.GetStr(), "Error", 0 );
	}

	// message pump
	MSG msg;
	bool bExitLoop = false;
	while ( !bExitLoop )
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{	
			if ( msg.message == WM_QUIT ) bExitLoop = true;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			// call app each frame
			try
			{
#if (DEMOBUILD)
				App.Loop(&bExitLoop);
#else
				App.Loop();
#endif
			}
			catch(...)
			{

				uString err = agk::GetLastError();
				err.Prepend( "Uncaught exception: \n\n" );
				MessageBox( NULL, err.GetStr(), "Error", 0 );
			}
		}
	}
	
	// call app end
	App.End();

#if (PLATFORM == PC)
	/**************MORE APPUP CODE*/
	//Cleanup code for Intel AppUp(TM) software
	/*if (appupApp != NULL) 
		delete appupApp;*/
	/*/////////////////END MORE APPUP CODE*/
#endif
	// finished
	return 0;
}

