#pragma once
#include<iostream>
#include<queue>
#include<sstream>
#include<windows.h>



//========================
// 완성된 문자열을 큐에 저장
//========================


enum class eLogLevel
{
	Debug,
	Info,
	Warning,
	Error,
	Fatal,
	END
};


#define DEBUG_LEVEL "[DEBUG]"
#define INFO_LEVEL "[INFO]"
#define WARNING_LEVEL "[WARNING]"
#define ERROR_LEVEL "[ERROR]"
#define FATAL_LEVEL "[FATAL]"



class Logger
{
	friend class LogManager;
private:
	std::queue<std::string> Logs;
	eLogLevel Level = eLogLevel::END;

public:
	// 디폴트 매개변수
	void Init(eLogLevel level = eLogLevel::Debug)
	{
		Level = level;
	}
public:
    //const Args& ...args 가변인자템플릿은 이렇게 써야 됩니다.
    template<typename T, typename ...Args>
    void Logging(eLogLevel level, const T& data, const Args& ...args)
    {
        // 로그를 남길지 판단하고, 로그 레벨 문자열을 붙여서 실제 작성 함수로 넘기는 함수.
        // 1. 현재 설정된 로그 레벨보다 낮은 로그는 무시한다.
        // 2. 로그 레벨에 맞는 문자열을 골라 Wrigte()로 넘긴다.

        if (Level > level)
            return;

        switch (level)
        {
        case eLogLevel::Debug:
            WriteLog(DEBUG_LEVEL, data, args...);
            break;
        case eLogLevel::Info:
            WriteLog(INFO_LEVEL, data, args...);
            break;
        case eLogLevel::Warning:
            WriteLog(WARNING_LEVEL, data, args...);
            break;
        case eLogLevel::Error:
            WriteLog(ERROR_LEVEL, data, args...);
            break;
        case eLogLevel::Fatal:
            WriteLog(FATAL_LEVEL, data, args...);
            break;
        case eLogLevel::END:
            break;
        default:
            break;
        }
    }
private:
    template<typename T, typename ...Args>
    void WriteLog(const char* logLevelString, const T& data, const Args& ...args)
    {
        // 로그 한 줄의 기본 형태를 만드는 함수.
        // 1.현재시간을 구한다
        // 2.시간을 문자열로 만든다.
        // 3.시간+로그레벨+로그내용을 stream에 합친다.
        // 4.완성된 문자열을 Logs 큐에 넣는다.


        //로컬 시간을 얻어오는 객체
        SYSTEMTIME sysTime;             // 날짜/ㅅ기나정보를 담는 구조체
        GetLocalTime(&sysTime);         // 현재 컴퓨터의 로컬 시간을 가져와서 채움

        char timeText[128] = {};
        sprintf_s(timeText, "[%d.%d.%d.%d.%d.%d]",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,
            sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

        std::ostringstream stream;
        stream << timeText;
        stream << logLevelString;

        //여기서 부터는 매개변수로 들어온 로그 정도들을 stream 객체에 넣어주면 됩니다.
        WriteLog(stream, data, args...);

        Logs.push(stream.str());

    }
    template<typename T, typename ...Args>
    void WriteLog(std::ostringstream& os, const T& data, const Args& ...args)
    {
        os << data;

        //C++17 폴드표현식
        ((os << args), ...);
    }

};