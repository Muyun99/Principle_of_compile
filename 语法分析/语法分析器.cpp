#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<string.h>

using namespace std;
typedef struct Word
{
    int line;
    string type;
    string value;
}Word;

fstream infile;
fstream outfile;
Word word,word1,word_pre;

void PrintWord();
void prog();
void block();
void condecl();
void Const();
void vardecl();
void proc();
void body();
void statement();
void lexp();
void exp();
void term();
void factor();

void GetWord()
{
    word.line = word1.line;
    word.type = word1.type;
    word.value = word1.value;

    //PrintWord();

    if(!infile.eof())
        infile >> word1.type >> word1.value >> word1.line;
    //if(infile.fail())
        //infile.close();
}

void PrintWord()
{
    cout << "*************" << endl;
    cout << "type:" << word.type << endl;
    cout << "value:" << word.value << endl;
    cout << "line:" << word.line << endl;
    cout << "*************"<< endl << endl;
}

void prog()
{
    //<prog> -> program <id>;<block>

    GetWord();
    word.line = word1.line;
    word.type = word1.type;
    word.value = word1.value;
    PrintWord();
    if(word.value != "program" )
        cout << "第" << word.line << "program关键字出错！" << endl;
    GetWord();
    if(word.type != "Identifier")
        cout << "第" <<word.line<<"行标识符（变量名）出错"<<endl;
    GetWord();
    if(word.value != ";")
        cout << "第" << word.line << "行缺少分号！" << endl;
    if(word.value != "const" || word.type != "Identifier" || word.value != "procedure"|| word.value != "begin")
    {
         GetWord();
    }
    block();
}

void block()
{
    //<block> -> [<condecl>][<vardecl>][<proc>]<body>
    word_pre.value = word.value;
    word_pre.type = word.type;
    word_pre.line = word.line;

    if(word.value == "begin")
        body();
    else
    {
        GetWord();
        if(word_pre.value == "const" || word1.value == ":=" )
        {
            if(word_pre.value != "const")
                cout << "第" << word_pre.line  << "行关键字（const）错误！" << endl;
            condecl();
        }

        if(word_pre.value == "var" || (word.type == "Identifier" && (word1.value == "," || word1.value == ";")))
        {
            if(word_pre.value != "var")
            {
                cout << "第" << word_pre.line  << "行关键字（var）错误！" << endl;
            }
            vardecl();
        }

        if(word_pre.value == "procedure" || word1.value == "("|| word.value == "(")
        {
            if(word_pre.value != "procedure")
                cout << "第" << word_pre.line  << "行关键字（procedure）错误！" << endl;
            proc();
        }
    }
}
void condecl()
{
    //<condecl> -> const <const>{,<const>};
    Const();
    while(word.value == "," || word.type == "Identifier")
    {
            if(word.value != ",")
                cout << "第" << word.line << "行缺少','号" << endl;
            else
                GetWord();
            Const();
    }
    if(word.value != ";")
        cout << "第" << word.line - 1 << "行缺少分号！" << endl;
    else
        GetWord();
    word_pre.value = word.value;
    word_pre.type = word.type;
    word_pre.line = word.line;
    //line = line1;
    while(word.value == "begin")
        body();
    GetWord();

}
void Const()
{
    //<const> -> <id>:=<integer>
    if(word.type != "Identifier")
    {
        cout << "第" << word.line << "行缺少标识符！" << endl;
    }
    if(word.value != ":=")
        GetWord();
    if(word.value != ":=")
    {
        cout << "第" << word.line << "行缺少赋值号或赋值号错误！" << endl;
    }

    if(word.type != "Integer")
        GetWord();
    if(word.type != "Integer")
        cout << "第" << word.line <<"行常量未赋值！" << endl;
    GetWord();
}

void vardecl()
{
    //<vardecl> -> var <id>{,<id>};
    if(word.type != "Identifier")
    {
        cout << "第" << word.line << "行标识符出错！" << endl;
    }
    if(word.value != ",")
        GetWord();
    while(word.value == "," || word.type != "Identifier")
    {
        if(word.value != ",")
            cout << "第" << word.line << "行缺少','号" << endl;
        else
        {
            GetWord();
            if(word.type != "Identifier")
                 cout << "第" << word.line << "行缺少标识符！" << endl;
        }
        GetWord();
    }
    if(word.value != ";")
        cout << "第" << word.line - 1 << "行缺少分号！" << endl;
    else
        GetWord();
    word_pre.value = word.value;
    word_pre.type = word.type;
    word_pre.line = word.line;
    //line = line1;

    while(word.value == "begin")
        body();
    GetWord();
}

void proc()
{
    //<proc> -> procedure <id>（<id>{,<id>}）;<block>{;<proc>}
    if(word.type != "Identifier")
    {
        cout << "第" << word.line << "行标识符出错！" << endl;
    }
    if(word.value!= "(")
        GetWord();
    if(word.value != "(")
        cout << "第" << word.line << "行缺少左括号！" << endl;
    if(word.type != "Identifier")
        GetWord();
    if(word.type != "Identifier")
        cout << "第" << word.line << "行标识符出错！" << endl;
    if(word.value != ",")
        GetWord();
    while(word.value == "," || word.type == "Identifier")
    {
        if(word.value != ",")
                cout << "第" << word.line << "行缺少','号" << endl;
        else
            GetWord();
    }
    if(word.value != ")")
        cout << "第" << word.line << "行缺少右括号！" << endl;
    if(word.value != ";")
        GetWord();
    if(word.value != ";")
        cout << "第" << word.line - 1 << "行缺少分号！" << endl;
    else
        GetWord();
    block();
    while(word.value == ";" || word.value == "procedure")
    {
        if(word.value != ";")
            cout << "第" << word.line - 1  << "行缺少分号！" << endl;
        if(word.value != "procedure")
            GetWord();
        if(word.value != "procedure")
        {
            cout << "第" << word.line  << "行关键字（procedure）错误！" << endl;
        }
        GetWord();
        proc();
    }

    GetWord();
    if(word.value == "begin")
        body();
}

void body()
{
    //<body> -> begin <statement>{;<statement>}end
    if(word.value == "begin")
    {
        GetWord();
        statement();
    }
    while(word.value == ";" || word.type == "Identifier" || word.value == "if"
           || word.value == "while" || word.value == "call"|| word.value == "begin"
           || word.value == "read"|| word.value == "write")
    {
        if(word.value != ";")
        {
            cout << "第" << word.line - 1  << "行缺少分号！" << endl;
        }

        else
            GetWord();
        statement();
    }
    if(word.value != "end")
    {
        cout << "第" << word.line  << "行关键字（end）错误！" << endl;

    }
    if(word.value != "if" || word.value != "while"
        ||word.value != "call" || word.value != "begin"
           ||word.value != "read" || word.value != "write")
        GetWord();
}

void statement()
{
//    <statement> → <id> := <exp>
//               |if <lexp> then <statement>[else <statement>]
//               |while <lexp> do <statement>
//               |call <id>[（<exp>{,<exp>}）]
//               |<body>
//               |read (<id>{，<id>})
//               |write (<exp>{,<exp>})

    if(word.type == "Identifier")
    {
        GetWord();
        if(word.value != ":=")
        {
            cout << "第" << word.line << "行赋值号错误！" << endl;
        }
        GetWord();
        exp();
        if(word.value == "end")
            cout << "第" << word.line - 1 << "行缺少分号！" <<endl;
    }
    else if(word.value == "if")
    {
        GetWord();
        lexp();
        if(word.value != "then")
        {
            cout << "第" << word.line << "行关键字（then）出错！" << endl;
        }
        GetWord();
        statement();
        if(word.value == "else")
        {
            GetWord();
            statement();
        }
    }
    else if(word.value == "while")
    {
        GetWord();
        lexp();
        if(word.value != "do")
        {
            cout << "第" << word.line << "行关键字（do）出错！" << endl;
        }
        GetWord();
        statement();
    }
    else if(word.value == "call")
    {
        GetWord();
        if(word.type != "Identifier")
        {
            cout << "第" << word.line << "行标识符出错！" << endl;
        }
        if(word.value != "(")
            GetWord();
        if(word.value == "(")
        {
            GetWord();
            exp();
            while(word.value == "," || word.type == "Identifier"
                || word.value == "-" || word.value == "+"
                || word.type == "Integer" || word.value == "(")
            {
                if(word.value != ",")
                    cout << "第" << word.line << "行缺少','号" << endl;
                else
                    GetWord();
                exp();
            }
            if(word.value != ")")
            {
                cout << "第" << word.line << "行缺少右括号！" << endl;
            }
            else
                GetWord();
        }
    }
    else if(word.value == "read")
    {
        GetWord();
        if(word.value != "(")
        {

            cout << "第" << word.line << "行缺少左括号！" << endl;
        }
        else
            GetWord();

        if(word.type != "Identifier")
        {

            cout << "第" << word.line << "行标识符出错！" << endl;//
        }
        if(word.value != "," && word.value != ")")
                GetWord();
        while(word.value == "," || word.type == "Identifier")
        {
            if(word.value != ",")
                cout << "第" << word.line << "行缺少','号！" << endl;
            else
                GetWord();
            if(word.type == "Identifier")
                GetWord();
        }
        if(word.value != ")" && word.type == "Identifier")
        {
             cout << "第" << word.line << "行缺少右括号！" << endl;
        }
        else
            GetWord();

    }
    else if(word.value == "write")
    {
        GetWord();
        if(word.value != "(" && word.type != "Identifier")
        {
            cout << "第" << word.line << "行缺少左括号！" << endl;
        }
        else
            GetWord();
        exp();
        while(word.value == "," || word.type == "Identifier"
                || word.value == "-" || word.value == "+"
                || word.type == "Integer" || word.value == "(")
        {
            if(word.value != ",")
                cout << "第" << word.line << "行缺少','号！" << endl;
            else
                GetWord();
            exp();
        }
        if(word.value != ")"&& word.type != "Identifier")
        {
                cout << "第" << word.line << "行缺少右括号！" << endl;
        }
        else
            GetWord();
        if(word.value == "end")
            cout << "第" << word.line - 1 << "行缺少分号！" <<endl;
    }
    else if(word.value == "begin")
        body();
}
void lexp()
{
    if(word.value == "odd")
    {
        GetWord();
        exp();
    }
    else
    {
        exp();
        if(word.type != "Lop")
        {
           cout << "第" << word.line << "行缺少关系运算符！" << endl;
        }
        else
            GetWord();
        exp();
    }
}

void exp()
{
    if(word.value == "+" || word.value == "-")
    {
        GetWord();
    }
    term();
    while(word.value == "+" || word.value == "-")
    {
        GetWord();
        term();
    }
}

void term()
{
    factor();
    while(word.value == "*" || word.value == "/")
    {
        GetWord();
        factor();
    }
}

void factor()
{
    if(word.type == "Identifier")
        GetWord();
    else if(word.type == "Integer")
        GetWord();
    else
    {
        if(word.value != "(")
        {
            cout << "第" << word.line << "行缺少右括号！" << endl;
        }
        else
            GetWord();
        exp();
        if(word.value != ")")
        {
            cout << "第" << word.line << "行缺少右括号！" << endl;
        }
        else
            GetWord();
    }
}

void OpenFile()
{
    string infilename = "la_output.txt";
    string outfilename = "ga_output.txt";
    infile.open(infilename.c_str(),ios::in);
    outfile.open(outfilename.c_str(),ios::out);
    if(!infile.is_open())
    {
        cout << "can not open la_output.txt" << endl;
        exit(1);
    }
    if(!outfile.is_open())
    {
        cout << "can not open ga_output.txt" << endl;
        exit(1);
    }
}
void CloseFile()
{
    if(!infile.eof())
        infile.close();
    if(!outfile.eof())
        outfile.close();
}
int main()
{

    OpenFile();
    prog();
    cout << "分析完成！" << endl;
    CloseFile();

    return 0;
}
