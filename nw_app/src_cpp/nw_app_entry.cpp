#include "nw_app_pch.hpp"

#define NW_APP_LAUNCH_TEST     1 << 0
#define NW_APP_LAUNCH_ENGINE   1 << 1
#define NW_APP_LAUNCH_CONSOLE  1 << 2
#define NW_APP_LAUNCH          NW_APP_LAUNCH_TEST

#include "nw_app.hpp"

template<typename vtype, NW::csize mat_size_x, NW::csize mat_size_y>
NW::mat_t<vtype, mat_size_x - 1u, mat_size_y - 1u> static constexpr make_erase(NW::mat_t<vtype, mat_size_x, mat_size_y>& matrix, NW::v1u erase_y, NW::v1u erase_x) {
	using namespace NW;
	mat_t<vtype, mat_size_x - 1u, mat_size_y - 1u> result(static_cast<vtype>(0));
	for (v1u iy = 0u; iy < mat_size_y; iy++) {
		for (v1u ix = 0u; ix < mat_size_x; ix++) {
			if (iy < erase_y) {
				if (ix < erase_x) {
					result[iy - 0u][ix - 0u] = matrix[iy + 0u][ix + 0u];
				}
				else if (ix > erase_x) {
					result[iy - 0u][ix - 1u] = matrix[iy + 0u][ix + 0u];
				}
			}
			else if (iy > erase_y) {
				if (ix < erase_x) {
					result[iy - 1u][ix - 0u] = matrix[iy + 0u][ix + 0u];
				}
				else if (ix > erase_x) {
					result[iy - 1u][ix - 1u] = matrix[iy + 0u][ix + 0u];
				}
			}
		}
	}
	return result;
}
template<typename vtype, NW::csize mat_size_x, NW::csize mat_size_y>
vtype static constexpr get_deter(NW::mat_t<vtype, mat_size_x, mat_size_y>& matrix) {
	using namespace NW;
	vtype result = static_cast<vtype>(0);
	if constexpr (mat_size_x == 1u && mat_size_y == 1u) {
		result = matrix[0][0];
	}
	else if constexpr (mat_size_x == 2u && mat_size_y == 2u) {
		result = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
	}
	else if constexpr (mat_size_x > 2u && mat_size_y > 2u) {
		v1u sign = +1; // -1 power(itr)
		for (v1u itr = 0u; itr < mat_size_x; itr++) {
			result += sign * matrix[0u][itr] * get_deter((make_erase(matrix, 0u, itr)));
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
		//game.new_state<NW::gui_core_state>();
		game.new_state<NW::gfx_core_state>();
		if (game.get_run_thread()->joinable()) { game.get_run_thread()->join(); }
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_CONSOLE)
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_TEST)
#	if true
		NW::m3f mtx = NW::m3f(0.0f);
		mtx[0][0] = 2.0f;
		mtx[0][1] = 4.0f;
		mtx[0][2] = 3.0f;
		mtx[1][0] = 5.0f;
		mtx[1][1] = 7.0f;
		mtx[1][2] = 8.0f;
		mtx[2][0] = 6.0f;
		mtx[2][1] = 9.0f;
		mtx[2][2] = 1.0f;
		std::cout << "matrix" << NW_STR_EOL << mtx;
		std::cout << "matrix determinant" << NW_STR_EOL << get_deter(mtx);
#	endif
#	if false
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
	catch (NW::a_error& exc) { std::cout << exc; }
	catch (std::exception& exc) { NW_ERR(exc.what()); }
	catch (...) { NW_ERR("somethig went wrong"); }

	return 0;
}