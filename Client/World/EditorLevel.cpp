#include "pch.h"
#include "EditorLevel.h"

bool EditorLevel::Init(const std::string& path)
{
    Level::Init(path);
    SpawnPlayer();
    return true;
}