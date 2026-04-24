#include "pch.h"
#include "DirectoryManager.h"


void DirectoryManager::Init()
{
	// 실행 파일이 있는 현재 폴더를 루트로 잡음.
	m_rootPath = filesystem::current_path();					
	if (!filesystem::exists(m_rootPath))
		return;

	if (!filesystem::is_directory(m_rootPath))
		return;

	// 한 단계 위 폴더를 저장함.
	m_rootParentPath = m_rootPath.parent_path();
	if (!filesystem::exists(m_rootParentPath))
		return;
	if (!filesystem::is_directory(m_rootParentPath))
		return;
	
	// 자주 사용될 폴더 등록
	//RegisterPath("Logs");
	//RegisterPath("HLSL");
	//RegisterPath("Resources");


}

void DirectoryManager::RegisterPath(const string& _path)		// 특정 폴더를 등록하는 함수
{	
	//std::filesystem::exists : 존재하는지 여부
	//std::filesystem::is_directory : 폴더 인지 여부

	filesystem::path registeredPath = m_rootParentPath / _path; // '/' 나누기가 아닌 경로 결합 연산자.
	if (!filesystem::exists(registeredPath))					// 폴더가 있는지 확인
		filesystem::create_directories(registeredPath);			// 폴더가 없을 경우 생성함

	if (!filesystem::is_directory(registeredPath))				// 경로에 존재하는게 폴더가 맞는지 확인.
		return;

	m_paths[_path] = registeredPath;							// 맵에 저장.
}

void DirectoryManager::Destroy()
{
	m_paths.clear();
}

optional<filesystem::path> DirectoryManager::GetCachePath(const string& _pathName) const
{
	// RegisterPath() 로 등록된 경로를 이름으로 찾는 함수.
	// 여기서 리소스 폴더를 찾고 세부 폴더 구분은 GetDirectory 함수 사용함.

	auto it = m_paths.find(_pathName);		// m_paths 안에서 _pathName 키를 찾는다. -> 폴더명/폴더명/fine("폴더선택")
	if(it == m_paths.end())					// 컨테이너의 마지막인지 확인 -> 키가 없다
		return nullopt;						// 반환할 값이 없음 표현

	return it->second;						// 컨테이너는 value 값 -> (1,2) 2를 가져오는거임.
}

bool DirectoryManager::GetDirectoryFromRoot(const string& _dirName, OUT filesystem::path& _outVal)
{
	std::filesystem::path foundPath = m_rootPath / _dirName;
	if (!std::filesystem::exists(foundPath))
		return false;

	if (!std::filesystem::is_directory(foundPath))
		return false;

	_outVal = foundPath;

	return true;
}

bool DirectoryManager::GetDirectoryFromRoot(const filesystem::path& _dir, OUT filesystem::path& _outVal)
{
	std::filesystem::path foundPath = m_rootPath / _dir;
	if (!std::filesystem::exists(foundPath))
		return false;

	if (!std::filesystem::is_directory(foundPath))
		return false;

	_outVal = foundPath;

	return true;
}

bool DirectoryManager::GetDirectory(const filesystem::path& _basePath, const filesystem::path& _dirName, OUT filesystem::path& _outVal)
{	// 세부 폴더 구분 -> 텍스처, HLSL 등등

	filesystem::path foundPath = _basePath / _dirName;
	if (!filesystem::exists(foundPath))
		return false;
	if(!filesystem::is_directory(foundPath))
		return false;
	_outVal = foundPath;

	return true;
}

bool DirectoryManager::GetFile(const filesystem::path& _basePath, const filesystem::path& _fileName, OUT filesystem::path& _outVal)
{
	filesystem::path foundPath = _basePath / _fileName;
	if(!filesystem::exists(foundPath))
		return false;

	if (!filesystem::is_regular_file(foundPath))
		return false;
	_outVal = foundPath;

	return true;
}

bool DirectoryManager::IsFile(const filesystem::path& _path)
{
	if (!filesystem::exists(_path))
		return false;
	if (!filesystem::is_regular_file(_path))
		return false;

	return true;
}

bool DirectoryManager::IsDirectory(const filesystem::path& _path)
{
	if (!filesystem::exists(_path))
		return false;
	if (!filesystem::is_regular_file(_path))
		return false;

	return true;
}

bool DirectoryManager::IsExtension(const filesystem::path& _path, const string _extension)
{
	filesystem::path fileExtension = _path.extension(); // 확장자를 추출하는 함수
	string strExtension = fileExtension.string();		// 객체를 일반 문자열로 바꾸는 함수.
	string compareExtension = _extension;				// 수정하기 위한 복사본 생성.

	//toupper -> 소문자 하나를 대문자로 바꾸는 함수, transform -> 범위 안의 요소를 하나씩 꺼내서 함수를 적용
	transform(strExtension.begin(), strExtension.end(), strExtension.begin(), toupper); //-> 소문자를 전부 대문자로 변경
	transform(compareExtension.begin(), compareExtension.end(), compareExtension.begin(), toupper);

	if (compareExtension == strExtension)
		return true;

	return false;


}
