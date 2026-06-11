#pragma once
#include "EditorUI.h"

class LoadUI : public EditorUI
{
public:
    LoadUI();
    virtual ~LoadUI();

private:
    char _fileName[64] = {};
    std::vector<std::string> _fileList;
    int32 _selectedIdx = -1;

public:
    virtual bool Init(const std::string& name);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;

private:
    void Load(const std::string& fileName);
    void RefreshFileList();
    
};

