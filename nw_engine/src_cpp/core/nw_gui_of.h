#ifndef NW_GUI_OF_H
#define NW_GUI_OF_H
#include <nw_core.hpp>
#include <lua/lua_engine.h>
#if (defined NW_GAPI)
#	include <gui/imgui_core.hpp>
#	include <gui/imgui_internal.h>
#	include <gui/nwg_imgui.h>
#	define GUI_DEFAULT_TREE_FLAGS ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
#endif	// NW_GAPI
namespace NW
{
	/// abstract gui_of struct
	class NW_API a_gui_of
	{
	public:
		bit m_is_enabled = false;
	public:
		a_gui_of();
		virtual ~a_gui_of();
		// --getters
		virtual inline cstring get_name() const = 0;
		// --core_methods
		virtual void on_draw() = 0;
	};
}
namespace NW
{
	/// gui_of_core_engine class
	/// description:
	/// --renders the core engine state and window gui
	class NW_API gui_of_core_engine : public a_gui_of
	{
	public:
		gui_of_core_engine();
		~gui_of_core_engine();
		// --getters
		virtual inline cstring get_name() const override { return "gui_of_core_engine"; }
		// --core_methods
		virtual void on_draw() override;
	};
	/// gui_of_graphics_engine class
	class NW_API gui_of_gfx_engine : public a_gui_of
	{
	public:
		gui_of_gfx_engine();
		~gui_of_gfx_engine();
		// --getters
		virtual inline cstring get_name() const override { return "gui_of_gfx_engine"; }
		// --core_methods
		virtual void on_draw() override;
	private:
	};
	/// gui_of_console_engine class
	class NW_API gui_of_cmd_engine : public a_gui_of
	{
	public:
		gui_of_cmd_engine();
		~gui_of_cmd_engine();
		// --getters
		virtual inline cstring get_name() const override { return "gui_of_cmd_engine"; }
		// --core_methods
		virtual void on_draw() override;
	};
}
namespace NW
{
	/// gui_of_data_sys class
	class NW_API gui_of_data_sys : public a_gui_of
	{
	public:
		gui_of_data_sys();
		~gui_of_data_sys();
		// --getters
		virtual inline cstring get_name() const override { return "gui_of_data_sys"; }
		// --core_methods
		virtual void on_draw() override;
	private:
		schar m_next_dir[256]{ 0 };
		schar m_curr_dir[256]{ 0 };
	};
}
namespace NW
{
	/// gui_of_graphics_material class
	class NW_API gui_of_material_editor : public a_gui_of
	{
	public:
		gui_of_material_editor();
		// --getters
		virtual inline cstring get_name() const override { return "gui_of_material_editor"; }
		// --setters
		void set_context(mem_ref<gfx_material>& ref);
		// --core_methods
		virtual void on_draw() override;
	private:
		mem_ref<gfx_material> m_context;
	};
	/// gui_of_sprite_editor class
	struct NW_API gui_of_sprite_editor : public a_gui_of
	{
	public:
		gui_of_sprite_editor();
		// --getters
		virtual inline cstring get_name() const override { return "gui_of_sprite_editor"; }
		// --setters
		void set_context(mem_ref<a_texture>& ref);
		// --core_methods
		virtual void on_draw() override;
	private:
		mem_ref<a_texture> m_context;
		schar m_context_name[128];
	};
}
#endif	// NW_GUI_OF_H