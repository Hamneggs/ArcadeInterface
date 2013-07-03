#ifndef APP_CONSTANTS_INCLUDED
#define APP_CONSTANTS_INCLUDED

/*
These definitions are the names of the attributes in the UI 
config file, as well as the config file path.
Stay safe and don't fuck with either.
*/

#define CONFIG_FILE_PATH "assets\\config\\ui.xml"
//=====================================================
#define LOAD_NODE			"loading"
#define EXT_SHD_ATTR		"ext_shaders"
#define EXT_TILE_ATTR		"ext_tiles"

#define SND_NODE			"sound"
#define BGM_ATTR			"mus_path"
#define M_SND_ATTR			"move_path"
#define S_SND_ATTR			"sel_path"
#define R_SND_ATTR			"rej_path"
#define VOL_ATTR			"vol"
#define BGM_VOL_ATTR		"mus_vol"
#define M_VOL_ATTR			"move_vol"
#define S_VOL_ATTR			"sel_vol"
#define R_VOL_ATTR			"rej_vol"

#define WIN_NODE			"window"
#define X_RES_ATTR			"x_res"
#define Y_RES_ATTR			"y_res"
#define WIN_X_ATTR			"window_x"
#define WIN_Y_ATTR			"window_y"
#define FS_ATTR				"fullscreen"
#define CLR_R_ATTR			"clear_r"
#define CLR_G_ATTR			"clear_g"
#define CLR_B_ATTR			"clear_b"
#define TITLE_ATTR			"title"

#define TEX_NODE			"vital_tex_paths"
#define FRAME_TEX_ATTR		"frame"
#define EXIT_ATTR			"exit"
#define OVR_ATTR			"overlay"
#define INTR_ATTR			"intro"

#define SHD_NODE			"shader_paths"
#define BGV_ATTR			"bg_v"
#define BGF_ATTR			"bg_f"
#define HUDV_ATTR			"hud_v"
#define HUDF_ATTR			"hud_f"
#define TILEV_ATTR			"tile_v"
#define TILEF_ATTR			"tile_f"

#define ANIM_NODE			"anim"
#define ANIM_FRAMES_ATTR	"anim_frames"
#define PL_INTR_ATTR		"play_intro"
#define SDT_ATTR			"shutdown_time"

#define TILE_ATTR_NODE		"tile_loading"
#define NN_ATTR				"nn"
#define AP_ATTR				"ap"
#define IP_ATTR				"ip"
#define BP_ATTR				"bp"
#define N_ATTR				"n"
#define C_ATTR				"c"
#define D_ATTR				"d"
#define I_ATTR				"i"
#define NI_ATTR				"ni"
#define SI_ATTR				"si"
#define EI_ATTR				"ei"
#define WI_ATTR				"wi"
#define HF_ATTR				"hf"
#define IS_ATTR				"is"
#define X_ATTR				"x"
#define Y_ATTR				"y"
#define SXA_ATTR			"sxa"
#define SYA_ATTR			"sya"
#define SXI_ATTR			"sxi"
#define SYI_ATTR			"syi"
#define SELABLE_ATTR		"s"

#define CONFIG_NODE			"tile_config"
#define PATH_ATTR			"path"

#define BUTTON_NODE			"control_config"
#define SDK_A_ATTR			"shutdown_key_a"
#define SDK_B_ATTR			"shutdown_key_b"
#define SEL_KEY_ATTR		"select_key"
#define WIN_ATTR			"window_key"
#define SDK_F_ATTR			"fast_shutdown_key"
#define NEXT_ATTR			"next_config_key"
#define PREV_ATTR			"prev_config_key"

#endif