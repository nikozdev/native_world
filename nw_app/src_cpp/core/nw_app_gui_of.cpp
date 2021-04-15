#include "nw_app_pch.hpp"
#include "nw_app_gui_of.h"
#if (defined NW_API)
#include "core/nw_app_engine.h"
#pragma warning(disable : 4312) // "reinterpret_cast' : conversion from "type" to "type" of greater size
namespace NW
{
	a_gui_of::a_gui_of() { }
	a_gui_of::~a_gui_of() { }
	// --setters
	void a_gui_of::set_enabled(v1b enable) {
		m_is_enabled = enable;
	}
	// --core_methods
	void a_gui_of::draw_checkbox()
	{
		NW_GUI::Checkbox(get_name(), &m_is_enabled);
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
		NW_GUI::Begin("core_engine", &m_is_enabled);
		NW_GUI::Text("updates per second: %f;", m_core->get_timer()->get_ups());
		
		NW_GUI::Columns(2, nullptr, NW_TRUE);
		if (NW_GUI::TreeNodeEx("entity_system", NW_GUI_TREE_FLAGS)) {
			for (auto& ietab : ent_sys::get().get_ent_reg()) {
				if (NW_GUI::TreeNodeEx(&ietab, NW_GUI_TREE_FLAGS, "type_id: %d", ietab.first)) {
					for (auto& ient : ietab.second) {
						if (NW_GUI::TreeNodeEx(&ient, NW_GUI_TREE_FLAGS, "ent_id: %d", ient.first)) {
							NW_GUI::TreePop();	// ent_id
						}
					}
					NW_GUI::TreePop();	// type_id
				}
			}
			NW_GUI::TreePop();	// entities
		}
		NW_GUI::NextColumn();
		if (NW_GUI::TreeNodeEx("component_system", NW_GUI_TREE_FLAGS)) {
			for (auto& ictab : cmp_sys::get().get_cmp_reg()) {
				if (NW_GUI::TreeNodeEx(&ictab, NW_GUI_TREE_FLAGS, "type_id: %d", ictab.first)) {
					for (auto& icmp : ictab.second) {
						if (NW_GUI::TreeNodeEx(&icmp, NW_GUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NW_GUI::TreePop();	// cmp_id
						}
					}
					NW_GUI::TreePop();	// type_id
				}
			}
			NW_GUI::TreePop();	// components
		}
		NW_GUI::Separator();
		
		NW_GUI::End();
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
		NW_GUI::Begin("graphics_engine", &m_is_enabled);
		
		if (NW_GUI::TreeNodeEx("graphics_engine", NW_GUI_TREE_FLAGS)) {
			NW_GUI::Columns(2, nullptr, NW_TRUE);
			if (NW_GUI::TreeNodeEx("entities", NW_GUI_TREE_FLAGS)) {
				for (auto& ients : m_gfx->get_ent_reg()) {
					if (NW_GUI::TreeNodeEx(&ients, NW_GUI_TREE_FLAGS, "type_id: %d", ients.first)) {
						for (auto& ient : ients.second) {
							if (NW_GUI::TreeNodeEx(&ient, NW_GUI_TREE_FLAGS, "ent_id: %d", ient.first)) {
								NW_GUI::TreePop();	// ent_id
							}
						}
						NW_GUI::TreePop();	// type_id
					}
				}
				NW_GUI::TreePop();	// entities
			}
			NW_GUI::NextColumn();
			if (NW_GUI::TreeNodeEx("components", NW_GUI_TREE_FLAGS)) {
				for (auto& icmps : m_gfx->get_cmp_reg()) {
					if (NW_GUI::TreeNodeEx(&icmps, NW_GUI_TREE_FLAGS, "type_id: %d", icmps.first)) {
						for (auto& icmp : icmps.second) {
							if (NW_GUI::TreeNodeEx(&icmp, NW_GUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
								NW_GUI::TreePop();
							}
						}
						NW_GUI::TreePop();	// type_id
					}
				}
				NW_GUI::TreePop();	// components
			}
			NW_GUI::TreePop();	// gfx_engine
		}
		NW_GUI::Separator();

		NW_GUI::End();
	}
	// --==</gui_of_graphics_enigne>==--

	// --==<gui_ofcmd_engine>==--
	gui_of_cmd_engine::gui_of_cmd_engine() { }
	gui_of_cmd_engine::~gui_of_cmd_engine() { }
	// --core_methods
	void gui_of_cmd_engine::on_draw() {
		if (!m_is_enabled) return;
		NW_GUI::Begin("console_engine");
		NW_GUI::End();
	}
	// --==</gui_ofcmd_engine>==--
}
// --==</gui_of_engines>==--
// --==<gui_of_systems>==--
namespace NW
{
	// --==<gui_of_io_sys>==--
	gui_of_io_sys::gui_of_io_sys()
	{
	}
	gui_of_io_sys::~gui_of_io_sys() { }
	// --core_methods
	void gui_of_io_sys::on_draw() {
		if (!m_is_enabled) return;
		NW_GUI::Begin("io_system", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NW_GUI::BeginMenuBar()) {
			if (NW_GUI::BeginMenu("files")) {
				if (NW_GUI::MenuItem("new...", "ctrl+n")) {
				}
				if (NW_GUI::MenuItem("save_as...", "ctrl+s")) {
				}
				if (NW_GUI::MenuItem("load...", "ctrl+l")) {
				}
				NW_GUI::EndMenu();
			}
			NW_GUI::EndMenuBar();
		}
#if false
		if (NW_GUI::TreeNodeEx("io_system", NW_GUI_TREE_FLAGS)) {
			for (auto& icmps : io_sys::get_registry()) {
				if (NW_GUI::TreeNodeEx(&icmps, NW_GUI_TREE_FLAGS, "type_id: %d", icmps.first)) {
					for (auto& icmp : icmps.second) {
						if (NW_GUI::TreeNodeEx(&icmp, NW_GUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NW_GUI::TreePop();
						}
					}
					NW_GUI::TreePop();
				}
			}
			NW_GUI::TreePop();
		}
#endif

		NW_GUI::End();
	}
	// --==</gui_ofio_sys>==--
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
			m_is_enabled = NW_TRUE;
		}
		else {
			m_is_enabled = false;
		}
	}
	// --core_methods
	void gui_of_material_editor::on_draw()
	{
		NW_GUI::Begin("shd_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);
		if (!m_context.is_valid()) { NW_GUI::End(); return; }
		NW_GUI::End();
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
			m_is_enabled = NW_TRUE;
		}
	}
	// --core_methods
	void gui_of_sprite_editor::on_draw() {
		if (!m_is_enabled) return;
		NW_GUI::Begin("sprite_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NW_GUI::BeginMenuBar()) {
			if (NW_GUI::BeginMenu("file")) {
				if (NW_GUI::MenuItem("save...")) {
				}
				else if (NW_GUI::MenuItem("load...")) {
				}
				NW_GUI::EndMenu();
			}
			NW_GUI::EndMenuBar();
		}
		if (!m_context.is_valid()) { NW_GUI::End(); return; }

		NW_GUI::Image(reinterpret_cast<ImTextureID>(m_context->get_handle()), { 64.0f, 64.0f });

		NW_GUI::End();
	}
	// --==</gui_ofSpriteEditor>==--
}
// --==</gui_of_editors>==--
#endif	// NW_API