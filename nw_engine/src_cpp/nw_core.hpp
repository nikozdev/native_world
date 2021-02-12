#ifndef NW_CORE_HPP
#define NW_CORE_HPP
/// This is the main header of the engine
/// Here are all relevant macros and configurations

// --==<linking>==--
#if defined NW_LINK_DYNAMIC
#ifdef NW_BUILD_DLL
#define NW_API __declspec(dllexport)
#elif defined NW_BUILD_EXE
#define NW_API __declspec(dllimport)
#endif
#elif defined NW_LINK_STATIC
#define NW_API
#endif
// --==</linking>==--

// --==<configurations>==--
// --window_choice
#define NW_WAPI_GLFW	1 << 1
#define NW_WAPI_WIN		1 << 2
#define NW_WAPI			NW_WAPI_WIN

// --graphics_api_choice
#define NW_GAPI_OGL		1 << 1
#define NW_GAPI_DX		1 << 2
#define NW_GAPI			NW_GAPI_DX

// --gui_library_choice
#define NW_GUI_NATIVE	1 << 1
#define NW_GUI_IMGUI	1 << 2
#define NW_GUI			NW_GUI_IMGUI | NW_GUI_NATIVE

#define NW_GLOBAL_MEMORY	1 << 24
// --==</configurations>==--

// --==</support_macroses>==--

// --==<pragmas>==--
#pragma warning(disable : 4005)
#pragma warning(disable : 4081)
#pragma warning(disable : 4099)
#pragma warning(disable : 4102)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4552)
#pragma warning(disable : 4996)
// --==</pragmas>==--

#include <nw_pch.hpp>

#if (NW_WAPI & NW_WAPI_GLFW)
// --==<keyboard_glfw>==--
#define GLFW_KEY_UNKNOWN            -1
// --keys_to_print
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39
#define GLFW_KEY_COMMA              44
#define GLFW_KEY_MINUS              45
#define GLFW_KEY_PERIOD             46
#define GLFW_KEY_SLASH              47
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59
#define GLFW_KEY_EQUAL              61
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91 
#define GLFW_KEY_BACKSLASH          92 
#define GLFW_KEY_RIGHT_BRACKET      93 
#define GLFW_KEY_GRAVE_ACCENT       96 
#define GLFW_KEY_WORLD_1            161
#define GLFW_KEY_WORLD_2            162
// --keys_to_control
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348

#define GLFW_KEY_LAST               GLFW_KEY_MENU
// --==</keyborad_glfw>==--

// --==<keyboard_nw>==--
// --keys_to_print
#define NW_KEY_FIRST	             32
#define NW_KEY_SPACE_32              GLFW_KEY_SPACE             
#define NW_KEY_APOSTROPHE_39         GLFW_KEY_APOSTROPHE        
#define NW_KEY_COMMA_44              GLFW_KEY_COMMA             
#define NW_KEY_MINUS_45              GLFW_KEY_MINUS             
#define NW_KEY_PERIOD_46             GLFW_KEY_PERIOD            
#define NW_KEY_SLASH_47              GLFW_KEY_SLASH             
#define NW_KEY_0_48                  GLFW_KEY_0                 
#define NW_KEY_1_49                  GLFW_KEY_1                 
#define NW_KEY_2_50                  GLFW_KEY_2                 
#define NW_KEY_3_51                  GLFW_KEY_3                 
#define NW_KEY_4_52                  GLFW_KEY_4                 
#define NW_KEY_5_53                  GLFW_KEY_5                 
#define NW_KEY_6_54                  GLFW_KEY_6                 
#define NW_KEY_7_55                  GLFW_KEY_7                 
#define NW_KEY_8_56                  GLFW_KEY_8                 
#define NW_KEY_9_57                  GLFW_KEY_9                 
#define NW_KEY_SEMICOLON_59          GLFW_KEY_SEMICOLON         
#define NW_KEY_EQUAL_61              GLFW_KEY_EQUAL             
#define NW_KEY_A_65                  GLFW_KEY_A                 
#define NW_KEY_B_66                  GLFW_KEY_B                 
#define NW_KEY_C_67                  GLFW_KEY_C                 
#define NW_KEY_D_68                  GLFW_KEY_D                 
#define NW_KEY_E_69                  GLFW_KEY_E                 
#define NW_KEY_F_70                  GLFW_KEY_F                 
#define NW_KEY_G_71                  GLFW_KEY_G                 
#define NW_KEY_H_72                  GLFW_KEY_H                 
#define NW_KEY_I_73                  GLFW_KEY_I                 
#define NW_KEY_J_74                  GLFW_KEY_J                 
#define NW_KEY_K_75                  GLFW_KEY_K                 
#define NW_KEY_L_76                  GLFW_KEY_L                 
#define NW_KEY_M_77                  GLFW_KEY_M                 
#define NW_KEY_N_78                  GLFW_KEY_N                 
#define NW_KEY_O_79                  GLFW_KEY_O                 
#define NW_KEY_P_80                  GLFW_KEY_P                 
#define NW_KEY_Q_81                  GLFW_KEY_Q                 
#define NW_KEY_R_82                  GLFW_KEY_R                 
#define NW_KEY_S_83                  GLFW_KEY_S                 
#define NW_KEY_T_84                  GLFW_KEY_T                 
#define NW_KEY_U_85                  GLFW_KEY_U                 
#define NW_KEY_V_86                  GLFW_KEY_V                 
#define NW_KEY_W_87                  GLFW_KEY_W                 
#define NW_KEY_X_88                  GLFW_KEY_X                 
#define NW_KEY_Y_89                  GLFW_KEY_Y                 
#define NW_KEY_Z_90                  GLFW_KEY_Z                 
#define NW_KEY_LEFT_BRACKET_91       GLFW_KEY_LEFT_BRACKET      
#define NW_KEY_BACKSLASH_92          GLFW_KEY_BACKSLASH         
#define NW_KEY_RIGHT_BRACKET_93      GLFW_KEY_RIGHT_BRACKET     
#define NW_KEY_GRAVE_ACCENT_96       GLFW_KEY_GRAVE_ACCENT      
#define NW_KEY_WORLD_1_161           GLFW_KEY_WORLD_1           
#define NW_KEY_WORLD_2_162           GLFW_KEY_WORLD_2           
// keys_to_control
#define NW_KEY_ESCAPE_27          GLFW_KEY_ESCAPE             
#define NW_KEY_BACKSPACE_8        GLFW_KEY_BACKSPACE          
#define NW_KEY_HTAB_9             GLFW_KEY_TAB                
#define NW_KEY_ENTER_10           GLFW_KEY_ENTER              
#define NW_KEY_INS_45             GLFW_KEY_INSERT             
#define NW_KEY_DEL_46             GLFW_KEY_DELETE             
#define NW_KEY_LEFT_37            GLFW_KEY_LEFT               
#define NW_KEY_RIGHT_39           GLFW_KEY_RIGHT              
#define NW_KEY_DOWN_40            GLFW_KEY_DOWN               
#define NW_KEY_UP_38              GLFW_KEY_UP                 
#define NW_KEY_PAGE_UP            GLFW_KEY_PAGE_UP            
#define NW_KEY_PAGE_DOWN          GLFW_KEY_PAGE_DOWN          
#define NW_KEY_HOME               GLFW_KEY_HOME               
#define NW_KEY_END                GLFW_KEY_END                
#define NW_KEY_CAPS_LOCK          GLFW_KEY_CAPS_LOCK          
#define NW_KEY_SCROLL_LOCK        GLFW_KEY_SCROLL_LOCK        
#define NW_KEY_NUM_LOCK           GLFW_KEY_NUM_LOCK           
#define NW_KEY_PRINT_SCREEN       GLFW_KEY_PRINT_SCREEN       
#define NW_KEY_PAUSE              GLFW_KEY_PAUSE              
#define NW_KEY_F1                 GLFW_KEY_F1                 
#define NW_KEY_F2                 GLFW_KEY_F2                 
#define NW_KEY_F3                 GLFW_KEY_F3                 
#define NW_KEY_F4                 GLFW_KEY_F4                 
#define NW_KEY_F5                 GLFW_KEY_F5                 
#define NW_KEY_F6                 GLFW_KEY_F6                 
#define NW_KEY_F7                 GLFW_KEY_F7                 
#define NW_KEY_F8                 GLFW_KEY_F8                 
#define NW_KEY_F9                 GLFW_KEY_F9                 
#define NW_KEY_F10                GLFW_KEY_F10                
#define NW_KEY_F11                GLFW_KEY_F11                
#define NW_KEY_F12                GLFW_KEY_F12                
#define NW_KEY_F13                GLFW_KEY_F13                
#define NW_KEY_F14                GLFW_KEY_F14                
#define NW_KEY_F15                GLFW_KEY_F15                
#define NW_KEY_F16                GLFW_KEY_F16                
#define NW_KEY_F17                GLFW_KEY_F17                
#define NW_KEY_F18                GLFW_KEY_F18                
#define NW_KEY_F19                GLFW_KEY_F19                
#define NW_KEY_F20                GLFW_KEY_F20                
#define NW_KEY_F21                GLFW_KEY_F21                
#define NW_KEY_F22                GLFW_KEY_F22                
#define NW_KEY_F23                GLFW_KEY_F23                
#define NW_KEY_F24                GLFW_KEY_F24                
#define NW_KEY_F25                GLFW_KEY_F25                
#define NW_KEY_KP_0               GLFW_KEY_KP_0               
#define NW_KEY_KP_1               GLFW_KEY_KP_1               
#define NW_KEY_KP_2               GLFW_KEY_KP_2               
#define NW_KEY_KP_3               GLFW_KEY_KP_3               
#define NW_KEY_KP_4               GLFW_KEY_KP_4               
#define NW_KEY_KP_5               GLFW_KEY_KP_5               
#define NW_KEY_KP_6               GLFW_KEY_KP_6               
#define NW_KEY_KP_7               GLFW_KEY_KP_7               
#define NW_KEY_KP_8               GLFW_KEY_KP_8               
#define NW_KEY_KP_9               GLFW_KEY_KP_9               
#define NW_KEY_KP_DECIMAL         GLFW_KEY_KP_DECIMAL         
#define NW_KEY_KP_DIVIDE          GLFW_KEY_KP_DIVIDE          
#define NW_KEY_KP_MULTIPLY        GLFW_KEY_KP_MULTIPLY        
#define NW_KEY_KP_SUBTRACT        GLFW_KEY_KP_SUBTRACT        
#define NW_KEY_KP_ADD             GLFW_KEY_KP_ADD             
#define NW_KEY_KP_ENTER_13           GLFW_KEY_KP_ENTER
#define NW_KEY_KP_EQUAL           GLFW_KEY_KP_EQUAL
#define NW_KEY_SHIFT_LEFT         GLFW_KEY_LEFT_SHIFT
#define NW_KEY_SHIFT_RIGHT        GLFW_KEY_RIGHT_SHIFT
#define NW_KEY_CONTROL_LEFT       GLFW_KEY_LEFT_CONTROL
#define NW_KEY_CONTROL_RIGHT      GLFW_KEY_RIGHT_CONTROL
#define NW_KEY_ALT_LEFT           GLFW_KEY_LEFT_ALT
#define NW_KEY_ALT_RIGHT          GLFW_KEY_RIGHT_ALT
#define NW_KEY_LEFT_SUPER         GLFW_KEY_LEFT_SUPER
#define NW_KEY_RIGHT_SUPER        GLFW_KEY_RIGHT_SUPER
#define NW_KEY_MENU               GLFW_KEY_MENU

#define NW_KEY_LAST					GLFW_KEY_LAST
#define NW_KEY_COUNT				NW_KEY_LAST
// --==</keyboard_nw>==--

// --==<mouse_glfw>==--
#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
// --==</mouse_glfw>==--

// --==<mouse_nw>==--
#define NW_MS_BTN_0         GLFW_MOUSE_BUTTON_1
#define NW_MS_BTN_1         GLFW_MOUSE_BUTTON_2
#define NW_MS_BTN_2         GLFW_MOUSE_BUTTON_3
#define NW_MS_BTN_3         GLFW_MOUSE_BUTTON_4
#define NW_MS_BTN_4         GLFW_MOUSE_BUTTON_5
#define NW_MS_BTN_5         GLFW_MOUSE_BUTTON_6
#define NW_MS_BTN_6         GLFW_MOUSE_BUTTON_7
#define NW_MS_BTN_7         GLFW_MOUSE_BUTTON_8
#define NW_MS_BTN_LAST      GLFW_MOUSE_BUTTON_8
#define NW_MS_BTN_LEFT      GLFW_MOUSE_BUTTON_1
#define NW_MS_BTN_RIGHT     GLFW_MOUSE_BUTTON_2
#define NW_MS_BTN_MIDDLE    GLFW_MOUSE_BUTTON_3
#define NW_MS_BTN_COUNT     GLFW_MOUSE_BUTTON_7
// --==</mouse_nw>==--

// --==<events_nw>==--
#define GLFW_RELEASE 0
#define GLFW_PRESS 1
#define GLFW_REPEAT 2

#define NW_RELEASE GLFW_RELEASE
#define NW_PRESS GLFW_PRESS
#define NW_REPEAT GLFW_REPEAT

#define NW_MS_PRESS_LEFT 0
#define NW_MS_PRESS_RIGHT 1
#define NW_MS_PRESS_2 2
#define NW_MS_PRESS_3 3
#define NW_MS_PRESS_4 4

#define NW_MS_PRESS_DOUBLE 0

#define NW_MS_MOVED 1
#define NW_MS_WHEELED 2
#define NW_MS_HWHEELED 3
// --==</events_nw>==--

#endif // NW_WAPI
#if (NW_WAPI & NW_WAPI_WIN)
// ========<key_codes_cmd>========
#define NW_KEY_NONE				0
#define NW_KEY_FIRST			32
// printable_keys
#define NW_KEY_SPACE_32			32
#define NW_KEY_APOSTROPHE_39	39
#define NW_KEY_COMMA_44			44
#define NW_KEY_MINUS_45			45
#define NW_KEY_PERIOD_46		46
#define NW_KEY_APOSTROPHE_39	39
#define NW_KEY_COMMA_44			44
#define NW_KEY_MINUS_45			45
#define NW_KEY_PERIOD_46		46
#define NW_KEY_SLASH_47			47
#define NW_KEY_0_48				48
#define NW_KEY_1_49				49
#define NW_KEY_2_50				50
#define NW_KEY_3_51				51
#define NW_KEY_4_52				52
#define NW_KEY_5_53				53
#define NW_KEY_6_54				54
#define NW_KEY_7_55				55
#define NW_KEY_8_56				56
#define NW_KEY_9_57				57
#define NW_KEY_SEMICOLON_59		59
#define NW_KEY_EQUAL_61			61
// alphabet_upper_case
#define NW_KEY_A_65				65
#define NW_KEY_B_66				66
#define NW_KEY_C_67				67
#define NW_KEY_D_68				68
#define NW_KEY_E_69				69
#define NW_KEY_F_70				70
#define NW_KEY_G_71				71
#define NW_KEY_H_72				72
#define NW_KEY_I_73				73
#define NW_KEY_J_74				74
#define NW_KEY_K_75				75
#define NW_KEY_L_76				76
#define NW_KEY_M_77				77
#define NW_KEY_N_78				78
#define NW_KEY_O_79				79
#define NW_KEY_P_80				80
#define NW_KEY_Q_81				81
#define NW_KEY_R_82				82
#define NW_KEY_S_83				83
#define NW_KEY_T_84				84
#define NW_KEY_U_85				85
#define NW_KEY_V_86				86
#define NW_KEY_W_87				87
#define NW_KEY_X_88				88
#define NW_KEY_Y_89				89
#define NW_KEY_Z_90				90
// additional
#define NW_KEY_LEFT_BRACKET_91	91
#define NW_KEY_BACKSLASH_92		92
#define NW_KEY_RBRACKET_93		93
// alphabet_lower_case
#define NW_KEY_a_97				97
#define NW_KEY_b_98				98
#define NW_KEY_c_99				99
#define NW_KEY_d_100			100
#define NW_KEY_e_101			101
#define NW_KEY_f_102			102
#define NW_KEY_g_103			103
#define NW_KEY_h_104			104
#define NW_KEY_i_105			105
#define NW_KEY_j_106			106
#define NW_KEY_k_107			107
#define NW_KEY_l_108			108
#define NW_KEY_m_109			109
#define NW_KEY_n_110			110
#define NW_KEY_o_111			111
#define NW_KEY_p_112			112
#define NW_KEY_q_113			113
#define NW_KEY_r_114			114
#define NW_KEY_s_115			115
#define NW_KEY_t_116			116
#define NW_KEY_u_117			117
#define NW_KEY_v_118			118
#define NW_KEY_w_119			119
#define NW_KEY_x_120			120
#define NW_KEY_y_121			121
#define NW_KEY_z_122			122
// functional_keys
#define NW_KEY_ESCAPE_27		27
#define NW_KEY_BACKSPACE_8		8
#define NW_KEY_HTAB_9			9
#define NW_KEY_ENTER_13			13
#define NW_KEY_HTAB_11			11
#define NW_KEY_INS_45			45            
#define NW_KEY_DEL_46			46
#define NW_KEY_ENTER_13			13
#define NW_KEY_HTAB_11			11
#define NW_KEY_INS_45			45            
#define NW_KEY_DEL_46			46
#define NW_KEY_RIGHT_38			39
#define NW_KEY_LEFT_37			37
#define NW_KEY_DOWN_40			40
#define NW_KEY_UP_38			38
#define NW_KEY_F1				' '
#define NW_KEY_F2				' '
#define NW_KEY_F3				' '
#define NW_KEY_F4				' '
#define NW_KEY_F5				' '
#define NW_KEY_F6				' '
#define NW_KEY_F7				' '
#define NW_KEY_F8				' '
#define NW_KEY_F9				' '
#define NW_KEY_F10				' '
#define NW_KEY_F11				' '
#define NW_KEY_F12				' '
#define NW_KEY_KP_0				' '
#define NW_KEY_KP_1				' '
#define NW_KEY_KP_2				' '
#define NW_KEY_KP_3				' '
#define NW_KEY_KP_4				' '
#define NW_KEY_KP_5				' '
#define NW_KEY_KP_6				' '
#define NW_KEY_KP_7				' '
#define NW_KEY_KP_8				' '
#define NW_KEY_KP_9				' '
#define NW_KEY_KP_DECIMAL		' '
#define NW_KEY_KP_DIVIDE		' '
#define NW_KEY_KP_MULTIPLY		' '
#define NW_KEY_KP_SUBTRACT		' '
#define NW_KEY_KP_ADD			' '
#define NW_KEY_KP_ENTER_13		' '
#define NW_KEY_KP_EQUAL			' '
#define NW_KEY_SHIFT_LEFT		16
#define NW_KEY_SHIFT_RIGHT		16
#define NW_KEY_CONTROL_LEFT		17
#define NW_KEY_CONTROL_RIGHT	17
#define NW_KEY_ALT_LEFT			18
#define NW_KEY_ALT_RIGHT		18

#define NW_KEY_LAST				256
#define NW_KEY_COUNT			NW_KEY_LAST
// ========</key_codes_cmd>========

// ========<device_codes_cmd>========
#define NW_MS_BTN_0		0
#define NW_MS_BTN_1		1
#define NW_MS_BTN_2		2
#define NW_MS_BTN_3		3
#define NW_MS_BTN_4		4
#define NW_MS_BTN_5		5
#define NW_MS_BTN_6		6
#define NW_MS_BTN_7		7
#define NW_MS_BTN_LAST		NW_MS_BTN_7
#define NW_MS_BTN_LEFT		NW_MS_BTN_0
#define NW_MS_BTN_RIGHT	NW_MS_BTN_1
#define NW_MS_BTN_MIDDLE	NW_MS_BTN_2
#define NW_MS_BTN_LAST		NW_MS_BTN_7
#define NW_MS_BTN_COUNT	5
// ========</device_codes_cmd>========

// ========<event_types_cmd>========
#define NW_BUTTON_RELEASE	0
#define NW_BUTTON_PRESS		1

#define NW_MS_PRESS_LEFT	0x0001
#define NW_MS_PRESS_RIGHT	0x0002
#define NW_MS_PRESS_2		0x0004
#define NW_MS_PRESS_3		0x0008
#define NW_MS_PRESS_4		0x0010

#define NW_MS_PRESS_DOUBLE	0x0002

#define NW_MS_MOVED			0x0001
#define NW_MS_WHEELED		0x0004
#define NW_MS_HWHEELED		0x0008

#define NW_KEY_EVT			0x0001
#define NW_MS_EVT			0x0002
#define NW_WND_SIZE_EVT		0x0004
#define NW_MENU_EVT			0x0008
#define NW_FOCUS_EVT			0x0010
// ========</event_types_cmd>========

#define NW_NO_SELECTION            0x0000
#define NW_SELECTION_IN_PROGRESS   0x0001   // selection has begun
#define NW_SELECTION_NOT_EMPTY     0x0002   // non-null select rectangle
#define NW_MOUSE_SELECTION         0x0004   // selecting with mouse
#define NW_MOUSE_DOWN              0x0008   // mouse is down
#endif	// NW_WAPI

// --debug_macro
#if (NW_GAPI & NW_GAPI_OGL)
#if (defined NW_DEBUG)
// --opengl_debug
namespace NW
{
	extern inline void OglClearErr();
	extern inline bool OglErrLog(const char* funcName, const char* file, int line);
}
#define NW_OGL_CALL(function) OglClearErr();\
		function;\
		NWL_ASSERT(OglErrLog(#function, NWL_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
#else
#define NW_OGL_CALL(function);
#endif // NW_DEBUG
#endif // NW_GAPI
#if (NW_GAPI & NW_GAPI_DX)
#endif // NW_GAPI
// --==</support_macroses>==--

#include <nw_pch.hpp>

#if (NW_GAPI & NW_GAPI_OGL)
// --==<data_types_ogl>==--
#define GL_BOOL					0x8B56
#define GL_BYTE					0x1400
#define GL_UNSIGNED_BYTE		0x1401
#define GL_SHORT				0x1402
#define GL_UNSIGNED_SHORT		0x1403
#define GL_INT					0x1404
#define GL_UNSIGNED_INT			0x1405
#define GL_FLOAT				0x1406
#define GL_DOUBLE				0x140A
#define GL_SAMPLER_1D			0x8B5D
#define GL_UNSIGNED_INT_24_8	0x84FA
// --==</data_types_ogl>==--

// --==<data_types_nw>==--
#define NW_ZERO			0
#define NW_ONE			1
#define NW_BOOL			GL_BOOL
#define NW_INT16		GL_SHORT
#define NW_UINT16		GL_UNSIGNED_SHORT
#define NW_INT8			GL_BYTE
#define NW_UINT8		GL_UNSIGNED_BYTE
#define NW_INT32		GL_INT
#define NW_UINT32		GL_UNSIGNED_INT
#define NW_FLOAT32		GL_FLOAT
#define NW_FLOAT64		GL_DOUBLE
#define NW_FLOAT32_VEC2	(NW_FLOAT32 + 0x0020)
#define NW_FLOAT32_VEC3	(NW_FLOAT32 + 0x0030)
#define NW_FLOAT32_VEC4	(NW_FLOAT32 + 0x0040)
#define NW_FLOAT32_MAT2	(NW_FLOAT32 + 0x0100)
#define NW_FLOAT32_MAT3	(NW_FLOAT32 + 0x0200)
#define NW_FLOAT32_MAT4	(NW_FLOAT32 + 0x0300)
#define NW_SAMPLER_1D	GL_SAMPLER_1D
#define NW_UINT24_8		GL_UNSIGNED_INT_24_8
// --==</data_types_nw>==--

// --==<draw_modes_ogl>==--
#define GL_POINT			0x1B00
#define GL_LINE				0x1B01
#define GL_FILL				0x1B02
// --primitives
#define GL_POINTS			0x0000
#define GL_LINES			0x0001
#define GL_LINE_STRIP		0x0003
#define GL_LINE_LOOP		0x0002
#define GL_TRIANGLES		0x0004
#define GL_TRIANGLE_STRIP	0x0005
#define GL_TRIANGLE_FAN		0x0006
#define GL_QUADS			0x0007
#define GL_QUAD_STRIP		0x0008
#define GL_POLYGON			0x0009
// --==</draw_modes_ogl>==--

// --==<draw_modes_nw>==--
#define NW_POINT			0x1B00
#define NW_LINE				0x1B01
#define NW_FILL				0x1B02
// --primitives
#define NW_POINTS			0x0000
#define NW_LINES			0x0001
#define NW_LINE_STRIP		0x0003
#define NW_LINE_LOOP		0x0002
#define NW_TRIANGLES		0x0004
#define NW_TRIANGLE_STRIP	0x0005
#define NW_TRIANGLE_FAN		0x0006
#define NW_QUADS			0x0007
#define NW_QUAD_STRIP		0x0008
#define NW_POLYGON			0x0009
// --==</draw_modes_GFX>==--

// --==<planes_ogl>==--
#define GL_FRONT			0x0404
#define GL_BACK				0x0405
#define GL_LEFT				0x0406
#define GL_RIGHT			0x0407
#define GL_FRONT_AND_BACK	0x0408
// --==</planes_ogl>==--
// --==<planes_nw>==--
#define NW_FRONT			GL_FRONT
#define NW_BACK				GL_BACK
#define NW_LEFT				GL_LEFT
#define NW_RIGHT			GL_RIGHT
#define NW_FRONT_AND_BACK	GL_FRONT_AND_BACK
// --==</planes_nw>==--

// --==<shaders_ogl>==--
#define GL_SHADER				0x82E1
#define GL_PROGRAM				0x82E2
#define GL_FRAGMENT_SHADER		0x8B30
#define GL_GEOMETRY_SHADER		0x8DD9
#define GL_VERTEX_SHADER		0x8B31
// --==<shaders_ogl>==--

// --==<shaders_nw>==--
#define NW_SHADER				GL_PROGRAM
#define NW_SHADER_VERTEX		GL_VERTEX_SHADER
#define NW_SHADER_GEOMETRY		GL_GEOMETRY_SHADER
#define NW_SHADER_PIXEL			GL_FRAGMENT_SHADER
// --==</shaders_nw>==--

// --==<configurations_ogl>==--
#define GL_POINT_SIZE			0x0B11
#define GL_LINE_SMOOTH			0x0B20
#define GL_LINE_WIDTH			0x0B21

#define GL_CULL_FACE			0x0B44
#define GL_CULL_FACE_MODE		0x0B45
#define GL_FRONT_FACE			0x0B46
#define GL_CW					0x0900
#define GL_CCW					0x0901

#define GL_DEPTH_TEST			0x0B71
#define GL_DEPTH_FUNC			0x0B74

#define GL_STENCIL_TEST			0x0B90
#define GL_STENCIL_FUNC			0x0B92

#define GL_BLEND				0x0BE2
#define GL_BLEND_DST			0x0BE0
#define GL_BLEND_SRC			0x0BE1
#define GL_SRC_COLOR			0x0300
#define GL_ONE_MINUS_SRC_COLOR	0x0301
#define GL_SRC_ALPHA			0x0302
#define GL_ONE_MINUS_SRC_ALPHA	0x0303
#define GL_DST_ALPHA			0x0304
#define GL_ONE_MINUS_DST_ALPHA	0x0305
#define GL_DST_COLOR			0x0306
#define GL_ONE_MINUS_DST_COLOR	0x0307
#define GL_SRC_ALPHA_SATURATE	0x0308

#define GL_MULTISAMPLE			0x809D

#define GL_KEEP					0x1E00
#define GL_REPLACE				0x1E01
#define GL_INCR					0x1E02
#define GL_DECR					0x1E03
// --==</configurations_ogl>==--

// --==<configurations_nw>==--
#define NW_POINT_SIZE				GL_POINT_SIZE
#define NW_LINE_SMOOTH				GL_LINE_SMOOTH
#define NW_LINE_WIDTH				GL_LINE_WIDTH

#define NW_CULL_FACE				GL_CULL_FACE
#define NW_CULL_FACE_MODE			GL_CULL_FACE_MODE
#define NW_FRONT_FACE				GL_FRONT_FACE
#define NW_CW						GL_CW
#define NW_CCW						GL_CCW

#define NW_DEPTH_TEST				GL_DEPTH_TEST
#define NW_DEPTH_FUNC				GL_DEPTH_FUNC

#define NW_STENCIL_TEST			GL_STENCIL_TEST
#define NW_STENCIL_FUNC			GL_STENCIL_FUNC

#define NW_BLEND					GL_BLEND
#define NW_BLEND_DST				GL_BLEND_DST
#define NW_BLEND_SRC				GL_BLEND_SRC
#define NW_SRC_COLOR				GL_SRC_COLOR
#define NW_ONE_MINUS_SRC_COLOR		GL_ONE_MINUS_SRC_COLOR
#define NW_SRC_ALPHA				GL_SRC_ALPHA
#define NW_ONE_MINUS_SRC_ALPHA		GL_ONE_MINUS_SRC_ALPHA
#define NW_DST_ALPHA				GL_DST_ALPHA
#define NW_ONE_MINUS_DST_ALPHA		GL_ONE_MINUS_DST_ALPHA
#define NW_DST_COLOR				GL_DST_COLOR
#define NW_ONE_MINUS_DST_COLOR		GL_ONE_MINUS_DST_COLOR
#define NW_SRC_ALPHA_SATURATE		GL_SRC_ALPHA_SATURATE

#define NW_MULTISAMPLE			GL_MULTISAMPLE

#define NW_KEEP					GL_KEEP
#define NW_REPLACE				GL_REPLACE
#define NW_INCR					GL_INCR
#define NW_DECR					GL_DECR
// --==</configurations_nw>==--

// --==<expressions_ogl>==--
// --constants
#define GL_NONE		0
#define GL_FALSE	0
#define GL_TRUE		1
#define GL_ZERO		0
#define GL_ONE		1
// --conditions
#define GL_NEVER		0x0200
#define GL_LESS			0x0201
#define GL_EQUAL		0x0202
#define GL_LEQUAL		0x0203
#define GL_GREATER		0x0204
#define GL_NOTEQUAL		0x0205
#define GL_GEQUAL		0x0206
#define GL_ALWAYS		0x0207
// --logic
#define GL_AND				0x1501
#define GL_AND_REVERSE		0x1502
#define GL_COPY				0x1503
#define GL_AND_INVERTED		0x1504
#define GL_NOOP				0x1505
#define GL_XOR				0x1506
#define GL_OR				0x1507
#define GL_NOR				0x1508
#define GL_EQUIV			0x1509
#define GL_INVERT			0x150A
#define GL_OR_REVERSE		0x150B
#define GL_COPY_INVERTED	0x150C
#define GL_OR_INVERTED		0x150D
#define GL_NAND				0x150E
// --==</expressions_ogl>==--

// --==<expressions_nw>==--
// --constants
#define NW_NONE			0
#define NW_FALSE		0
#define NW_TRUE			1
// --conditions
#define NW_NEVER		0x0200
#define NW_LESS			0x0201
#define NW_EQUAL		0x0202
#define NW_LEQUAL		0x0203
#define NW_GREATER		0x0204
#define NW_NOTEQUAL		0x0205
#define NW_GEQUAL		0x0206
#define NW_ALWAYS		0x0207
// --logic
#define NW_AND				0x1501
#define NW_AND_REVERSE		0x1502
#define NW_COPY				0x1503
#define NW_AND_INVERTED		0x1504
#define NW_NOOP				0x1505
#define NW_XOR				0x1506
#define NW_OR				0x1507
#define NW_NOR				0x1508
#define NW_EQUIV			0x1509
#define NW_INVERT			0x150A
#define NW_OR_REVERSE		0x150B
#define NW_COPY_INVERTED	0x150C
#define NW_OR_INVERTED		0x150D
#define NW_NAND				0x150E
// --==</expressions_nw>==--

// --==<pixel_formats_ogl>==--
#define GL_RED				0x1903
#define GL_GREEN			0x1904
#define GL_BLUE				0x1905
#define GL_ALPHA			0x1906
#define GL_RGB				0x1907
#define GL_RGBA				0x1908
#define GL_RED_INTEGER		0x8D94
#define GL_GREEN_INTEGER	0x8D95
#define GL_BLUE_INTEGER		0x8D96
#define GL_RGB_INTEGER		0x8D98
#define GL_RGBA_INTEGER		0x8D99
#define GL_R8UI				0x8232
#define GL_R16I				0x8233
#define GL_R16UI			0x8234
#define GL_R32I				0x8235
#define GL_R32UI			0x8236
#define GL_RG8I				0x8237
#define GL_RGBA8			0x8058
#define GL_DEPTH			0x1801
#define GL_STENCIL			0x1802
#define GL_DEPTH24_STENCIL8	0x88F0
// --==</pixel_formats_ogl>==--

// --==<pixel_formats_nw>==--
#define NW_RED				GL_RED
#define NW_GREEN			GL_GREEN
#define NW_BLUE				GL_BLUE
#define NW_ALPHA			GL_ALPHA
#define NW_RGB				GL_RGB
#define NW_RGBA				GL_RGBA
#define NW_RED_INT			GL_RED_INTEGER
#define NW_GREEN_INT		GL_GREEN_INTEGER
#define NW_BLUE_INT			GL_BLUE_INTEGER
#define NW_RGB_INT			GL_RGB_INTEGER
#define NW_RGBA_INT			GL_RGBA_INTEGER
#define NW_DEPTH			GL_DEPTH
#define NW_STENCIL			GL_STENCIL
#define NW_RED_INT32		GL_R32I
#define NW_RED_UINT32		GL_R32UI
#define NW_RGBA8			GL_RGBA8
#define NW_DEPTH24			GL_DEPTH
#define NW_STENCIL8			GL_STENCIL
#define NW_DEPTH24_STENCIL8	GL_DEPTH24_STENCIL8
// --==</pixel_formats_nw>==--

// --==<textures_ogl>==--
#define GL_TEXTURE_1D					0x0DE0
#define GL_TEXTURE_2D					0x0DE1
#define GL_TEXTURE_3D					0x0DE2
#define GL_TEXTURE_2D_MULTISAMPLE		0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE	0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY	0x9102
#define GL_DEPTH_STENCIL				0x84F9
#define GL_DEPTH_STENCIL_ATTACHMENT		0x821A
#define GL_TEXTURE_MAG_FILTER			0x2800
#define GL_TEXTURE_MIN_FILTER			0x2801
#define GL_NEAREST						0x2600
#define GL_LINEAR						0x2601
#define GL_TEXTURE_WRAP_S				0x2802
#define GL_TEXTURE_WRAP_T				0x2803
#define GL_TEXTURE_WRAP_R				0x8072
#define GL_CLAMP						0x2900
#define GL_REPEAT						0x2901
// --==</textures_ogl>==--

// --==<textures_nw>==--
// --types
#define NW_TEXTURE_1D					GL_TEXTURE_1D
#define NW_TEXTURE_2D					GL_TEXTURE_2D
#define NW_TEXTURE_3D					GL_TEXTURE_3D
#define NW_TEXTURE_2D_MULTISAMPLE		GL_TEXTURE_2D_MULTISAMPLE
#define NW_TEXTURE_3D_MULTISAMPLE		GL_TEXTURE_2D_MULTISAMPLE_ARRAY
#define NW_DEPTH_STENCIL				GL_DEPTH_STENCIL
#define NW_DEPTH_STENCIL_ATTACHMENT		GL_DEPTH_STENCIL_ATTACHMENT
// --filters
#define NW_TEXTURE_MAG_FILTER			GL_TEXTURE_MAG_FILTER
#define NW_TEXTURE_MIN_FILTER			GL_TEXTURE_MIN_FILTER
#define NW_TEXTURE_FILTER_NEAREST		GL_NEAREST
#define NW_TEXTURE_FILTER_LINEAR		GL_LINEAR
// --wrappers
#define NW_TEXTURE_WRAP_S				GL_TEXTURE_WRAP_S
#define NW_TEXTURE_WRAP_T				GL_TEXTURE_WRAP_T
#define NW_TEXTURE_WRAP_R				GL_TEXTURE_WRAP_R
#define NW_TEXTURE_WRAP_CLAMP			GL_CLAMP
#define NW_TEXTURE_WRAP_REPEAT			GL_REPEAT
// --==</textures_nw>==--

// --==<gbuffers_ogl>==--
#define GL_ARRAY_BUFFER				0x8892
#define GL_ELEMENT_ARRAY_BUFFER		0x8893
#define GL_UNIFORM_BUFFER			0x8A11
// --==</gbuffers_ogl>==--


// --==<gbuffers_nw>==--
#define NW_GFX_BUFFER_VERTEX		GL_ARRAY_BUFFER
#define NW_GFX_BUFFER_INDEX			GL_ELEMENT_ARRAY_BUFFER
#define NW_GFX_BUFFER_SHADER		GL_UNIFORM_BUFFER
// --==</gbuffers_nw>==--

// --==<framebuffers_ogl>==--
#define GL_DRAW_BUFFER			0x0C01
#define GL_READ_BUFFER			0x0C02
#define GL_FRAMEBUFFER			0x8D40
#define GL_COLOR_BUFFER_BIT		0x00004000
#define GL_DEPTH_BUFFER_BIT		0x00000100
#define GL_ACCUM_BUFFER_BIT		0x00000200
#define GL_STENCIL_BUFFER_BIT	0x00000400
#define GL_DEPTH_ATTACHMENT		0x8D00
#define GL_STENCIL_ATTACHMENT	0x8D20
// --==</buffers_ogl>==--

// --==<framebuffers_nw>==--
#define NW_FRAMEBUF_IN			GL_DRAW_BUFFER
#define NW_FRAMEBUF_OUT			GL_READ_BUFFER
#define NW_FRAMEBUF_IN_OUT		GL_FRAMEBUFFER
#define NW_BUFFER_COLOR_BIT		GL_COLOR_BUFFER_BIT
#define NW_BUFFER_DEPTH_BIT		GL_DEPTH_BUFFER_BIT
#define NW_BUFFER_STENCIL_BIT	GL_STENCIL_BUFFER_BIT
// --==</framebuffers_nw>==--

// --==<data_load_types_ogl>==--
#define GL_STATIC_DRAW			0x88E4
#define GL_STREAM_DRAW			0x88E0
#define GL_DYNAMIC_DRAW			0x88E8
// --==</data_load_types_ogl>==--

// --==<data_load_types_nw>==--
#define NW_STATIC_DRAW		GL_STATIC_DRAW 
#define NW_STREAM_DRAW		GL_STREAM_DRAW 
#define NW_DYNAMIC_DRAW		GL_DYNAMIC_DRAW
// --==</data_load_types_nw>==--

// --==<error_codes_ogl>==--
#define GL_NO_ERROR				0
#define GL_INVALID_ENUM			0x0500
#define GL_INVALID_VALUE		0x0501
#define GL_INVALID_OPERATION	0x0502
#define GL_STACK_OVERFLOW		0x0503
#define GL_STACK_UNDERFLOW		0x0504
#define GL_OUT_OF_MEMORY		0x0505
// --==</error_codes_ogl>==--

// --==<error_codes_nw>==--
#define NW_OK					GL_NO_ERROR         
#define NW_INVALID_ENUM			GL_INVALID_ENUM     
#define NW_INVALID_VALUE		GL_INVALID_VALUE    
#define NW_INVALID_OPERATION	GL_INVALID_OPERATION
#define NW_STACK_OVERFLOW		GL_STACK_OVERFLOW   
#define NW_STACK_UNDERFLOW		GL_STACK_UNDERFLOW  
#define NW_OUT_OF_MEMORY		GL_OUT_OF_MEMORY  
// --==</error_codes_nw>==--

// --==<info_ogl>==--
#define GL_VENDOR				0x1F00
#define GL_RENDERER				0x1F01
#define GL_VERSION				0x1F02
#define GL_EXTENSIONS			0x1F03

#define GL_COMPILE_STATUS		0x8B81
#define GL_LINK_STATUS			0x8B82
#define GL_INFO_LOG_LENGTH		0x8B84
#define GL_ATTACHED_SHADERS		0x8B85
// --==</info_ogl>==--

// --==<info_nw>==--
#define NW_COMPILE_STATUS		GL_COMPILE_STATUS
#define NW_LINK_STATUS			GL_LINK_STATUS
// --==</info_nw>==--
#endif // NW_GAPI
#if (NW_GAPI & NW_GAPI_DX)
// --==<data_types_dx11>==--
// --==</data_types_dx11>==--

// --==<data_types_nw>==--
#define NW_ZERO			0
#define NW_ONE			1
#define NW_BOOL			2
#define NW_INT16		3
#define NW_UINT16		4
#define NW_INT8			5
#define NW_UINT8		6
#define NW_INT32		7
#define NW_UINT32		8
#define NW_FLOAT32		9
#define NW_FLOAT64		10
#define NW_FLOAT32_VEC2	11
#define NW_FLOAT32_VEC3	12
#define NW_FLOAT32_VEC4	13
#define NW_FLOAT32_MAT2	14
#define NW_FLOAT32_MAT3	15
#define NW_FLOAT32_MAT4	16
#define NW_SAMPLER_1D	17
#define NW_UINT24_8		18
// --==</data_types_nw>==--

// --==<draw_modes_dx11>==--
// --primitives
// --==</draw_modes_dx11>==--

// --==<draw_modes_nw>==--
#define NW_POINT		3
#define NW_LINE			2
#define NW_FILL			1
// --primitives
#define NW_POINTS			10
#define NW_LINES			11
#define NW_LINE_STRIP		9
#define NW_LINE_LOOP		8
#define NW_LINE_BIT			7
#define NW_TRIANGLES		6
#define NW_TRIANGLE_STRIP	5
#define NW_TRIANGLE_FAN		4
#define NW_QUADS			3
#define NW_QUAD_STRIP		2
#define NW_POLYGON			1
// --==</draw_modes_nw>==--

// --==<planes_dx11>==--
// --==</planes_dx11>==--

// --==<planes_nw>==--
#define NW_FRONT			5
#define NW_BACK				4
#define NW_LEFT				3
#define NW_RIGHT			2
#define NW_FRONT_AND_BACK	1
// --==</planes_nw>==--

// --==<shaders_dx11>==--
// --==<shaders_dx11>==--

// --==<shaders_nw>==--
#define NW_SHADER					4
#define NW_SHADER_VERTEX			3
#define NW_SHADER_GEOMETRY			2
#define NW_SHADER_PIXEL				1
// --==</shaders_nw>==--

// --==<configurations_dx11>==--
// --==</configurations_dx11>==--

// --==<configurations_nw>==--
#define NW_POINT_SIZE			8
#define NW_LINE_SMOOTH			7
#define NW_LINE_WIDTH			6

#define NW_CULL_FACE			5
#define NW_CULL_FACE_MODE		4
#define NW_FRONT_FACE			3
#define NW_CW					2
#define NW_CCW					1

#define NW_DEPTH_TEST			51
#define NW_DEPTH_FUNC			52
#define NW_STENCIL_TEST			53
#define NW_STENCIL_FUNC			54

#define NW_BLEND				1
#define NW_BLEND_DST			2
#define NW_BLEND_SRC			3
#define NW_SRC_COLOR			4
#define NW_ONE_MINUS_SRC_COLOR	5
#define NW_SRC_ALPHA			6
#define NW_ONE_MINUS_SRC_ALPHA	7
#define NW_DST_ALPHA			8
#define NW_ONE_MINUS_DST_ALPHA	9
#define NW_DST_COLOR			10
#define NW_ONE_MINUS_DST_COLOR	11
#define NW_SRC_ALPHA_SATURATE	12

#define NW_MULTISAMPLE			0

#define NW_KEEP					0
#define NW_REPLACE				0
#define NW_INCR					0
#define NW_DECR					0
// --==</configurations_nw>==--

// --==<expressions_dx11>==--
// --constants
// --conditions
// --logic
// --==</expressions_dx11>==--

// --==<expressions_nw>==--
// --constants
#define NW_NONE		0
#define NW_FALSE	0
#define NW_TRUE		1
// --conditions
#define NW_NEVER		0x0200
#define NW_LESS			0x0201
#define NW_EQUAL		0x0202
#define NW_LEQUAL		0x0203
#define NW_GREATER		0x0204
#define NW_NOTEQUAL		0x0205
#define NW_GEQUAL		0x0206
#define NW_ALWAYS		0x0207
// --logic
#define NW_AND				0x1501
#define NW_AND_REVERSE		0x1502
#define NW_COPY				0x1503
#define NW_AND_INVERTED		0x1504
#define NW_NOOP				0x1505
#define NW_XOR				0x1506
#define NW_OR				0x1507
#define NW_NOR				0x1508
#define NW_EQUIV			0x1509
#define NW_INVERT			0x150A
#define NW_OR_REVERSE		0x150B
#define NW_COPY_INVERTED	0x150C
#define NW_OR_INVERTED		0x150D
#define NW_NAND				0x150E
// --==</expressions_nw>==--

// --==<pixel_formats_dx11>==--
// --==</pixel_formats_dx11>==--

// --==<pixel_formats_nw>==--
#define NW_RED				1
#define NW_GREEN			2
#define NW_BLUE				3
#define NW_ALPHA			4
#define NW_RGB				5
#define NW_RGBA				6
#define NW_RED_INT			7
#define NW_GREEN_INT		8
#define NW_BLUE_INT			9
#define NW_RGB_INT			10
#define NW_RGBA_INT			11
#define NW_DEPTH			12
#define NW_STENCIL			13
#define NW_RED_INT32		14
#define NW_RED_UINT32		15
#define NW_RGBA8			16
#define NW_DEPTH24			17
#define NW_STENCIL8			18
#define NW_DEPTH24_STENCIL8	19
// --==</pixel_formats_nw>==--

// --==<textures_dx11>==--
// --==</textures_dx11>==--

// --==<textures_nw>==--
// --types
#define NW_TEXTURE_1D					1
#define NW_TEXTURE_2D					2
#define NW_TEXTURE_3D					3
#define NW_TEXTURE_2D_MULTISAMPLE		4
#define NW_TEXTURE_3D_MULTISAMPLE		5
#define NW_DEPTH_STENCIL				6
#define NW_DEPTH_STENCIL_ATTACHMENT		7
// --filters
#define NW_TEXTURE_MAG_FILTER			1
#define NW_TEXTURE_MIN_FILTER			2
#define NW_TEXTURE_FILTER_NEAREST		3
#define NW_TEXTURE_FILTER_LINEAR		4
// --wrappers
#define NW_TEXTURE_WRAP_S				5
#define NW_TEXTURE_WRAP_T				6
#define NW_TEXTURE_WRAP_R				7
#define NW_TEXTURE_WRAP_CLAMP			8
#define NW_TEXTURE_WRAP_REPEAT			9
// --==</textures_nw>==--

// --==<gbuffers_dx11>==--
// --==</gbuffers_dx11>==--


// --==<gbuffers_nw>==--
#define NW_GFX_BUFFER_VERTEX	1
#define NW_GFX_BUFFER_INDEX		2
#define NW_GFX_BUFFER_SHADER	3
// --==</gbuffers_nw>==--

// --==<framebuffers_dx11>==--
// --==</framebuffers_dx11>==--

// --==<framebuffers_nw>==--
#define NW_FRAMEBUF_IN			1
#define NW_FRAMEBUF_OUT			2
#define NW_FRAMEBUF_IN_OUT		3
#define NW_BUFFER_COLOR_BIT		4
#define NW_BUFFER_DEPTH_BIT		5
#define NW_BUFFER_STENCIL_BIT	6
// --==</framebuffers_nw>==--

// --==<data_load_types_dx11>==--
// --==</data_load_types_dx11>==--

// --==<data_load_types_nw>==--
#define NW_STATIC_DRAW		1
#define NW_STREAM_DRAW		2
#define NW_DYNAMIC_DRAW		3
// --==</data_load_types_nw>==--

// --==<error_codes_dx11>==--
// --==</error_codes_dx11>==--

// --==<error_codes_nw>==--
#define NW_OK					0
#define NW_INVALID_ENUM			1
#define NW_INVALID_VALUE		2
#define NW_INVALID_OPERATION	3
#define NW_STACK_OVERFLOW		4
#define NW_STACK_UNDERFLOW		5
#define NW_OUT_OF_MEMORY		6
// --==</error_codes_nw>==--

// --==<info_dx11>==--
// --==</info_dx11>==--

// --==<info_nw>==--
#define NW_COMPILE_STATUS		7
#define NW_LINK_STATUS			8
// --==</info_nw>==--
#endif	// NW_GAPI


namespace NW
{
	class NW_API CoreEngine;
	class NW_API CoreWindow;

	using Keyboard = NWL::KeyboardT<NW_KEY_COUNT>;
	using Cursor = NWL::CursorT<NW_MS_BTN_COUNT>;
}

#endif // NW_CORE_HPP

/// 18.10.2020
/// To save your changes in github:
/// --Create repo in your project directory with solution
/// --make .gitingore file where are files you don't want to save
/// --Write in the cmd:
/// -->"git clone {url of your repo}" - copy all files from repository into the curr dir
/// -->"git status" - to get info about files you are working with
/// -->"git add *" - add all files except of .gitignore list
/// -->"git reset ." - remove all files from "download stack"
/// -->"git commit -m {"commentary"}"
/// -->"git push origin master" - push all files in repository