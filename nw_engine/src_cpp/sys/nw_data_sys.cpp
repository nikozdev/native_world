#include <nw_pch.hpp>
#include "nw_data_sys.h"

#include <core/nw_core_engine.h>
#include <core/nw_window.h>

#include <glib_texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
        IOFStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);

        ImageInfo imgInfo;
        if (LoadFImage(R"(D:\dev\native_world\nw_engine\data\graphics\ico\ico_bat.png)", imgInfo)) {
            CoreEngine::Get().GetWindow()->SetIcon(imgInfo);
            delete[] imgInfo.pClrData;
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
        IOFStream fStream;
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
            IOFStream fStream;
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
        IOFStream fStream;
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
        IOFStream fStream;
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
        IOFStream fStream;
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
        IOFStream fStream;
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

    // --images
    bool DataSys::SaveFImage(const char* strFPath, ImageInfo& rImage)
    {
        stbi_write_png(strFPath, rImage.nWidth, rImage.nHeight, 0, rImage.pClrData, 0);

        return true;
    }
    bool DataSys::LoadFImage(const char* strFPath, ImageInfo& rImage)
    {
        rImage.pClrData = stbi_load(strFPath, &rImage.nWidth, &rImage.nHeight, &rImage.nChannels, 0);
        if (rImage.pClrData == nullptr) return false;

        return true;
    }
    // --meshes
    bool DataSys::SaveFMesh(const char* strFPath, GMeshInfo& rMesh)
    {
        return true;
    }
    bool DataSys::LoadFMesh(const char* strFPath, GMeshInfo& rMesh)
    {
        String strBuf;
        if (!LoadFString(strFPath, strBuf)) { return false; }
        const char* strFormat = SubStrDelimR(strFPath, '.', 1);
        if (strcmp(strFormat, "obj") == 0) { return LoadFMeshObj(&strBuf[0], rMesh); }
        else { NWL_ERR("Unsupported format"); return false; }

        return true;
    }
    bool DataSys::SaveFModel(const char* strFPath, GModelInfo& rModel)
    {
        return true;
    }
    bool DataSys::LoadFModel(const char* strFPath, GModelInfo& rModel)
    {
        String strBuf;
        if (!LoadFString(strFPath, strBuf)) { return false; }
        const char* strFormat = SubStrDelimR(strFPath, '.', 1);
        if (strcmp(strFormat, "obj") == 0) { return LoadFModelObj(&strBuf[0], rModel); }
        else { NWL_ERR("Unsupported format"); return false; }

        return true;
    }
    // --==</core_methods>==--

    // --==<implementation_methods>==--
    inline bool DataSys::LoadFMeshObj(const char* strFile, GMeshInfo& rMesh) {
        String strBuf;
        StrStream strStream(strFile);
        DArray<V3f> vtxCrd;
        DArray<V2f> texCrd;
        DArray<V3f> nrmCrd;
        DArray<UInt32> idxData;

        vtxCrd.reserve(1 << 12);
        texCrd.reserve(1 << 12);
        nrmCrd.reserve(1 << 12);
        idxData.reserve(1 << 12);

        while (strStream >> strBuf) { if (strBuf != rMesh.strName) { continue; } }
        if (strBuf != rMesh.strName) { return false; }
        while (strStream >> strBuf) {
            if (strBuf == "usemtl") { strStream >> rMesh.strMtlName; }
            if (strBuf == "v") {
                vtxCrd.push_back(V3f());
                for (UInt32 vti = 0; vti < 3; vti++) {
                    if (!(strStream >> vtxCrd.back()[vti])) { return false; }
                }
            }
            if (strBuf == "vt") {
                texCrd.push_back(V3f());
                for (UInt32 vti = 0; vti < 2; vti++) {
                    if (!(strStream >> texCrd.back()[vti])) { return false; }
                }
            }
            if (strBuf == "vn") {
                nrmCrd.push_back(V3f());
                for (UInt32 vti = 0; vti < 3; vti++) { if (!(strStream >> nrmCrd.back()[vti])) { return false; } }
            }
            if (strBuf == "f") {
                strStream >> strBuf;
                idxData.push_back(0);
                if (sscanf(&strBuf[0], "%d/%d/%d", &idxData.back(), &idxData.back(), &idxData.back()) != 3) { return false; }
            }
        }

        UInt32 nMax = 0;
        nMax = nMax > vtxCrd.size() ? nMax : vtxCrd.size();
        nMax = nMax > texCrd.size() ? nMax : texCrd.size();
        nMax = nMax > nrmCrd.size() ? nMax : nrmCrd.size();
        rMesh.vtxData.resize(nMax * sizeof(Float32) * (3 + 2 + 3));
        for (UInt32 vti = 0; vti < nMax; vti++) {
            Size szOffset = 0;
            szOffset = vti * sizeof(Float32) * (3 + 2 + 3) + sizeof(Float32) * (0);
            memcpy(&rMesh.vtxData[szOffset], &vtxCrd[vti], sizeof(vtxCrd[vti]));
            szOffset = vti * sizeof(Float32) * (3 + 2 + 3) + sizeof(Float32) * (3);
            memcpy(&rMesh.vtxData[szOffset], &texCrd[vti], sizeof(texCrd[vti]));
            szOffset = vti * sizeof(Float32) * (3 + 2 + 3) + sizeof(Float32) * (3 + 2);
            memcpy(&rMesh.vtxData[szOffset], &texCrd[vti], sizeof(texCrd[vti]));
        }

        rMesh.idxData.resize(idxData.size() * sizeof(UInt32));
        memcpy(&rMesh.idxData[0], &idxData[0], rMesh.idxData.size());
        
        return true;
    }
    inline bool DataSys::LoadFModelObj(const char* strFile, GModelInfo& rModel) {
        String strBuf;
        StrStream strStream(strFile);
        DArray<V3f> vtxCrd;
        DArray<V2f> texCrd;
        DArray<V3f> nrmCrd;
        DArray<UInt32> idxData;

        auto fnUploadMesh = [&](GMeshInfo& rMesh)->bool {
            if (vtxCrd.size() == 0 || texCrd.size() == 0 || nrmCrd.size() == 0 || idxData.size() == 0) { return false; }
            UInt32 nMin = 0;
            nMin = nMin > vtxCrd.size() ? nMin : vtxCrd.size();
            nMin = nMin > texCrd.size() ? nMin : texCrd.size();
            nMin = nMin > nrmCrd.size() ? nMin : nrmCrd.size();
            rMesh.vtxData.resize(nMin * sizeof(Float32) * (3 + 2 + 3));
            for (UInt32 vti = 0; vti < nMin; vti++) {
                Size szOffset = 0;
                szOffset = vti * sizeof(Float32) * (3 + 2 + 3) + sizeof(Float32) * (0);
                memcpy(&rMesh.vtxData[szOffset], &vtxCrd[vti], sizeof(vtxCrd[vti]));
                szOffset = vti * sizeof(Float32) * (3 + 2 + 3) + sizeof(Float32) * (3);
                memcpy(&rMesh.vtxData[szOffset], &texCrd[vti], sizeof(texCrd[vti]));
                szOffset = vti * sizeof(Float32) * (3 + 2 + 3) + sizeof(Float32) * (3 + 2);
                memcpy(&rMesh.vtxData[szOffset], &texCrd[vti], sizeof(texCrd[vti]));
            }

            rMesh.idxData.resize(idxData.size() * sizeof(UInt32));
            memcpy(&rMesh.idxData[0], &idxData[0], rMesh.idxData.size());
            vtxCrd.clear();
            texCrd.clear();
            nrmCrd.clear();
            idxData.clear();
            vtxCrd.reserve(1 << 12);
            texCrd.reserve(1 << 12);
            nrmCrd.reserve(1 << 12);
            idxData.reserve(1 << 12);
            return true;
        };

        vtxCrd.reserve(1 << 12);
        texCrd.reserve(1 << 12);
        nrmCrd.reserve(1 << 12);
        idxData.reserve(1 << 12);
        while (strStream >> strBuf) {
            if (strBuf == "g") {
                rModel.Meshes.push_back(GMeshInfo());
                strStream >> rModel.Meshes.back().strName;
                while (strStream >> strBuf) {
                    if (strBuf == "usemtl") { strStream >> rModel.Meshes.back().strMtlName; }
                    if (strBuf == "v") {
                        vtxCrd.push_back(V3f());
                        for (UInt32 vti = 0; vti < 3; vti++) {
                            if (!(strStream >> vtxCrd.back()[vti])) { return false; }
                        }
                    }
                    if (strBuf == "vt") {
                        texCrd.push_back(V3f());
                        for (UInt32 vti = 0; vti < 2; vti++) {
                            if (!(strStream >> texCrd.back()[vti])) { return false; }
                        }
                    }
                    if (strBuf == "vn") {
                        nrmCrd.push_back(V3f());
                        for (UInt32 vti = 0; vti < 3; vti++) { if (!(strStream >> nrmCrd.back()[vti])) { return false; } }
                    }
                    if (strBuf == "f") {
                        for (UInt32 idi = 0; idi < 3; idi++) {
                            strStream >> strBuf;
                            idxData.push_back(0);
                            if (sscanf(&strBuf[0], "%d/%d/%d", &idxData.back(), &idxData.back(), &idxData.back()) != 3) { return false; }
                        }
                    }
                    if (strBuf == "g") { break; }
                }
                if (!fnUploadMesh(rModel.Meshes.back())) { return false; }
            }
        }
        return true;
    }
    // --==</implementation_methods>==--
}