#ifndef NW_DEFINES_H
#define NW_DEFINES_H
/// Here are described macroses and codes depending on window specification
/// Firstly: specification codes
/// Secondly: Engine codes equaled to previous

// ========<Alias>========
namespace NW
{
	// --number
	typedef __int8				Int8;
	typedef __int16				Int16;
	typedef __int32				Int32;
	typedef __int64				Int64;
	typedef unsigned __int8		UInt8;
	typedef unsigned __int16	UInt16;
	typedef unsigned __int32	UInt32;
	typedef unsigned __int64	UInt64;
	typedef float				Float32;
	typedef double				Float64;
	typedef long double			Float128;
	// --memory
	typedef size_t				Size;
	typedef char				Byte;
	typedef unsigned char		UByte;
	template <typename RefType>
	using RefOwner = std::unique_ptr<RefType>;
	template <typename RefType>
	using RefKeeper = std::shared_ptr<RefType>;
	// --string
	typedef char Char;
	typedef char Char8;
	typedef unsigned char UChar;
	typedef unsigned char UChar8;
	typedef wchar_t Char16;
	using CString = const Char8*;
	using UCString = const UChar8*;
	using String = std::string;
	using WString = std::wstring;
	// --stream
	using InStream = std::istream;
	using OutStream = std::ostream;
	using IOStream = std::iostream;
	using StringStream = std::stringstream;
	// --container
	template <typename ValType, Size szCapacity>
	using SArray = std::array<ValType, szCapacity>;
	template <typename ValType>
	using DArray = std::vector<ValType>;
	template <typename ValType>
	using DStack = std::stack<ValType>;
	template <typename ValType>
	using List2 = std::list<ValType>;
	template <typename KeyType, typename ValType>
	using HashMap = std::unordered_map<KeyType, ValType>;
	// --platform
#if defined NW_PLATFORM_WINDOWS
	using V2xy = COORD;
	using V2wh = COORD;
	using V4xywh = SMALL_RECT;
	using CPixel = CHAR_INFO;
	using CSBufInfo = CONSOLE_SCREEN_BUFFER_INFO;
	using CSBufInfoX = CONSOLE_SCREEN_BUFFER_INFOEX;
	using CCursorInfo = CONSOLE_CURSOR_INFO;
	using CFontInfoX = CONSOLE_FONT_INFOEX;
#endif // NW_PLATFORM
}
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
typedef void GLvoid;
typedef char GLchar;
typedef signed char GLbyte;
typedef unsigned char GLubyte;
typedef unsigned char GLboolean;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLuint;
typedef unsigned int GLbitfield;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef unsigned int GLenum;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
#endif // NW_GRAPHICS
// ========</TypeDefs>========

#if (NW_WINDOW & NW_WINDOW_CONSOLE)
// ========<KeyCodes_CN>========
#define NW_KEY_NONE					0
// Printable keys
#define NW_KEY_SPACE_32				32
#define NW_KEY_APOSTROPHE_39		39
#define NW_KEY_COMMA_44				44
#define NW_KEY_MINUS_45				45
#define NW_KEY_PERIOD_46			46
#define NW_KEY_SLASH_47				47
#define NW_KEY_0_48					48
#define NW_KEY_1_49					49
#define NW_KEY_2_50					50
#define NW_KEY_3_51					51
#define NW_KEY_4_52					52
#define NW_KEY_5_53					53
#define NW_KEY_6_54					54
#define NW_KEY_7_55					55
#define NW_KEY_8_56					56
#define NW_KEY_9_57					57
#define NW_KEY_SEMICOLON_59			59
#define NW_KEY_EQUAL_61				61
// Alphabet_upper_case
#define NW_KEY_A_65					65
#define NW_KEY_B_66					66
#define NW_KEY_C_67					67
#define NW_KEY_D_68					68
#define NW_KEY_E_69					69
#define NW_KEY_F_70					70
#define NW_KEY_G_71					71
#define NW_KEY_H_72					72
#define NW_KEY_I_73					73
#define NW_KEY_J_74					74
#define NW_KEY_K_75					75
#define NW_KEY_L_76					76
#define NW_KEY_M_77					77
#define NW_KEY_N_78					78
#define NW_KEY_O_79					79
#define NW_KEY_P_80					80
#define NW_KEY_Q_81					81
#define NW_KEY_R_82					82
#define NW_KEY_S_83					83
#define NW_KEY_T_84					84
#define NW_KEY_U_85					85
#define NW_KEY_V_86					86
#define NW_KEY_W_87					87
#define NW_KEY_X_88					88
#define NW_KEY_Y_89					89
#define NW_KEY_Z_90					90
// Additional
#define NW_KEY_LEFT_BRACKET_91		91
#define NW_KEY_BACKSLASH_92			92
#define NW_KEY_RIGHT_BRACKET_93		93
// Alphabet_lower_case
#define NW_KEY_a_97					97
#define NW_KEY_b_98					98
#define NW_KEY_c_99					99
#define NW_KEY_d_100				100
#define NW_KEY_e_101				101
#define NW_KEY_f_102				102
#define NW_KEY_g_103				103
#define NW_KEY_h_104				104
#define NW_KEY_i_105				105
#define NW_KEY_j_106				106
#define NW_KEY_k_107				107
#define NW_KEY_l_108				108
#define NW_KEY_m_109				109
#define NW_KEY_n_110				110
#define NW_KEY_o_111				111
#define NW_KEY_p_112				112
#define NW_KEY_q_113				113
#define NW_KEY_r_114				114
#define NW_KEY_s_115				115
#define NW_KEY_t_116				116
#define NW_KEY_u_117				117
#define NW_KEY_v_118				118
#define NW_KEY_w_119				119
#define NW_KEY_x_120				120
#define NW_KEY_y_121				121
#define NW_KEY_z_122				122
// Functional keys
#define NW_KEY_ESCAPE_27			27
#define NW_KEY_BACKSPACE_8			8
#define NW_KEY_HTAB_9				9
#define NW_KEY_ENTER_10				10
#define NW_KEY_HTAB_11				11
#define NW_KEY_INS_45				45            
#define NW_KEY_DEL_46				46
#define NW_KEY_RIGHT_38				39
#define NW_KEY_LEFT_37				37
#define NW_KEY_DOWN_40				40
#define NW_KEY_UP_38				38
#define NW_KEY_F1					''
#define NW_KEY_F2					''
#define NW_KEY_F3					''
#define NW_KEY_F4					''
#define NW_KEY_F5					''
#define NW_KEY_F6					''
#define NW_KEY_F7					''
#define NW_KEY_F8					''
#define NW_KEY_F9					''
#define NW_KEY_F10					''
#define NW_KEY_F11					''
#define NW_KEY_F12					''
#define NW_KEY_KP_0					''
#define NW_KEY_KP_1					''
#define NW_KEY_KP_2					''
#define NW_KEY_KP_3					''
#define NW_KEY_KP_4					''
#define NW_KEY_KP_5					''
#define NW_KEY_KP_6					''
#define NW_KEY_KP_7					''
#define NW_KEY_KP_8					''
#define NW_KEY_KP_9					''
#define NW_KEY_KP_DECIMAL			''
#define NW_KEY_KP_DIVIDE			''
#define NW_KEY_KP_MULTIPLY			''
#define NW_KEY_KP_SUBTRACT			''
#define NW_KEY_KP_ADD				''
#define NW_KEY_KP_ENTER_13			''
#define NW_KEY_KP_EQUAL				''
#define NW_KEY_SHIFT_LEFT			16
#define NW_KEY_SHIFT_RIGHT			16
#define NW_KEY_CONTROL_LEFT			17
#define NW_KEY_CONTROL_RIGHT		17
#define NW_KEY_ALT_LEFT				18
#define NW_KEY_ALT_RIGHT			18

#define NW_KEY_LAST 256
// ========<KeyCodes_CN>========

// ========<DeviceCodes_CN>========
#define NW_MS_BTN_0         0
#define NW_MS_BTN_1         1
#define NW_MS_BTN_2         2
#define NW_MS_BTN_3         3
#define NW_MS_BTN_4         4
#define NW_MS_BTN_5         5
#define NW_MS_BTN_6         6
#define NW_MS_BTN_7         7
#define NW_MS_BTN_LAST      NW_MS_BTN_7
#define NW_MS_BTN_LEFT      NW_MS_BTN_0
#define NW_MS_BTN_RIGHT     NW_MS_BTN_1
#define NW_MS_BTN_MIDDLE    NW_MS_BTN_2
// ========<DeviceCodes_CN>========

// ========<EventTypes_CN>========
#define NW_PRESS 0

#define NW_MS_PRESS_LEFT 0x0001
#define NW_MS_PRESS_RIGHT 0x0002
#define NW_MS_PRESS_2 0x0004
#define NW_MS_PRESS_3 0x0008
#define NW_MS_PRESS_4 0x0010

#define NW_MS_PRESS_DOUBLE 0x0002

#define NW_MS_MOVED 0x0001
#define NW_MS_WHEELED 0x0004
#define NW_MS_HWHEELED 0x0008

#define NW_KEY_EVT 0x0001
#define NW_MS_EVT 0x0002
#define NW_WND_SIZE_EVT 0x0004
#define NW_MENU_EVT 0x0008
#define NW_FOCUS_EVT 0x0010
// ========</EventTypes_CN>========
#elif (NW_WINDOW & NW_WINDOW_GLFW)
// ========<KeyCodes_GLFW>========
#define GLFW_KEY_UNKNOWN            -1
// Printable keys
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
// Functional keys
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
// ========</KeyCodes_GLFW>========

// ========<KeyCodes_CN>========
// Printable keys
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
// Functional keys
#define NW_KEY_ESCAPE_27             GLFW_KEY_ESCAPE             
#define NW_KEY_BACKSPACE_8          GLFW_KEY_BACKSPACE          
#define NW_KEY_HTAB_9                GLFW_KEY_TAB                
#define NW_KEY_ENTER_10              GLFW_KEY_ENTER              
#define NW_KEY_INS_45             GLFW_KEY_INSERT             
#define NW_KEY_DEL_46             GLFW_KEY_DELETE             
#define NW_KEY_RIGHT_38              GLFW_KEY_RIGHT              
#define NW_KEY_LEFT_37               GLFW_KEY_LEFT               
#define NW_KEY_DOWN_40               GLFW_KEY_DOWN               
#define NW_KEY_UP_38                 GLFW_KEY_UP                 
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

#define NW_KEY_LAST GLFW_KEY_LAST
// ========</KeyCodes_CN>========

// ========<DeviceCodes_GLFW>========
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
// ========</DeviceCodes_GLFW>========

// ========<DeviceCodes_CN>========
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
// ========</DeviceCodes_CN>========

// ========<Events_CN>========
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
// ========</Events_CN>========

#elif (NW_WINDOW & NW_WINDOW_WIN)
// ========<KeyCodes_WIN>========
// Functional
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5
// Numeric
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87
// ========</KeyCodes_WIN>========

// ========<KeyCodes_CN>========
#define NW_KEY_NONE					'0'
// Printable keys
#define NW_KEY_SPACE_32				'32'
#define NW_KEY_APOSTROPHE_39		'39'
#define NW_KEY_COMMA_44				'44'
#define NW_KEY_MINUS_45				'45'
#define NW_KEY_PERIOD_46			'46'
#define NW_KEY_SLASH_47				'47'
#define NW_KEY_0_48					'48'
#define NW_KEY_1_49					'49'
#define NW_KEY_2_50					'50'
#define NW_KEY_3_51					'51'
#define NW_KEY_4_52					'52'
#define NW_KEY_5_53					'53'
#define NW_KEY_6_54					'54'
#define NW_KEY_7_55					'55'
#define NW_KEY_8_56					'56'
#define NW_KEY_9_57					'57'
#define NW_KEY_SEMICOLON_59			'59'
#define NW_KEY_EQUAL_61				'61'
// Alphabet_upper_case
#define NW_KEY_A_65					'65'
#define NW_KEY_B_66					'66'
#define NW_KEY_C_67					'67'
#define NW_KEY_D_68					'68'
#define NW_KEY_E_69					'69'
#define NW_KEY_F_70					'70'
#define NW_KEY_G_71					'71'
#define NW_KEY_H_72					'72'
#define NW_KEY_I_73					'73'
#define NW_KEY_J_74					'74'
#define NW_KEY_K_75					'75'
#define NW_KEY_L_76					'76'
#define NW_KEY_M_77					'77'
#define NW_KEY_N_78					'78'
#define NW_KEY_O_79					'79'
#define NW_KEY_P_80					'80'
#define NW_KEY_Q_81					'81'
#define NW_KEY_R_82					'82'
#define NW_KEY_S_83					'83'
#define NW_KEY_T_84					'84'
#define NW_KEY_U_85					'85'
#define NW_KEY_V_86					'86'
#define NW_KEY_W_87					'87'
#define NW_KEY_X_88					'88'
#define NW_KEY_Y_89					'89'
#define NW_KEY_Z_90					'90'
// Additional
#define NW_KEY_LEFT_BRACKET_91		'91'
#define NW_KEY_BACKSLASH_92			'92'
#define NW_KEY_RIGHT_BRACKET_93		'93'
// Alphabet_lower_case
#define NW_KEY_a_97					'97'
#define NW_KEY_b_98					'98'
#define NW_KEY_c_99					'99'
#define NW_KEY_d_100				'100'
#define NW_KEY_e_101				'101'
#define NW_KEY_f_102				'102'
#define NW_KEY_g_103				'103'
#define NW_KEY_h_104				'104'
#define NW_KEY_i_105				'105'
#define NW_KEY_j_106				'106'
#define NW_KEY_k_107				'107'
#define NW_KEY_l_108				'108'
#define NW_KEY_m_109				'109'
#define NW_KEY_n_110				'110'
#define NW_KEY_o_111				'111'
#define NW_KEY_p_112				'112'
#define NW_KEY_q_113				'113'
#define NW_KEY_r_114				'114'
#define NW_KEY_s_115				'115'
#define NW_KEY_t_116				'116'
#define NW_KEY_u_117				'117'
#define NW_KEY_v_118				'118'
#define NW_KEY_w_119				'119'
#define NW_KEY_x_120				'120'
#define NW_KEY_y_121				'121'
#define NW_KEY_z_122				'122'
// Functional keys
#define NW_KEY_ESCAPE_27			'27'
#define NW_KEY_BACKSPACE_8			'8'
#define NW_KEY_VTAB_9				'9'
#define NW_KEY_ENTER_10				'10'
#define NW_KEY_HTAB_11				'11'
#define NW_KEY_INSERT_45			'45'
#define NW_KEY_DELETE_46			'46'
#define NW_KEY_RIGHT_38				'39'
#define NW_KEY_LEFT_37				'37'
#define NW_KEY_DOWN_40				'40'
#define NW_KEY_UP_38				'38'
#define NW_KEY_F1					''
#define NW_KEY_F2					''
#define NW_KEY_F3					''
#define NW_KEY_F4					''
#define NW_KEY_F5					''
#define NW_KEY_F6					''
#define NW_KEY_F7					''
#define NW_KEY_F8					''
#define NW_KEY_F9					''
#define NW_KEY_F10					''
#define NW_KEY_F11					''
#define NW_KEY_F12					''
#define NW_KEY_KP_0					''
#define NW_KEY_KP_1					''
#define NW_KEY_KP_2					''
#define NW_KEY_KP_3					''
#define NW_KEY_KP_4					''
#define NW_KEY_KP_5					''
#define NW_KEY_KP_6					''
#define NW_KEY_KP_7					''
#define NW_KEY_KP_8					''
#define NW_KEY_KP_9					''
#define NW_KEY_KP_DECIMAL			''
#define NW_KEY_KP_DIVIDE			''
#define NW_KEY_KP_MULTIPLY			''
#define NW_KEY_KP_SUBTRACT			''
#define NW_KEY_KP_ADD				''
#define NW_KEY_KP_ENTER_13				''
#define NW_KEY_KP_EQUAL				''
#define NW_KEY_LEFT_SHIFT			''
#define NW_KEY_LEFT_CONTROL			''
#define NW_KEY_LEFT_ALT				''
#define NW_KEY_RIGHT_SHIFT			''
#define NW_KEY_RIGHT_CONTROL		''
#define NW_KEY_RIGHT_ALT			''

#define NW_KEY_LAST 256
// ========</KeyCodes_CN>========

// ========<DeviceCodes_WIN>========
#define FROM_LEFT_1ST_BUTTON_PRESSED 0
#define RIGHTMOST_BUTTON_PRESSED 1
#define FROM_LEFT_2ND_BUTTON_PRESSED 2
#define FROM_LEFT_3RD_BUTTON_PRESSED 3
#define FROM_LEFT_4TH_BUTTON_PRESSED 4
// ========</DeviceCodes_WIN>========

// ========<DeviceCodes_CN>========
#define NW_MS_BTN_0         FROM_LEFT_1ST_BUTTON_PRESSED
#define NW_MS_BTN_1         RIGHTMOST_BUTTON_PRESSED
#define NW_MS_BTN_2         FROM_LEFT_2ND_BUTTON_PRESSED
#define NW_MS_BTN_3         FROM_LEFT_3RD_BUTTON_PRESSED
#define NW_MS_BTN_4         FROM_LEFT_4TH_BUTTON_PRESSED
#define NW_MS_BTN_5         0
#define NW_MS_BTN_6         0
#define NW_MS_BTN_7         0
#define NW_MS_BTN_LAST      NW_MS_BTN_4
#define NW_MS_BTN_LEFT      NW_MS_BTN_0
#define NW_MS_BTN_RIGHT     NW_MS_BTN_1
#define NW_MS_BTN_MIDDLE    NW_MS_BTN_2
// ========</DeviceCodes_CN>========

// ========<Events_CN>========
#define NW_PRESS 0

#define NW_MS_PRESS_LEFT 0x0001
#define NW_MS_PRESS_RIGHT 0x0002
#define NW_MS_PRESS_2 0x0004
#define NW_MS_PRESS_3 0x0008
#define NW_MS_PRESS_4 0x0010

#define NW_MS_PRESS_DOUBLE 0x0002

#define NW_MS_MOVED 0x0001
#define NW_MS_WHEELED 0x0004
#define NW_MS_HWHEELED 0x0008

#define NW_KEY_EVT 0x0001
#define NW_MS_EVT 0x0002
#define NW_WND_SIZE_EVT 0x0004
#define NW_MENU_EVT 0x0008
#define NW_FOCUS_EVT 0x0010
// ========</Events_CN>========
#endif // NW_WINDOW

#if (NW_GRAPHICS & NW_GRAPHICS_COUT) || (NW_GRAPHICS & NW_GRAPHICS_WIN)
// 5th bit from right + first 4 - default system types
// 8th bit from right + first 4 - graphics types
// 12th bit from right + second 4 - graphics operations & modes
// ========<DataTypes_CN>========
#define NW_BOOL							0b0000'0001'0001
#define NW_SINT							0b0000'0001'0010
#define NW_USINT						0b0000'0001'0011
#define NW_CHAR							0b0000'0001'0100
#define NW_UCHAR						0b0000'0001'0101
#define NW_INT							0b0000'0001'0111
#define NW_UINT							0b0000'0001'1000
#define NW_FLOAT						0b0000'0001'1001
#define NW_DOUBLE						0b0000'0001'1010
// ========</DataTypes_CN>========

// ========<ObjectTypes_CN>========
#define NW_BUFFER						0b0000'1000'0001
#define NW_BUFFER_VERTEX				0b0000'1000'0010
#define NW_BUFFER_INDEX					0b0000'1000'0011
#define NW_ARRAY						0b0000'1000'0100
#define NW_ARRAY_VERTEX					0b0000'1000'0101
// ========</ObjectTypes_CN>========

// ========<ShaderTypes_CN>========
#define NW_SHADER						0b0000'1000'1000
#define NW_SHADER_PROG					0b0000'1000'1001
#define NW_SHADER_VERTEX				0b0000'1000'1010
#define NW_SHADER_PIXEL					0b0000'1000'1100
// ========<ShaderTypes_CN>========

// ========<Textures_CN>========
// --Types
#define NW_TEXTURE_1D					0b0000'1001'0001
#define NW_TEXTURE_2D					0b0000'1001'0010
#define NW_TEXTURE_3D					0b0000'1001'0100
// --Types
// ========</Textures_CN>========

// ========<DrawModes_CN>========
#define NW_POINT						0b0000'0000'0000
#define NW_LINE							0b0000'0000'0000
#define NW_FILL							0b0000'0000'0000
// --Primitives
#define NW_POINTS						0b1000'0001'0000
#define NW_LINES						0b1000'0010'0000
#define NW_LINE_STRIP					0b1000'0011'0000
#define NW_LINE_LOOP					0b1000'0101'0000
#define NW_LINE_BIT						0b1000'0110'0000
#define NW_TRIANGLES					0b1000'0111'0000
#define NW_TRIANGLE_STRIP				0b1000'1000'0000
#define NW_TRIANGLE_FAN					0b1000'1001'0000
#define NW_QUADS						0b1000'1010'0000
#define NW_QUAD_STRIP					0b1000'1011'0000
#define NW_POLYGON						0b1000'1100'0000
// --Primitives
// ========</DrawModes_CN>========

// ========<Configurations_CN>========
// ========</Configurations_CN>========

// ========<Expressions_CN>========
// --Constants
#define NW_NONE							0b0000'0000'0000
#define NW_FALSE						0b0000'0000'0000
#define NW_TRUE							0b0000'0000'0001
// --Constants

// --Conditions
#define NW_NEVER						0b0000'0000'0000
#define NW_ALWAYS						0b0000'0000'0001
// --Conditions

// --Logic
// --Logic
// ========</Expressions_CN>========

// ========<Colors_CN>========
#define NW_COLOR						0b0000'0000'0000
#define NW_RED							0b0000'0000'0000
#define NW_GREEN						0b0000'0000'0000
#define NW_BLUE							0b0000'0000'0000
#define NW_ALPHA						0b0000'0000'0000
#define NW_RGB							0b0000'0000'0000
#define NW_RGBA							0b0000'0000'0000
// ========</Colors_CN>========

// ========<GBuffers_CN>========
#define NW_GBUFFER_VERTEX				0b0000'0000'0001
#define NW_GBUFFER_INDEX				0b0000'0000'0001
// ========</GBuffers_CN>========

// ========<Buffers_CN>========
#define NW_BUFFER_COLOR_BIT				0b0000'0000'0001
#define NW_BUFFER_DEPTH_BIT				0b0000'0000'0010
// ========</Buffers_CN>========


// ========<DataLoadTypes_CN>========
#define NW_STATIC_DRAW					0b0001'1000'0000
#define NW_STREAM_DRAW					0b0010'1000'0000
#define NW_DYNAMIC_DRAW					0b0011'1000'0000
// ========</DataLoadTypes_CN>========

// ========<ErrorCodes_CN>========
#define NW_OK							0b0000'0000'0000
#define NW_INVALID_ENUM					0b0001'0000'0000
#define NW_INVALID_VALUE				0b0010'0000'0000
#define NW_INVALID_OPERATION			0b0011'0000'0000
#define NW_STACK_OVERFLOW				0b0100'0000'0000
#define NW_STACK_UNDERFLOW				0b0101'0000'0000
#define NW_OUT_OF_MEMORY				0b0110'0000'0000

#define NW_ERR_SHADER_LOAD_PATH			0b1000'0000'0000
#define NW_ERR_SHADER_LOAD_FAIL			0b1001'0000'0000
#define NW_ERR_SHADER_LINK				0b1011'0000'0000
#define NW_ERR_SHADER_COMPILE			0b1100'0000'0000

#define NW_STATUS						0b1111'0000'0000
#define NW_COMPILE_STATUS				0b1110'0000'0000
#define NW_LINK_STATUS					0b1100'0000'0000
// ========</ErrorCodes_CN>========

// ========<Info_OGL>========
// ========</Info_CN>========
#elif (NW_GRAPHICS & NW_GRAPHICS_OGL)
// ========<DataTypes_OGL>========
#define GL_BOOL								0x8B56
#define GL_BYTE								0x1400
#define GL_UNSIGNED_BYTE					0x1401
#define GL_SHORT							0x1402
#define GL_UNSIGNED_SHORT					0x1403
#define GL_INT								0x1404
#define GL_UNSIGNED_INT						0x1405
#define GL_FLOAT							0x1406
#define GL_DOUBLE							0x140A
// ========</DataTypes_OGL>========

// ========<DataTypes_CN>========
#define NW_BOOL			GL_BOOL
#define NW_INT16		GL_SHORT
#define NW_UINT16		GL_UNSIGNED_SHORT
#define NW_INT8			GL_BYTE
#define NW_UINT8		GL_UNSIGNED_BYTE
#define NW_INT32		GL_INT
#define NW_UINT32		GL_UNSIGNED_INT
#define NW_FLOAT32		GL_FLOAT
#define NW_FLOAT64		GL_DOUBLE
// ========</DataTypes_CN>========

// ========<DrawModes_OGL>========
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
// --Primitives
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_STRIP 0x0003
#define GL_LINE_LOOP 0x0002
#define GL_LINE_BIT 0x00000004
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON 0x0009
// --Primitives
// ========</DrawModes_OGL>========
// ========<DrawModes_CN>========
#define NW_POINT 0x1B00
#define NW_LINE 0x1B01
#define NW_FILL 0x1B02
// --Primitives
#define NW_POINTS 0x0000
#define NW_LINES 0x0001
#define NW_LINE_STRIP 0x0003
#define NW_LINE_LOOP 0x0002
#define NW_LINE_BIT 0x00000004
#define NW_TRIANGLES 0x0004
#define NW_TRIANGLE_STRIP 0x0005
#define NW_TRIANGLE_FAN 0x0006
#define NW_QUADS 0x0007
#define NW_QUAD_STRIP 0x0008
#define NW_POLYGON 0x0009
// --Primitives
// ========</DrawModes_CN>========

// ========<Planes_OGL>========
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408
// ========</Planes_OGL>========
// ========<Planes_CN>========
#define NW_FRONT GL_FRONT
#define NW_BACK GL_BACK
#define NW_LEFT GL_LEFT
#define NW_RIGHT GL_RIGHT
#define NW_FRONT_AND_BACK GL_FRONT_AND_BACK
// ========</Planes_OGL>========

// ========<Shaders_OGL>========
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_VERTEX_SHADER 0x8B31

#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
// ========<Shaders_OGL>========
// ========<Shaders_CN>========
#define NW_SHADER				GL_PROGRAM
#define NW_SHADER_VERTEX		GL_VERTEX_SHADER
#define NW_SHADER_GEOMETRY		GL_GEOMETRY_SHADER
#define NW_SHADER_PIXEL			GL_FRAGMENT_SHADER

#define NW_COMPILE_STATUS		GL_COMPILE_STATUS
#define NW_LINK_STATUS			GL_LINK_STATUS
// ========</Shaders_CN>========

// ========<Configurations_OGL>========
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_POINT_SIZE 0x0B11
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_WIDTH 0x0B21

#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_CW 0x0900
#define GL_CCW 0x0901

#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_FUNC 0x0B74

#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_FUNC 0x0B92

#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND 0x0BE2
// ========</Configurations_OGL>========

// ========<Configurations_CN>========
#define NW_SRC_COLOR GL_SRC_COLOR
#define NW_ONE_MINUS_SRC_COLOR GL_ONE_MINUS_SRC_COLOR
#define NW_SRC_ALPHA GL_SRC_ALPHA
#define NW_ONE_MINUS_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
#define NW_DST_ALPHA GL_DST_ALPHA
#define NW_ONE_MINUS_DST_ALPHA GL_ONE_MINUS_DST_ALPHA
#define NW_DST_COLOR GL_DST_COLOR
#define NW_ONE_MINUS_DST_COLOR GL_ONE_MINUS_DST_COLOR
#define NW_SRC_ALPHA_SATURATE GL_SRC_ALPHA_SATURATE
#define NW_POINT_SIZE GL_POINT_SIZE
#define NW_LINE_SMOOTH GL_LINE_SMOOTH
#define NW_LINE_WIDTH GL_LINE_WIDTH

#define NW_CULL_FACE		GL_CULL_FACE
#define NW_CULL_FACE_MODE	GL_CULL_FACE_MODE
#define NW_FRONT_FACE		GL_FRONT_FACE
#define NW_CW				GL_CW
#define NW_CCW				GL_CCW

#define NW_DEPTH_TEST		GL_DEPTH_TEST
#define NW_DEPTH_FUNC		GL_DEPTH_FUNC

#define NW_STENCIL_TEST		GL_STENCIL_TEST
#define NW_STENCIL_FUNC		GL_STENCIL_FUNC

#define NW_BLEND_DST		GL_BLEND_DST
#define NW_BLEND_SRC		GL_BLEND_SRC
#define NW_BLEND			GL_BLEND
// ========</Configurations_CN>========

// ========<Expressions_OGL>========
// --Constants
#define GL_NONE 0
#define GL_FALSE 0
#define GL_TRUE 1
// --Constants

// --Conditions
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
// --Conditions

// --Logic
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
// --Logic
// ========</Expressions_OGL>========

// ========<Colors_OGL>========
#define GL_COLOR 0x1800
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_RGBA8 0x8058
// ========</Colors_OGL>========

// ========<Colors_CN>========
#define NW_COLOR		GL_COLOR
#define NW_RED			GL_RED
#define NW_GREEN		GL_GREEN
#define NW_BLUE			GL_BLUE
#define NW_ALPHA		GL_ALPHA
#define NW_RGB			GL_RGB
#define NW_RGBA			GL_RGBA
#define NW_RGBA8		GL_RGBA8
// ========</Colors_CN>========

// ========<Textures_OGL>========
// --Types
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x0DE2
// --Types

// --Filters
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
// --Filters

// --Wrappers
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
// --Wrappers
// ========</Textures_OGL>========

// ========<Textures_OGL>========
// --Types
#define NW_TEXTURE_1D	GL_TEXTURE_1D
#define NW_TEXTURE_2D	GL_TEXTURE_2D
#define NW_TEXTURE_3D	GL_TEXTURE_3D
// --Types

// --Filters
#define NW_TEXTURE_MAG_FILTER			GL_TEXTURE_MAG_FILTER
#define NW_TEXTURE_MIN_FILTER			GL_TEXTURE_MIN_FILTER
#define NW_TEXTURE_FILTER_NEAREST		GL_NEAREST
#define NW_TEXTURE_FILTER_LINEAR		GL_LINEAR
// --Filters

// --Wrappers
#define NW_TEXTURE_WRAP_S			GL_TEXTURE_WRAP_S
#define NW_TEXTURE_WRAP_T			GL_TEXTURE_WRAP_T
#define NW_TEXTURE_WRAP_R			GL_TEXTURE_WRAP_R
#define NW_TEXTURE_WRAP_CLAMP		GL_CLAMP
#define NW_TEXTURE_WRAP_REPEAT		GL_REPEAT
// --Wrappers
// ========</Textures_OGL>========

// ========<GBuffers_OGL>========
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
// ========</GBuffers_OGL>========

// ========<Buffers_OGL>========
#define GL_COLOR_BUFFER_BIT					0x00004000
#define GL_DEPTH_BUFFER_BIT					0x00000100
#define GL_ACCUM_BUFFER_BIT					0x00000200
#define GL_STENCIL_BUFFER_BIT				0x00000400
// ========</Buffers_OGL>========


// ========<GBuffers_CN>========
#define NW_GBUFFER_VERTEX GL_ARRAY_BUFFER
#define NW_GBUFFER_INDEX GL_ELEMENT_ARRAY_BUFFER
// ========</GBuffers_CN>========

// ========<Buffers_CN>========
#define NW_BUFFER_COLOR_BIT					GL_COLOR_BUFFER_BIT
#define NW_BUFFER_DEPTH_BIT					GL_DEPTH_BUFFER_BIT
#define NW_BUFFER_STENCIL_BIT				GL_STENCIL_BUFFER_BIT
// ========</Buffers_CN>========

// ========<DataLoadTypes_OGL>========
#define GL_STATIC_DRAW 0x88E4
#define GL_STREAM_DRAW 0x88E0
#define GL_DYNAMIC_DRAW 0x88E8
// ========</DataLoadTypes_OGL>========

// ========<DataLoadTypes_CN>========
#define NW_STATIC_DRAW		GL_STATIC_DRAW 
#define NW_STREAM_DRAW		GL_STREAM_DRAW 
#define NW_DYNAMIC_DRAW		GL_DYNAMIC_DRAW
// ========</DataLoadTypes_CN>========

// ========<ErrorCodes_OGL>========
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504
#define GL_OUT_OF_MEMORY                  0x0505
// ========</ErrorCodes_OGL>========

// ========<ErrorCodes_CN>========
#define NW_OK							GL_NO_ERROR         
#define NW_INVALID_ENUM					GL_INVALID_ENUM     
#define NW_INVALID_VALUE				GL_INVALID_VALUE    
#define NW_INVALID_OPERATION			GL_INVALID_OPERATION
#define NW_STACK_OVERFLOW				GL_STACK_OVERFLOW   
#define NW_STACK_UNDERFLOW				GL_STACK_UNDERFLOW  
#define NW_OUT_OF_MEMORY				GL_OUT_OF_MEMORY  

#define NW_ERR_DATA_LOSS				0b1110'0000'0000

#define NW_ERR_NO_SUPPORT				0b1110'0000'0001
#define NW_ERR_NO_DATA					0b1110'0000'0001
#define NW_ERR_NO_INIT					0b1110'0000'0010
#define NW_ERR_NO_QUIT					0b1110'0000'0011
#define NW_ERR_NO_STATE					0b1110'0000'0100

#define NW_ERR_UNKNOWN_ID				0b1111'0000'0000

#define NW_ERR_SHADER_INVALID_FORMAT	0b0011'0000'0000
#define NW_ERR_SHADER_LOAD_PATH			0b1000'0000'0000
#define NW_ERR_SHADER_LOAD_FAIL			0b1001'0000'0000
#define NW_ERR_SHADER_LINK				0b1011'0000'0000
#define NW_ERR_SHADER_COMPILE			0b1100'0000'0000
// ========</ErrorCodes_CN>========

// ========<Info_OGL>========
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
// ========</Info_OGL>========
#elif (NW_GRAPHICS & NW_GRAPHICS_DX)
// Nothing for now... 08.11.20
#endif // NW_GRAPHICS

#endif // NATIVE_WORLD_DEFINES