#ifndef _CALC_H_
#define _CALC_H_
#define M 500

struct sqstack  //定义储存运算符的栈 
{ 
	char data[M]; 
	int top; 
};

int initstack(struct sqstack *s);
int stackempty(struct sqstack *s);
int stackfull(struct sqstack *s);
int push(struct sqstack *s,char e);
char pop(struct sqstack *s);
char gettop(struct sqstack *s);
float operate(float a,float b,char ch);
int isoperate(char ch);
int isnum(char ch);
char precede(char a,char b);
int check(char c);
float mi(int x,int y);

#endif