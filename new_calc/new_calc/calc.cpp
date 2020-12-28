#include "calc.h"

int initstack(struct sqstack *s)  //����һ����ջ 
{ 
	s->top=-1; 
	return 1; 
} 

int stackempty(struct sqstack *s)   //��ջ�� 
{ 
	return((s->top==-1)? 1:0); 
}

int stackfull(struct sqstack *s)    //����ջ 
{ 
	return((s->top==M-1)? 1:0); 
} 

int push(struct sqstack *s,char e)   //��ջ 
{ 
	if(stackfull(s)) 
	return 0; 
	s->top++; 
	s->data[s->top]=e; 
	return 1; 
} 

char pop(struct sqstack *s)    //��ջ 
{ 
	char e; 
	if(stackempty(s)) 
	return 0; 
	e=s->data[s->top]; 
	s->top--; 
	return e; 
} 

char gettop(struct sqstack *s)   //�õ�ջ��Ԫ�� 
{ 
	char e; 
	if(stackempty(s)) 
	return 0; 
	e=s->data[s->top]; 
	return e; 
} 

float operate(float a,float b,char ch)   //������� 
{ 
	if(ch=='+')
	{
		return a+b; 
	}
	else if(ch=='-') 
	{
		return a-b; 
	}
	else if(ch=='/') 
	{
	    return a/b; 
	}
	else if(ch=='*') 
	{
	    return a*b; 
	}
	else 
	{
	    return 0; 
	}
} 

int isoperate(char ch)  //�ж������ 
{ 
	if(ch=='+'||ch=='-') 
		return 1; 
	else if(ch=='*'||ch=='/') 
		return 1; 
	else 
		return 0; 
} 

int isnum(char ch)  //�ж����� 
{ 
	if((ch>='0'&&ch<='9')||ch=='.') 
		return 1; 
	else 
		return 0; 
} 

char precede(char a,char b)  //�ж����ȼ� 
{ 
	if(a=='+'||a=='-') 
	{ 
		if(b=='*'||b=='/') 
		{
			return '<';
		}
		/*else if(b=='(') 
		{
			return '<'; 
		}*/
		else
		{
			return '>'; 
		}
	} 

	else if(a=='*'||a=='/') 
	{ 
		if(b=='(') 
		{
			return '<';
		}
		else
		{
			return '>';
		}
	} 

	else if(a=='(') 
	{ 
		if(b==')') 
		{
			return '='; 
	
		}	
		else
		{
			return '<'; 
		}
	} 

	else if(a==')') 
	{
		return '>'; 
	}

	else if(a=='#') 
	{ 
		if(b=='#') 
		{
			return '=';
		}
		else 
		{
			return '<';
		}
	} 

	else 
	{
		return 0; 
	}
} 

int check(char c)    //�ó�ĳ�����ǰ�滹�м�������� 
{ 
	if(c=='+'||c=='-') 
	{
		return 1;
	}
	else if(c=='*'||c=='/') 
	{
		return 1; 
	}
	else
	{
		return 0; 
	}
} 

float mi(int x,int y)    //��x��y���� 
{ 
	float n; 

	if(y==0)
	{
		n=1;
	}

	if(y>0)
	{
		n=mi(x,y-1)*x;
	}

	if(y<0)
	{
		n=mi(x,y+1)/x;
	}
	return n; 
} 