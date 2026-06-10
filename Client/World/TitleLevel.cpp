#include "pch.h"
#include "TitleLevel.h"
#include "RoomManager.h"
#include "../Object/Actor.h"
#include "../Object/Player.h"

bool TitleLevel::Init(const std::string& path)
{
    Level::Init(path); 
    return true;

}
