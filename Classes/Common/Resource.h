#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#define MATRIX_WIDTH (9)
#define MATRIX_HEIGHT (9)
#define SUSHI_GAP (0)
#define SUSHI_COUNT (6)
#define GRID_COUNT (3)

#define BASE_DB_IS_INITED "BASE_DB_IS_INITED"

#define TABLE_USER_BASIC_DATA "basic_data"
#define TABLE_ROUND_DATA "round_data"

#define USER_NAME "testuser1"

// Start scene
const char s_startBackground[] = "play_start_start_back_normal.png";
const char s_startNormal[] = "play_start_start_btn_normal.png";
const char s_startSelected[] = "play_start_start_btn_selected.png";
const char s_musicNormal[] = "play_start_music_btn_normal.png";
const char s_musicSelected[] = "play_start_music_btn_selected.png";
const char s_soundNormal[] = "play_start_sound_btn_normal.png";
const char s_soundSelected[] = "play_start_sound_btn_selected.png";
const char s_rateNormal[] = "play_start_rate_btn_normal.png";
const char s_rateSelected[] = "play_start_rate_btn_selected.png";

// Guide map scene
const char s_guideMap[] = "guideMap.jpg";
const char s_roundButtonsNormal[][24] = {
										  "round1ButtonNormal.png",
										  "round2ButtonNormal.png",
										  "round3ButtonNormal.png",
										  "round4ButtonNormal.png",
										  "round5ButtonNormal.png",
										  "round6ButtonNormal.png",
										  "round7ButtonNormal.png",
										  "round8ButtonNormal.png",
										  "round9ButtonNormal.png",
										  "round10ButtonNormal.png" };
const char s_roundButtonsSelected[][26] = { "round1ButtonSelected.png",
											"round2ButtonSelected.png",
											"round3ButtonSelected.png",
											"round4ButtonSelected.png",
											"round5ButtonSelected.png",
											"round6ButtonSelected.png",
											"round7ButtonSelected.png",
											"round8ButtonSelected.png",
											"round9ButtonSelected.png",
											"round10ButtonSelected.png" };

// Play scene
const char s_pathLose[] = "play_main_lose_img_normal.png";
const char s_pathWin[] = "play_main_win_img_normal.png";
const char s_mainBackground[] = "play_main_background_back_normal.png";
const char s_backToStartButtonNormal[] = "common_backToStart_btn_normal.png";
const char s_backToStartButtonSelected[] = "common_backToStart_btn_selected.png";
const char s_moves[] = "play_moves_moves_img_normal.png";
const char s_target[] = "play_target_target_img_normal.png";
const char s_colorHRight[] = "play_play_colorHRight_img_normal.png";
const char s_colorLRight[] = "play_play_colorHLeft_img_normal.png";
const char s_colorVDown[] = "play_play_colorVDown_img_normal.png";
const char s_colorVUp[] = "play_play_colorVUp_img_normal.png";
const char s_circle[] = "play_play_circle_img_normal.png";
const char s_stars[] = "stars.plist";
const char s_postPlayBackground[] = "play_postPlay_back_back_normal.png";
const char s_postPlayResumeNormal[] = "common_resume_btn_normal.png";
const char s_postPlayResumeSelected[] = "common_resume_btn_selected.png";
const char s_postPlayNextNormal[] = "common_next_btn_normal.png";
const char s_postPlayNextSelected[] = "common_next_btn_selected.png";
const char s_postPlayListNormal[] = "common_list_btn_normal.png";
const char s_postPlayListSelected[] = "common_list_btn_selected.png";
const char s_prePlayBackground[] = "play_prePlay_back_back_normal.png";
const char s_starLeftDone[] = "common_starLeft_img_normal.png";
const char s_starMidDone[] = "common_starMid_img_normal.png";
const char s_starRightDone[] = "common_starRight_img_normal.png";
const char s_starLeftUndone[] = "common_starLeft_img_disable.png";
const char s_starMidUndone[] = "common_starMid_img_disable.png";
const char s_starRightUndone[] = "common_starRight_img_disable.png";
const char s_preLayerStartNormal[] = "common_start_btn_normal.png";
const char s_preLayerStartSelected[] = "common_start_btn_selected.png";
const char s_targetTipsBackground[] = "play_targetTips_target_img_normal.png";
const char s_gridNormal[] = "play_play_grid_normal.png";
const char s_gridJelly[] = "play_play_grid_jelly.png";
const char s_gridDoubleJelly[] = "play_play_grid_double_jelly.png";
const char s_gridCream[] = "play_play_grid_cream.png";
const char s_gridDoubleCream[] = "play_play_grid_double_cream.png";
const char s_sushiPlist[] = "sushi.plist";
const char s_sushiRes[] = "sushi.png";//"sushi.pvr.ccz";
const char s_resPlist[] = "res.plist";
const char s_absExploadCross[] = "common/common_starRight_img_normal.png";
const char s_scoreTarget[] = "score_target.png";
const char s_jellyTarget[] = "jelly_target.png";
const char s_doubleJellyTarget[] = "double_jelly_target.png";
const char s_creamTarget[] = "cream_target.png";
const char s_doubleCreamTarget[] = "double_cream_target.png";
const char s_sushi1Target[] = "sushi_1n_target.png";
const char s_sushi2Target[] = "sushi_2n_target.png";
const char s_sushi3Target[] = "sushi_3n_target.png";
const char s_sushi4Target[] = "sushi_4n_target.png";
const char s_sushi5Target[] = "sushi_5n_target.png";
const char s_sushi6Target[] = "sushi_6n_target.png";

// Common
const char s_commonConfig[] = "commonConfig.xml";

#endif
