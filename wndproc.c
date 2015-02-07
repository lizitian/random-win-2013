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
#define IDC_BUTTON_0 0x1f00
#define IDC_BUTTON_1 0x1f01
#define IDC_BUTTON_2 0x1f02
#define IDC_BUTTON_3 0x1f03
#define IDC_BUTTON_4 0x1f04
#define IDC_BUTTON_5 0x1f05
#define NAME_STRING "Go! "
#define REFRESH_STRING "refresh"
#define NAME_FILE "name.txt"
#define EXTRA_FILE "extra.txt"
static void selectmessage(HWND hwnd, data *dat)
{
	char *name;
	name = selectname(dat);
	MessageBox(hwnd,name?name:"没有未选的人...请点击\"" REFRESH_STRING "\"重置",TEXT("select"),MB_OK);
}
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	int			tmp;
	static data	*dat;
	HDC			hdc;
	PAINTSTRUCT	ps;
	RECT		rect;
	switch(message){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0; 
		case WM_CREATE:
			SetClassLong(hwnd,GCL_HICON,(LONG)LoadIcon(GetModuleHandle(NULL),"IDI_ICON"));
			CreateWindow("BUTTON","饮料惩罚",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,80,60,150,40,hwnd,(HMENU)(IDC_BUTTON_0),(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),NULL);
			//CreateWindow("BUTTON","惩罚",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,100,120,200,50,hwnd,(HMENU)(IDC_BUTTON_1),(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),NULL);
			CreateWindow("BUTTON",REFRESH_STRING,WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,80,120,150,40,hwnd,(HMENU)(IDC_BUTTON_2),(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),NULL);
			CreateWindow("BUTTON",NAME_STRING,WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,400,200,150,40,hwnd,(HMENU)(IDC_BUTTON_3),(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),NULL);
			CreateWindow("BUTTON",NAME_STRING "(女生)",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,400,280,150,40,hwnd,(HMENU)(IDC_BUTTON_4),(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),NULL);
			CreateWindow("BUTTON",NAME_STRING "(男生)",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,400,350,150,40,hwnd,(HMENU)(IDC_BUTTON_5),(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),NULL);
			if(!(dat = init(NAME_FILE, EXTRA_FILE))) {
				MessageBox(hwnd,TEXT("初始化失败!\n请检查文件\"" NAME_FILE "\"和\"" EXTRA_FILE "\"格式是否正确!"),"程序初始化失败",MB_ICONERROR);
				PostQuitMessage(0);
			}
			return 0;
		case WM_PAINT:
			hdc = BeginPaint(hwnd,&ps);
			GetClientRect(hwnd,&rect);
			HBITMAP bmpRs = LoadBitmap(GetModuleHandle(NULL), "BGBMP");
			HDC dcmem = CreateCompatibleDC(hdc);
			SelectObject(dcmem, bmpRs);
			StretchBlt(hdc,0,0,rect.right,rect.bottom,dcmem,0,0,640,480,SRCCOPY);
			EndPaint(hwnd,&ps);
			return 0;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDC_BUTTON_0:
					MessageBox(hwnd,selectdrink(dat),"饮料",MB_OK);
					return 0;
				case IDC_BUTTON_1:
					MessageBox(hwnd,selectextra(dat),"惩罚",MB_OK);
					return 0;
				case IDC_BUTTON_2:
					refresh(dat);
					MessageBox(hwnd,"选中信息已重置...\n技术支持:Zitian Li.","Success",MB_OK);
					return 0;
				case IDC_BUTTON_3:
					selectmessage(hwnd, dat);
					return 0;
				case IDC_BUTTON_4:
					tmp = dat->percent;
					dat->percent = 100;
					selectmessage(hwnd, dat);
					dat->percent = tmp;
					return 0;
				case IDC_BUTTON_5:
					tmp = dat->percent;
					dat->percent = 0;
					selectmessage(hwnd, dat);
					dat->percent = tmp;
					return 0;
			}
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
