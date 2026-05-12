#include "pch.h"
#include "FontCollection.h"
#include "DirectoryManager.h"

FontCollection::FontCollection()
{}

FontCollection::~FontCollection()
{}

bool FontCollection::LoadCollection(IDWriteFactory5* factory,const std::wstring& fileName)
{
    std::filesystem::path filePath;
    std::optional<std::filesystem::path> resPath = DirectoryManager::Instance().GetCachePath("Resources");

    if (!DirectoryManager::Instance().GetDirectory(resPath.value(), "Font", filePath))
        return false;

    if (!DirectoryManager::Instance().GetFile(filePath, fileName, filePath))
        return false;

    ComPtr<IDWriteFontFile> fontFile = nullptr;

    //폰트파일 참조 생성
    //폰트파일에 대한 참조 혹은 핸들 생성
    if (FAILED(factory->CreateFontFileReference(filePath.c_str(), nullptr, fontFile.GetAddressOf())))
        return false;

    ComPtr<IDWriteFontSetBuilder1> fontBuilder = nullptr;

    if (FAILED(factory->CreateFontSetBuilder(fontBuilder.GetAddressOf())))
        return false;

    //폰트 빌더에 폰트파일을 추가
    fontBuilder->AddFontFile(fontFile.Get());

    ComPtr<IDWriteFontSet> fontSet = nullptr;

    //빌더에 추가된 폰트파일들을 하나의 폰트셋 객체로 생성
    if (FAILED(fontBuilder->CreateFontSet(fontSet.GetAddressOf())))
        return false;

    if (FAILED(factory->CreateFontCollectionFromFontSet(fontSet.Get(), _collection.GetAddressOf())))
        return false;


    //폰트패밀리
    //나눔고딕 <--- 폰트패밀리
    //나눔 고딕 : 굵게(bold), 일반(regular) 등등
    UINT32 count = _collection->GetFontFamilyCount();

    ComPtr<IDWriteFontFamily> family = nullptr;

    if(FAILED(_collection->GetFontFamily(0, family.GetAddressOf())))
        return false;

    ComPtr<IDWriteLocalizedStrings> localName = nullptr;

    if (FAILED(family->GetFamilyNames(localName.GetAddressOf())))
        return false;

    if (FAILED(localName->GetString(0, _fontFaceName, 128)))
        return false;

    return true;
}

ComPtr<IDWriteFontCollection1> FontCollection::GetCollection()
{
    return _collection;
}

void FontCollection::Destroy()
{}
