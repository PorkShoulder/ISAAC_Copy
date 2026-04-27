#pragma once
#include "Singleton.h"
#include "Info.h"
#include "../Core/Logger.h"

#include <fstream>	// c++에서 사용하는 파일 입,출력
#include <thread>	// thread를 사용하기 위해 추가해야하는 헤더
#include <mutex>	// 멀티 스레드 동기화


//=================================
// 로그를 모아서 파일로 저장하는 관리자
//=================================


class LogManager : public Singleton<LogManager>
{
	SINGLETON(LogManager)

private:
	Logger _logger;				// 실제 로그를 저장하는 객체
	std::ofstream _file;		// 로그를 파일에 저장하기 위한 파일 출력 스트림
	uint32 _lineCount = 0;		 
	uint32 _fileIndex = 0;		 
	bool _IsRun = false;
	
	// 로그 저장작업을 별도 쓰레드에서 처리하고 쓰레드가 동시에 로그를 남길때 충돌을 막음
	std::thread	_logThread;		
	std::mutex _m;			// 여러 쓰레드가 동시에 같은 데이터에 접근하지 못하게 막는 잠금 장치.

public:
    void Init();
    virtual void Destroy() override;
    void FlushLog();
    void ProcessLog();


private:
    void OpenNewFile();

    template<typename T, typename ...Args>
    void Debug(const T& data, const Args& ...args)
    {
        std::lock_guard lock(_m);

        //임계영역
        _logger.Logging(eLogLevel::Debug, data, args...);
    }

    template<typename T, typename ...Args>
    void Info(const T& data, const Args& ...args)
    {
        std::lock_guard lock(_m);

        _logger.Logging(eLogLevel::Info, data, args...);
    }

    template<typename T, typename ...Args>
    void Warning(const T& data, const Args& ...args)
    {
        std::lock_guard lock(_m);

        _logger.Logging(eLogLevel::Warning, data, args...);
    }

    template<typename T, typename ...Args>
    void Error(const T& data, const Args& ...args)
    {
        std::lock_guard lock(_m);

        _logger.Logging(eLogLevel::Error, data, args...);
    }

    template<typename T, typename ...Args>
    void Fatal(const T& data, const Args& ...args)
    {
        std::lock_guard lock(_m);

        _logger.Logging(eLogLevel::Fatal, data, args...);
    }

};

