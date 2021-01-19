#include <nw_pch.hpp>
#include "nw_data_sys.h"

#include <core/nw_core_state.h>
#include <lua/nw_lua_engine.h>

#include <glib/gcontext/nw_window.h>
#include <glib/vision/nw_gmaterial.h>
#include <glib/vision/nw_shader.h>
#include <glib/nw_texture.h>

#include <sys/nw_mem_sys.h>

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

NW::DataSys::ADRs NW::DataSys::s_ADRs;
NW::String NW::DataSys::s_strRscDir = &std::filesystem::current_path().generic_string()[0];

namespace NW
{
    // --getters
    template <> AShader* DataSys::GetDataRes<AShader>(UInt32 unId) {
        if (AShader* pShader = dynamic_cast<AShader*>(GetADataRes(unId))) {
            return pShader;
        }
        return nullptr;
    }
    // --setters
    void DataSys::SetDirectory(const char* strDir) { s_strRscDir = strDir; }
    void DataSys::AddADataRes(ADataRes* pDataRes) {
        if (pDataRes == nullptr) return;
        s_ADRs[pDataRes->GetId()] = (pDataRes);
    }
    void DataSys::RmvADataRes(UInt32 unId) {
        ADRs::iterator itDR = s_ADRs.find(unId);
        if (s_ADRs.size() == 0) return;
        if (itDR == s_ADRs.end()) return;
        s_ADRs.erase(itDR);
    }

    // --==<core_methods>==--=
    bool DataSys::OnInit()
    {
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);

        if (true) {
            AShader::Create("shd_2d_batch")->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_2d_batch.glsl");
            AShader::Create("shd_2d_tile")->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_2d_tile.glsl");
            AShader::Create("shd_3d_batch")->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_3d_batch.glsl");
        }
        if (true) {
            ATexture2d::Create("tex_white_solid")->LoadF("");
        }
        if (true) {
            MemSys::NewT<GMaterial>("gmt_2d_batch")->SetShader(GetDataRes<AShader>("shd_2d_batch"));
            MemSys::NewT<GMaterial>("shd_2d_tile")->SetShader(GetDataRes<AShader>("shd_2d_tile"));
            MemSys::NewT<GMaterial>("gmt_3d_batch")->SetShader(GetDataRes<AShader>("shd_3d_batch"));
        }
        return true;
    }

    void DataSys::OnQuit()
    {
        if (true) {
            delete (GetDataRes<AShader>("shd_2d_tile"));
            delete (GetDataRes<AShader>("shd_3d_batch"));
        }
        if (true) {
            delete (GetDataRes<ATexture2d>("tex_white_solid"));
        }
        if (true) {
            MemSys::DelT<GMaterial>(GetDataRes<GMaterial>("gmt_2d_tile"));
            MemSys::DelT<GMaterial>(GetDataRes<GMaterial>("gmt_2d_batch"));
            MemSys::DelT<GMaterial>(GetDataRes<GMaterial>("gmt_3d_batch"));
        }
    }
    // -- File Dialogs
    String DataSys::FDialog_save(const char* strFilter)
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
    String DataSys::FDialog_load(const char* strFilter)
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

    // -- BinaryData
    bool DataSys::SaveF_data(const char* filePath,
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
    bool DataSys::SaveF_data(const char* directory, const char* name, const char* format,
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
    bool DataSys::LoadF_data(const char* filePath,
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
    bool DataSys::LoadF_data(const char* directory, const char* name, const char* format,
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

    // --Strings
    bool DataSys::SaveF_string(const char *strFPath, const char* strSrc, UInt64 unBytes)
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
    bool DataSys::LoadF_string(const char* strFPath, String& strDest)
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

    // -- Objects
    UByte* DataSys::LoadF_image(const char* strFPath,
        Int32* pnW, Int32* pnH, Int32* pnChannels)
    {
        return stbi_load(strFPath, pnW, pnH, pnChannels, 0);
    }
    bool DataSys::LoadF_image(const char* strFPath,
        UByte* pClrDataBuf, Int32* pnW, Int32* pnH, Int32* pnChannels)
    {
        if (pClrDataBuf != nullptr) return false;
        pClrDataBuf = stbi_load(strFPath, pnW, pnH, pnChannels, 0);
        if (pClrDataBuf == nullptr) return false;
        return true;
    }
    bool DataSys::LoadF_image(const char* strFPath, ImageInfo* pImage)
    {
        if (pImage == nullptr) return false;

        pImage->ClrData = LoadF_image(strFPath, &pImage->nWidth, &pImage->nHeight, &pImage->nChannels);
        if (pImage->ClrData == nullptr) return false;

        return true;
    }

    // -- Mesh data
    bool DataSys::LoadF_mesh(const String& strFilePath, DArray<VertexShape3d>* pVtxData, DArray<UInt32>* punIndData)
    {
        UInt16 dotPos = strFilePath.rfind('.') + 1;
        String format = strFilePath.substr(dotPos, strFilePath.size() - dotPos);
        FStream fStream;
        fStream.exceptions(std::ios::badbit | std::ios::failbit);
        StrStream strStream;
        try {
            fStream.open(strFilePath, std::ios::in, std::ios::binary);
            strStream << fStream.rdbuf();
            if (format == "obj") { LoadF_mesh_obj(strStream.str(), *pVtxData, *punIndData); }
            else if (format == "dae") {}
            else { throw std::exception("Unknown format"); }
            fStream.close();
            fStream.clear();
            return true;
        } catch (std::exception ex) {
            NW_ERR("Failed to load a file by path " + strFilePath + "\n" + ex.what());
            fStream.clear();
            fStream.close();
            return false;
        }
    }
    // --==</core_methods>==--=

    // --==<Implementation Methods>==--=
    bool DataSys::LoadF_mesh_obj(const String& strFileData, DArray<UInt32>& arrIndicesDest,
        DArray<float>& vtxCoordsDest, DArray<float>& texCoordsDest, DArray<float>& normCoordsDest)
    {
        String
            nameToken = "g ",
            vtxToken = "v ",
            uvToken = "vt ",
            normToken = "vn ",
            elemsToken = "f ",
            mtlSrcToken = "mtllib ",
            mtlUseToken = "usemtl ";
        String
            nameFormat = nameToken + "%s",
            vtxFormat = vtxToken + "%f %f %f",
            uvFormat = uvToken + "%f %f",
            normFormat = normToken + "%f %f %f",
            elemsFormat = elemsToken + "%d/%d/%d %d/%d/%d %d/%d/%d";
        // Count |objects_amount|, |vertices|, |uvs|, |normals| and |indices|
        std::unordered_map<const char*, UInt32> name_counter
        { std::make_pair("nameCount", 0),
            std::make_pair("vtxCount", 0),
            std::make_pair("uvCount", 0),
            std::make_pair("normCount", 0),
            std::make_pair("indexCount", 0)
        };
        float fTemp[3]{ 0.0f, 0.0f, 0.0f };
        UInt32 uiTemp[3]{ 0, 0, 0 };
        
        Int32 currPos = 0, nextPos = 0;
        Int32 currMesh = 0, nextMesh = 0;
        currMesh = strFileData.find(nameToken, currPos);
        nextMesh = strFileData.find(nameToken, currPos + nameToken.size());
        while (currMesh != -1)   // Find the line position where the mesh name is
        {
            currPos = nextPos = 0;
            String strData = strFileData.substr(currMesh, nextMesh - currMesh);
            // mesh name is found
            char cMeshName[128];
            name_counter["nameCount"]++;
            // Copy the mesh name : |currPos|_meshName|lineEnd|
            sscanf(&strFileData.c_str()[currMesh], nameFormat.c_str(), cMeshName);
            printf("NW::LOAD_MASTER::LOAD_MESH_DATA_OBJ: loading the \"%s\" mesh\n", cMeshName);
            
            do { nextPos = strData.find(vtxToken, currPos);  // Find from the end of the last line
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], vtxFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    vtxCoordsDest.push_back(fTemp[0]);
                    vtxCoordsDest.push_back(fTemp[1]);
                    vtxCoordsDest.push_back(fTemp[2]);
                    name_counter["vtxCount"]++;
                    currPos = nextPos + vtxToken.size();    // Curr = end of token; next = beginning
                }
            } while (true);
            
            do { nextPos = strData.find(uvToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], uvFormat.c_str(), &fTemp[0], &fTemp[1]))
                {
                    texCoordsDest.push_back(fTemp[0]);
                    texCoordsDest.push_back(fTemp[1]);
                    name_counter["uvCount"]++;
                    currPos = nextPos + uvToken.size();
                }
            } while (true);
            
            do { nextPos = strData.find(normToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], normFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    normCoordsDest.push_back(fTemp[0]);
                    normCoordsDest.push_back(fTemp[1]);
                    normCoordsDest.push_back(fTemp[2]);
                    name_counter["normCount"]++;
                    currPos = nextPos + normToken.size();
                }
            } while (true);
            
            do { nextPos = strData.find(elemsToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], elemsFormat.c_str(),
                    &uiTemp[0], &uiTemp[0], &uiTemp[0],
                    &uiTemp[1], &uiTemp[1], &uiTemp[1],
                    &uiTemp[2], &uiTemp[2], &uiTemp[2]))
                {
                    arrIndicesDest.push_back(uiTemp[0]);
                    arrIndicesDest.push_back(uiTemp[1]);
                    arrIndicesDest.push_back(uiTemp[2]);
                    name_counter["indexCount"]+=3;
                    currPos = nextPos + elemsToken.size();
                }
            } while (true);
            if (nextMesh == -1) break;
             (currMesh = (strFileData.find(nameToken, nextMesh)));
             nextMesh = strFileData.find(nameToken, currMesh + nameToken.size());
        }
        return true;
    }
    bool DataSys::LoadF_mesh_obj(const String& strFileData, DArray<VertexShape3d>& rVtxData, DArray<UInt32>& runIndData)
    {
        String
            nameToken = "g ",
            vtxToken = "v ",
            uvToken = "vt ",
            normToken = "vn ",
            elemsToken = "f ",
            mtlSrcToken = "mtllib ",
            mtlUseToken = "usemtl ";
        String
            nameFormat = nameToken + "%s",
            vtxFormat = vtxToken + "%f %f %f",
            uvFormat = uvToken + "%f %f",
            normFormat = normToken + "%f %f %f",
            elemsFormat = elemsToken + "%d/%d/%d %d/%d/%d %d/%d/%d";
        // Count |objects_amount|, |vertices|, |uvs|, |normals| and |indices|
        std::unordered_map<const char*, UInt32> name_counter
        { std::make_pair("nameCount", 0),
            std::make_pair("vtxCount", 0),
            std::make_pair("uvCount", 0),
            std::make_pair("normCount", 0),
            std::make_pair("indexCount", 0)
        };
        float fTemp[3]{ 0.0f, 0.0f, 0.0f };
        UInt32 uiTemp[3]{ 0, 0, 0 };

        DArray<V3f> VtxCoords;
        DArray<V2f> TexCoords;
        DArray<V3f> NormCoords;
        DArray<UInt32> unIndices;
        VtxCoords.resize(strFileData.size() / 150);
        TexCoords.resize(strFileData.size() / 150);
        NormCoords.resize(strFileData.size() / 150);
        unIndices.resize(strFileData.size() / 50);

        Int32 currPos = 0, nextPos = 0;
        Int32 currMesh = 0, nextMesh = 0;
        currMesh = strFileData.find(nameToken, currPos);
        nextMesh = strFileData.find(nameToken, currPos + nameToken.size());
        while (currMesh != -1)   // Find the line position where the mesh name is
        {
            currPos = nextPos = 0;
            String strData = strFileData.substr(currMesh, nextMesh - currMesh);
            // mesh name is found
            char cMeshName[128];
            name_counter["nameCount"]++;
            // Copy the mesh name : |currPos|_meshName|lineEnd|
            sscanf(&strFileData.c_str()[currMesh], nameFormat.c_str(), cMeshName);
            printf("NW::LOAD_MASTER::LOAD_MESH_DATA_OBJ: loading the \"%s\" mesh\n", cMeshName);

            do {
                nextPos = strData.find(vtxToken, currPos);  // Find from the end of the last line
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], vtxFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    VtxCoords[name_counter["vtxCount"]] = (V3f{fTemp[0], fTemp[1], fTemp[2] });
                    name_counter["vtxCount"]++;
                    currPos = nextPos + vtxToken.size();    // Curr = end of token; next = beginning
                }
            } while (true);

            do {
                nextPos = strData.find(uvToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], uvFormat.c_str(), &fTemp[0], &fTemp[1]))
                {
                    TexCoords[name_counter["uvCount"]] = (V2f{fTemp[0], fTemp[1]});
                    name_counter["uvCount"]++;
                    currPos = nextPos + uvToken.size();
                }
            } while (true);

            do {
                nextPos = strData.find(normToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], normFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    NormCoords[name_counter["normCount"]] = (V3f{fTemp[0], fTemp[1], fTemp[2] });
                    name_counter["normCount"]++;
                    currPos = nextPos + normToken.size();
                }
            } while (true);

            do {
                nextPos = strData.find(elemsToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], elemsFormat.c_str(),
                    &uiTemp[0], &uiTemp[0], &uiTemp[0],
                    &uiTemp[1], &uiTemp[1], &uiTemp[1],
                    &uiTemp[2], &uiTemp[2], &uiTemp[2]))
                {
                    unIndices[name_counter["indexCount"]] = (uiTemp[0]);
                    name_counter["indexCount"] ++;
                    unIndices[name_counter["indexCount"]] = (uiTemp[1]);
                    name_counter["indexCount"] ++;
                    unIndices[name_counter["indexCount"]] = (uiTemp[2]);
                    name_counter["indexCount"] ++;
                    currPos = nextPos + elemsToken.size();
                }
            } while (true);
            if (nextMesh == -1) break;
            (currMesh = (strFileData.find(nameToken, nextMesh)));
            nextMesh = strFileData.find(nameToken, currMesh + nameToken.size());
        }

        UInt32 unMaxVtx = 0;
        for (auto itCount = name_counter.begin(); itCount != name_counter.end(); advance(itCount, 1))
            unMaxVtx = itCount->second > unMaxVtx ? itCount->second : unMaxVtx;
        rVtxData.reserve(unMaxVtx);
        for (UInt32 vti = 0; vti < unMaxVtx; vti++)
        {
            rVtxData.emplace_back(VertexShape3d{VtxCoords[vti], TexCoords[vti], NormCoords[vti]});
        }
        unIndices.reserve(name_counter["indexCount"]);
        for (UInt32 idi = 0; idi < name_counter["indexCount"]; idi++)
        {
            runIndData.emplace_back(unIndices[idi]);
        }
        return true;

    }
    // --==</Implementation Methods>==--=
}