#include <windows.h>
#include "strsafe.h"
#include "calc.h"
#define NUM 4
#define M 500

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR  *button_text[NUM]=
{
	TEXT("7"),
	TEXT("4"),
	TEXT("1"),
	TEXT("0"),

};

TCHAR  *button_text1[NUM]=
{
	TEXT("8"),
	TEXT("5"),
	TEXT("2"),
	TEXT("."),
};

TCHAR  *button_text2[NUM+1]=
{
	TEXT("C"),
	TEXT("9"),
	TEXT("6"),
	TEXT("3"),
	TEXT("="),
};

TCHAR  *button_text3[NUM+1]=
{	
	TEXT("<--"),
	TEXT("/"),
	TEXT("*"),
	TEXT("-"),
	TEXT("+"),
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	
	static TCHAR szAppName[] = TEXT("MyWindows");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;//定义窗口类（windows窗口总是基于窗口类来创建的）

	wndclass.style = CS_HREDRAW | CS_VREDRAW;//指定窗口类型，各种“类风格”（详见下方↓）可以使用按位或操作符组合起来
	wndclass.lpfnWndProc = WndProc;//指定窗口过程（必须是回调函数）
	wndclass.cbClsExtra = 0;//预留的额外空间，一般为 0
	wndclass.cbWndExtra = 0;//预留的额外空间，一般为 0
	wndclass.hInstance = hInstance;//应用程序的实例句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//为所有基于该窗口类的窗口设定一个图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//为所有基于该窗口类的窗口设定一个鼠标指针
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);//指定窗口背景色
	wndclass.lpszMenuName = NULL;//指定窗口菜单
	wndclass.lpszClassName = szAppName;//指定窗口类名

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("这个程序需要在 Windows NT 才能执行！"), szAppName, MB_ICONERROR);
		return 0;
	} 

	hwnd = CreateWindow(szAppName, 
			TEXT("计算器"), 
			WS_SYSMENU|WS_MINIMIZEBOX,
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT,
			NULL, 
			NULL, 
			hInstance, 
			NULL);
        
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	static int cxChar,cyChar;
	HWND hButton ;
	int btn_id,len=0;
	struct sqstack S;
	char text[M];
	int num[M];	
	char a[M];     //存储输入 
	char ch='c'; 
	char b[M];     //存储运算符 
	char c[M];     //存储数据 
	char cn='A',theta;    //theta存放运算符 
	float d[M];   //存放数据 
	int i=0,j; 
	int m=0;       //统计运算符数量 
	int n=0;     //存放输入字符数 
	int k=-1; 
	int flag=1000;    //浮点数分别实数标志 
	float x=0;    

	//临时存放数据变量 
	int y=0;           //在浮点型与 
	int z=0;         //统计数据数目 
	int count=0;      
	char result[M];//存放计算结果
	char tmp[M];
	char op_tmp[M];
	float a1=0.0,b1=0.0; 
	initstack(&S); 

	switch (message)
	{

		case WM_CREATE:
			cxChar = LOWORD(GetDialogBaseUnits());
			cyChar = HIWORD(GetDialogBaseUnits());

			GetWindowRect(hwnd,&rect);
			MoveWindow(hwnd,rect.left,rect.top,45 * cxChar,35 * cyChar,TRUE);//设置主窗体的尺寸

			CreateWindow(TEXT("EDIT"),NULL,WS_CHILD|WS_VISIBLE|ES_RIGHT,2 * cxChar,9 * cyChar,39 * cxChar,2 * cyChar,hwnd,(HMENU)25,((LPCREATESTRUCT) lParam) -> hInstance,NULL);

			CreateWindow(TEXT("button"),
					button_text[0],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar,cyChar * (8 + 4 *2),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(55),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
					); 

			CreateWindow(TEXT("button"),
					button_text[1],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar,cyChar * (8 + 4 * 3),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(52),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text[2],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar,cyChar * (8 + 4 * 4),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(49),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text[3],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar,cyChar * (8 + 4 * 5),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(48),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
					); 

			CreateWindow(TEXT("button"),
					button_text1[0],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*6,cyChar * (8 + 4 *2),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(56),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
					); 

			CreateWindow(TEXT("button"),
					button_text1[1],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*6,cyChar * (8 + 4 * 3),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(53),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text1[2],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*6,cyChar * (8 + 4 * 4),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(50),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text1[3],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*6,cyChar * (8 + 4 * 5),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(161),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
				); 

			CreateWindow(TEXT("button"),
					button_text2[0],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*11,cyChar * (8 + 4 ),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(0),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
					); 

			CreateWindow(TEXT("button"),
					button_text2[1],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*11,cyChar * (8 + 4 * 2),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(57),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
					); 

			CreateWindow(TEXT("button"),
					button_text2[2],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*11,cyChar * (8 + 4 * 3),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(54),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text2[3],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*11,cyChar * (8 + 4 * 4),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(51),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text2[4],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*11,cyChar * (8 + 4 * 5),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(61),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
				); 

			CreateWindow(TEXT("button"),
					button_text3[0],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*16,cyChar * (8 + 4 ),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(8),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
					);
				
			CreateWindow(TEXT("button"),
					button_text3[1],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*16,cyChar * (8 + 4 * 2),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(47),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
					); 

			CreateWindow(TEXT("button"),
					button_text3[2],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*16,cyChar * (8 + 4 * 3),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(42),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text3[3],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*16,cyChar * (8 + 4 * 4),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(45),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
			); 

			CreateWindow(TEXT("button"),
					button_text3[4],
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
					2*cxChar*16,cyChar * (8 + 4 * 5),9 * cxChar,14 * cyChar / 4,
					hwnd,(HMENU)(43),
					((LPCREATESTRUCT) lParam)->hInstance,NULL					
				);
			return 0;

		case WM_KEYDOWN:
			if (wParam != VK_LEFT)
   				break ;
			wParam = VK_BACK ;		

		case WM_CHAR:
			switch(wParam)
			{
				case VK_MULTIPLY://乘
					;
				case VK_DECIMAL://小数点
					;
				case VK_ADD://加
					;
				case VK_DIVIDE://除
					;
				case VK_SUBTRACT://减
					;
				case VK_SEPARATOR://等于
					;
				/*键盘数字0-9*/
				case 0x30:
					;
				case 0x31:
					;
				case 0x32:
					;
				case 0x33:
					;
				case 0x34:
					;
				case 0x35:
					;
				case 0x36:
					;
				case 0x37:
					;
				case 0x38:
					;
				case 0x39:
					;
			}

			if ((wParam = (WPARAM) CharUpper ((TCHAR *) wParam)) == VK_RETURN)
			{
				wParam = '=' ;
			}

 			hButton = GetDlgItem (hwnd, wParam);
			SendMessage (hButton,BM_SETSTATE,1,0) ;
			Sleep(100);
			SendMessage (hButton, BM_SETSTATE, 0, 0) ;

		case WM_COMMAND:
			btn_id = LOWORD(wParam);
			SetFocus(hwnd);

			/*数字键*/
			if(btn_id>=48&&btn_id<=57||btn_id==161)
			{
				GetDlgItemText(hwnd,wParam,(LPSTR)num,sizeof(num));
				GetDlgItemText(hwnd,25,(LPSTR)text,sizeof(text));
				wsprintf(text,"%s%s",text,num);
				SetDlgItemText(hwnd,25,text);
				
			}

			/*运算符*/
			if(btn_id==43||btn_id==45||btn_id==42||btn_id==47)
			{
				GetDlgItemText(hwnd,25,(LPSTR)tmp,sizeof(tmp));
				GetDlgItemText(hwnd,btn_id,(LPSTR)op_tmp,sizeof(op_tmp));
				wsprintf(tmp,"%s%s",tmp,op_tmp);
				SetDlgItemText(hwnd,25,tmp);
			}

			/*清除键*/
			if(btn_id==0)
			{
				SetDlgItemText(hwnd,25,"");
			}

			/*等号键*/
			if(btn_id==61)
			{
				memset(text,0,sizeof(text));
				memset(d,0,sizeof(d));
				GetDlgItemText(hwnd,25,(LPSTR)text,sizeof(text));

				while(i<strlen(text))
				{
					a[i]=text[i];
					i++;
				}

				for(j=0;j<i;j++)
				{
					ch=a[j];
					if(isoperate(ch))
					{
							b[m]=ch;//运算符放到数组b 
							m++;
					}
					if(isnum(ch))
					{
						c[n]=ch;//存放0到9以及小数点,每一个数据输入一个空格 
						n++;
					}
					if(isoperate(a[j+1]))
					{
						c[n] = ' ';
						n++;
					}
				}

				c[n]= ' ';

				for(j=0;j<n+1;j++)
				{
					if(c[j]=='.')
					{
						flag=j;//小数点位置 
					}

					if(flag!=1000&&c[j]==' ')//浮点数    
					{ 
						for(i=k+1;i<j;i++)      
						{ 
							if(i<flag&&c[i]>48) 
							{
								x=x+mi(10,flag-i-1)*(c[i]-48); 
							}
							if(i>flag&&c[i]>48)
							{
								x=x+mi(10,flag-i)*(c[i]-48); 
								k=j;
							}
						} 
						d[z]=x;        //数据放入数组d 
						x=0.0;     //x重新置0 
						z++;        
						flag=1000;     
						y=1; 
					}

					if(flag==1000&&c[j]==' '&&y==0)//实数 
					{   
						for(i=k+1;i<j;i++)  
						{ 
							x=x+mi(10,(j-i-1))*(c[i]-48); 
							k=j; 
						} 
						d[z]=x; 
						x=0.0; 
						z++; 
					} 
						y=0; 
				} 

				b[m]='#'; 
				push(&S,'#'); 
				i=0; 
				ch=b[i++];//读取运算符 

				while(ch!='#'||gettop(&S)!='#')    
				{ 
					switch(precede(gettop(&S),ch)) 
					{ 
						case '<':        //栈顶运算符优先级低 
						{ 
							push(&S,ch); 
							ch=b[i++]; 
							break; 
						} 
						case '=':         //优先级相同 
						{  
							cn=pop(&S); 
							ch=b[i++]; 
							break; 
						} 
						case '>':         //栈顶优先级高 
						{  
							theta=pop(&S); 
							j=S.top; 
							count=0; 

							for(n=0;n<=j;n++) 
							{ 
								if(check(S.data[n])) 
								{
									count++; 
								}
							} 

						a1=d[count];    //读入数据 
						b1=d[count+1]; 
						d[count]=operate(a1,b1,theta); 

						for(;count<z-2;count++) 
							d[count+1]=d[count+2];   //向前覆盖数据 
							break; 
						} 
					} 
				}  

				GetDlgItemText(hwnd,25,(LPSTR)text,sizeof(text));
				GetDlgItemText(hwnd,wParam,(LPSTR)tmp,sizeof(tmp));
				sprintf(result,"%s%s%f",text,tmp,d[0]);
				SetDlgItemText(hwnd,25,result);
					
			}

			/*退格键*/
			if(btn_id == 8)
			{			
				memset(tmp,0,sizeof(tmp));
				GetDlgItemText(hwnd,25,(LPSTR)text,sizeof(text));
				len = strlen(text);

				if(len == 1)
				{
					SetDlgItemText(hwnd,25,"");
				}
				else
				{
					for(i=0;i<len-1;i++)
					{
						tmp[i]=text[i];
					}
					memset(text,0,sizeof(text));
					wsprintf(text,"%s",tmp);
					SetDlgItemText(hwnd,25,text);
				}
			}
			return 0;

		case WM_CLOSE:
			if(MessageBox(NULL,TEXT("确定关闭吗？"),TEXT("提示"),MB_YESNO)==IDYES)
			{
				DestroyWindow(hwnd);
			}
			else
			{
				return 0;
			}

		case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

