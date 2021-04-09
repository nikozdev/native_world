#include "nw_app_pch.hpp"

#define NW_APP_LAUNCH_TEST     1 << 0
#define NW_APP_LAUNCH_ENGINE   1 << 1
#define NW_APP_LAUNCH_CONSOLE  1 << 2
#define NW_APP_LAUNCH          NW_APP_LAUNCH_TEST

#include "nw_app.hpp"

template<typename vtype, NW::csize mat_size_x, NW::csize mat_size_y>
vtype static constexpr get_deter(const NW::mat_t<vtype, mat_size_x, mat_size_y>& matrix) {
	using namespace NW;
	vtype result = static_cast<vtype>(0);
	if constexpr (mat_size_x == 1u && mat_size_y == 1u) {
		result = matrix[0][0];
	}
	else if constexpr (mat_size_x == 2u && mat_size_y == 2u) {
		result = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
	}
	else if constexpr (mat_size_x > 2u && mat_size_y > 2u) {
		v1s sign = +1; // -1 power(itr)
		for (v1u itr = 0u; itr < mat_size_x; itr++) {
			result += sign * matrix[0u][itr] * get_deter(matrix.erase(0u, itr));
			sign = -sign;
		}
	}
	else if constexpr (mat_size_x < 1 || mat_size_y < 1) {
		static_assert(mat_size_x >= 1 && mat_size_y >= 1, "cannot find determinant!");
	}
	return result;
}

int main(int nof_arguments, char* arguments[])
{
	try {
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_ENGINE)
		NW::core_engine game = NW::core_engine("native_world");
		game.run();
		game.new_state<NW::gui_core_state>();
		// game.new_state<NW::gfx_core_state>();
		if (game.get_run_thread()->joinable()) { game.get_run_thread()->join(); }
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_CONSOLE)
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_TEST)
#	if (NW_TRUE)
		// build a structure
		unsigned char root_buf[2 << 12]{ NW_NULL };
		NW::mem_table root;
		if constexpr (NW_TRUE) {
			// create elements
			auto& materials = root.add_elem<NW::mem_array>("materials").get<NW::mem_array>();
			materials.set_elems<NW::mem_table>(3u);
			auto& colors = root.add_elem<NW::mem_array>("colors").get<NW::mem_array>();
			colors.set_elems<NW::mem_array>(3u);
			// configure elements
			for (NW::v1s ei = 0u; ei < materials.get_count(); ei++) {
				auto& material = materials[ei].get<NW::mem_table>();
				material.add_elem<NW::v4u*>("color_ref");
				material.add_elem<NW::mem_array>("diffuse").get<NW::mem_array>().set_elems<NW::v1u>(4u);
				// colors
				auto& color = colors[ei].get<NW::mem_array>();
				color.set_elems<NW::v1u>(4u);
			}
			// and now the game begins
			NW_CHECK(root.remake(root_buf), "failed to build a layout", return -1);
			// itterate throught every material
			for (NW::v1s ei = 0u; ei < materials.get_count(); ei++) {
				colors[ei][0u] = NW::get_rand<NW::v1u>(0x00, 0xff);
				colors[ei][1u] = NW::get_rand<NW::v1u>(0x00, 0xff);
				colors[ei][2u] = NW::get_rand<NW::v1u>(0x00, 0xff);
				colors[ei][3u] = NW::get_rand<NW::v1u>(0x00, 0xff);
				materials[ei]["color_ref"] = reinterpret_cast<NW::v4u*>(colors[ei].get_data());
				materials[ei]["diffuse"][0u] = colors[ei][0u].get<NW::v1u>();
				materials[ei]["diffuse"][1u] = colors[ei][1u].get<NW::v1u>();
				materials[ei]["diffuse"][2u] = colors[ei][2u].get<NW::v1u>();
				materials[ei]["diffuse"][3u] = colors[ei][3u].get<NW::v1u>();
			}
		}
#	endif
#	if (NW_FALSE)
		STARTUPINFO spInfo{ 0 };
		PROCESS_INFORMATION pcInfo{ 0 };
		if (!CreateProcess(
			LR"(F:\dev\lua_jit\lua_jit.exe)",
			//&(reinterpret_cast<TCHAR*>(R"(F:\dev\lua_jit\lua_jit.exe)"))[0],
			NULL,		// cmd arguments
			NULL,		// process is not inheritable
			NULL,		// not inherit the handle
			FALSE,		// handle inheritance to false
			0,			// no creation flags
			NULL,		// parrent's environment
			NULL,		// parent's directory
			&spInfo,
			&pcInfo
		)) {
			throw(NW::error("failed to create a process", ::GetLastError()));
		}
		WaitForSingleObject(pcInfo.hProcess, INFINITE);
		CloseHandle(pcInfo.hProcess);
		CloseHandle(pcInfo.hThread);
#	endif
#endif
	}
	catch (NW::a_error& exc) { NW_ERROR(exc.get_str(), return -1); }
	catch (std::exception& exc) { NW_ERROR(exc.what(), return -1); }
	catch (...) { NW_ERROR("somethig went wrong", return -1); }

	return 0;
}