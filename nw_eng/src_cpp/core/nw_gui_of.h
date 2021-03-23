#ifndef NW_GUI_OF_H
#define NW_GUI_OF_H
#include <nw_core.hpp>
#include <lua/lua_engine.h>
namespace NW
{
	/// abstract gui_of struct
	class NW_API a_gui_of
	{
	public:
		a_gui_of();
		virtual ~a_gui_of();
		// --getters
		virtual inline cstr get_name() const = 0;
		// --setters
		void set_enabled(bit enable);
		// --predicates
		inline bit is_enabled() const	{ return m_is_enabled; }
		// --core_methods
		virtual void on_draw() = 0;
		void draw_checkbox();
	protected:
		bit m_is_enabled = false;
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
		gui_of_core_engine(core_engine& core);
		~gui_of_core_engine();
		// --getters
		virtual inline cstr get_name() const override { return "gui_of_core_engine"; }
		// --core_methods
		virtual void on_draw() override;
	private:
		core_engine* m_core;
	};
	/// gui_of_graphics_engine class
	class NW_API gui_of_gfx_engine : public a_gui_of
	{
	public:
		gui_of_gfx_engine(gfx_engine& graphics);
		~gui_of_gfx_engine();
		// --getters
		virtual inline cstr get_name() const override { return "gui_of_gfx_engine"; }
		// --core_methods
		virtual void on_draw() override;
	private:
		gfx_engine* m_gfx;
	};
	/// gui_of_console_engine class
	class NW_API gui_of_cmd_engine : public a_gui_of
	{
	public:
		gui_of_cmd_engine();
		~gui_of_cmd_engine();
		// --getters
		virtual inline cstr get_name() const override { return "gui_of_cmd_engine"; }
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
		virtual inline cstr get_name() const override { return "gui_of_data_sys"; }
		// --core_methods
		virtual void on_draw() override;
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
		virtual inline cstr get_name() const override { return "gui_of_material_editor"; }
		// --setters
		void set_context(mem_ref<gfx_mtl>& ref);
		// --core_methods
		virtual void on_draw() override;
	private:
		context m_context;
	};
	/// gui_of_sprite_editor class
	struct NW_API gui_of_sprite_editor : public a_gui_of
	{
		using context = mem_ref<a_txr>;
	public:
		gui_of_sprite_editor();
		// --getters
		virtual inline cstr get_name() const override { return "gui_of_sprite_editor"; }
		// --setters
		void set_context(context& ref);
		// --core_methods
		virtual void on_draw() override;
	private:
		context m_context;
		schar m_context_name[128];
	};
}
#endif	// NW_GUI_OF_H