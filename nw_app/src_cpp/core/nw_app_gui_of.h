#ifndef NW_APP_GUI_OF_H
#define NW_APP_GUI_OF_H
#include "nw_app_core.hpp"
#if (defined NW_API)
namespace NW
{
	/// abstract gui_of struct
	class NW_API a_gui_of
	{
	public:
		a_gui_of();
		virtual ~a_gui_of();
		// --getters
		virtual inline cstr_t get_name() const = 0;
		// --setters
		v1nil set_enabled(v1b enable);
		// --predicates
		inline v1b is_enabled() const	{ return m_is_enabled; }
		// --core_methods
		virtual v1nil on_draw() = 0;
		v1nil draw_checkbox();
	protected:
		v1b m_is_enabled = false;
	};
}
namespace NW
{
	/// gui_of_app_engine class
	/// description:
	/// --renders the core engine state and window gui
	class NW_API gui_of_app_engine : public a_gui_of
	{
	public:
		gui_of_app_engine();
		~gui_of_app_engine();
		// --getters
		virtual inline cstr_t get_name() const override { return "gui_of_app_engine"; }
		// --core_methods
		virtual v1nil on_draw() override;
	private:
	};
	/// gui_of_graphics_engine class
	class NW_API gui_of_gfx_engine : public a_gui_of
	{
	public:
		gui_of_gfx_engine();
		~gui_of_gfx_engine();
		// --getters
		virtual inline cstr_t get_name() const override { return "gui_of_gfx_engine"; }
		// --core_methods
		virtual v1nil on_draw() override;
	private:
	};
	/// gui_of_console_engine class
	class NW_API gui_of_cmd_engine : public a_gui_of
	{
	public:
		gui_of_cmd_engine();
		~gui_of_cmd_engine();
		// --getters
		virtual inline cstr_t get_name() const override { return "gui_of_cmd_engine"; }
		// --core_methods
		virtual v1nil on_draw() override;
	};
}
namespace NW
{
	/// gui_of_io_sys class
	class NW_API gui_of_io_sys : public a_gui_of
	{
	public:
		gui_of_io_sys();
		~gui_of_io_sys();
		// --getters
		virtual inline cstr_t get_name() const override { return "gui_of_io_sys"; }
		// --core_methods
		virtual v1nil on_draw() override;
	};
}
namespace NW
{
	/// gui_of_graphics_material class
	class NW_API gui_of_material_editor : public a_gui_of
	{
		using context = mem_ref<gfx_mtl>;
	public:
		gui_of_material_editor();
		// --getters
		virtual inline cstr_t get_name() const override { return "gui_of_material_editor"; }
		// --setters
		v1nil set_context(mem_ref<gfx_mtl>& ref);
		// --core_methods
		virtual v1nil on_draw() override;
	private:
		context m_context;
	};
	/// gui_of_sprite_editor class
	struct NW_API gui_of_sprite_editor : public a_gui_of
	{
		using context = mem_ref<a_gfx_txr>;
	public:
		gui_of_sprite_editor();
		// --getters
		virtual inline cstr_t get_name() const override { return "gui_of_sprite_editor"; }
		// --setters
		v1nil set_context(context& ref);
		// --core_methods
		virtual v1nil on_draw() override;
	private:
		context m_context;
		char_t m_context_name[128];
	};
}
#endif	// NW_API
#endif	// NW_APP_GUI_OF_H
