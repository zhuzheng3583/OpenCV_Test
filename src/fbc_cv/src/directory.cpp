// fbc_cv is free software and uses the same licence as OpenCV
// Email: fengbingchun@163.com

#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <dirent.h>
#endif
#include <string.h>
#include "directory.hpp"

// reference: contrib/src/inputoutput.cpp (2.4.9)

namespace fbc{

	std::vector<std::string> Directory::GetListFiles(const std::string& path, const std::string& exten, bool addPath)
	{
		std::vector<std::string> list;
		list.clear();
#ifdef _MSC_VER
		std::string path_f = path + "/" + exten;
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind;

		hFind = FindFirstFileA((LPCSTR)path_f.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			return list;
		} else {
			do {
				if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_NORMAL ||
					FindFileData.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE ||
					FindFileData.dwFileAttributes == FILE_ATTRIBUTE_HIDDEN ||
					FindFileData.dwFileAttributes == FILE_ATTRIBUTE_SYSTEM ||
					FindFileData.dwFileAttributes == FILE_ATTRIBUTE_READONLY) {
					char* fname;
					fname = FindFileData.cFileName;

					if (addPath) {
						list.push_back(path + "/" + std::string(fname));
					} else {
						list.push_back(std::string(fname));
					}
				}
			} while (FindNextFileA(hFind, &FindFileData));

			FindClose(hFind);
		}
#else		
    		DIR* dp = nullptr;
    		struct dirent* dirp = nullptr;
    		if ((dp = opendir(path.c_str())) == nullptr) {
        		return list;
    		}

    		while ((dirp = readdir(dp)) != nullptr) {
        		if (dirp->d_type == DT_REG) {
            			if (exten.compare("*") == 0)
                			list.emplace_back(static_cast<std::string>(dirp->d_name));
            			else
                			if (std::string(dirp->d_name).find(exten) != std::string::npos)
                    				list.emplace_back(static_cast<std::string>(dirp->d_name));
        		}
    		}

    		closedir(dp);
#endif

		return list;
	}

	std::vector<std::string> Directory::GetListFilesR(const std::string& path, const std::string & exten, bool addPath)
	{
		std::vector<std::string> list = Directory::GetListFiles(path, exten, addPath);
		std::vector<std::string> dirs = Directory::GetListFolders(path, exten, addPath);

		std::vector<std::string>::const_iterator it;
		for (it = dirs.begin(); it != dirs.end(); ++it) {
			std::vector<std::string> cl = Directory::GetListFiles(*it, exten, addPath);
			list.insert(list.end(), cl.begin(), cl.end());
		}

		return list;
	}

	std::vector<std::string> Directory::GetListFolders(const std::string& path, const std::string & exten, bool addPath)
	{
		std::vector<std::string> list;
		std::string path_f = path + "/" + exten;
		list.clear();
#ifdef _MSC_VER
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind;

		hFind = FindFirstFileA((LPCSTR)path_f.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			return list;
		} else {
			do {
				if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
					strcmp(FindFileData.cFileName, ".") != 0 &&
					strcmp(FindFileData.cFileName, "..") != 0) {
					char* fname;
					fname = FindFileData.cFileName;

					if (addPath) {
						list.push_back(path + "/" + std::string(fname));
					} else {
						list.push_back(std::string(fname));
					}
				}
			} while (FindNextFileA(hFind, &FindFileData));

			FindClose(hFind);
		}
#else
    		DIR* dp = nullptr;
    		struct dirent* dirp = nullptr;
    		if ((dp = opendir(path.c_str())) == nullptr) {
        		return list;
    		}

    		while ((dirp = readdir(dp)) != nullptr) {
        		if (dirp->d_type == DT_DIR && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
            			if (exten.compare("*") == 0)
                			list.emplace_back(static_cast<std::string>(dirp->d_name));
            			else
                			if (std::string(dirp->d_name).find(exten) != std::string::npos)
                    				list.emplace_back(static_cast<std::string>(dirp->d_name));
        		}
    		}

    		closedir(dp);
#endif
		return list;
	}
}

