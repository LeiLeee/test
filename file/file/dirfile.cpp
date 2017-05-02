#include <direct.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <map>
#include <fstream>
using namespace std;


map<string,string>filename;
bool isdir(const char* path,const char* destination)
{
	struct stat idir;
	string tem = path;
	string::size_type pos = tem.find_first_of("/");
	tem = tem.substr(pos,string::npos);
	string tem1 = destination;

//	printf(tem.c_str());
	tem1 += tem;
//	printf(tem1.c_str());
	if(stat(path,&idir)==0)
	{
		if(S_IFDIR & idir.st_mode)
		{
			mkdir(tem1.c_str());
			return true;
		}
		else
		{
			filename[path] = tem1;
			return false;
		}
	}

}

void copyfile()
{
	string filein;
	char context;
	map<string,string>::iterator it = filename.begin();
	for(int i = 0;i<filename.size();i++)
	{
		for(it;it!=filename.end();++it)
		{
			ofstream fout(it->second,ios::out);
			ifstream fin(it->first,ios::in);
			while(fin.peek()!=EOF)
			{
				fin.get(context);
				fout<<context;
			}
			fout.close();
			fin.close();
		}
	}



}

void infiledir(char* argv1,char* argv2)
{
	
	string path = argv1;
	string path1 = argv2;
	string path2 = path+"/*";
	
	struct _finddata_t fileinfo;
	long handle = _findfirst(path2.c_str(),&fileinfo);
	string fullname = fileinfo.name;
	if(fullname == "." || fullname == "..")
	{
		while(_findnext(handle,&fileinfo)==0)
		{
			fullname = fileinfo.name;
			if(fullname == "." || fullname == "..")
			{
			}
			else
				break;
		}
	}
	

	fullname = string(argv1)+"/"+fullname;
	isdir(fullname.c_str(),path1.c_str());
	while(_findnext(handle,&fileinfo)==0)
	{
		fullname = fileinfo.name;
		fullname = string(argv1)+"/"+fullname;
		if(isdir(fullname.c_str(),argv2))
		{	char* s1,*s2;
			s1 = (char*)malloc(200);
			s2 = (char*)malloc(200);
			strcpy(s1,(char*)fullname.c_str());
			strcpy(s2,argv2);
			printf("%s\n,%s\n",s1,s2);
			infiledir(s1,s2);
			free(s1);
			free(s2);
		}
	}
}


int main(int argc,char* argv[])
{
	//isdir("f:/config1","d:/baidu");

	if(argc<2)
		exit(0);

	
	
	isdir(argv[1],argv[2]);
	infiledir(argv[1],argv[2]);

	copyfile();

	/*
	struct _finddata_t fileinfo;
	long int sign = _findfirst("f:\\config\\*",&fileinfo);
	string s = fileinfo.name;
	printf(s.c_str());
	vector<string>filename;
	if(-1!=sign)
	{
		while(0==_findnext(sign,&fileinfo))
		{
			filename.push_back(fileinfo.name);
			
		}
	}

	_findclose(sign);
	

	struct _stat fileor;
	char* s1="f:/config";

	int result = _stat(s1,&fileor);
	printf("%d\n",result);
	if(_S_IFDIR & fileor.st_mode)
		printf("is dir\n");
	if(_S_IFREG & fileor.st_mode)
		printf("is file\n");
*/

	

	system("pause");
	return 0;
}