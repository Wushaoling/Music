#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <commdlg.h>
#include <mmsystem.h>
#include "resource.h"
#include "MainDlg.h"
#pragma comment(lib, "Winmm.lib")
TCHAR all[100]={0};
TCHAR szOpenFileNames[80*MAX_PATH];
int index=0;
int in=0;

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
    }

    return FALSE;
}

void CALLBACK MyTimerProc (HWND hwnd, UINT message, UINT iTimerID,DWORD dwTime)
{
	SYSTEMTIME stLocal;
	char chBuf[256];
	GetLocalTime(&stLocal);
	wsprintf(chBuf,TEXT("%02d:%02d:%02d"),stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
	SetDlgItemText(hwnd,IDC_EDIT5,chBuf);
}

BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    FILE *fp;
    fp=fopen("E:\MyMusic.txt","r+");
	if(fp==NULL)
	{
          fp=fopen("E:\MyMusic.txt","w+");
	}
    else
	{
		char ch;
		int ii=0;
		while((ch=fgtec(fp))!=EOF)
			szOpenFileNames[ii++]=ch;
		szOpenFileNames[ii++]=0;
	}
	SetTimer(hwnd,1,1000,MyTimerProc);
    return TRUE;
}

void con(int i,HWND hwnd)
{
	char statue[10]={0};
	char statue2[10]={0};
	switch (i)
	{
		case 1 : wsprintf(statue,"%s","Playing");  wsprintf(statue2,"%s","play"); break;
		case 2 : wsprintf(statue,"%s","Stopping"); wsprintf(statue2,"%s","stop"); break;
		case 3 : wsprintf(statue,"%s","Pausing");  wsprintf(statue2,"%s","pause"); break;
		case 4 : wsprintf(statue,"%s","Playing");  wsprintf(statue2,"%s","resume"); break;
	}
    TCHAR finalname[100];
	if(1==i)
	{
        in = index;
	}
	wsprintf(finalname,"%s %s",statue2,all);
	mciSendString(finalname,"",0,NULL);
}

void print(int i,HWND hwnd)
{
	switch (i)
	{
		case 1 : 
			{
				SetDlgItemText(hwnd,IDC_EDIT1,all);
                SetDlgItemText(hwnd,IDC_EDIT2,"Playing");
			}
			break;
		case 2 : 
			{
				SetDlgItemText(hwnd,IDC_EDIT1," ");
                SetDlgItemText(hwnd,IDC_EDIT2," ");
				SetDlgItemText(hwnd,IDC_EDIT3," ");
			}
			break;
		case 3 : 
			{
				SetDlgItemText(hwnd,IDC_EDIT1,all);
                SetDlgItemText(hwnd,IDC_EDIT2,"Pausing");
			}
			break;
		case 4 :
			{
				SetDlgItemText(hwnd,IDC_EDIT1,all);
                SetDlgItemText(hwnd,IDC_EDIT2,"Playing");
			}
			break;
	}
	TCHAR position[100];
	TCHAR len1[100];
	long length;
	wsprintf(len1,"Status %s length",all);
	mciSendString(len1,position,255,0);
	length=strtol(position,NULL,10);
	int len = (int )length/1000;
    int min =  len/60;
	int sec = len - min * 60 ;
	TCHAR time[20];
	wsprintf(time,"%i:%i",min,sec);
    SetDlgItemText(hwnd,IDC_EDIT3,time);
}

void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
        case IDC_ADD:
			{
			        OPENFILENAME ofn;
					TCHAR szOpenFileNames[80*MAX_PATH];
					TCHAR szPath[100];
					TCHAR szFileName[100];
					TCHAR szFileName1[100];
					TCHAR shortname[100];
					TCHAR* p;
					int nLen = 0;
					ZeroMemory(&ofn,sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFile =  szOpenFileNames;
					ofn.lpstrFile[0] = TEXT('\0');
					ofn.nMaxFile = sizeof(szOpenFileNames);
					ofn.lpstrFilter = TEXT("ALL\0*.*\0mp3\0*.mp3\0");
					ofn.nFilterIndex = 1;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrFileTitle = NULL;
					ofn.hwndOwner = hwnd;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
					if( GetOpenFileName( &ofn ) )
					{
						//把第一个文件名前的复制到szPath,即:
						//如果只选了一个文件,就复制到最后一个'/'
						//如果选了多个文件,就复制到第一个NULL字符
						lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
						//当只选了一个文件时,下面这个NULL字符是必需的.
						//这里不区别对待选了一个和多个文件的情况
						szPath[ ofn.nFileOffset ] = '/0';
						nLen = lstrlen(szPath);
						p = szOpenFileNames + ofn.nFileOffset;//把指针移到第一个文件	
						while( *p )
						{
							ZeroMemory(szFileName, sizeof(szFileName));
							ZeroMemory(szFileName1, sizeof(szFileName1));
							lstrcat(szFileName, szPath);//给文件名加上路径??
							wsprintf(szFileName1,"%s\\",szFileName);
							lstrcat(szFileName1, p);//加上文件名??
							GetShortPathName(szFileName1,shortname,sizeof(shortname));
							SendDlgItemMessage(hwnd,IDC_LIST3,LB_ADDSTRING,0,(LPARAM)shortname);
							p += lstrlen(p) +1;//移至下一个文件

						}
						//MessageBox(NULL, szFileName, TEXT("MultiSelect"), MB_OK);
					}
			}
            break;
		case IDC_LIST3 :
			{
				if(LBN_DBLCLK==codeNotify)
				{
					TCHAR finalname[100];
					SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,in,(LPARAM)all);
					wsprintf(finalname,"stop %s",all);
	                mciSendString(finalname,"",0,NULL);//停止当前
					
					memset(all,0,sizeof(all));
					memset(finalname,0,sizeof(finalname));

				    index=SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETCURSEL,0,0);				
					SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,index,(LPARAM)all);	
					print(1,hwnd);
                    con(1,hwnd);
				}
				else if(LBN_SELCHANGE==codeNotify)
				{
				    index=SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETCURSEL,0,0);
					memset(all,0,sizeof(all));
					SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,index,(LPARAM)all);
				}
			}
			break;
		case IDC_DELETE :
			{
				    SendDlgItemMessage(hwnd,IDC_LIST3,LB_DELETESTRING,index,0);
			}
			break;
loop:  case IDC_PLAY:
			{		    
				    if(lstrlen(all)>0)
					{
						TCHAR finalname1[100];
						SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,in,(LPARAM)all);
						wsprintf(finalname1,"close %s",all);
						mciSendString(finalname1,"",0,NULL);//停止当前
						SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,index,(LPARAM)all);
						print(1,hwnd);
					    con(1,hwnd);
					}
					else
						MessageBox(NULL, TEXT("请选择歌曲"), TEXT(""), MB_OK|MB_ICONWARNING);
			}
		    break;
		case IDC_STOP :
				{								
					if(lstrlen(all)>0)
					{	print(2,hwnd);
					con(2,hwnd);}
					else
						MessageBox(NULL, TEXT("请选择歌曲"), TEXT(""), MB_OK|MB_ICONWARNING);
				}
				break;
		case IDC_PAUSE :
				{				
					if(lstrlen(all)>0)
					{	print(3,hwnd);
					con(3,hwnd);}
					else
						MessageBox(NULL, TEXT("请选择歌曲"), TEXT(""), MB_OK|MB_ICONWARNING);
				}
				break;
		case IDC_CONTINUE :
				{
					if(lstrlen(all)>0)
					{print(4,hwnd);
					con(4,hwnd);}
					else
						MessageBox(NULL, TEXT("请选择歌曲"), TEXT(""), MB_OK|MB_ICONWARNING);
				}
				break;
		case IDC_UP :
				{
					TCHAR cmd[256];//媒体命令
					TCHAR volume[256];//音量
					int vol;//音量，整型
					wsprintf(cmd,"status %s volume",all);
					mciSendString(cmd,volume,sizeof(volume),0);//获取当前音量至volume字符串中
					//MessageBox(hwnd,volume,"",NULL);
					vol = atoi(volume);
					memset(cmd,0,sizeof(cmd));
					wsprintf(cmd,"setaudio %s volume to %i",all,vol+200);
					//MessageBox(hwnd,cmd,"",NULL);
					mciSendString(cmd,"",0,NULL);
				}
				break;
			case IDC_DOWN :
				{
					TCHAR cmd[256];//媒体命令
					TCHAR volume[256];//音量
					int vol;//音量，整型
					wsprintf(cmd,"status %s volume",all);
					mciSendString(cmd,volume,sizeof(volume),0);//获取当前音量至volume字符串中
					//MessageBox(hwnd,volume,"",NULL);
					vol = atoi(volume);
					memset(cmd,0,sizeof(cmd));
					wsprintf(cmd,"setaudio %s volume to %i",all,vol-200);
					//MessageBox(hwnd,cmd,"",NULL);
					mciSendString(cmd,"",0,NULL);
				}
				break;
			case IDC_X :
				{
					TCHAR m[100];
					TCHAR MM[65536];
					FILE *fp;
					int i;
					fp=fopen("E:\MyMusic.txt","w+");
					for(i=1;;i++)
					{    
						SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,i,(LPARAM)m);
						if(lstrlen(m)==0)
							break;
						else
							lstrcat(MM,m);
					}

					fclose(fp);
                    EndDialog(hwnd, 0);
				}
				break;
			case IDC_NEXT :
				{
			        TCHAR finalname[100];
					SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,in,(LPARAM)all);
					wsprintf(finalname,"close %s",all);
	                mciSendString(finalname,"",0,NULL);
					in++;
					index = in;						
					memset(all,0,sizeof(all));
					SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,in,(LPARAM)all);
					if(lstrlen(all)==0)
						MessageBox(hwnd,TEXT("当前已是最后一首"),"",MB_OK|MB_ICONWARNING);
					else
						goto loop;
				}
				break;
			case IDC_LAST :
				{
					TCHAR finalname[100];
					SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,in,(LPARAM)all);
					wsprintf(finalname,"close %s",all);
	                mciSendString(finalname,"",0,NULL);
					in--;
					index = in;					
					memset(all,0,sizeof(all));
					SendDlgItemMessage(hwnd,IDC_LIST3,LB_GETTEXT,in,(LPARAM)all);
					if(lstrlen(all)==0)
						MessageBox(hwnd,TEXT("当前是第一首"),"",MB_OK|MB_ICONWARNING);
					else
						goto loop;
				}
				break;
        default:
		break;
    }
}
void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}
//GetShortPathName(all,shortname,sizeof(shortname));
//MessageBox(hwnd,File,"",NULL);
				     	
