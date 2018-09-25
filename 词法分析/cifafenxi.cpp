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
                //cout << "strToken(ch) :" << strToken <<endl;
            }
            //strToken = strToken - ch;
            Retract(id,ch);
            if(Reserve(keyword,strToken,keywordcount) == 1)//若strToken为保留字
            {
                ResultId++;
                result[ResultId].type = "Reserve";
                result[ResultId].value = strToken;
                strToken.clear();
                cout <<endl;
                cout << "<" << result[ResultId].type << "," << result[ResultId].value << ">" <<endl;
                cout <<endl;
            }
            else
            {
                ResultId++;
                result[ResultId].type = "identifier";
                result[ResultId].value = strToken;
                strToken.clear();
                cout <<endl;
                cout << "<" << result[ResultId].type << "," << result[ResultId].value << ">" <<endl;
                cout <<endl;
                //cout << "Is a BiaoShiFu!" << endl;
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
            result[ResultId].type = "Digital";
            result[ResultId].value = strToken;
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
                }
                else
                {
                    Retract(id,ch);
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "*";
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
                }
                else if(ch == '=')
                {
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "<=";
                }
                else
                {
                    Retract(id,ch);
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = "<";
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
                }
                else
                {
                    Retract(id,ch);
                    ResultId++;
                    result[ResultId].type = "Operator";
                    result[ResultId].value = ">";
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
        //cout << KEYWORD << endl;
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
    fpin.open("code_in.txt",ios::in | ios::out);
    fpout.open("code_out.txt",ios::in | ios::out);
    fstream file("error.txt", ios::out);
    file.close();

    int keywordcount = writeKeyword(keyword);
    //for(int i = 0;i < keywordcount;i++)
        //cout << keyword[i].value << endl;

    while(!fpin.eof())
    {
        getline(fpin,code);
        row++;
        //cout << code << endl;
        //cout << code.length() <<endl;
        fun(code,keyword,result,ResultId,keywordId,keywordcount,row);

        //code_in += code + "\n";
    }
    for(int i = 1;i <= ResultId;i++)
    {
        fpout << '<';
        fpout << result[i].type;
        fpout << ',';
        fpout << result[i].value;
        fpout << '>' << endl;
    }
    fpin.close();
    fpout.close();
}
