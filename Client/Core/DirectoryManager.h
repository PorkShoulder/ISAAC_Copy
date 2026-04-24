#pragma once

//======================
// 파일 경로 관리 매니저.
//======================

#include "../Common/Singleton.h"
#include <fstream>
#include <filesystem>
#include <unordered_map>
using namespace std;





class DirectoryManager : public Singleton<DirectoryManager>
{
	SINGLETON(DirectoryManager)

private:
	// 경로 선언
	filesystem::path m_rootPath;						// 실행 파일이 있는 현재 경로 기준점.
	filesystem::path m_rootParentPath;					// 루프패스의 한 단계 위 폴더. 리소스 폴더들이 여기 기준으로 있으니 따로 저장함.
	unordered_map<string, filesystem::path> m_paths;	// 자주 쓰는 경로를 이름-경로 쌍으로 캐싱해둠 -> 매번 찾지 않고 이름으로 바로 꺼내스려고.

	
private:
	void Init();										// 루트 경로 설정하고 자주 쓰는 경로들을 등록(초기화)
	void RegisterPath(const string& _path);				// 경로를 _path에 등록하는 내부 함수
	void Destroy() override;							// 캐싱된 경로 정리 
	const filesystem::path& GetRootPath() const			// 루트 경로 반환
	{ return m_rootPath; }
	optional<filesystem::path> GetCachePath(const string& _pathName) const;	// 캐싱된 경로를 이름으로 찾아서 반환 
	
	// 루트 패스를 통해 파일의 경로를 얻어온다.
	bool GetDirectoryFromRoot(const string& _dirnName,
								OUT filesystem::path& _outVal);

	// 루트 패스를 통해 파일의 경로를 얻어온다.
	bool GetDirectoryFromRoot(const filesystem::path& _dir, 
								OUT filesystem::path& _outVal);

	// basePath + _dirName(디렉토리이름)(폴더이름) -> 이걸 통해 폴더 경로를 얻어온다. (풀패스)
	bool GetDirectory(const filesystem::path& _basePath, 
						const filesystem::path& _dirName,	
						OUT filesystem::path& _outVal);

	// basePath + _fileName(파일이름) -> 이걸 통해 파일 경로를 얻어 온다 (파일의 풀패스)
	bool GetFile(const filesystem::path& _basePath,
				 const filesystem::path& _fileName, 
				 OUT filesystem::path& outVal);

	// 매개변수로 들어온 경로가 파일이 맞는지 확인.
	bool IsFile(const filesystem::path& _path);

	// 매개변수로 들어온 경로가 폴더가 맞는지 확인.
	bool IsDirectory(const filesystem::path& _path);

	//확장자 비교
	bool IsExtension(const filesystem::path& _path, const string _extension);



};

