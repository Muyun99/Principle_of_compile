#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<fstream>
using namespace std;

typedef struct
{
    string type;
    string value;
    int line;
} Word;

Word word1;
Word word2;
fstream infile;
fstream outfile;

void Prog();
void Block();
void Condecl();
void Const();
void Vardecl();
void Proc();
void Body();
void Statement();
void Lexp();
void Exp();
void Term();
void Factor();
void ReadLine();
void OpenFile();
void CloseFile();
void ReadLine();



void Prog()
{
    //<prog> -> program <id> ; <block>
    ReadLine(word1);
    if(word1.value != "program")
        cout << "第" << word1.line << "行发生错误：" << "缺少program" << endl;
    ReadLine();
    if(word1.type != "Identifier")
        cout << "第" << word1.line << "行发生错误：" << "缺少Id" << endl;
    ReadLine();
    if(word1.value != ";")
        cout << "第" << word1.line << "行发生错误：" << "缺少分号" << endl;
    ReadLine();
    Block();
}

void Block()
{
    //<block> -> [<condecl>][<vardecl>][<proc>]<body>
    if(word1.value != "begin")
    {
        if(word1.value == "const")
        {
            ReadLine(word1);
            Condecl();
        }
        if(word1.value == "var")
        {
            ReadLine(word1);
            Vardecl();
        }
        if(word1.value == "procedure")
        {
            ReadLine(word1);
            Proc();
        }
    }
    ReadLine();
    Body();
}
void Condecl()
{
    //<condecl> → const <const>{,<const>};
    if(word1.type != "Identifier")
    {

    }
    ReadLine(word1);
    Const();
    while(word1.value == "Identifier" | )

}
void Const()
{
    //<const> → <id>:=<integer>
}
void Vardecl()
{
    //<vardecl> → var <id>{,<id>};
}
void Proc()
{
    //<proc> → procedure <id>（<id>{,<id>}）;<block>{;<proc>}
}
void Body()
{
    //<body> → begin <statement>{;<statement>}end
}
void Statement()
{

}
void Lexp()
{

}
void Exp()
{

}
void Term()
{

}
void Factor()
{

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
void PrintWord(Word word_local)
{
    cout << "*************" << endl;
    cout << "type:" << word_local.type << endl;
    cout << "value:" << word_local.value << endl;
    cout << "line:" << word_local.line << endl;
    cout << "*************"<< endl << endl;
}
void CloseFile()
{
    infile.close();
    outfile.close();
}
void ReadLine(Word &word_local)
{
    infile >> word_local.type;
    infile >> word_local.value;
    infile >> word_local.line;
//    if(infile)
//        PrintWord();
}


int main()
{
    OpenFile();
    while(!infile.eof())
        ReadLine();
    CloseFile();
    return 1;
}
