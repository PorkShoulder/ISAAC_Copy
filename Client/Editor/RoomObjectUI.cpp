#include "pch.h"
#include "EditorEngine.h"
#include "RoomObjectUI.h"

namespace
{
	std::vector<std::string> ObstacleType { "Rock", "Pit", "Spike" };
	std::vector<std::string> DoorType{ "NextDoor", "LockDoor", "HiddenDoor" };

}



RoomObjectUI::RoomObjectUI()
{
}

RoomObjectUI::~RoomObjectUI()
{
}

bool RoomObjectUI::Init(const std::string& name)
{
	return false;
}

void RoomObjectUI::Render(float deltaTime)
{
}

void RoomObjectUI::Destroy()
{
}
