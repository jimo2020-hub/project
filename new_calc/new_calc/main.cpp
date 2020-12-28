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
	WNDCLASS wndclass;//���崰���ࣨwindows�������ǻ��ڴ������������ģ�

	wndclass.style = CS_HREDRAW | CS_VREDRAW;//ָ���������ͣ����֡����񡱣�����·���������ʹ�ð�λ��������������
	wndclass.lpfnWndProc = WndProc;//ָ�����ڹ��̣������ǻص�������
	wndclass.cbClsExtra = 0;//Ԥ���Ķ���ռ䣬һ��Ϊ 0
	wndclass.cbWndExtra = 0;//Ԥ���Ķ���ռ䣬һ��Ϊ 0
	wndclass.hInstance = hInstance;//Ӧ�ó����ʵ�����
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//Ϊ���л��ڸô�����Ĵ����趨һ��ͼ��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//Ϊ���л��ڸô�����Ĵ����趨һ�����ָ��
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);//ָ�����ڱ���ɫ
	wndclass.lpszMenuName = NULL;//ָ�����ڲ˵�
	wndclass.lpszClassName = szAppName;//ָ����������

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("���������Ҫ�� Windows NT ����ִ�У�"), szAppName, MB_ICONERROR);
		return 0;
	} 

	hwnd = CreateWindow(szAppName, 
			TEXT("������"), 
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
	char a[M];     //�洢���� 
	char ch='c'; 
	char b[M];     //�洢����� 
	char c[M];     //�洢���� 
	char cn='A',theta;    //theta�������� 
	float d[M];   //������� 
	int i=0,j; 
	int m=0;       //ͳ����������� 
	int n=0;     //��������ַ��� 
	int k=-1; 
	int flag=1000;    //�������ֱ�ʵ����־ 
	float x=0;    

	//��ʱ������ݱ��� 
	int y=0;           //�ڸ������� 
	int z=0;         //ͳ��������Ŀ 
	int count=0;      
	char result[M];//��ż�����
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
			MoveWindow(hwnd,rect.left,rect.top,45 * cxChar,35 * cyChar,TRUE);//����������ĳߴ�

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
				case VK_MULTIPLY://��
					;
				case VK_DECIMAL://С����
					;
				case VK_ADD://��
					;
				case VK_DIVIDE://��
					;
				case VK_SUBTRACT://��
					;
				case VK_SEPARATOR://����
					;
				/*��������0-9*/
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

			/*���ּ�*/
			if(btn_id>=48&&btn_id<=57||btn_id==161)
			{
				GetDlgItemText(hwnd,wParam,(LPSTR)num,sizeof(num));
				GetDlgItemText(hwnd,25,(LPSTR)text,sizeof(text));
				wsprintf(text,"%s%s",text,num);
				SetDlgItemText(hwnd,25,text);
				
			}

			/*�����*/
			if(btn_id==43||btn_id==45||btn_id==42||btn_id==47)
			{
				GetDlgItemText(hwnd,25,(LPSTR)tmp,sizeof(tmp));
				GetDlgItemText(hwnd,btn_id,(LPSTR)op_tmp,sizeof(op_tmp));
				wsprintf(tmp,"%s%s",tmp,op_tmp);
				SetDlgItemText(hwnd,25,tmp);
			}

			/*�����*/
			if(btn_id==0)
			{
				SetDlgItemText(hwnd,25,"");
			}

			/*�Ⱥż�*/
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
							b[m]=ch;//������ŵ�����b 
							m++;
					}
					if(isnum(ch))
					{
						c[n]=ch;//���0��9�Լ�С����,ÿһ����������һ���ո� 
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
						flag=j;//С����λ�� 
					}

					if(flag!=1000&&c[j]==' ')//������    
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
						d[z]=x;        //���ݷ�������d 
						x=0.0;     //x������0 
						z++;        
						flag=1000;     
						y=1; 
					}

					if(flag==1000&&c[j]==' '&&y==0)//ʵ�� 
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
				ch=b[i++];//��ȡ����� 

				while(ch!='#'||gettop(&S)!='#')    
				{ 
					switch(precede(gettop(&S),ch)) 
					{ 
						case '<':        //ջ����������ȼ��� 
						{ 
							push(&S,ch); 
							ch=b[i++]; 
							break; 
						} 
						case '=':         //���ȼ���ͬ 
						{  
							cn=pop(&S); 
							ch=b[i++]; 
							break; 
						} 
						case '>':         //ջ�����ȼ��� 
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

						a1=d[count];    //�������� 
						b1=d[count+1]; 
						d[count]=operate(a1,b1,theta); 

						for(;count<z-2;count++) 
							d[count+1]=d[count+2];   //��ǰ�������� 
							break; 
						} 
					} 
				}  

				GetDlgItemText(hwnd,25,(LPSTR)text,sizeof(text));
				GetDlgItemText(hwnd,wParam,(LPSTR)tmp,sizeof(tmp));
				sprintf(result,"%s%s%f",text,tmp,d[0]);
				SetDlgItemText(hwnd,25,result);
					
			}

			/*�˸��*/
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
			if(MessageBox(NULL,TEXT("ȷ���ر���"),TEXT("��ʾ"),MB_YESNO)==IDYES)
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

