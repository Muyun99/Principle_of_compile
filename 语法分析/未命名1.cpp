#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <cstdlib>

using namespace std;

#define Max 655	 //�����볤��
#define WordMaxNum 256	 //����������
#define DigitNum 256	//����������
#define MaxKeyWord	13	//�ؼ�������
#define MaxOptANum 8	//�����������
#define MaxOptBNum 4	//�����������
#define MaxEndNum 11	//���������

typedef struct DisplayTable
{
    int Index;  //��ʶ�����ڱ���±�
    int type;	//��ʶ��������
    int line;	//��ʶ�����ڱ������
    char symbol[20];	//��ʶ�����ڱ������
} Table;

int TableNum = 0;  //display��ı�������
char Word[WordMaxNum][20]; //��ʶ����
char Digit[WordMaxNum][20]; //���ֱ�
int WordNum = 0;   //��������±�
int DigNum = 0;	 //��������±�
bool errorFlag = 0; //�����־
int TableIndex = -1;  //display ����±�����
int beginCount = 0;//����begin��1������end��1
int ifCount = 0; //����if��1
Table *table = new Table[Max];

//�ؼ���
const char* const KeyWord[MaxKeyWord] = {"begin","if","then","while","do","end","var","program","const","procedure","read","write","call"};


// ��Ŀ����
const char OptA[] = {'+','-','*','/','=','#','<','>'};

//˫Ŀ�����
const char *OptB[] = {"<=",">=",":=","<>"};
// ���
const char End[] = {'(', ')', ',', ';', '.', '[', ']', ':', '{', '}', '"'};

void error(char str[20],int nLine, int errorType)
{
    errorFlag = 1;
    cout <<" \nError :    ";
    switch(errorType)
    {
    case 1:
        cout << "��" << nLine-1 <<"��" << str << " �����ĳ��ȳ������ƣ�\n";
        break;
    case 2:
        cout << "��" << nLine-1 <<"��" << str << " С�������\n";
        break;
    case 3:
        cout << "��" << nLine-1 <<"��" << str << " �����ĳ��ȳ������ƣ�\n";
        break;
    }//switch
}//error

void Scanner(char ch[],int chLen,int nLine)
{
    int chIndex = 0;
    while(chIndex < chLen) //��������ַ�ɨ��
    {
        /****************����ո��tab***************************/
        //���Կո��tab
        while(ch[chIndex] == ' ' || ch[chIndex] == '	' )
        {
            chIndex ++;
        }
        /*************************�����з�*********************/
        //�������з���������1
        while(ch[chIndex] == '!')
        {
            nLine++;
            chIndex ++;
        }

        /***************************��ʶ��**********************/
        if( isalpha(ch[chIndex])) //����ĸ���»��߿�ͷ
        {
            char str[256];
            int strLen = 0;
//����ĸ���»���
            while(isalpha(ch[chIndex]) || ch[chIndex] == '_' )
            {
                str[strLen ++] = ch[chIndex];
                chIndex ++;
                while(isdigit(ch[chIndex]))//���ǵ�һλ������Ϊ����
                {
                    str[strLen ++] = ch[chIndex];
                    chIndex ++;
                }
            }
            str[strLen] = 0; //�ַ���������
            if(strlen(str) > 20) //��ʶ�������涨���ȣ�������
            {
                error(str,nLine,1);
            }
            else
            {
                int i;
                for(i = 0; i < MaxKeyWord; i++) //��ؼ���ƥ��
//�ǹؼ��֣�д��table����
                    if(strcmp(str, KeyWord[i]) == 0)
                    {
                        strcpy(table[TableNum].symbol,str);
                        table[TableNum].type = 1;  //�ؼ���
                        table[TableNum].line = nLine;
                        table[TableNum].Index = i;
                        TableNum ++;
                        break;
                    }
                if(i >= MaxKeyWord) //���ǹؼ���
                {
                    table[TableNum].Index = WordNum;
                    strcpy(Word[WordNum++],str);
                    table[TableNum].type = 2; //������ʶ��
                    strcpy(table[TableNum].symbol,str);
                    table[TableNum].line = nLine;
                    TableNum ++;
                }
            }
        }

        /**************************����**************************/
        else if(isdigit(ch[chIndex])) //��������
        {
            int flag = 0;
            char str[256];
            int strLen = 0;
//���ֺ�С����
            while(isdigit(ch[chIndex]) || ch[chIndex] == '.')
            {
//flag���С����ĸ�����0ʱΪ������1ʱΪС����2ʱ����
                if(ch[chIndex] == '.')
                    flag ++;
                str[strLen ++] = ch[chIndex];
                chIndex ++;
            }
            str[strLen] = 0;
            if(strlen(str) > 20) //������ʶ�������涨����20��������
            {
                error(str,nLine,3);
            }
            if(flag == 0)
            {
                table[TableNum].type = 3; //����
            }
            if(flag == 1)
            {
                table[TableNum].type = 4; //С��
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
        /*************************�����************************/
        else
        {
            //���������ǲ����޷�ʶ��ı�ʶ����0Ϊ�������1Ϊ���
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
                /*************************���************************/
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
                /********************�����޷�ʶ���ַ�*****************/
//��ͷ�Ĳ�����ĸ�����֡�����������
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
/**************��ʮ����С��תΪ16����******************/
void Trans(double x,int p)  //��ʮ����С��תΪ16����
{
    int i=0;                  //���Ʊ�������Чλ��
    while(i<p)
    {
        if(x==0)              //���С��������0
            break;            //���˳�ѭ��
        else
        {
            int k=int(x*16);  //ȡ��������
            x=x*16-int(k);    //�õ�С������
            if(k<=9)
                cout<<k;
            else
                cout<<char(k+55);
        };
        i++;
    };
};
/***********************�﷨����*************************/
void Gerror(int errorType,int nIndex)
{
    errorFlag = 1;
    switch(errorType)
    {
    case 1:
        cout << "��" << table[nIndex].line <<"�У�" << table[nIndex].symbol <<" Ӧ��Ϊ��ֵ�ţ�= \n";
        break;
    case 2:
        cout << "��" << table[nIndex].line <<"�У�" << table[nIndex].symbol <<" ӦΪ���� \n";
        break;
    case 3:
        cout << "��" << table[nIndex].line <<"�У�" << table[nIndex].symbol <<" ӦΪ���� \n";
        break;
    case 4:
        cout << "��" << table[nIndex].line <<"�У�" << table[nIndex].symbol <<" ӦΪ�ֺ� \n";
        break;
    case 5:
        cout << "��" << table[nIndex].line <<"�У�" << table[nIndex].symbol <<" ӦΪ����� \n";
        break;
    case 6:
        cout << "��" << table[nIndex].line <<"�У�" << table[nIndex].symbol <<" ӦΪ�������� \n";
        break;
    case 7:
        cout << "��" <<table[TableIndex].line  <<"�� "
             << table[nIndex].symbol << "��"
             <<table[TableIndex + 1].symbol
             <<"֮��ȱ������� \n";
        break;
    case 8:
        cout << "��" << table[nIndex].line <<"�У�" << table[nIndex + 1 ].symbol <<" ӦΪ'(' \n";
        break;
    case 9:
        cout << "��" <<table[TableIndex].line  <<"�� "
             << table[TableIndex].symbol << "��" <<table[nIndex + 1].symbol <<"֮��ȱ��'(' \n";
        break;
    case 10:
        cout << "��"  << table[TableIndex - 1].line
             << " ��: ȱ��'then'" << endl;
        break;
    case 11:
        cout << "��"  << table[TableIndex].line  << " ��:"
             <<table[nIndex].symbol << "ӦΪthen \n";
        break;
    case 12:
        cout << "��"  << table[TableIndex].line  << " ��: end ���ܽ� " <<table[TableIndex].symbol << endl;
        break;
    case 13:
        cout << "��" << table[nIndex].line <<"�У� "
             << table[nIndex - 1].symbol <<"�� "
             <<table[TableIndex].symbol <<"֮��ȱ�ٱ��� \n";
        break;
    case 14:
        cout << "��" <<table[nIndex ].line  <<"�� "
             << table[nIndex ].symbol << "��ȱ��';' \n";
        break;
    case 15:
        cout << "��"  << table[TableIndex].line  << " ��:"
             <<table[nIndex].symbol << "ӦΪ')' \n";
        break;
    case 16:
        cout << "��"  << table[TableIndex].line
             << " ��,begin ���ܽ� "
             <<table[TableIndex].symbol << endl;
        break;
    }
}
/************************���ʽ�ж�********************/
bool express()
{
    while(1)
    {
        if(table[TableIndex].type==2||table[TableIndex].type == 3 )
        {
            if(table[TableIndex].type==3&&table[TableIndex+ 1].type == 2 && table[TableIndex].line == table[TableIndex + 1].line)
            {
                Gerror(7,TableIndex); //������Ϣ���ô�ȱ�������
                //TableIndex = TableIndex + 2;
                TableIndex ++;
            }
            if(table[TableIndex].line != table[TableIndex + 1].line)
            {
                //	Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
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
                Gerror(6,TableIndex); //������Ϣ���ô�ӦΪ��������
            }
            TableIndex ++;
        }
        if(table[TableIndex].type ==5|| table[TableIndex].type == 6 )
            TableIndex ++;
        else if(table[TableIndex].type == 7)
            return 1;
        else if(TableIndex >= TableNum) //||  )
        {
            Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
            return 1;
        }
        else
        {
            Gerror(5,TableIndex); //������Ϣ���˴�ӦΪ�����
            TableIndex ++;
        }
    }
}

/*******************��ֵ����ж�*************************/
bool Assign()  //��ֵ�����ж�
{
    TableIndex ++;
    if(strcmp( ":=", table[TableIndex].symbol) == 0)
    {
        TableIndex ++;
    }
    else
    {
        Gerror(1,TableIndex);  //������Ϣ����ֵ��Ӧ��Ϊ":="
        TableIndex ++;
    }
    if(express())  //":="�����Ϊ��������
    {
        if(strcmp(";",table[TableIndex].symbol) == 0)
        {
            return 1;
        }
        else
        {
            if(TableIndex >= TableNum)
            {
                Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
                return 1;
            }
            else if(table[TableIndex].line != table[TableIndex + 1].line)
            {
                Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
                return 1;
                //TableIndex ++;
            }
        }
    }
    else
    {
        Gerror(6,TableIndex);  //������Ϣ��":="��ӦΪ��������
        TableIndex ++;
    }
    return 0;
}
/**********************����ж�*************************/
bool judge() //������ѭ������ʼ�������ж�
{
    /**************************begin**********************/
    if(strcmp("begin",table[TableIndex].symbol)==0) //ƥ��begin
    {
        beginCount ++;
        if(table[TableIndex + 1].type == 7)
        {
            TableIndex ++;
            cout << "��"  << table[TableIndex].line
                 << " ��,begin ���ܽ� " ;
        }
        else
            return 1;
    }

    if(strcmp("while",table[TableIndex].symbol)==0) //ƥ��begin
    {
        TableIndex ++;
        if(table[TableIndex].type == 2)
        {
            TableIndex ++;
            /*	if(table[TableIndex].type == 2)
            	{
            		 Gerror(5,TableIndex); //������Ϣ���˴�ӦΪ�����
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
                        cout << "�˴�ӦΪdo" <<endl;
                }
                else
                {
                    Gerror(6,TableIndex);  //������Ϣ��":="��ӦΪ��������
                    TableIndex ++;
                }
            }
            else
            {
                Gerror(5,TableIndex); //������Ϣ���˴�ӦΪ�����
                TableIndex ++;
            }
        }
        else
        {
            Gerror(2,TableIndex);//������Ϣ���˴�ӦΪ����
            TableIndex ++;
        }
    }

    if(strcmp("do",table[TableIndex].symbol)==0) //ƥ��do
    {
        if(table[TableIndex + 1].type == 7)
        {
            TableIndex ++;
            cout << "��"  << table[TableIndex].line
                 << " ��,do	 ���ܽ� "
                 <<table[TableIndex].symbol << endl;
            return 1;
        }
    }
    /**************************end***********************/
    if(strcmp("end",table[TableIndex].symbol) == 0) //ƥ��end
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
    if(strcmp("else",table[TableIndex].symbol) == 0) //ƥ��else
    {
        ifCount --;
        return 1;
    }
    if(strcmp("program",table[TableIndex].symbol) == 0)	//ƥ��program
    {
        TableIndex++;
        if(express())  //":="�����Ϊ��������
        {
            if(strcmp(";",table[TableIndex].symbol) == 0)
            {
                return 1;
            }
            else
            {
                if(TableIndex >= TableNum)
                {
                    Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
                    return 1;
                }
                else if(table[TableIndex].line != table[TableIndex + 1].line)
                {
                    Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
                    return 1;
                    //TableIndex ++;
                }
            }
        }
    }


    if(strcmp("procedure",table[TableIndex].symbol) == 0 || strcmp("call",table[TableIndex].symbol) == 0)	//ƥ��program
    {
        TableIndex ++;
        if(table[TableIndex].type == 2 || table[TableIndex].type == 3)  // program ��Ϊ��������
        {
            TableIndex ++;
//��������������
            if(strcmp("(",table[TableIndex].symbol) == 0)
            {
                TableIndex ++;
                if(table[TableIndex].type == 2)
                {
                    TableIndex ++;

                    if(strcmp(")",table[TableIndex].symbol) == 0)
                    {
                        if(1)  //":="�����Ϊ��������
                        {
                            if(strcmp(";",table[TableIndex].symbol) == 0)
                            {
                                return 1;
                            }
                            else
                            {
                                if(TableIndex >= TableNum)
                                {
                                    Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
                                    return 1;
                                }
                                else if(table[TableIndex].line != table[TableIndex + 1].line)
                                {
                                    Gerror(14,TableIndex); //������Ϣ�������ȱ�ٷֺ�
                                    return 1;
                                    //TableIndex ++;
                                }
                            }
                        }
                    }
                    else
                    {
                        Gerror(15,TableIndex);//������Ϣ���˴�ӦΪ")"
                    }
                }
                else
                {
                    Gerror(2,TableIndex);//������Ϣ���˴�ӦΪ����
                }
            }
            else
            {
                Gerror(8,TableIndex);//������Ϣ���˴�ӦΪ"("
            }
        } //if
        else
        {
            Gerror(2,TableIndex);//������Ϣ���˴�ӦΪ����
            TableIndex ++;
        }
    }

    if(strcmp("write",table[TableIndex].symbol) == 0 || strcmp("read",table[TableIndex].symbol) == 0)	//ƥ��program
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
                        return 1;//������Ϣ���˴�ӦΪ";"
                    }
                }
                else
                {
                    Gerror(15,TableIndex);//������Ϣ���˴�ӦΪ")"
                }
            }
            else
            {
                Gerror(2,TableIndex);//������Ϣ���˴�ӦΪ����
            }
        }
        else
        {
            Gerror(8,TableIndex);//������Ϣ���˴�ӦΪ"("
        }
    }
    /**********************var��������**********************/
    if(strcmp("var",table[TableIndex].symbol) == 0)// var a,b,c;
    {
        TableIndex ++;
        if(table[TableIndex].type != 2)
        {
            if(strcmp(",",table[TableIndex].symbol) == 0)
                Gerror(13,TableIndex);
            else  Gerror(2,TableIndex);//������Ϣ���˴�ӦΪ����
        }
        TableIndex ++;
        if(strcmp(",",table[TableIndex].symbol) !=0)
        {
            Gerror(3,TableIndex); //������Ϣ���˴�ӦΪ","
            TableIndex ++;
        }
        while(1)
        {
            while(strcmp(",",table[TableIndex].symbol)==0)
            {
                TableIndex = TableIndex + 1;
                if(table[TableIndex].type !=2)
                {
                    Gerror(2,TableIndex);//������Ϣ���˴�ӦΪ����
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
                Gerror(4,TableIndex);//������Ϣ���˴�ӦΪ�ֺ�";"
                return 0;
            }
        }
    }
    /*****************if����ж�************************/
//if���
    else if(strcmp("if",table[TableIndex].symbol) == 0)
    {
        ifCount ++; //if������1
        if(table[TableIndex +1].type == 2 || table[TableIndex + 1].type == 3)
        {
            Gerror(9,TableIndex); //������Ϣ���˴�ȱ��')'
            TableIndex ++;
        }
        else if(strcmp("(",table[TableIndex + 1].symbol) != 0)
        {
            Gerror(8,TableIndex);//������Ϣ���˴�ӦΪ�ֺ�"("
        }
        TableIndex = TableIndex + 2;
        if(express())
        {
            if(strcmp(")",table[TableIndex].symbol) == 0) //'('ƥ��
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
                    Gerror(10,TableIndex);//������Ϣ���˴�ȱ��"then"
                    return 0;
                }
                if(strcmp("then",table[TableIndex].symbol) != 0)
                {
                    Gerror(11,TableIndex);//������Ϣ���˴�ӦΪ"then"
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
    /**********************���ôʷ�����**********************/
    Scanner(ch, strlen(ch),nLine);
    /*for(int i = 0; i < TableNum;i ++)
    	cout << table[i].type<< "		"<< table[i].symbol<< "		"<<table[i].Index<<"		"<<table[i].line<< endl;*/
    /**********************�����﷨����**********************/
    cout << endl << "�﷨���������\n "<< endl;
    while(TableIndex <= TableNum)
    {
        TableIndex ++;
        if(table[TableIndex].type == 1)
            judge();
        else if(table[TableIndex].type == 2)
            Assign();   //��ֵ���
    }
    /*if(ifCount < 0)
    {
    	   errorFlag = 1;
    		cout << "����ȱ��if \n";
    	}
    if(beginCount <0)
    	{
    		errorFlag = 1;
    		cout << "����ȱ��begin \n";
    	}
    if(beginCount >0)
    {
    	  errorFlag = 1;
    	  cout << "����ȱ��end \n";
    }
    if(errorFlag == 0)
    {
    	cout << "�﷨�����ɹ���";
    }*/
    return 0;
}
