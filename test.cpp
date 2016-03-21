#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include"memcp.h"

int main(){
	memfile::Folder rt{};
	rt.Name = "root";
	std::vector<char> data = {'a','b','c'};
	memfile::Write("/root/test.txt",data,&rt);
	memfile::File* f = static_cast<memfile::File*>(memfile::ValidPath("/root/test.txt",&rt));
	std::cout<<f<<'\n';
	memfile::DeleteFile("/root/test.txt",&rt);
	memfile::File* c = static_cast<memfile::File*>(memfile::ValidPath("/root/test.txt",&rt));
	std::cout<<c<<'\n';
}
