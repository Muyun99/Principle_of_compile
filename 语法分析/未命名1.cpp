#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <cstdlib>

using namespace std;

#define Max 655	 //最大代码长度
#define WordMaxNum 256	 //变量最大个数
#define DigitNum 256	//常量最大个数
#define MaxKeyWord	13	//关键字数量
#define MaxOptANum 8	//运算符最大个数
#define MaxOptBNum 4	//运算符最大个数
#define MaxEndNum 11	//界符最大个数

typedef struct DisplayTable
{
    int Index;  //标识符所在表的下标
    int type;	//标识符的类型
    int line;	//标识符所在表的行数
    char symbol[20];	//标识符所在表的名称
} Table;

int TableNum = 0;  //display表的表项总数
char Word[WordMaxNum][20]; //标识符表
char Digit[WordMaxNum][20]; //数字表
int WordNum = 0;   //变量表的下标
int DigNum = 0;	 //常量表的下标
bool errorFlag = 0; //错误标志
int TableIndex = -1;  //display 表的下标索引
int beginCount = 0;//遇到begin加1，遇到end减1
int ifCount = 0; //遇到if加1
Table *table = new Table[Max];

//关键字
const char* const KeyWord[MaxKeyWord] = {"begin","if","then","while","do","end","var","program","const","procedure","read","write","call"};


// 单目运算
const char OptA[] = {'+','-','*','/','=','#','<','>'};

//双目运算符
const char *OptB[] = {"<=",">=",":=","<>"};
// 界符
const char End[] = {'(', ')', ',', ';', '.', '[', ']', ':', '{', '}', '"'};

void error(char str[20],int nLine, int errorType)
{
    errorFlag = 1;
    cout <<" \nError :    ";
    switch(errorType)
    {
    case 1:
        cout << "第" << nLine-1 <<"行" << str << " 变量的长度超过限制！\n";
        break;
    case 2:
        cout << "第" << nLine-1 <<"行" << str << " 小数点错误！\n";
        break;
    case 3:
        cout << "第" << nLine-1 <<"行" << str << " 常量的长度超过限制！\n";
        break;
    }//switch
}//error

void Scanner(char ch[],int chLen,int nLine)
{
    int chIndex = 0;
    while(chIndex < chLen) //对输入的字符扫描
    {
        /****************处理空格和tab***************************/
        //忽略空格和tab
        while(ch[chIndex] == ' ' || ch[chIndex] == '	' )
        {
            chIndex ++;
        }
        /*************************处理换行符*********************/
        //遇到换行符，行数加1
        while(ch[chIndex] == '!')
        {
            nLine++;
            chIndex ++;
        }

        /***************************标识符**********************/
        if( isalpha(ch[chIndex])) //以字母、下划线开头
        {
            char str[256];
            int strLen = 0;
//是字母、下划线
            while(isalpha(ch[chIndex]) || ch[chIndex] == '_' )
            {
                str[strLen ++] = ch[chIndex];
                chIndex ++;
                while(isdigit(ch[chIndex]))//不是第一位，可以为数字
                {
                    str[strLen ++] = ch[chIndex];
                    chIndex ++;
                }
            }
            str[strLen] = 0; //字符串结束符
            if(strlen(str) > 20) //标识符超过规定长度，报错处理
            {
                error(str,nLine,1);
            }
            else
            {
                int i;
                for(i = 0; i < MaxKeyWord; i++) //与关键字匹配
//是关键字，写入table表中
                    if(strcmp(str, KeyWord[i]) == 0)
                    {
                        strcpy(table[TableNum].symbol,str);
                        table[TableNum].type = 1;  //关键字
                        table[TableNum].line = nLine;
                        table[TableNum].Index = i;
                        TableNum ++;
                        break;
                    }
                if(i >= MaxKeyWord) //不是关键字
                {
                    table[TableNum].Index = WordNum;
                    strcpy(Word[WordNum++],str);
                    table[TableNum].type = 2; //变量标识符
                    strcpy(table[TableNum].symbol,str);
                    table[TableNum].line = nLine;
                    TableNum ++;
                }
            }
        }

        /**************************常数**************************/
        else if(isdigit(ch[chIndex])) //遇到数字
        {
            int flag = 0;
            char str[256];
            int strLen = 0;
//数字和小数点
            while(isdigit(ch[chIndex]) || ch[chIndex] == '.')
            {
//flag表记小数点的个数，0时为整数，1时为小数，2时出错
                if(ch[chIndex] == '.')
                    flag ++;
                str[strLen ++] = ch[chIndex];
                chIndex ++;
            }
            str[strLen] = 0;
            if(strlen(str) > 20) //常量标识符超过规定长度20，报错处理
            {
                error(str,nLine,3);
            }
            if(flag == 0)
            {
                table[TableNum].type = 3; //整数
            }
            if(flag == 1)
            {
                table[TableNum].type = 4; //小数
            }
            if(flag > 1)
            {
                error(str,nLine,2);
            }
            table[TableNum].Index = DigNum;
            strcpy(Digit[DigNum ++],str);
            strcpy(table[TableNum].symbol,str);
            table[TableNum].line = nLine;
            TableNum ++;
        }
        /*************************运算符************************/
        else
        {
            //用来区分是不是无法识别的标识符，0为运算符，1为界符
            int errorFlag;
            char str[3];
            str[0] = ch[chIndex];
            str[1] = ch[chIndex + 1];
            str[2] = '\0';
            int i;
            for( i = 0; i < MaxOptBNum; i++) //MaxOptBNum)
                if(strcmp(str,OptB[i]) == 0)
                {
                    errorFlag = 0;
                    table[TableNum].type = 6;
                    strcpy(table[TableNum].symbol,str);
                    table[TableNum].line = nLine;
                    table[TableNum].Index = i;
                    TableNum ++;
                    chIndex  = chIndex + 2;
                    break;
                }
            if(i >= MaxOptBNum)
            {
                for( int k = 0; k < MaxOptANum; k++)
                    if(OptA[k] == ch[chIndex])
                    {
                        errorFlag = 0;
                        table[TableNum].type = 5;
                        table[TableNum].symbol[0] = ch[chIndex];
                        table[TableNum].symbol[1] = 0;
                        table[TableNum].line = nLine;
                        table[TableNum].Index = k;
                        TableNum ++;
                        chIndex ++;
                        break;
                    }
                /*************************界符************************/
                for(int j = 0; j < MaxEndNum; j ++)
                    if(End[j] ==ch[chIndex])
                    {
                        errorFlag = 1;
                        table[TableNum].line = nLine;
                        table[TableNum].symbol[0] = ch[chIndex];
                        table[TableNum].symbol[1] = 0;
                        table[TableNum].Index = j;
                        table[TableNum].type = 7;
                        TableNum ++;
                        chIndex ++;
                    }
                /********************其他无法识别字符*****************/
//开头的不是字母、数字、运算符、界符
                if(errorFlag != 0 && errorFlag != 1)
                {
                    char str[256];
                    int strLen = -1;
                    str[strLen ++] = ch[chIndex];
                    chIndex ++;
                    while(*ch != ' ' || *ch != 9 || ch[chIndex] != 10)
                    {
                        str[strLen ++] = ch[chIndex];
                        chIndex ++;
                    }
                    str[strLen] = 0;
                    table[TableNum].type = 8;
                    strcpy(table[TableNum].symbol,str);
                    table[TableNum].line = nLine;
                    table[TableNum].Index = -2;
                    TableNum ++;
                }
            }
        }
    }
}
/**************把十进制小数转为16进制******************/
void Trans(double x,int p)  //把十进制小数转为16进制
{
    int i=0;                  //控制保留的有效位数
    while(i<p)
    {
        if(x==0)              //如果小数部分是0
            break;            //则退出循环
        else
        {
            int k=int(x*16);  //取整数部分
            x=x*16-int(k);    //得到小数部分
            if(k<=9)
                cout<<k;
            else
                cout<<char(k+55);
        };
        i++;
    };
};
/***********************语法错误*************************/
void Gerror(int errorType,int nIndex)
{
    errorFlag = 1;
    switch(errorType)
    {
    case 1:
        cout << "第" << table[nIndex].line <<"行：" << table[nIndex].symbol <<" 应该为赋值号：= \n";
        break;
    case 2:
        cout << "第" << table[nIndex].line <<"行：" << table[nIndex].symbol <<" 应为变量 \n";
        break;
    case 3:
        cout << "第" << table[nIndex].line <<"行：" << table[nIndex].symbol <<" 应为逗号 \n";
        break;
    case 4:
        cout << "第" << table[nIndex].line <<"行：" << table[nIndex].symbol <<" 应为分号 \n";
        break;
    case 5:
        cout << "第" << table[nIndex].line <<"行：" << table[nIndex].symbol <<" 应为运算符 \n";
        break;
    case 6:
        cout << "第" << table[nIndex].line <<"行：" << table[nIndex].symbol <<" 应为变量或常量 \n";
        break;
    case 7:
        cout << "第" <<table[TableIndex].line  <<"行 "
             << table[nIndex].symbol << "与"
             <<table[TableIndex + 1].symbol
             <<"之间缺少运算符 \n";
        break;
    case 8:
        cout << "第" << table[nIndex].line <<"行：" << table[nIndex + 1 ].symbol <<" 应为'(' \n";
        break;
    case 9:
        cout << "第" <<table[TableIndex].line  <<"行 "
             << table[TableIndex].symbol << "与" <<table[nIndex + 1].symbol <<"之间缺少'(' \n";
        break;
    case 10:
        cout << "第"  << table[TableIndex - 1].line
             << " 行: 缺少'then'" << endl;
        break;
    case 11:
        cout << "第"  << table[TableIndex].line  << " 行:"
             <<table[nIndex].symbol << "应为then \n";
        break;
    case 12:
        cout << "第"  << table[TableIndex].line  << " 行: end 后不能接 " <<table[TableIndex].symbol << endl;
        break;
    case 13:
        cout << "第" << table[nIndex].line <<"行： "
             << table[nIndex - 1].symbol <<"与 "
             <<table[TableIndex].symbol <<"之间缺少变量 \n";
        break;
    case 14:
        cout << "第" <<table[nIndex ].line  <<"行 "
             << table[nIndex ].symbol << "后缺少';' \n";
        break;
    case 15:
        cout << "第"  << table[TableIndex].line  << " 行:"
             <<table[nIndex].symbol << "应为')' \n";
        break;
    case 16:
        cout << "第"  << table[TableIndex].line
             << " 行,begin 后不能接 "
             <<table[TableIndex].symbol << endl;
        break;
    }
}
/************************表达式判断********************/
bool express()
{
    while(1)
    {
        if(table[TableIndex].type==2||table[TableIndex].type == 3 )
        {
            if(table[TableIndex].type==3&&table[TableIndex+ 1].type == 2 && table[TableIndex].line == table[TableIndex + 1].line)
            {
                Gerror(7,TableIndex); //出错信息：该处缺少运算符
                //TableIndex = TableIndex + 2;
                TableIndex ++;
            }
            if(table[TableIndex].line != table[TableIndex + 1].line)
            {
                //	Gerror(14,TableIndex); //出错信息：该语句缺少分号
                return 1;
            }
            TableIndex ++;
        }
        else
        {
            if(table[TableIndex].type == 5||table[TableIndex].type == 6)
            {
                Gerror(13,TableIndex);
                TableIndex ++;
            }
            else
            {
                Gerror(6,TableIndex); //出错信息：该处应为变量或常量
            }
            TableIndex ++;
        }
        if(table[TableIndex].type ==5|| table[TableIndex].type == 6 )
            TableIndex ++;
        else if(table[TableIndex].type == 7)
            return 1;
        else if(TableIndex >= TableNum) //||  )
        {
            Gerror(14,TableIndex); //出错信息：该语句缺少分号
            return 1;
        }
        else
        {
            Gerror(5,TableIndex); //出错信息：此处应为运算符
            TableIndex ++;
        }
    }
}

/*******************赋值语句判断*************************/
bool Assign()  //赋值语句的判断
{
    TableIndex ++;
    if(strcmp( ":=", table[TableIndex].symbol) == 0)
    {
        TableIndex ++;
    }
    else
    {
        Gerror(1,TableIndex);  //出错信息：赋值号应该为":="
        TableIndex ++;
    }
    if(express())  //":="后可以为变量或常量
    {
        if(strcmp(";",table[TableIndex].symbol) == 0)
        {
            return 1;
        }
        else
        {
            if(TableIndex >= TableNum)
            {
                Gerror(14,TableIndex); //出错信息：该语句缺少分号
                return 1;
            }
            else if(table[TableIndex].line != table[TableIndex + 1].line)
            {
                Gerror(14,TableIndex); //出错信息：该语句缺少分号
                return 1;
                //TableIndex ++;
            }
        }
    }
    else
    {
        Gerror(6,TableIndex);  //出错信息：":="后应为变量或常量
        TableIndex ++;
    }
    return 0;
}
/**********************语句判断*************************/
bool judge() //条件、循环、初始化语句的判断
{
    /**************************begin**********************/
    if(strcmp("begin",table[TableIndex].symbol)==0) //匹配begin
    {
        beginCount ++;
        if(table[TableIndex + 1].type == 7)
        {
            TableIndex ++;
            cout << "第"  << table[TableIndex].line
                 << " 行,begin 后不能接 " ;
        }
        else
            return 1;
    }

    if(strcmp("while",table[TableIndex].symbol)==0) //匹配begin
    {
        TableIndex ++;
        if(table[TableIndex].type == 2)
        {
            TableIndex ++;
            /*	if(table[TableIndex].type == 2)
            	{
            		 Gerror(5,TableIndex); //出错信息：此处应为运算符
            	}*/
            if(table[TableIndex].type == 5||table[TableIndex].type == 6)
            {
                TableIndex ++;
                if(table[TableIndex].type==2||table[TableIndex].type == 3 )
                {
                    TableIndex ++;
                    if(strcmp("do",table[TableIndex].symbol)==0)
                        return 1;
                    else
                        cout << "此处应为do" <<endl;
                }
                else
                {
                    Gerror(6,TableIndex);  //出错信息：":="后应为变量或常量
                    TableIndex ++;
                }
            }
            else
            {
                Gerror(5,TableIndex); //出错信息：此处应为运算符
                TableIndex ++;
            }
        }
        else
        {
            Gerror(2,TableIndex);//出错信息：此处应为变量
            TableIndex ++;
        }
    }

    if(strcmp("do",table[TableIndex].symbol)==0) //匹配do
    {
        if(table[TableIndex + 1].type == 7)
        {
            TableIndex ++;
            cout << "第"  << table[TableIndex].line
                 << " 行,do	 后不能接 "
                 <<table[TableIndex].symbol << endl;
            return 1;
        }
    }
    /**************************end***********************/
    if(strcmp("end",table[TableIndex].symbol) == 0) //匹配end
    {
        beginCount --;
        if(TableIndex < TableNum)
            if(table[TableIndex+1].type==7||table[TableIndex+ 1].type == 8)
            {
                TableIndex ++;
                Gerror(12,TableIndex);
                return 1;
            }
    }
    /**************************else**********************/
    if(strcmp("else",table[TableIndex].symbol) == 0) //匹配else
    {
        ifCount --;
        return 1;
    }
    if(strcmp("program",table[TableIndex].symbol) == 0)	//匹配program
    {
        TableIndex++;
        if(express())  //":="后可以为变量或常量
        {
            if(strcmp(";",table[TableIndex].symbol) == 0)
            {
                return 1;
            }
            else
            {
                if(TableIndex >= TableNum)
                {
                    Gerror(14,TableIndex); //出错信息：该语句缺少分号
                    return 1;
                }
                else if(table[TableIndex].line != table[TableIndex + 1].line)
                {
                    Gerror(14,TableIndex); //出错信息：该语句缺少分号
                    return 1;
                    //TableIndex ++;
                }
            }
        }
    }


    if(strcmp("procedure",table[TableIndex].symbol) == 0 || strcmp("call",table[TableIndex].symbol) == 0)	//匹配program
    {
        TableIndex ++;
        if(table[TableIndex].type == 2 || table[TableIndex].type == 3)  // program 后为变量或常量
        {
            TableIndex ++;
//语句结束，“；”
            if(strcmp("(",table[TableIndex].symbol) == 0)
            {
                TableIndex ++;
                if(table[TableIndex].type == 2)
                {
                    TableIndex ++;

                    if(strcmp(")",table[TableIndex].symbol) == 0)
                    {
                        if(1)  //":="后可以为变量或常量
                        {
                            if(strcmp(";",table[TableIndex].symbol) == 0)
                            {
                                return 1;
                            }
                            else
                            {
                                if(TableIndex >= TableNum)
                                {
                                    Gerror(14,TableIndex); //出错信息：该语句缺少分号
                                    return 1;
                                }
                                else if(table[TableIndex].line != table[TableIndex + 1].line)
                                {
                                    Gerror(14,TableIndex); //出错信息：该语句缺少分号
                                    return 1;
                                    //TableIndex ++;
                                }
                            }
                        }
                    }
                    else
                    {
                        Gerror(15,TableIndex);//出错信息：此处应为")"
                    }
                }
                else
                {
                    Gerror(2,TableIndex);//出错信息：此处应为变量
                }
            }
            else
            {
                Gerror(8,TableIndex);//出错信息：此处应为"("
            }
        } //if
        else
        {
            Gerror(2,TableIndex);//出错信息：此处应为变量
            TableIndex ++;
        }
    }

    if(strcmp("write",table[TableIndex].symbol) == 0 || strcmp("read",table[TableIndex].symbol) == 0)	//匹配program
    {
        TableIndex ++;
        if(strcmp("(",table[TableIndex].symbol) == 0)
        {
            TableIndex ++;
            if(table[TableIndex].type == 2)
            {
                TableIndex ++;
                if(strcmp(")",table[TableIndex].symbol) == 0)
                {
                    TableIndex ++;
                    if(strcmp(";",table[TableIndex].symbol) == 0)
                    {
                        return 1;
                    }
                    else
                    {
                        Gerror(4,TableIndex);
                        return 1;//出错信息：此处应为";"
                    }
                }
                else
                {
                    Gerror(15,TableIndex);//出错信息：此处应为")"
                }
            }
            else
            {
                Gerror(2,TableIndex);//出错信息：此处应为变量
            }
        }
        else
        {
            Gerror(8,TableIndex);//出错信息：此处应为"("
        }
    }
    /**********************var变量定义**********************/
    if(strcmp("var",table[TableIndex].symbol) == 0)// var a,b,c;
    {
        TableIndex ++;
        if(table[TableIndex].type != 2)
        {
            if(strcmp(",",table[TableIndex].symbol) == 0)
                Gerror(13,TableIndex);
            else  Gerror(2,TableIndex);//出错信息：此处应为变量
        }
        TableIndex ++;
        if(strcmp(",",table[TableIndex].symbol) !=0)
        {
            Gerror(3,TableIndex); //出错信息：此处应为","
            TableIndex ++;
        }
        while(1)
        {
            while(strcmp(",",table[TableIndex].symbol)==0)
            {
                TableIndex = TableIndex + 1;
                if(table[TableIndex].type !=2)
                {
                    Gerror(2,TableIndex);//出错信息：此处应为变量
                    TableIndex ++;
                }
                TableIndex ++;
            }
            if(strcmp(";",table[TableIndex].symbol)==0)
            {
                return 1;
            }
            else
            {
                Gerror(4,TableIndex);//出错信息：此处应为分号";"
                return 0;
            }
        }
    }
    /*****************if语句判断************************/
//if语句
    else if(strcmp("if",table[TableIndex].symbol) == 0)
    {
        ifCount ++; //if个数加1
        if(table[TableIndex +1].type == 2 || table[TableIndex + 1].type == 3)
        {
            Gerror(9,TableIndex); //出错信息：此处缺少')'
            TableIndex ++;
        }
        else if(strcmp("(",table[TableIndex + 1].symbol) != 0)
        {
            Gerror(8,TableIndex);//出错信息：此处应为分号"("
        }
        TableIndex = TableIndex + 2;
        if(express())
        {
            if(strcmp(")",table[TableIndex].symbol) == 0) //'('匹配
            {
                TableIndex ++;
                if(strcmp("begin",table[TableIndex ].symbol) == 0)
                {
                    beginCount ++;
                    if(table[TableIndex + 1].type == 7)
                    {
                        Gerror(16,TableIndex);
                        TableIndex ++;
                        return 1;
                    }
                    Gerror(10,TableIndex);//出错信息：此处缺少"then"
                    return 0;
                }
                if(strcmp("then",table[TableIndex].symbol) != 0)
                {
                    Gerror(11,TableIndex);//出错信息：此处应为"then"
                    return 0;
                }//if_then
                else
                {
                    return 1;
                }
            }//if_)
            else
            {
                Gerror(15,TableIndex);
                return 0;
            }
        } //if_express
    }//if_if
//return 1;
}
int main()
{
    ifstream in;


    ofstream out;
    char in_file_name[26],out_file_name[26];
    char ch[Max];
    int nLine = 1;
    FILE *fp;
    char cha;
    int Index = 0;
    if((fp = fopen("F:\\code.txt","r")) == NULL)
    {
        printf("cannot open the file!");
        exit(0);
    }
    cha = fgetc(fp);
    while(cha != EOF)
    {
        if(cha != '\n')
        {
            ch[Index] = cha;
            Index = Index + 1;
        }
        else if(cha == '\n')
        {
            ch[Index] = '!';
            Index = Index + 1;
        }
        cha = fgetc(fp);
    }
    fclose(fp);
//	cout << ch;
    /**********************调用词法分析**********************/
    Scanner(ch, strlen(ch),nLine);
    /*for(int i = 0; i < TableNum;i ++)
    	cout << table[i].type<< "		"<< table[i].symbol<< "		"<<table[i].Index<<"		"<<table[i].line<< endl;*/
    /**********************调用语法分析**********************/
    cout << endl << "语法分析结果：\n "<< endl;
    while(TableIndex <= TableNum)
    {
        TableIndex ++;
        if(table[TableIndex].type == 1)
            judge();
        else if(table[TableIndex].type == 2)
            Assign();   //赋值语句
    }
    /*if(ifCount < 0)
    {
    	   errorFlag = 1;
    		cout << "程序缺少if \n";
    	}
    if(beginCount <0)
    	{
    		errorFlag = 1;
    		cout << "程序缺少begin \n";
    	}
    if(beginCount >0)
    {
    	  errorFlag = 1;
    	  cout << "程序缺少end \n";
    }
    if(errorFlag == 0)
    {
    	cout << "语法分析成功！";
    }*/
    return 0;
}
