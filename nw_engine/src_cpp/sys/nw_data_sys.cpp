#include <nw_pch.hpp>
#include "nw_data_sys.h"

#include <core/nw_core_engine.h>
#include <core/nw_window.h>

#include <glib_texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#if (defined NW_PLATFORM_WINDOWS)
#include <commdlg.h>
#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif  // NW_WINDOW
#endif  // NW_PLATFORM

String NW::DataSys::s_strRscDir = &std::filesystem::current_path().generic_string()[0];

namespace NW
{
    // --setters
    void DataSys::SetDirectory(const char* strDir) { s_strRscDir = strDir; }

    // --==<core_methods>==--
    bool DataSys::OnInit()
    {
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);

        GLIB::ImageInfo imgInfo;
        if (LoadFImage(R"(D:\dev\native_world\nw_engine\data\graphics\ico\ico_bat.png)", &imgInfo)) {
            CoreEngine::Get().GetWindow()->SetIcon(imgInfo.ClrData, imgInfo.nWidth, imgInfo.nHeight);
            delete[] imgInfo.ClrData;
        }

        return true;
    }

    void DataSys::OnQuit()
    {
    }
    // -- file_dialogs
    String DataSys::FDialogSave(const char* strFilter)
    {
    #if (defined NW_PLATFORM_WINDOWS)
        constexpr Int32 nMaxChars = 256;
        char strRes[nMaxChars]{ 0 };
        OPENFILENAMEA ofn{ 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window(glfwGetCurrentContext());
        ofn.lpstrFile = &strRes[0];
        ofn.nMaxFile = sizeof(strRes);
        ofn.lpstrFilter = strFilter;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        } else { return String(); }
    #endif  // NW_PLATFORM
    }
    String DataSys::FDialogLoad(const char* strFilter)
    {
    #if (defined NW_PLATFORM_WINDOWS)
        constexpr Int32 nMaxChars = 256;
        char strRes[nMaxChars] { 0 };
        OPENFILENAMEA ofn{ 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window(glfwGetCurrentContext());
        ofn.lpstrFile = &strRes[0];
        ofn.nMaxFile = nMaxChars;
        ofn.lpstrFilter = strFilter;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        } else { return ""; }
    #endif  // NW_PLATFORM
    }

    // -- binary_data
    bool DataSys::SaveFData(const char* filePath,
        void* pData, UInt64 unBytes)
    {
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);
        try {
            fStream.open(filePath, std::ios::out | std::ios::binary);
            fStream.write(static_cast<char*>(pData), unBytes);
            fStream.close();
            return true;
        } catch (std::ios_base::failure ex) { return false; }
    }
    bool DataSys::SaveFData(const char* directory, const char* name, const char* format,
        void* pData, UInt64 unBytes)
    {
        try {
            FStream fStream;
            StrStream strStream;
            strStream << directory << name << "." << format;
            fStream.open(strStream.str().c_str(), std::ios::out | std::ios::binary);
            fStream.write(static_cast<char*>(pData), unBytes);
            fStream.close();
            return true;
        } catch (std::ios_base::failure ex) {
            return false;
        }
    }
    bool DataSys::LoadFData(const char* filePath,
        void* pData, UInt64 unBytes)
    {
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);
        try {
            fStream.open(filePath, std::ios::in | std::ios::binary);
            fStream.read(static_cast<char*>(pData), unBytes);
            fStream.close();
            return true;
        } catch (std::ios_base::failure ex)
        {
            return false;
        }
    }
    bool DataSys::LoadFData(const char* directory, const char* name, const char* format,
        void* pData, UInt64 unBytes)
    {
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);
        StrStream strStream;
        try {
            strStream << directory << name << "." << format;
            fStream.open(strStream.str(), std::ios::in | std::ios::binary);
            fStream.read(static_cast<char*>(pData), unBytes);
            fStream.close();
            return true;
        } catch (std::ios_base::failure ex) {
            return false;
        }
    }

    // --strings
    bool DataSys::SaveFString(const char *strFPath, const char* strSrc, UInt64 unBytes)
    {
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);
        StrStream strStream;
        try {
            fStream.open(strFPath, std::ios::out, std::ios::binary);
            fStream.write(&strSrc[0], unBytes);
            fStream.close();
            return true;
        } catch (std::ios_base::failure ex) {
            return false;
        }
    }
    bool DataSys::LoadFString(const char* strFPath, String& strDest)
    {
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);
        StrStream strStream;
        try {
            fStream.open(strFPath, std::ios::in, std::ios::binary);
            fStream.seekg(0, std::ios::end);
            strDest.resize(fStream.tellg());
            fStream.seekg(0, std::ios::beg);
            strStream << fStream.rdbuf();
            strcpy(&strDest[0], &strStream.str()[0]);
            fStream.close();
            return true;
        } catch (std::ios_base::failure ex) {
            return false;
        }
    }

    // --objects
    UByte* DataSys::LoadFImage(const char* strFPath,
        Int32* pnW, Int32* pnH, Int32* pnChannels)
    {
        return stbi_load(strFPath, pnW, pnH, pnChannels, 0);
    }
    bool DataSys::LoadFImage(const char* strFPath,
        UByte* pClrDataBuf, Int32* pnW, Int32* pnH, Int32* pnChannels)
    {
        if (pClrDataBuf != nullptr) return false;
        pClrDataBuf = stbi_load(strFPath, pnW, pnH, pnChannels, 0);
        if (pClrDataBuf == nullptr) return false;
        return true;
    }
    bool DataSys::LoadFImage(const char* strFPath, GLIB::ImageInfo* pImage)
    {
        if (pImage == nullptr) return false;

        pImage->ClrData = LoadFImage(strFPath, &pImage->nWidth, &pImage->nHeight, &pImage->nChannels);
        if (pImage->ClrData == nullptr) return false;

        return true;
    }

    // --==</core_methods>==--

    // --==<implementation_methods>==--
    // --==</implementation_methods>==--
}