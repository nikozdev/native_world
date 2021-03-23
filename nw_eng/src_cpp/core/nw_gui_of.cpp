#include <nw_pch.hpp>
#include "nw_gui_of.h"
#pragma warning(disable:4312)
#include <core/nw_engine.h>
namespace NW
{
	a_gui_of::a_gui_of() { }
	a_gui_of::~a_gui_of() { }
	// --setters
	void a_gui_of::set_enabled(bit enable) {
		m_is_enabled = enable;
	}
	// --core_methods
	void a_gui_of::draw_checkbox()
	{
		GUI::Checkbox(get_name(), &m_is_enabled);
	}
}
// --==<gui_of_engines>==--
namespace NW
{
	// --==<gui_of_core_engine>==--
	gui_of_core_engine::gui_of_core_engine(core_engine& core) :
		m_core(&core)
	{
	}
	gui_of_core_engine::~gui_of_core_engine() { }
	// --setters
	// --core_methods
	void gui_of_core_engine::on_draw() {
		if (!m_is_enabled) { return; }
		GUI::Begin("core_engine", &m_is_enabled);
		GUI::Text("updates per second: %f;", m_core->get_timer()->get_ups());
		GUI::Separator();
		if (GUI::TreeNodeEx("entity_system", GUI_DEFAULT_TREE_FLAGS)) {
			for (auto& ietab : ent_sys::get().get_ent_reg()) {
				if (GUI::TreeNodeEx(&ietab, GUI_DEFAULT_TREE_FLAGS, "type_id: %d", ietab.first)) {
					for (auto& ient : ietab.second) {
						if (GUI::TreeNodeEx(&ient, GUI_DEFAULT_TREE_FLAGS, "ent_id: %d", ient.first)) {
							GUI::TreePop();	// ent_id
						}
					}
					GUI::TreePop();	// type_id
				}
			}
			GUI::TreePop();	// entities
		}
		GUI::Separator();
		if (GUI::TreeNodeEx("component_system", GUI_DEFAULT_TREE_FLAGS)) {
			for (auto& ictab : cmp_sys::get().get_cmp_reg()) {
				if (GUI::TreeNodeEx(&ictab, GUI_DEFAULT_TREE_FLAGS, "type_id: %d", ictab.first)) {
					for (auto& icmp : ictab.second) {
						if (GUI::TreeNodeEx(&icmp, GUI_DEFAULT_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							GUI::TreePop();	// cmp_id
						}
					}
					GUI::TreePop();	// type_id
				}
			}
			GUI::TreePop();	// components
		}
		GUI::Separator();
		
		GUI::End();
	}
	// --==</gui_ofcore_engine>==--
	// --==<gui_of_graphics_engine>==--
	gui_of_gfx_engine::gui_of_gfx_engine(gfx_engine& graphics) :
		m_gfx(&graphics)
	{
	}
	gui_of_gfx_engine::~gui_of_gfx_engine() { }
	// --setters
	// --core_methods
	void gui_of_gfx_engine::on_draw() {
		if (!m_is_enabled) { return; }
		GUI::Begin("graphics_engine", &m_is_enabled);
		
		if (GUI::TreeNodeEx("graphics_engine", GUI_DEFAULT_TREE_FLAGS)) {
			if (GUI::TreeNodeEx("entities", GUI_DEFAULT_TREE_FLAGS)) {
				for (auto& ients : m_gfx->get_ent_reg()) {
					if (GUI::TreeNodeEx(&ients, GUI_DEFAULT_TREE_FLAGS, "type_id: %d", ients.first)) {
						for (auto& ient : ients.second) {
							if (GUI::TreeNodeEx(&ient, GUI_DEFAULT_TREE_FLAGS, "ent_id: %d", ient.first)) {
								GUI::TreePop();	// ent_id
							}
						}
						GUI::TreePop();	// type_id
					}
				}
				GUI::TreePop();	// entities
			}
			if (GUI::TreeNodeEx("components", GUI_DEFAULT_TREE_FLAGS)) {
				for (auto& icmps : m_gfx->get_cmp_reg()) {
					if (GUI::TreeNodeEx(&icmps, GUI_DEFAULT_TREE_FLAGS, "type_id: %d", icmps.first)) {
						for (auto& icmp : icmps.second) {
							if (GUI::TreeNodeEx(&icmp, GUI_DEFAULT_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
								GUI::TreePop();
							}
						}
						GUI::TreePop();	// type_id
					}
				}
				GUI::TreePop();	// components
			}
			GUI::TreePop();	// gfx_engine
		}
		GUI::Separator();

		GUI::End();
	}
	// --==</gui_of_graphics_enigne>==--

	// --==<gui_ofcmd_engine>==--
	gui_of_cmd_engine::gui_of_cmd_engine() { }
	gui_of_cmd_engine::~gui_of_cmd_engine() { }
	// --core_methods
	void gui_of_cmd_engine::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("console_engine");
		GUI::End();
	}
	// --==</gui_ofcmd_engine>==--
}
// --==</gui_of_engines>==--
// --==<gui_of_systems>==--
namespace NW
{
	// --==<gui_of_data_sys>==--
	gui_of_data_sys::gui_of_data_sys()
	{
	}
	gui_of_data_sys::~gui_of_data_sys() { }
	// --core_methods
	void gui_of_data_sys::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("data_system", &m_is_enabled, GWF_MenuBar);

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("files")) {
				if (GUI::MenuItem("new...", "ctrl+n")) {
				}
				if (GUI::MenuItem("save_as...", "ctrl+s")) {
				}
				if (GUI::MenuItem("load...", "ctrl+l")) {
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}
#if false
		if (GUI::TreeNodeEx("data_system", GUI_DEFAULT_TREE_FLAGS)) {
			for (auto& icmps : data_sys::get_registry()) {
				if (GUI::TreeNodeEx(&icmps, GUI_DEFAULT_TREE_FLAGS, "type_id: %d", icmps.first)) {
					for (auto& icmp : icmps.second) {
						if (GUI::TreeNodeEx(&icmp, GUI_DEFAULT_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							GUI::TreePop();
						}
					}
					GUI::TreePop();
				}
			}
			GUI::TreePop();
		}
#endif

		GUI::End();
	}
	// --==</gui_ofdata_sys>==--
}
// --==</gui_of_systems>==--
// --==<gui_of_editors>==--
namespace NW
{
	// --==<gui_of_material_editor>==--
	gui_of_material_editor::gui_of_material_editor() { }
	// --setters
	void gui_of_material_editor::set_context(context& ref) {
		m_context.set_ref(ref);
		if (ref.is_valid()) {
			m_is_enabled = true;
		}
		else {
			m_is_enabled = false;
		}
	}
	// --core_methods
	void gui_of_material_editor::on_draw()
	{
		GUI::Begin("shd_editor", &m_is_enabled, GWF_MenuBar);
		if (!m_context.is_valid()) { GUI::End(); return; }
		GUI::End();
	}
	// --==</gui_of_material_editor>==--

	// --==<gui_of_sprite_editor>==--
	gui_of_sprite_editor::gui_of_sprite_editor() { }
	// --setters
	void gui_of_sprite_editor::set_context(context& ref) {
		m_context.set_ref(ref);
		if (!ref.is_valid()) {
			m_is_enabled = false;
		}
		else {
			m_is_enabled = true;
		}
	}
	// --core_methods
	void gui_of_sprite_editor::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("sprite_editor", &m_is_enabled, GWF_MenuBar);

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("file")) {
				if (GUI::MenuItem("save...")) {
				}
				else if (GUI::MenuItem("load...")) {
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}
		if (!m_context.is_valid()) { GUI::End(); return; }

		GUI::Image(reinterpret_cast<imgui_txr_id>(m_context->get_handle()), { 64.0f, 64.0f });

		GUI::End();
	}
	// --==</gui_ofSpriteEditor>==--
}
// --==</gui_of_editors>==--