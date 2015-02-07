/* Copyright 2014 Zitian Li
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Author: Zitian Li
 * Email: me@zitianli.com
 * License: Apache License, Version 2.0
 */
#include "global.h"
#include <windows.h>
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
static inline int myabs(int bInt) { return bInt > 0 ? bInt : 0; }
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	TCHAR szAppName[] = "Midautumn";
	int bret, width = 640, height = 480, xWindow = myabs(GetSystemMetrics(SM_CXSCREEN)/2-width/2), yWindow = myabs(GetSystemMetrics(SM_CYSCREEN)/2-height/2);
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	wndclass;
	wndclass.style			= CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= LoadIcon(NULL,"IDI_ICON");
	wndclass.hCursor		= LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szAppName;
	if (!RegisterClass(&wndclass) ||
		!(hwnd = CreateWindow(szAppName,"2013ÖÐÇïÍí»á",WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,xWindow,yWindow,width,height,NULL,NULL,hInstance,NULL)) ||
		ShowWindow(hwnd,iCmdShow) || !UpdateWindow(hwnd)) {
		MessageBox(NULL,TEXT("Error!"),szAppName,MB_ICONERROR);
		return 1;
	}
	while((bret = GetMessage(&msg,NULL,0,0))) {
		if(bret == -1) {
			MessageBox(NULL,TEXT("Error!"),szAppName,MB_ICONERROR);
			return 1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
