#include "nw_gui_pch.hpp"
#include "nw_gui_gapi.h"
#if (defined NW_GAPI)
#include "nw_gfx.hpp"
#   if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
    // --types
    /// opengl_viewport_data struct
    struct ogl_viewport_data
    {
    };
    /// openg_context_state struct
    /// automatically backups and restores graphical state
    struct ogl_context_state
    {
    public:
        ogl_context_state()
        {
            // shaders
            glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_shd_prog);
            // textures
            glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_txr);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_bound_txr);
            glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&last_sampler);
            // vertex data
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_vbuf);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_varr);
            // rasteriser state
            glGetIntegerv(GL_POLYGON_MODE, last_poly_mode);
            glGetIntegerv(GL_VIEWPORT, last_viewport);
            glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
            // blending state
            glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
            glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
            glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_aplha);
            glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
            glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_eq_rgb);
            glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_eq_alpha);
            // enable flags
            last_blending = glIsEnabled(GL_BLEND);
            last_culling = glIsEnabled(GL_CULL_FACE);
            last_depth_test = glIsEnabled(GL_DEPTH_TEST);
            last_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
        }
        ~ogl_context_state()
        {
            // enable flags
            if (last_blending) { glEnable(GL_BLEND); }
            else { glDisable(GL_BLEND); }
            if (last_culling) { glEnable(GL_CULL_FACE); }
            else { glDisable(GL_CULL_FACE); }
            if (last_depth_test) { glEnable(GL_DEPTH_TEST); }
            else { glDisable(GL_DEPTH_TEST); }
            if (last_scissor_test) { glEnable(GL_SCISSOR_TEST); }
            else { glDisable(GL_SCISSOR_TEST); }
            // shaders
            glUseProgram(last_shd_prog);
            // texutes
            glBindTexture(GL_TEXTURE_2D, last_bound_txr);
            glBindSampler(0u, last_sampler);
            glActiveTexture(last_bound_txr);
            // vertex data
            glBindVertexArray(last_varr);
            glBindBuffer(GL_ARRAY_BUFFER, last_vbuf);
            // rasterizer
            glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_poly_mode[0]);
            glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
            glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
            // blending
            glBlendEquationSeparate(last_blend_eq_rgb, last_blend_eq_alpha);
            glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_aplha, last_blend_dst_alpha);
        }
    public:
        // shaders
        GLuint last_shd_prog;
        // textures
        GLenum last_active_txr;
        GLuint last_bound_txr;
        GLuint last_sampler;
        // vertexdata
        GLuint last_vbuf;
        GLuint last_varr;
        // rasterizer
        GLint last_viewport[4];
        GLint last_scissor_box[4];
        GLint last_poly_mode[2];
        // blending
        GLenum last_blend_src_rgb;
        GLenum last_blend_dst_rgb;
        GLenum last_blend_src_aplha;
        GLenum last_blend_dst_alpha;
        GLenum last_blend_eq_rgb;
        GLenum last_blend_eq_alpha;
        // enable flags
        GLboolean last_blending;
        GLboolean last_culling;
        GLboolean last_depth_test;
        GLboolean last_scissor_test;
    };
    struct ogl_vao
    {
    public:
        ogl_vao() : vao_id(0u) { glGenVertexArrays(1, &vao_id); }
        ~ogl_vao() { if (vao_id != 0) { glDeleteVertexArrays(1, &vao_id); vao_id = 0; } }
    public:
        GLuint vao_id;
    };
    // --data
    static device_handle s_device = NW_NULL;
    static context_handle s_context = NW_NULL;
    static GLuint s_txr_font_id = 0;
    static GLuint s_prog_id = 0, s_vshd_id = 0, s_pshd_id = 0;
    static GLuint s_vbuf_id = 0, s_ibuf_id = 0;
#define NW_GUI_VSI_LOC_VTX_CRD 0
#define NW_GUI_VSI_LOC_TXR_CRD 1
#define NW_GUI_VSI_LOC_VTX_CLR 2
#define NW_GUI_UNF_LOC_PROJMAT 0
#define NW_GUI_UNF_LOC_TEXTURE 1
#define NW_GUI_VTYPE_IDX       sizeof(ImDrawIdx) == 2u ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT
    static const GLchar* s_vshd_source = R"(
#version 460 core
layout(location = 0) in vec2 vsi_vtx_crd;    // 0-th input location
layout(location = 1) in vec2 vsi_txr_crd;    // 1-th input location
layout(location = 2) in vec4 vsi_vtx_clr;    // 2-th input location

layout(location = 0) uniform mat4 cst_projmat;  // 0-th uniform location

out vec2 vso_txr_crd;
out vec4 vso_vtx_clr;

void main()
{
    vso_txr_crd = vsi_txr_crd;
    vso_vtx_clr = vsi_vtx_clr;
    
    gl_Position =
        cst_projmat *
        vec4(vsi_vtx_crd.xy, 0, 1);
})";
    const GLchar* s_pshd_source =
        R"(
#version 460 core

in vec2 vso_txr_crd;
in vec4 vso_vtx_clr;

layout(location = 1) uniform sampler2D cst_texture;   // 1-th uniform location

layout(location = 0) out vec4 pso_pxl_clr;

void main()
{
    vec4 txr_clr = texture(cst_texture, vso_txr_crd);
    
    pso_pxl_clr = txr_clr * vso_vtx_clr;
})";
    // --aux_functions
    v1nil gui_gapi_update_state(ImDrawData* draw_data, v1s fmbuf_size_x, v1s fmbuf_size_y, GLuint varr)
    {
        // setup render state:
        // -> alpha-blending enabled;
        // -> disable culling;
        // -> disable depth;
        // -> enable scissor;
        // -> set polygon fill;
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        v1b clip_lower_left = NW_TRUTH;
        // setup viewport, orthographic projection matrix
        // our visible imgui space lies from draw_data->DisplayPos (top left)
        // to draw_data->DisplayPos+data_data->DisplaySize (bottom right).
        // DisplayPos is (0,0) for single viewport apps.
        glViewport(0, 0, static_cast<GLsizei>(fmbuf_size_x), static_cast<GLsizei>(fmbuf_size_y));
        v1f lft = draw_data->DisplayPos.x;
        v1f rht = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        v1f top = draw_data->DisplayPos.y;
        v1f bot = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
        if (!clip_lower_left) { v1f tmp = top; top = bot; bot = tmp; } // swap top and bottom if origin is upper left
        if constexpr (NW_TRUTH) {
            float projection[] = {
                +2.0f / (rht - lft),          +0.0f,                       +0.0f,          +0.0f,
                +0.0f,                        +2.0f / (top - bot),         +0.0f,          +0.0f,
                +0.0f,                        +0.0f,                       -1.0f,          +0.0f,
                (rht + lft) / (lft - rht),    (top + bot) / (bot - top),   +0.0f,          +1.0f,
            };
            // setup the orthographics projection
            glUseProgram(s_prog_id);
            glUniformMatrix4fv(NW_GUI_UNF_LOC_PROJMAT, 1, GL_FALSE, projection);
        }
        else {
            cm4f projection = gfx_tform_cam::make_ortho(-1.0f, +1.0f, (rht - lft) / (top - bot), 1.0f);
            // setup the orthographics projection
            glUseProgram(s_prog_id);
            glUniformMatrix4fv(NW_GUI_UNF_LOC_PROJMAT, 1, GL_FALSE, &projection[0u][0u]);
        }
        // setup the texture uniform
        glUniform1i(NW_GUI_UNF_LOC_TEXTURE, 0);
        glActiveTexture(GL_TEXTURE0);
        // set temporary vertex array to save front-end one;
        glBindVertexArray(varr);
        // bind vertex/index buffers and setup attributes for ImDrawVert;
        glBindBuffer(GL_ARRAY_BUFFER, s_vbuf_id);
        // vertex coordinates
        glEnableVertexAttribArray(NW_GUI_VSI_LOC_VTX_CRD);
        glVertexAttribPointer(NW_GUI_VSI_LOC_VTX_CRD, 2, GL_FLOAT, GL_FALSE,
            sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
        // texture coordinates
        glEnableVertexAttribArray(NW_GUI_VSI_LOC_TXR_CRD);
        glVertexAttribPointer(NW_GUI_VSI_LOC_TXR_CRD, 2, GL_FLOAT, GL_FALSE,
            sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
        // vertex color
        glEnableVertexAttribArray(NW_GUI_VSI_LOC_VTX_CLR);
        glVertexAttribPointer(NW_GUI_VSI_LOC_VTX_CLR, 4, GL_UNSIGNED_BYTE, GL_TRUE,
            sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
        // don't forget about the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibuf_id);
    }
}
namespace NW
{
    // --core_methods
    v1bit gui_gapi_init(device_handle device, context_handle context)
    {
        if (s_device != NW_NULL || s_context != NW_NULL) { return NW_FALSE; }
        s_device = device;
        s_context = context;
        // setup back-end capabilities flags
        if constexpr (NW_TRUTH) {
            s_imlib_io->BackendRendererName = "nw_gfx_ogl";
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        }
        // initialize platform interface
        if (gui_is_viewp_enabled()) { if (gui_gapi_wnd_init() == NW_FALSE) { return NW_FALSE; } }
        // create graphical resources
        // save open_gl state
        ogl_context_state last_state;
        if constexpr (NW_TRUTH) {
            // create shaders
            if constexpr (NW_TRUTH) {
                // vertex shader
                s_vshd_id = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(s_vshd_id, 1, &s_vshd_source, NW_NULL);
                glCompileShader(s_vshd_id);
                if (!gfx_check_shader(s_vshd_id)) { return NW_FALSE; }
                // pixel shader
                s_pshd_id = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(s_pshd_id, 1, &s_pshd_source, NW_NULL);
                glCompileShader(s_pshd_id);
                if (!gfx_check_shader(s_pshd_id)) { return NW_FALSE; }
                // shader program
                s_prog_id = glCreateProgram();
                glAttachShader(s_prog_id, s_vshd_id);
                glAttachShader(s_prog_id, s_pshd_id);
                glLinkProgram(s_prog_id);
                if (!gfx_check_shader(s_prog_id)) { return NW_FALSE; }
                // check program attribute and uniform locations
                if (glGetUniformLocation(s_prog_id, "cst_projmat") != NW_GUI_UNF_LOC_PROJMAT) { return NW_FALSE; }
                if (glGetUniformLocation(s_prog_id, "cst_texture") != NW_GUI_UNF_LOC_TEXTURE) { return NW_FALSE; }
                if (glGetAttribLocation(s_prog_id, "vsi_vtx_crd") != NW_GUI_VSI_LOC_VTX_CRD) { return NW_FALSE; }
                if (glGetAttribLocation(s_prog_id, "vsi_txr_crd") != NW_GUI_VSI_LOC_TXR_CRD) { return NW_FALSE; }
                if (glGetAttribLocation(s_prog_id, "vsi_vtx_clr") != NW_GUI_VSI_LOC_VTX_CLR) { return NW_FALSE; }
            }
            // create buffers
            if constexpr (NW_TRUTH) {
                glGenBuffers(1, &s_vbuf_id);
                glGenBuffers(1, &s_ibuf_id);
            }
            // build texture atlas
            if constexpr (NW_TRUTH) {
                ubyte_t* pxl_data = NW_NULL;
                v1s size_x = 0;
                v1s size_y = 0;
                // load as rgba 32-bit (75% of the memory is wasted, but default font is so small)
                // because it is more likely to be compatible with user's existing shaders.
                // if your ImTextureID represent a higher-level concept than just a gl texture id,
                // consider calling GetTexDataAsAlpha8() instead to save on gpu memory.
                s_imlib_io->Fonts->GetTexDataAsRGBA32(&pxl_data, &size_x, &size_y);
                // save open_gl state
                // upload texture to graphics system
                glGenTextures(1, &s_txr_font_id);
                glBindTexture(GL_TEXTURE_2D, s_txr_font_id);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0u, GL_RGBA, size_x, size_y, 0u, GL_RGBA, GL_UNSIGNED_BYTE, pxl_data);
                // set this font to gui state
                s_imlib_io->Fonts->SetTexID((ImTextureID)(intptr_t)s_txr_font_id);
            }
        }

        return NW_TRUTH;
    }
    v1bit gui_gapi_quit()
    {
        if (s_device == NW_NULL || s_context == NW_NULL) { return NW_FALSE; }
        
        if (gui_is_viewp_enabled()) { gui_gapi_wnd_quit(); }

        if constexpr (NW_TRUTH) {
            if (s_vbuf_id != 0) { glDeleteBuffers(1, &s_vbuf_id); s_vbuf_id = 0; }
            if (s_ibuf_id != 0) { glDeleteBuffers(1, &s_ibuf_id); s_ibuf_id = 0; }
            if (s_prog_id != 0 && s_vshd_id != 0) { glDetachShader(s_prog_id, s_vshd_id); }
            if (s_prog_id != 0 && s_pshd_id != 0) { glDetachShader(s_prog_id, s_pshd_id); }
            if (s_vshd_id != 0) { glDeleteShader(s_vshd_id); s_vshd_id = 0; }
            if (s_pshd_id != 0) { glDeleteShader(s_pshd_id); s_pshd_id = 0; }
            if (s_prog_id != 0) { glDeleteProgram(s_prog_id); s_prog_id = 0; }
            if (s_txr_font_id != 0) {
                glDeleteTextures(1, &s_txr_font_id);
                s_imlib_io->Fonts->TexID = 0;
                s_txr_font_id = 0;
            }
        }
        
        s_device = NW_NULL; s_context = NW_NULL;
        
        return NW_TRUTH;
    }
    v1nil gui_gapi_update(ImDrawData* draw_data)
    {
        // avoid rendering when minimized;
        // scale coordinates for retina displays;
        // (screen coordinates != framebuffer coordinates);
        v1s fmbuf_size_x = static_cast<v1s>(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
        v1s fmbuf_size_y = static_cast<v1s>(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
        if (fmbuf_size_x <= 0 || fmbuf_size_y <= 0) { return; }
        // backup opengl state
        ogl_context_state last_state;
        // setup desired opengl state
        // recreate the vao every time (this is to easily allow multiple open_gl contexts
        // to be rendered to vao that are not shared among open_gl contexts;
        // the renderer would actually work without any vao bound,
        // but then our VertexAttrib calls would overwrite the default one currently bound
        ogl_vao temp_vao;
        gui_gapi_update_state(draw_data, fmbuf_size_x, fmbuf_size_y, temp_vao.vao_id);
        // will project scissor/clipping rect
        // (angles into framebuffer space (0,0) unless using multi-viewports
        ImVec2 clip_off = draw_data->DisplayPos;
        // (1,1) unless using retina display which are often (2,2)
        ImVec2 clip_scale = draw_data->FramebufferScale;
        // render command lists
        for (v1s icmd = 0; icmd < draw_data->CmdListsCount; icmd++) {
            // get the commands
            const ImDrawList* cmd_list = draw_data->CmdLists[icmd];
            // upload vertex/index buffers
            glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
                (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx),
                (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);
            // process every single command
            for (v1s icmd = 0; icmd < cmd_list->CmdBuffer.Size; icmd++) {
                // get the current command
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[icmd];
                // user callback, registered via ImDrawList::AddCallback();
                // (ImDrawCallback_ResetRenderState is a special callback value
                // used by the user to request the renderer to reset render state);
                if (pcmd->UserCallback != NW_NULL) {
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState) {
                        gui_gapi_update_state(draw_data, fmbuf_size_x, fmbuf_size_y, temp_vao.vao_id);
                    }
                    else { pcmd->UserCallback(cmd_list, pcmd); }
                }
                else {
                    // project scissor/clipping ImGuiRect(angles into framebuffer space
                    v4s clip_rect;
                    clip_rect[0] = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                    clip_rect[1] = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                    clip_rect[2] = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                    clip_rect[3] = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;
                    if (clip_rect[0] < fmbuf_size_x && clip_rect[1] < fmbuf_size_y && clip_rect[2] >= 0.0f && clip_rect[3] >= 0.0f) {
                        // apply scissor/clipping rectaangle
                        glScissor(
                            clip_rect[0], fmbuf_size_y - clip_rect[3],
                            clip_rect[2] - clip_rect[0], clip_rect[3] - clip_rect[1]
                        );
                        // use the texture;
                        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(reinterpret_cast<intptr_t>(pcmd->TextureId)));
                        // draw indexed data
                        const GLvoid* idx_offset = reinterpret_cast<const GLvoid*>( static_cast<const intptr_t>(pcmd->IdxOffset * sizeof(ImDrawIdx)) );
                        const GLsizei idx_count = static_cast<const GLsizei>(pcmd->ElemCount);
                        glDrawElements(GL_TRIANGLES, idx_count, NW_GUI_VTYPE_IDX, idx_offset);
                    }
                }
            }
        }
    }
}
namespace NW
{
    // --wnd_setters
    v1nil gui_gapi_wnd_set_size(ImGuiViewport* viewport, ImVec2 size)
    {
        //
    }
    // --wnd_core_functions
    v1bit gui_gapi_wnd_init()
    {
        if (s_imsys_io == NW_NULL) { return NW_FALSE; }
        
        s_imsys_io->Renderer_CreateWindow = gui_gapi_wnd_create;
        s_imsys_io->Renderer_DestroyWindow = gui_gapi_wnd_delete;
        s_imsys_io->Renderer_RenderWindow = gui_gapi_wnd_render;
        s_imsys_io->Renderer_SwapBuffers = gui_gapi_wnd_swap;
        s_imsys_io->Renderer_SetWindowSize = gui_gapi_wnd_set_size;
       
        return NW_TRUTH;
    }
    v1nil gui_gapi_wnd_quit()
    {
        if (s_imsys_io == NW_NULL) { return; }
    }
    v1nil gui_gapi_wnd_create(ImGuiViewport* viewport)
    {
        //
    }
    v1nil gui_gapi_wnd_delete(ImGuiViewport* viewport)
    {
        //
    }
    // --wnd_graphics
    v1nil gui_gapi_wnd_render(ImGuiViewport* viewport, ptr_t draw_arg)
    {
        if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear)) {
            ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        gui_gapi_update(viewport->DrawData);
    }
    v1nil gui_gapi_wnd_swap(ImGuiViewport* viewport, ptr_t draw_arg)
    {
        //
    }
}
#   endif   // GAPI_OGL
#   if (NW_GAPI & NW_GAPI_D3D)
namespace NW
{
    // data
    context_handle s_context = NW_NULL;
    device_handle s_device = NW_NULL;
    // types
    struct vsbuf_m16f { v1f mdl_view_proj[4][4]; };
    struct dx_viewport_data
    {
    public:
        IDXGISwapChain* SwapChain;
        ID3D11RenderTargetView* RTView;
    public:
        dx_viewport_data() { SwapChain = NW_NULL; RTView = NW_NULL; }
        ~dx_viewport_data() { IM_ASSERT(SwapChain == NW_NULL && RTView == NW_NULL); }
    };
    struct dx_context_state
    {
    public:
        dx_context_state()
        {
            ScissorRectsCount = ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
            // rasterizer
            s_context->RSGetScissorRects(&ScissorRectsCount, ScissorRects);
            s_context->RSGetViewports(&ViewportsCount, Viewports);
            s_context->RSGetState(&RS);
            // output_manager
            s_context->OMGetBlendState(&BlendState, BlendFactor, &SampleMask);
            s_context->OMGetDepthStencilState(&DepthStencilState, &StencilRef);
            // shaders
            PSInstancesCount = VSInstancesCount = GSInstancesCount = 256;
            s_context->PSGetShaderResources(0, 1, &PSShaderResource);
            s_context->PSGetSamplers(0, 1, &PSSampler);
            s_context->PSGetShader(&PS, PSInstances, &PSInstancesCount);
            s_context->VSGetShader(&VS, VSInstances, &VSInstancesCount);
            s_context->VSGetConstantBuffers(0, 1, &VSConstantBuffer);
            s_context->GSGetShader(&GS, GSInstances, &GSInstancesCount);
            // input_assembler
            s_context->IAGetPrimitiveTopology(&PrimitiveTopology);
            s_context->IAGetIndexBuffer(&IndexBuffer, &IndexBufferFormat, &IndexBufferOffset);
            s_context->IAGetVertexBuffers(0, 1, &VertexBuffer, &VertexBufferStride, &VertexBufferOffset);
            s_context->IAGetInputLayout(&input_layout);
        }
        ~dx_context_state()
        {
            // rasterizer
            s_context->RSSetScissorRects(ScissorRectsCount, ScissorRects);
            s_context->RSSetViewports(ViewportsCount, Viewports);
            s_context->RSSetState(RS);
            if (RS != NW_NULL) { RS->Release(); RS = NW_NULL; }
            // output_manager
            s_context->OMSetBlendState(BlendState, BlendFactor, SampleMask);
            if (BlendState != NW_NULL) { BlendState->Release(); BlendState = NW_NULL; }
            s_context->OMSetDepthStencilState(DepthStencilState, StencilRef);
            if (DepthStencilState != NW_NULL) { DepthStencilState->Release(); DepthStencilState = NW_NULL; }
            // shaders
            s_context->PSSetShaderResources(0, 1, &PSShaderResource);
            s_context->PSSetShader(PS, PSInstances, PSInstancesCount);
            if (PS != NW_NULL) { PS->Release(); PS = NW_NULL; }
            for (UINT i = 0; i < PSInstancesCount; i++) {
                if (PSInstances[i] != NW_NULL) { PSInstances[i]->Release(); PSInstances[i] = NW_NULL; }
            }
            s_context->VSSetShader(VS, VSInstances, VSInstancesCount);
            if (VS != NW_NULL) { VS->Release(); VS = NW_NULL; }
            s_context->GSSetShader(GS, GSInstances, GSInstancesCount);
            if (GS != NW_NULL) { GS->Release(); GS = NW_NULL; }
            for (UINT i = 0; i < VSInstancesCount; i++) {
                if (VSInstances[i] != NW_NULL) { VSInstances[i]->Release(); VSInstances[i] = NW_NULL; }
            }
            // textures
            if (PSShaderResource != NW_NULL) { PSShaderResource->Release(); PSShaderResource = NW_NULL; }
            s_context->PSSetSamplers(0, 1, &PSSampler);
            if (PSSampler != NW_NULL) { PSSampler->Release(); PSSampler = NW_NULL; }
            s_context->VSSetConstantBuffers(0, 1, &VSConstantBuffer);
            if (VSConstantBuffer != NW_NULL) { VSConstantBuffer->Release(); VSConstantBuffer = NW_NULL; }
            // input_assembler
            s_context->IASetPrimitiveTopology(PrimitiveTopology);
            s_context->IASetIndexBuffer(IndexBuffer, IndexBufferFormat, IndexBufferOffset);
            if (IndexBuffer != NW_NULL) { IndexBuffer->Release(); IndexBuffer = NW_NULL; }
            s_context->IASetVertexBuffers(0, 1, &VertexBuffer, &VertexBufferStride, &VertexBufferOffset);
            if (VertexBuffer != NW_NULL) { VertexBuffer->Release(); VertexBuffer = NW_NULL; }
            s_context->IASetInputLayout(input_layout);
            if (input_layout != NW_NULL) { input_layout->Release(); input_layout = NW_NULL; }

        }
    public:
        UINT                        ScissorRectsCount, ViewportsCount;
        D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        ID3D11RasterizerState* RS;
        ID3D11BlendState* BlendState;
        FLOAT                       BlendFactor[4];
        UINT                        SampleMask;
        UINT                        StencilRef;
        ID3D11DepthStencilState* DepthStencilState;
        ID3D11ShaderResourceView* PSShaderResource;
        ID3D11SamplerState* PSSampler;
        ID3D11PixelShader* PS;
        ID3D11VertexShader* VS;
        ID3D11GeometryShader* GS;
        UINT                        PSInstancesCount, VSInstancesCount, GSInstancesCount;
        ID3D11ClassInstance* PSInstances[256], * VSInstances[256], * GSInstances[256];   // 256 is max according to PSSetShader documentation
        D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
        ID3D11Buffer* IndexBuffer, * VertexBuffer, * VSConstantBuffer;
        UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
        DXGI_FORMAT                 IndexBufferFormat;
        ID3D11InputLayout* input_layout;
    };
    // --data
    static IDXGIFactory* s_factory = NW_NULL;
    static ID3D11Buffer* s_vbuf_id = NW_NULL;
    static ID3D11Buffer* s_ibuf_id = NW_NULL;
    static ID3D11Buffer* s_vsbuf = NW_NULL;
    static ID3D11VertexShader* s_vshd = NW_NULL;
    static ID3D11PixelShader* s_pshd = NW_NULL;
    static ID3D11InputLayout* s_layout = NW_NULL;
    static ID3D11SamplerState* s_fnt_smp = NW_NULL;
    static ID3D11ShaderResourceView* s_fnt_view = NW_NULL;
    static ID3D11RasterizerState* s_rst_state = NW_NULL;
    static ID3D11BlendState* s_blend_state = NW_NULL;
    static ID3D11DepthStencilState* s_dpt_stc_state = NW_NULL;
    static v1s s_vtx_size = 5000, s_idx_size = 10000;
    static cstr vshd_src_code =
        "cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";
    static cstr pshd_src_code =
        "struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";
    // --aux_functions
    v1nil gui_gapi_update_state(ImDrawData* draw_data, context_handle context)
    {
        // setup viewport
        if constexpr (NW_TRUTH) {
            D3D11_VIEWPORT vp;
            memset(&vp, 0, sizeof(D3D11_VIEWPORT));
            vp.Width = draw_data->DisplaySize.x;
            vp.Height = draw_data->DisplaySize.y;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = vp.TopLeftY = 0;
            context->RSSetViewports(1, &vp);
        }
        // setup shader and vertex buffers
        if constexpr (NW_TRUTH) {
            UINT stride_size = sizeof(ImDrawVert);
            UINT offset_size = 0;
            context->IASetInputLayout(s_layout);
            context->IASetVertexBuffers(0, 1, &s_vbuf_id, &stride_size, &offset_size);
            context->IASetIndexBuffer(s_ibuf_id, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            context->VSSetShader(s_vshd, NW_NULL, 0);
            context->VSSetConstantBuffers(0, 1, &s_vsbuf);
            context->PSSetShader(s_pshd, NW_NULL, 0);
            context->PSSetSamplers(0, 1, &s_fnt_smp);
            // in theory we should backup and restore this as well.. very infrequently used..
            context->GSSetShader(NW_NULL, NW_NULL, 0);
            context->HSSetShader(NW_NULL, NW_NULL, 0);
            context->DSSetShader(NW_NULL, NW_NULL, 0);
            context->CSSetShader(NW_NULL, NW_NULL, 0);
            // setup blend state
            v1f blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            context->OMSetBlendState(s_blend_state, blend_factor, 0xffffffff);
            context->OMSetDepthStencilState(s_dpt_stc_state, 0);
            context->RSSetState(s_rst_state);
        }
    }
}
namespace NW
{
    // --core_functions
    v1bit gui_gapi_init(device_handle device, context_handle context)
    {
        if (s_device != NW_NULL || s_context != NW_NULL) { return NW_FALSE; }

        s_device = device; s_context = context;

        // setup backend capabilities flags
        if constexpr (NW_TRUTH) {
            s_imlib_io->BackendRendererName = "nw_gfx_d3d";
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // we can honor the ImDrawCmd::VtxOffset field, allowing for large meshes;
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_RendererHasViewports;  // we can create multi-viewports on the renderer side (optional);
        }
        // initialize platform interface
        if (gui_is_viewp_enabled()) { if (!gui_gapi_wnd_init()) { return NW_FALSE; } }
        // get factory from the device
        if constexpr (NW_TRUTH) {
            IDXGIDevice* dxgi_device = NW_NULL;
            IDXGIAdapter* dxgi_adapter = NW_NULL;
            IDXGIFactory* dxgi_factory = NW_NULL;

            if (device->QueryInterface(IID_PPV_ARGS(&dxgi_device)) == S_OK) {
                if (dxgi_device->GetParent(IID_PPV_ARGS(&dxgi_adapter)) == S_OK) {
                    if (dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory)) == S_OK) {
                        s_device = device;
                        s_context = context;
                        s_factory = dxgi_factory;
                    }
                }
            }
            if (dxgi_device != NW_NULL) { dxgi_device->Release(); }
            if (dxgi_adapter != NW_NULL) { dxgi_adapter->Release(); }
            s_device->AddRef();
            s_context->AddRef();
        }
        // create graphical resources
        if constexpr (NW_TRUTH) {
            // create vertex shader
            if constexpr (NW_TRUTH) {
                ID3DBlob* vshd_bin_code;
                if (FAILED(D3DCompile(
                    vshd_src_code,
                    strlen(vshd_src_code),
                    NW_NULL,
                    NW_NULL,
                    NW_NULL,
                    "main",
                    "vs_4_0",
                    0u,
                    0u,
                    &vshd_bin_code,
                    NW_NULL))
                    ) {
                    return NW_FALSE;
                }
                if (s_device->CreateVertexShader(vshd_bin_code->GetBufferPointer(), vshd_bin_code->GetBufferSize(), NW_NULL, &s_vshd) != S_OK)
                {
                    vshd_bin_code->Release();
                    return NW_FALSE;
                }
                // create the input layout
                D3D11_INPUT_ELEMENT_DESC local_layout[] = {
                    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)IM_OFFSETOF(ImDrawVert, col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                };
                if (s_device->CreateInputLayout(local_layout, 3, vshd_bin_code->GetBufferPointer(), vshd_bin_code->GetBufferSize(), &s_layout) != S_OK)
                {
                    vshd_bin_code->Release();
                    return NW_FALSE;
                }
                vshd_bin_code->Release();
                // create the constant buffer
                {
                    D3D11_BUFFER_DESC desc;
                    desc.ByteWidth = sizeof(vsbuf_m16f);
                    desc.Usage = D3D11_USAGE_DYNAMIC;
                    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    desc.MiscFlags = 0;
                    s_device->CreateBuffer(&desc, NW_NULL, &s_vsbuf);
                }
            }
            // create pixel shader
            if constexpr (NW_TRUTH) {
                ID3DBlob* pshd_bin_code;
                if (FAILED(D3DCompile(
                    pshd_src_code,
                    strlen(pshd_src_code),
                    NW_NULL,
                    NW_NULL,
                    NW_NULL,
                    "main",
                    "ps_4_0",
                    0u,
                    0u,
                    &pshd_bin_code,
                    NW_NULL))
                    ) {
                    return NW_FALSE;
                }
                if (s_device->CreatePixelShader(pshd_bin_code->GetBufferPointer(), pshd_bin_code->GetBufferSize(), NW_NULL, &s_pshd) != S_OK) {
                    pshd_bin_code->Release();
                    return NW_FALSE;
                }
                pshd_bin_code->Release();
            }
            // create blending setup
            if constexpr (NW_TRUTH) {
                D3D11_BLEND_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.AlphaToCoverageEnable = NW_FALSE;
                desc.RenderTarget[0].BlendEnable = NW_TRUTH;
                desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
                desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
                desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
                s_device->CreateBlendState(&desc, &s_blend_state);
            }
            // create rasterizer state
            if constexpr (NW_TRUTH) {
                D3D11_RASTERIZER_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.FillMode = D3D11_FILL_SOLID;
                desc.CullMode = D3D11_CULL_NONE;
                desc.ScissorEnable = NW_TRUTH;
                desc.DepthClipEnable = NW_TRUTH;
                s_device->CreateRasterizerState(&desc, &s_rst_state);
            }
            // create depth-stencil State
            if constexpr (NW_TRUTH) {
                D3D11_DEPTH_STENCIL_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.DepthEnable = NW_FALSE;
                desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
                desc.StencilEnable = NW_FALSE;
                desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                desc.BackFace = desc.FrontFace;
                s_device->CreateDepthStencilState(&desc, &s_dpt_stc_state);
            }
            // create font texture
            if constexpr (NW_TRUTH) {
                ubyte* pixels = NW_NULL;
                v1s width = 0, height = 0;
                s_imlib_io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
                // create texture and it's view
                if constexpr (NW_TRUTH) {
                    D3D11_TEXTURE2D_DESC desc;
                    ZeroMemory(&desc, sizeof(desc));
                    desc.Width = width;
                    desc.Height = height;
                    desc.MipLevels = 1;
                    desc.ArraySize = 1;
                    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                    desc.SampleDesc.Count = 1;
                    desc.Usage = D3D11_USAGE_DEFAULT;
                    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                    desc.CPUAccessFlags = 0;

                    ID3D11Texture2D* txr = NW_NULL;
                    D3D11_SUBRESOURCE_DATA sub_rsc;
                    sub_rsc.pSysMem = pixels;
                    sub_rsc.SysMemPitch = desc.Width * 4;
                    sub_rsc.SysMemSlicePitch = 0;
                    s_device->CreateTexture2D(&desc, &sub_rsc, &txr);

                    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
                    ZeroMemory(&view_desc, sizeof(view_desc));
                    view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                    view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                    view_desc.Texture2D.MipLevels = desc.MipLevels;
                    view_desc.Texture2D.MostDetailedMip = 0;
                    s_device->CreateShaderResourceView(txr, &view_desc, &s_fnt_view);
                    txr->Release();
                }
                // store our identifier
                s_imlib_io->Fonts->TexID = (ImTextureID)s_fnt_view;
                // create texture sampler
                if constexpr (NW_TRUTH) {
                    D3D11_SAMPLER_DESC desc;
                    ZeroMemory(&desc, sizeof(desc));
                    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                    desc.MipLODBias = 0.f;
                    desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
                    desc.MinLOD = 0.f;
                    desc.MaxLOD = 0.f;
                    s_device->CreateSamplerState(&desc, &s_fnt_smp);
                }
            }
        }
        return NW_TRUTH;
    }
    v1bit gui_gapi_quit()
    {
        if (s_device == NW_NULL || s_context == NW_NULL) { return NW_FALSE; }

        if constexpr (NW_TRUTH) {
            if (s_fnt_smp) { s_fnt_smp->Release(); s_fnt_smp = NW_NULL; }
            if (s_fnt_view) { s_fnt_view->Release(); s_fnt_view = NW_NULL; GetIO().Fonts->TexID = NW_NULL; } // we copied s_fnt_viewto io.Fonts->TexID so let's clear that as well.
            if (s_ibuf_id) { s_ibuf_id->Release(); s_ibuf_id = NW_NULL; }
            if (s_vbuf_id) { s_vbuf_id->Release(); s_vbuf_id = NW_NULL; }

            if (s_blend_state) { s_blend_state->Release(); s_blend_state = NW_NULL; }
            if (s_dpt_stc_state) { s_dpt_stc_state->Release(); s_dpt_stc_state = NW_NULL; }
            if (s_rst_state) { s_rst_state->Release(); s_rst_state = NW_NULL; }
            if (s_pshd) { s_pshd->Release(); s_pshd = NW_NULL; }
            if (s_vsbuf) { s_vsbuf->Release(); s_vsbuf = NW_NULL; }
            if (s_layout) { s_layout->Release(); s_layout = NW_NULL; }
            if (s_vshd) { s_vshd->Release(); s_vshd = NW_NULL; }
        }
        if (s_factory != NW_NULL) { s_factory->Release(); s_factory = NW_NULL; }
        if (s_device != NW_NULL) { s_device->Release(); s_device = NW_NULL; }
        if (s_context != NW_NULL) { s_context->Release(); s_context = NW_NULL; }
        // shutdown platform interface
        if (gui_is_viewp_enabled()) { gui_gapi_wnd_quit(); }

        return NW_TRUTH;
    }
    v1nil gui_gapi_update(ImDrawData* draw_data)
    {
        // avoid rendering when minimized
        if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f) { return; }

        ID3D11DeviceContext* context = s_context;

        // Create and grow vertex/index buffers if needed
        if (!s_vbuf_id || s_vtx_size < draw_data->TotalVtxCount)
        {
            if (s_vbuf_id) { s_vbuf_id->Release(); s_vbuf_id = NW_NULL; }
            s_vtx_size = draw_data->TotalVtxCount + 5000;
            D3D11_BUFFER_DESC desc;
            memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.ByteWidth = s_vtx_size * sizeof(ImDrawVert);
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            if (s_device->CreateBuffer(&desc, NW_NULL, &s_vbuf_id) < 0) { return; }
        }
        if (!s_ibuf_id || s_idx_size < draw_data->TotalIdxCount)
        {
            if (s_ibuf_id) { s_ibuf_id->Release(); s_ibuf_id = NW_NULL; }
            s_idx_size = draw_data->TotalIdxCount + 10000;
            D3D11_BUFFER_DESC desc;
            memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.ByteWidth = s_idx_size * sizeof(ImDrawIdx);
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            if (s_device->CreateBuffer(&desc, NW_NULL, &s_ibuf_id) < 0) { return; }
        }

        // upload vertex/index data into a single contiguous gpu buffer
        D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
        if (context->Map(s_vbuf_id, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK) { return; }
        if (context->Map(s_ibuf_id, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK) { return; }
        ImDrawVert* vtx_dst = (ImDrawVert*)vtx_resource.pData;
        ImDrawIdx* idx_dst = (ImDrawIdx*)idx_resource.pData;
        for (int n = 0; n < draw_data->CmdListsCount; n++) {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
            vtx_dst += cmd_list->VtxBuffer.Size;
            idx_dst += cmd_list->IdxBuffer.Size;
        }
        context->Unmap(s_vbuf_id, 0);
        context->Unmap(s_ibuf_id, 0);

        // setup orthographic projection matrix into our constant buffer
        // our visible imgui space lies from draw_data->DisplayPos (top left) to
        // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
        {
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            if (context->Map(s_vsbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK) { return; }
            vsbuf_m16f* constant_buffer = (vsbuf_m16f*)mapped_resource.pData;
            v1f Lt = draw_data->DisplayPos.x;
            v1f Rt = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
            v1f Tp = draw_data->DisplayPos.y;
            v1f Bt = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
            v1f mdl_view_proj[4][4] = {
                { 2.0f / (Rt - Lt),      0.0f,                  0.0f, 0.0f },
                { 0.0f,                  2.0f / (Tp - Bt),      0.0f, 0.0f },
                { 0.0f,                  0.0f,                  0.5f, 0.0f },
                { (Rt + Lt) / (Lt - Rt), (Tp + Bt) / (Bt - Tp), 0.5f, 1.0f },
            };
            memcpy(&constant_buffer->mdl_view_proj, mdl_view_proj, sizeof(mdl_view_proj));
            context->Unmap(s_vsbuf, 0);
        }

        // backup directx state that will be modified to restore it afterwards
        // (unfortunately this is very ugly looking and verbose; close your eyes!)
        dx_context_state last_state;
        // setup desired dx state
        gui_gapi_update_state(draw_data, context);
        // render command lists
        // because we merged all buffers into a single one, we maintain our own offset into them)
        v1s global_idx_offset = 0;
        v1s global_vtx_offset = 0;
        ImVec2 clip_off = draw_data->DisplayPos;
        for (int n = 0; n < draw_data->CmdListsCount; n++) {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback != NW_NULL) {
                    // user callback, registered via ImDrawList::AddCallback()
                    // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState) { gui_gapi_update_state(draw_data, context); }
                    else { pcmd->UserCallback(cmd_list, pcmd); }
                }
                else {
                    // apply scissor/clipping ImGuiRect(angle
                    const D3D11_RECT r = { (LONG)(pcmd->ClipRect.x - clip_off.x), (LONG)(pcmd->ClipRect.y - clip_off.y), (LONG)(pcmd->ClipRect.z - clip_off.x), (LONG)(pcmd->ClipRect.w - clip_off.y) };
                    context->RSSetScissorRects(1, &r);
                    // bind texture, draw
                    ID3D11ShaderResourceView* gfx_txrsrv = (ID3D11ShaderResourceView*)pcmd->TextureId;
                    context->PSSetShaderResources(0, 1, &gfx_txrsrv);
                    context->DrawIndexed(pcmd->ElemCount, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset);
                }
            }
            global_idx_offset += cmd_list->IdxBuffer.Size;
            global_vtx_offset += cmd_list->VtxBuffer.Size;
        }
    }
}
namespace NW
{
    // --wnd_core_functions
    v1bit gui_gapi_wnd_init()
    {
        if (s_imsys_io == NW_NULL) { return NW_FALSE; }
        
        s_imsys_io->Renderer_CreateWindow = gui_gapi_wnd_create;
        s_imsys_io->Renderer_DestroyWindow = gui_gapi_wnd_delete;
        s_imsys_io->Renderer_RenderWindow = gui_gapi_wnd_render;
        s_imsys_io->Renderer_SwapBuffers = gui_gapi_wnd_swap;
        s_imsys_io->Renderer_SetWindowSize = gui_gapi_wnd_set_size;

        return NW_TRUTH;
    }
    v1nil gui_gapi_wnd_quit()
    {
        if (s_imsys_io == NW_NULL) { return; }
        
        DestroyPlatformWindows();
    }
    v1nil gui_gapi_wnd_create(ImGuiViewport* viewport)
    {
        dx_viewport_data* data = IM_NEW(dx_viewport_data)();
        viewport->RendererUserData = data;
        // platformHandleRaw should always be a HWND, whereas PlatformHandle might be a higher-level handle (e.g. GLFWWindow*, SDL_Window*).
        // Some back-end will leave PlatformHandleRaw NW_NULL, in which case we assume PlatformHandle will contain the HWND.
        HWND hwnd = viewport->PlatformHandleRaw ? (HWND)viewport->PlatformHandleRaw : (HWND)viewport->PlatformHandle;
        IM_ASSERT(hwnd != NW_NULL);
        // create swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferDesc.Width = (UINT)viewport->Size.x;
        sd.BufferDesc.Height = (UINT)viewport->Size.y;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = hwnd;
        sd.Windowed = NW_TRUTH;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        IM_ASSERT(data->SwapChain == NW_NULL && data->RTView == NW_NULL);
        s_factory->CreateSwapChain(s_device, &sd, &data->SwapChain);

        // create the render target
        if (data->SwapChain) {
            ID3D11Texture2D* pBackBuffer;
            data->SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            s_device->CreateRenderTargetView(pBackBuffer, NW_NULL, &data->RTView);
            pBackBuffer->Release();
        }
    }
    v1nil gui_gapi_wnd_delete(ImGuiViewport* viewport)
    {
        // the main viewport (owned by the application) will always have RendererUserData == NW_NULL;
        // since we didn't create the data for it.
        if (dx_viewport_data* data = (dx_viewport_data*)viewport->RendererUserData) {
            if (data->SwapChain) { data->SwapChain->Release(); }
            data->SwapChain = NW_NULL;
            if (data->RTView) { data->RTView->Release(); }
            data->RTView = NW_NULL;
            IM_DELETE(data);
        }
        viewport->RendererUserData = NW_NULL;
    }
    // --wnd_graphics
    v1nil gui_gapi_wnd_render(ImGuiViewport* viewport, ptr)
    {
        dx_viewport_data* data = (dx_viewport_data*)viewport->RendererUserData;
        ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        s_context->OMSetRenderTargets(1, &data->RTView, NW_NULL);
        if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear)) {
            s_context->ClearRenderTargetView(data->RTView, (float*)&clear_color);
        }
        gui_gapi_update(viewport->DrawData);
    }
    v1nil gui_gapi_wnd_swap(ImGuiViewport* viewport, ptr draw_arg)
    {
        dx_viewport_data* data = reinterpret_cast<dx_viewport_data*>(viewport->RendererUserData);
        // present without vsync
        data->SwapChain->Present(0u, 0u);
    }
    // wnd_setters
    v1nil gui_gapi_wnd_set_size(ImGuiViewport* viewport, ImVec2 size)
    {
        dx_viewport_data* data = (dx_viewport_data*)viewport->RendererUserData;
        if (data->RTView) {
            data->RTView->Release();
            data->RTView = NW_NULL;
        }
        if (data->SwapChain) {
            ID3D11Texture2D* pBackBuffer = NW_NULL;
            data->SwapChain->ResizeBuffers(0, (UINT)size.x, (UINT)size.y, DXGI_FORMAT_UNKNOWN, 0);
            data->SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            if (pBackBuffer == NW_NULL) { fprintf(stderr, "Dx11SetWindowSize() failed creating buffers.\n"); return; }
            s_device->CreateRenderTargetView(pBackBuffer, NW_NULL, &data->RTView);
            pBackBuffer->Release();
        }
    }
}
#   endif   // GAPI_D3D
#endif	// NW_GAPI