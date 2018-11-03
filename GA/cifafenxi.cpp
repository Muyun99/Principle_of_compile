#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<string>
using namespace std;

typedef struct
{
    string type;
    string value;
    int row;
    int column;
}wordTable;


void GetChar(string code,int &id,char &ch)
{
    ch = code[id];
    id++;
    return;
}
int GetBC(string code,int &id,char &ch)
{
    while(id <= code.length())
    {
        GetChar(code,id,ch);
        if(ch != ' ')
            return 1;//get ch return 1
    }
    if(id > code.length())
        return -1;;//not get return -1
}

void Concat(string &strToken,char &ch)
{
    strToken = strToken + ch;
    return;
}

bool IsLetter(char ch)
{
    if(('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
    {
        //cout << ch << "is a Letter"<<endl;
        return 1;
    }
    else
        return 0;
}

bool IsDigit(char ch)
{
    if('0' <= ch && ch <= '9')
    {
        //cout << ch << "is a digit"<<endl;
        return 1;
    }
    else
        return 0;
}
bool IsOperator(char ch)
{
    if(ch == '-')
        return 1;
    else if(ch == '*')
        return 1;
    else if(ch == '/')
        return 1;
    else if(ch == '+')
        return 1;
    else if(ch == '=')
        return 1;
    else if(ch == '<')
        return 1;
    else if(ch == '>')
        return 1;
    else if(ch == '@')
        return 1;
    else if(ch == '^')
        return 1;
    else if(ch == ':')
        return 1;
    else
        return 0;
}
bool IsDelimiters(char ch)
{
    if(ch == '(')
        return 1;
    else if(ch == ')')
        return 1;
    else if(ch == '{')
        return 1;
    else if(ch == '}')
        return 1;
    else if(ch == ',')
        return 1;
    else if(ch == ';')
        return 1;
    else
        return 0;
}
int Reserve(wordTable *keyword,string strToken,int keywordcount)
{
    for(int i = 0;i < keywordcount;i++ )
    {
        if(strToken == keyword[i].value)
        {
            //cout << "Is a Reserve!" << endl;
            return i;
        }
    }
    return -1;
}

bool Retract(int &id,char &ch)
{
    if(id != 0)
    {
        id--;
        ch = ' ';
    }
    else
    {
        printf("Retreact Error!\n");
        return -1;
    }
}
bool InsertId(wordTable *result,string strToken,int &ResultId)
{
   ResultId++;
   result[ResultId].value = strToken;
   result[ResultId].type = "BiaoShiFu";
   return 1;
}

bool InsertConst(wordTable *keyword,wordTable *result,int &ResultId,int keywordId)
{
    ResultId++;
    result[ResultId] = keyword[keywordId];
    return 1;
}

void fun(string code,wordTable *keyword,wordTable *result,int &ResultId,int &keywordId,int keywordcount,int row)
{
    int id = 0;
    char ch;
    string strToken;
    while(id < code.length())
    {
        GetChar(code,id,ch);
        if(ch == ' ' || ch == '\t')
            continue;
        else if(IsLetter(ch) == 1)//检查strToken是否合法以及是否为关键字
        {
            while(IsLetter(ch) == 1 || IsDigit(ch) == 1)//若还为字母或数字
            {
                strToken = strToken + ch;
                GetChar(code,id,ch);
            }

            Retract(id,ch);
            if(Reserve(keyword,strToken,keywordcount) == 1)//若strToken为保留字
            {
                ResultId++;
                result[ResultId].type = "Reserve";
                result[ResultId].value = strToken;
                result[ResultId].row = row;
                strToken.clear();
                cout <<endl;
                cout << "<" << result[ResultId].type << "," << result[ResultId].value << ">" <<endl;
                cout <<endl;
            }
            else
            {
                ResultId++;
                result[ResultId].type = "Identifier";
                result[ResultId].value = strToken;
                result[ResultId].row = row;
                strToken.clear();
                cout <<endl;
                cout << "<" << result[ResultId].type << "," << result[ResultId].value << ">" <<endl;
                cout <<endl;
            }

        }
        else if(IsDigit(ch) == 1)
        {
            while(IsDigit(ch) == 1)
            {
                strToken = strToken + ch;
                GetChar(code,id,ch);
                //cout << "strToken(digit) :" << strToken <<endl;

                if(IsLetter(ch))
                {
                    fstream fpError;
                    fpError.open("Error.txt",ios::app);
                    fpError << "<";
                    fpError << row << "," << id << "," << "  identifier Error:" << strToken + ch;
                    fpError << ">" << endl;
                    fpError.close();
                }
            }
            Retract(id,ch);
            ResultId++;
            result[ResultId].type = "Integer";
            result[ResultId].value = strToken;
            result[ResultId].row = row;
            cout << endl;
            cout << "<" << result[ResultId].type << "," << result[ResultId].value << ">" <<endl;
            cout << endl;
            strToken.clear();
        }
        else if(IsOperator(ch) == 1)
        {
            //cout << "strToken(ope) :" << strToken <<endl;
            if(ch == '*')
            {
                GetChar(code,id,ch);
                if(ch == '*')
                {
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "**";
                    result[ResultId].row = row;
                }
                else
                {
                    Retract(id,ch);
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "*";
                    result[ResultId].row = row;
                }
            }
            else if(ch == '<')
            {
                GetChar(code,id,ch);
                if(ch == '>')
                {
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "<>";
                    result[ResultId].row = row;
                }
                else if(ch == '=')
                {
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "<=";
                    result[ResultId].row = row;
                }
                else
                {
                    Retract(id,ch);
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "<";
                    result[ResultId].row = row;
                }
            }
            else if(ch == '>')
            {
                GetChar(code,id,ch);
                if(ch == '=')
                {
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = ">=";
                    result[ResultId].row = row;
                }
                else
                {
                    Retract(id,ch);
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = ">";
                    result[ResultId].row = row;
                }
            }
            else if(ch == ':')
            {
                GetChar(code,id,ch);
                if(ch == '=')
                {
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = ":=";
                    result[ResultId].row = row;
                }
                else
                {
                    fstream fpError;
                    fpError.open("Error.txt",ios::app);
                    fpError << "<";
                    fpError << row << "," << id << "," << ":= -> : Error!";
                    fpError << ">" << endl;
                    fpError.close();
                }
            }
            else
            {
                ResultId++;
                result[ResultId].type = "Operator";
                result[ResultId].value.clear();
                result[ResultId].value = ch;
                result[ResultId].row = row;
                cout <<endl;
                cout << "<" << result[ResultId].type << "," << result[ResultId].value << ">" <<endl;
                cout <<endl;
            }
        }
        else if(IsDelimiters(ch) == 1)
        {
            //cout << "strToken(dilim) :" << strToken <<endl;
            ResultId++;
            result[ResultId].type = "Delimiter";
            result[ResultId].value = ch;
            result[ResultId].row = row;
            cout <<endl;
            cout << "<" << result[ResultId].type << "," << result[ResultId].value << ">" <<endl;
            cout <<endl;
        }
        else
        {
            fstream fpError;
            fpError.open("Error.txt",ios::app);
            fpError << "<";
            fpError << row << "," << id << "," << ch << "        Error!";
            fpError << ">" << endl;
            fpError.close();
        }
    }
}

int writeKeyword(wordTable *keyword)
{
    fstream fpkeyword_in;
    int count = 0;
    fpkeyword_in.open("keyword.txt",ios::in | ios::out);
    string KEYWORD;
    while(!fpkeyword_in.eof())
    {
        getline(fpkeyword_in,KEYWORD);
        keyword[count].value = KEYWORD;
        count++;
    }
    fpkeyword_in.close();
    return count;
}
int main()
{
    string code;
    string code_in;
    string code_output;
    fstream fpin;
    fstream fpout;


    wordTable *keyword = new wordTable[100];
    wordTable *result = new wordTable[1000];
    int ResultId = 0;
    int keywordId = 0;
    int row = 0;
    string infileName = "code_in.txt";
    string outfileName = "la_output.txt";
    fpin.open(infileName.c_str(),ios::in | ios::out);
    fpout.open(outfileName.c_str(),ios::in | ios::out);
    if(!fpin.is_open())
    {
        cout << "string 1!"  << endl;
        exit(0);
    }
    if(!fpout.is_open())
    {
        cout << "文件打开失败2！"  << endl;
        exit(0);
    }

    int keywordcount = writeKeyword(keyword);

    while(!fpin.eof())
    {
        getline(fpin,code);
        row++;
        fun(code,keyword,result,ResultId,keywordId,keywordcount,row);

    }
    for(int i = 1;i <= ResultId;i++)
    {
        cout << "debug:i:" << i <<endl;
        fpout << result[i].type;
        fpout << '\t';
        fpout << result[i].value;
        fpout << '\t';
        fpout << result[i].row << endl;
    }
    fpin.close();
    fpout.close();
}
