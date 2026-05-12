#pragma once
#include "Asset.h"

class FontCollection : public Asset
{
    friend class FontManager;

public:
    FontCollection();
    virtual ~FontCollection();

private:
    ComPtr<IDWriteFontCollection1> _collection;
    TCHAR _fontFaceName[128] = {};


public:
    const TCHAR* GetFontFaceName()
    {
        return _fontFaceName;
    }

public:
    bool LoadCollection(IDWriteFactory5* factory,const std::wstring& fileName);

    ComPtr<IDWriteFontCollection1> GetCollection();

public:
    virtual void Destroy() override;

};

