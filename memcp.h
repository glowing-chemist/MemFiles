/* requires std::vector std::sstream and std::string to be included above */

struct InvalidAdress{}; /* throwable objects */
struct InvalidPath{};

namespace memfile{

struct File{
	std::string Name; /* Name for path finding */
	std::vector<char> Data; /* files payload */
};

struct Folder{
	std::string Name;
	std::vector<Folder*> DaughterFolders;
	std::vector<File*> DaughterFiles;
};

std::vector<std::string> ParsePath(std::string Path){
        std::istringstream s{Path};
        char c;
        s>>c;
	if(c != '/')throw InvalidPath{};
        std::vector<std::string> res;
        std::string path;
        while(s>>c){
                if(c != '/')path += c;
                else{
                        res.push_back(path);
                        path.clear();
                }
        }
        res.push_back(path);
	return res;
}

std::string GetParent(std::vector<std::string> Paths){
	std::string newpath{"/"};
	        for(int x = 0;x < (Paths.size()-1);x++){
                newpath += Paths[x];
                if(x != (Paths.size()-2))newpath += "/";
        }
	return newpath;
}

void* ValidPath(std::string Path,Folder* Root){
	std::vector<std::string> res = ParsePath(Path);
	if(res[0] != Root->Name)return nullptr;
	Folder* CurrentFolder = Root;
	if(res.size() == 1)return CurrentFolder;
	for(int i = 1;i < res.size();i++){
		for(int x = 0;x<CurrentFolder->DaughterFolders.size();x++){
			if(CurrentFolder->DaughterFolders[x]->Name == res[i]){
				CurrentFolder = CurrentFolder->DaughterFolders[x];
				break;
			}
		}
		for(int x = 0;x<CurrentFolder->DaughterFiles.size();x++){
			if(CurrentFolder->DaughterFiles[x]->Name == res[i])return CurrentFolder->DaughterFiles[x];
		}
		if(CurrentFolder->Name == res[i] && i == (res.size()-1))return CurrentFolder;
	}
	return nullptr;
}

File* Read(std::string Path,Folder* root){
	File* addr = static_cast<File*>(ValidPath(Path,root));
	if(addr == nullptr)throw InvalidAdress{};
	return addr;
}

File* MakeFile(std::string Path, Folder* Root){
	std::vector<std::string> PathElements = ParsePath(Path);
	std::string NewPath = GetParent(PathElements);
	Folder* ParentFolder = static_cast<Folder*>(ValidPath(NewPath,Root));
	if(ParentFolder == nullptr)throw InvalidPath{};
	File* NewFile = new File{PathElements[PathElements.size()-1]}; /* create new File instance */
	ParentFolder->DaughterFiles.push_back(NewFile); /* link NewFile to parent folder */
	return NewFile;
}

Folder* MakeFolder(std::string Path, Folder* Root){
	std::vector<std::string> PathElements = ParsePath(Path);
	std::string NewPath = GetParent(PathElements);
	Folder* ParentFolder = static_cast<Folder*>(ValidPath(NewPath, Root));
	if(ParentFolder == nullptr)throw InvalidPath{};
	Folder* NewFolder = new Folder{PathElements[PathElements.size()-1]};
	ParentFolder->DaughterFolders.push_back(NewFolder);
	return NewFolder;
}

void Write(std::string Path, std::vector<char> &NewData, Folder* Root){ 
	std::vector<std::string> PathElements = ParsePath(Path);
	File* addr = static_cast<File*>(ValidPath(Path,Root));
	if(addr == nullptr){/* File doesn't exist, needs to be made */
		addr = MakeFile(Path, Root);
	}
	for(int x = 0;x<NewData.size();x++){
		addr->Data.push_back(NewData[x]);
	}
}

void DeleteFile(std::string Path, Folder* Root){
	std::vector<std::string> PathElements = ParsePath(Path);
	File* FileAddr = static_cast<File*>(ValidPath(Path, Root));
	if(FileAddr == nullptr)throw InvalidPath{};/* check for valid File */
	std::string Parent = GetParent(PathElements);
	Folder* ParentAddr = static_cast<Folder*>(ValidPath(Parent, Root));
	if(ParentAddr == nullptr)throw InvalidAdress{};
	for(int x = 0;x < ParentAddr->DaughterFiles.size();x++){
		if(ParentAddr->DaughterFiles[x] == 
FileAddr)ParentAddr->DaughterFiles.erase(ParentAddr->DaughterFiles.begin() + x);
	}
	delete FileAddr;
}

} /* end of namespace */
