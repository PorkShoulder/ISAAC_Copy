#include "pch.h"
#include "DirectoryManager.h"




void DirectoryManager::RegisterPath(const std::string& path)		// 특정 폴더를 등록하는 함수
{	
	//std::std::filesystem::exists : 존재하는지 여부
	//std::std::filesystem::is_directory : 폴더 인지 여부

	std::filesystem::path registeredPath = _rootParentPath / path; // '/' 나누기가 아닌 경로 결합 연산자.
	if (!std::filesystem::exists(registeredPath))					// 폴더가 있는지 확인
		std::filesystem::create_directories(registeredPath);			// 폴더가 없을 경우 생성함

	if (!std::filesystem::is_directory(registeredPath))				// 경로에 존재하는게 폴더가 맞는지 확인.
		return;

	_paths[path] = registeredPath;							// 맵에 저장.
}

void DirectoryManager::Init()
{
	// 실행 파일이 있는 현재 폴더를 루트로 잡음.
	_rootPath = std::filesystem::current_path();
	if (!std::filesystem::exists(_rootPath))
		return;

	if (!std::filesystem::is_directory(_rootPath))
		return;

	// 한 단계 위 폴더를 저장함.
	_rootParentPath = _rootPath.parent_path();
	if (!std::filesystem::exists(_rootParentPath))
		return;
	if (!std::filesystem::is_directory(_rootParentPath))
		return;

	// 자주 사용될 폴더 등록
	RegisterPath("Logs");
	RegisterPath("HLSL");
	RegisterPath("Resources");


}

void DirectoryManager::Destroy()
{
	_paths.clear();
}

std::optional<std::filesystem::path> DirectoryManager::GetCachePath(const std::string& pathName) const
{
	// RegisterPath() 로 등록된 경로를 이름으로 찾는 함수.
	// 여기서 리소스 폴더를 찾고 세부 폴더 구분은 GetDirectory 함수 사용함.

	auto it = _paths.find(pathName);		// m_paths 안에서 _pathName 키를 찾는다. -> 폴더명/폴더명/fine("폴더선택")
	if(it == _paths.end())					// 컨테이너의 마지막인지 확인 -> 키가 없다
		return std::nullopt;						// 반환할 값이 없음 표현

	return it->second;						// 컨테이너는 value 값 -> (1,2) 2를 가져오는거임.
}

bool DirectoryManager::GetDirectoryFromRoot(const std::string& dirName, OUT std::filesystem::path& outVal)
{
	std::filesystem::path foundPath = _rootPath / dirName;
	if (!std::filesystem::exists(foundPath))
		return false;

	if (!std::filesystem::is_directory(foundPath))
		return false;

	outVal = foundPath;

	return true;
}

bool DirectoryManager::GetDirectoryFromRoot(const std::filesystem::path& dir, OUT std::filesystem::path& outVal)
{
	std::filesystem::path foundPath = _rootPath / dir;
	if (!std::filesystem::exists(foundPath))
		return false;

	if (!std::filesystem::is_directory(foundPath))
		return false;

	outVal = foundPath;

	return true;
}

bool DirectoryManager::GetDirectory(const std::filesystem::path& basePath, const std::filesystem::path& dirName, OUT std::filesystem::path& outVal)
{	// 세부 폴더 구분 -> 텍스처, HLSL 등등

	std::filesystem::path foundPath = basePath / dirName;
	if (!std::filesystem::exists(foundPath))
		return false;
	if(!std::filesystem::is_directory(foundPath))
		return false;
	outVal = foundPath;

	return true;
}

bool DirectoryManager::GetFile(const std::filesystem::path& basePath, const std::filesystem::path& fileName, OUT std::filesystem::path& outVal)
{
	std::filesystem::path foundPath = basePath / fileName;
	if(!std::filesystem::exists(foundPath))
		return false;

	if (!std::filesystem::is_regular_file(foundPath))
		return false;
	outVal = foundPath;

	return true;
}

bool DirectoryManager::IsFile(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
		return false;

	if (!std::filesystem::is_regular_file(path))
		return false;

	return true;
}

bool DirectoryManager::IsDirectory(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
		return false;
	if (!std::filesystem::is_directory(path))
		return false;

	return true;
}

bool DirectoryManager::IsExtension(const std::filesystem::path& path, const std::string& extension)
{
	std::filesystem::path fileExtension = path.extension();

	std::string strExtension = fileExtension.string();

	std::string compareExtension = extension;
	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), toupper);
	std::transform(compareExtension.begin(), compareExtension.end(), compareExtension.begin(), toupper);

	if (compareExtension == strExtension)
		return true;

	return false;

}
