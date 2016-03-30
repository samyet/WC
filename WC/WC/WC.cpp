#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
#include<windows.h>
using namespace std;
bool cMark,wMark,lMark,aMark;//����������־��������ʾ��ȫ��
//�������� �����ļ������� �ַ��� ����
void countFile(istream& in, int& chars,int& words,int& lines)
{

	char cur = '\0';
	char last = '\0';
	chars = words = lines = 0;
	while (in.get(cur))
	{
		if (cur == '\n' || (cur == '\f' && last == '\r'))
			lines++;
		else
			chars++;
		if (!std::isalnum(cur) && std::isalnum(last))
			words++;
		last = cur;
	}
		if (chars > 0)
		{
			if (std::isalnum(last))
				words++;
			lines++;
		}
}

//�жϱ����Ƿ��ǿ�
bool isNull(char cur)
{
	if(cur=='\t' || cur==' ' || cur=='\n' || cur=='\f' || cur=='\r' || cur=='\0' || cur=='\v')
		return true;
	else
		return false;
}
//-a���ܣ�ͳ��ע��������������������������
void exCountFile(istream& hi,int& code,int& null,int& note)
{
	char cur = '\0';
	char last = '\0';
	int lchar=0;
	int line=0;
	int start,end;
	string s;
	code = null = note = start = end =0;
	while (hi.get(cur))
	{
		if (cur == '\n' || (cur == '\f' && last == '\r'))
		{
			line++;
			if(s.length()<=1) 
				null++;
			else if(s[0]=='/' && s[1]=='/')
				note++;
			else if(s[0]=='/' && s[1]=='*' && start==0)
			{
				start=line;
				end=0;
			}
			else if(start!=0 && s[s.length()-1]=='/' && s[s.length()-2]=='*')
			{
				end=line;
				note+=(end-start+1);
				start=0;
			}
			s="";
		}
		if(!isNull(cur))
			s+=cur;
	}
	line++;//�ļ����һ��û�л��У�����ѭ������һ��
	if(end=0 && start!=0 && s[s.length()-1]=='*/')
	{
		end=line;
		note+=(end-start+1);
	}
	else if(s[0]=='/' && s[1]=='/')
		note++;
	else if(s.length()<=1)
		null++;
	code=line-null-note;
	last = cur;
}

string GetProgramDir()//��ȡrelease����Ŀ¼�������ļ��� 
{   
	char exeFullPath[MAX_PATH]; // Full path
	string strPath = "";
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	strPath=(string)exeFullPath;    // Get full path of the file
	int pos = strPath.find_last_of('\\', strPath.length());
	strPath=strPath.substr(0, pos);
	int t=1,j=0;
	string s="";
	for(int i=0;i<strPath.length();i++)
	{
		if(strPath[i]=='\\')
		{
			s+=strPath.substr(j,t);
			s+="\\";
			t=0;
			j=i+1;
		}
		t++;
	}
	s+=strPath.substr(j,t);
	strPath=s;
	strPath+="\\";
	strPath+="\\";
	return strPath;  // Return the directory without the file name
}
//��ȡ��ReleaseĿ¼�µ��ļ�·�����������ļ���
string getPath(string p)
{
	string path;
	path="";
	for(int i=0;i<p.length();i++)
	{
		if(p[i]!='*' && p[i]!='\0')
			path+=p[i];
		else
			break;
	}
	return path;
}
void deal(string path)//�����ֵ�����������
{
	ifstream in(path);
	ifstream inEx(path);
	int c, w, l,code,null,note;
	countFile(in, c, w, l);
	exCountFile(inEx,code,null,note);
	if(cMark)
		cout << "chars: " << c <<endl;
	if(wMark)
		cout << "words: " << w <<endl;
	if(lMark)
		cout << "lines: " << l <<endl;
	if(aMark)
	{
		cout<<"����������"<<code<<endl;
		cout<<"��������"<<null<<endl;
		cout<<"ע��������"<<note<<endl;
	}
}
void searchFile(string path,string ex)//�ݹ鴦��Ѱ���ļ�
{
	WIN32_FIND_DATA file;
	string sPath=path+"*.*";
	string myPath="";
	HANDLE hFile = FindFirstFile(sPath.c_str(),&file);
	if(INVALID_HANDLE_VALUE == hFile)  
	{  
		cout<<"���ļ�ʧ�ܣ�����·��";
	}
	do
	{
		if (file.cFileName[0] != '.')//���Ǹ�Ŀ¼���߱�Ŀ¼�Ŀ�ݷ�ʽ
		{
			if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //���ļ��еݹ�
			{
				searchFile(path+file.cFileName+ "\\",ex);
			}
			myPath=file.cFileName;
			if(myPath.length()>ex.length())//���ļ��д���
				if(myPath.substr(myPath.length()-myPath.length(),ex.length())==ex)
				{
					cout<<myPath<<":";
					deal(path+myPath);
				}
		}
	}
	while(FindNextFile(hFile, &file));
}

int main(int argc, char** argv) 
{
	string path;
	string ex;
	if (argc < 3) 
	cout<<"����������������";
	path=getPath(argv[argc-1]);
	ex=argv[argc-1];
	bool sMark;//������־������ѭ������
	cMark = wMark = lMark = aMark = sMark = false;
	for(int i=1;i<argc-1;i++)
	{
		string operate(argv[i]);
		if(operate =="-c")
			cMark=true;
		if(operate =="-w")
			wMark=true;
		if(operate =="-l")
			lMark=true;
		if(operate =="-a")
			aMark=true;
		if(operate =="-s")
		{
			sMark=true;
			if(path=="")
				path=GetProgramDir();
			else
				ex=ex.substr(path.length(),ex.length()-path.length());
		}

	}
	if(sMark)
		searchFile(path,ex.substr(1,ex.length()-1));
	else
		deal(path);
	return 0;

}