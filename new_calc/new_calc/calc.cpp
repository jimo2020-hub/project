#include "calc.h"

int initstack(struct sqstack *s)  //构造一个空栈 
{ 
	s->top=-1; 
	return 1; 
} 

int stackempty(struct sqstack *s)   //判栈空 
{ 
	return((s->top==-1)? 1:0); 
}

int stackfull(struct sqstack *s)    //判满栈 
{ 
	return((s->top==M-1)? 1:0); 
} 

int push(struct sqstack *s,char e)   //入栈 
{ 
	if(stackfull(s)) 
	return 0; 
	s->top++; 
	s->data[s->top]=e; 
	return 1; 
} 

char pop(struct sqstack *s)    //出栈 
{ 
	char e; 
	if(stackempty(s)) 
	return 0; 
	e=s->data[s->top]; 
	s->top--; 
	return e; 
} 

char gettop(struct sqstack *s)   //得到栈顶元素 
{ 
	char e; 
	if(stackempty(s)) 
	return 0; 
	e=s->data[s->top]; 
	return e; 
} 

float operate(float a,float b,char ch)   //运算操作 
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

int isoperate(char ch)  //判断运算符 
{ 
	if(ch=='+'||ch=='-') 
		return 1; 
	else if(ch=='*'||ch=='/') 
		return 1; 
	else 
		return 0; 
} 

int isnum(char ch)  //判断数字 
{ 
	if((ch>='0'&&ch<='9')||ch=='.') 
		return 1; 
	else 
		return 0; 
} 

char precede(char a,char b)  //判断优先级 
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

int check(char c)    //得出某运算符前面还有几个运算符 
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

float mi(int x,int y)    //求x的y次幂 
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