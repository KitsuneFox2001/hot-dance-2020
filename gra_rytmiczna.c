/* HOT DANCE 2020 */
/* a DLC for Hot Dance 2000 by adrian09_01*/

#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "Sprites.h" // holds our metasprite data
#include "HotDance_Testing.h" //holds screen data
#include "HotDance_China.h" //holds screen data
#include "HotDance_Star.h" //holds screen data
#include "HotDance_Title.h" //holds screen data
#include "HotDance_Lose.h" //holds screen data
#include "HotDance_Win.h" //holds screen data

#define MAX_ARROWS 15
#define SCORE_DIGITS 6

#pragma bss-name(push, "ZEROPAGE")

//const unsigned char MAX_ARROWS = 15;
//const unsigned char SCORE_DIGITS = 6;
// LOOP STUFF
unsigned char i;

// GLOBAL VARIABLES
unsigned char first_arrow;
unsigned char second_arrow;
char none_enabled;
char game_mode;
char song_select;
char spawn_two;
char spawned;
char dance_meter;
int y_position[15];
unsigned char type[15];
unsigned char enabled[15];
char tempo_multiplier;
char arrow_spawn_timer;
char spawn_arrow_time;
char spawn_chance_regular;
char spawn_chance_double;
unsigned char pad1;
unsigned char pad1_new;
char score[6];
char last_score[6];

const unsigned char x_position1=12*8;
const unsigned char x_position2=14*8;
const unsigned char x_position3=16*8;
const unsigned char x_position4=18*8;

const unsigned char top_number_id[10] = {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99};
const unsigned char bot_number_id[10] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9};
const unsigned char arrow_types[4] = {PAD_LEFT, PAD_DOWN, PAD_UP, PAD_RIGHT};

int arrow_color;
int arrow_timer;

int pal_color;
int pal_timer;

char message_number;
char message_timer;


unsigned char y_pos_arrow;
unsigned char tempo_mult_d2;
unsigned char tempo_mult_t2;
unsigned char tempo_mult_t3;
unsigned char tempo_mult_t4;

unsigned char index2; // fix bug with i variable
unsigned char index3; // fix bug with i variable
unsigned char update_flag; // only update scoreboard once per frame

// leave the arrows on screen a little after hitting, feedback
unsigned char floaty_active; // timer counts down to 0
unsigned char floaty_y;
unsigned char floaty_type;
unsigned char floaty_active2; // timer counts down to 0
unsigned char floaty_y2;
unsigned char floaty_type2;


const unsigned char palette_bg_title[16]={ 0x0f,0x02,0x37,0x27,0x0f,0x15,0x25,0x05,0x0f,0x06,0x16,0x26,0x0f,0x3c,0x2c,0x1c };
const unsigned char palette_bg_star[16]={ 0x0f,0x27,0x17,0x07,0x0f,0x06,0x16,0x26,0x0f,0x06,0x16,0x26,0x0f,0x1a,0x2a,0x0f };
const unsigned char palette_bg_china[16]={ 0x0f,0x16,0x28,0x38,0x0f,0x19,0x29,0x39,0x0f,0x06,0x16,0x26,0x0f,0x06,0x16,0x06 };
const unsigned char palette_bg_heart[16]={ 0x0f,0x25,0x15,0x05,0x0f,0x01,0x11,0x21,0x0f,0x37,0x27,0x17,0x0f,0x21,0x3c,0x11 };
const unsigned char palette_bg_win[16]={ 0x0f,0x3a,0x2a,0x1a,0x0f,0x09,0x19,0x29,0x0f,0x06,0x16,0x26,0x0f,0x3c,0x2c,0x1c };
const unsigned char palette_bg_lose[16]={ 0x0f,0x16,0x26,0x06,0x0f,0x05,0x16,0x26,0x0f,0x06,0x16,0x26,0x0f,0x3c,0x2c,0x1c };

const unsigned char palette_sp[16]={ 0x0f,0x0c,0x1c,0x2c,0x0f,0x08,0x18,0x28,0x0f,0x0a,0x1a,0x2a,0x0f,0x06,0x16,0x26 };

void put_str(unsigned int adr,const char *str)
{
	while(1)
	{
		if(!*str) break;

		one_vram_buffer((*str++)-0x20, adr);//-0x20 because ASCII code 0x20 is placed in tile 0 of the CHR
		adr++;
	}
}

void render_statusbar() {
	//set_vram_buffer(); !!
	clear_vram_buffer();
	for (i=0; i<6; i++) {
		if (score[i] > 9) {
			if (i!=0) {
				score[i-1] = score[i-1] + 1;
				score[i] = 0;
			}			
		}
	};
	for (i=0; i<6; i++) {
		if (game_mode == 2) {
			one_vram_buffer(top_number_id[score[i]], NTADR_A(14+i, 15));
			one_vram_buffer(bot_number_id[score[i]], NTADR_A(14+i, 16));
			if (dance_meter > 8) {
				dance_meter = 8;
			}
			if (dance_meter > 4) {
				one_vram_buffer(0x0A+(8-dance_meter), NTADR_A(20, 15));
				one_vram_buffer(0x0A, NTADR_A(20, 16));
			}
			if (dance_meter <= 4) {
				one_vram_buffer(0x0E, NTADR_A(20, 15));
				one_vram_buffer(0x0A+(4-dance_meter), NTADR_A(20, 16));
			}
		}
		else {
			one_vram_buffer(top_number_id[score[i]], NTADR_A(14+i, 26));
			one_vram_buffer(bot_number_id[score[i]], NTADR_A(14+i, 27));
			if (dance_meter > 8) {
				dance_meter = 8;
			}
			if (dance_meter > 4) {
				one_vram_buffer(0x0A+(8-dance_meter), NTADR_A(20, 26));
				one_vram_buffer(0x0A, NTADR_A(20, 27));
			}
			if (dance_meter <= 4) {
				one_vram_buffer(0x0E, NTADR_A(20, 26));
				one_vram_buffer(0x0A+(4-dance_meter), NTADR_A(20, 27));
			}
		}
	}
	//ppu_wait_nmi(); // !!
}

void draw_arrows() {
	for (i=0; i<MAX_ARROWS; i++) {
		if (enabled[i] == 1) {
			if (arrow_color == 0) {
				if (type[i] == 0) {
					oam_meta_spr(x_position1, y_position[i], metasprite2);
				}
				if (type[i] == 1) {
					oam_meta_spr(x_position2, y_position[i], metasprite3);
				}
				if (type[i] == 2) {
					oam_meta_spr(x_position3, y_position[i], metasprite);
				}
				if (type[i] == 3) {
					oam_meta_spr(x_position4, y_position[i], metasprite4);
				}
			}
			if (arrow_color == 1) {
				if (type[i] == 0) {
					oam_meta_spr(x_position1, y_position[i], metasprite6);
				}
				if (type[i] == 1) {
					oam_meta_spr(x_position2, y_position[i], metasprite7);
				}
				if (type[i] == 2) {
					oam_meta_spr(x_position3, y_position[i], metasprite5);
				}
				if (type[i] == 3) {
					oam_meta_spr(x_position4, y_position[i], metasprite8);
				}
			}
			if (arrow_color == 2) {
				if (type[i] == 0) {
					oam_meta_spr(x_position1, y_position[i], metasprite10);
				}
				if (type[i] == 1) {
					oam_meta_spr(x_position2, y_position[i], metasprite11);
				}
				if (type[i] == 2) {
					oam_meta_spr(x_position3, y_position[i], metasprite9);
				}
				if (type[i] == 3) {
					oam_meta_spr(x_position4, y_position[i], metasprite12);
				}
			}
			if (arrow_color == 3) {
				if (type[i] == 0) {
					oam_meta_spr(x_position1, y_position[i], metasprite14);
				}
				if (type[i] == 1) {
					oam_meta_spr(x_position2, y_position[i], metasprite15);
				}
				if (type[i] == 2) {
					oam_meta_spr(x_position3, y_position[i], metasprite13);
				}	
				if (type[i] == 3) {
					oam_meta_spr(x_position4, y_position[i], metasprite16);
				}
			}
		}
	}
	
	if(floaty_active){
		if (floaty_type == 0) {
			oam_meta_spr(x_position1, floaty_y, metasprite14);
		}
		if (floaty_type == 1) {
			oam_meta_spr(x_position2, floaty_y, metasprite15);
		}
		if (floaty_type == 2) {
			oam_meta_spr(x_position3, floaty_y, metasprite13);
		}	
		if (floaty_type == 3) {
			oam_meta_spr(x_position4, floaty_y, metasprite16);
		}
		--floaty_active;
	}
	
	if(floaty_active2){
		if (floaty_type2 == 0) {
			oam_meta_spr(x_position1, floaty_y2, metasprite14);
		}
		if (floaty_type2 == 1) {
			oam_meta_spr(x_position2, floaty_y2, metasprite15);
		}
		if (floaty_type2 == 2) {
			oam_meta_spr(x_position3, floaty_y2, metasprite13);
		}	
		if (floaty_type2 == 3) {
			oam_meta_spr(x_position4, floaty_y2, metasprite16);
		}
		--floaty_active2;
	}
}


void make_floaty() {
	if(floaty_active == 0)
	{
		floaty_active = 10;
		floaty_y = y_pos_arrow;
		floaty_type = type[index2];
	}
	else
	{
		floaty_active2 = 10;
		floaty_y2 = y_pos_arrow;
		floaty_type2 = type[index2];
	}
}

void perfect_rating() {
	score[3] = score[3] + 2;
	score[4] = score[4] + 5;
	dance_meter = dance_meter + 1;
	//render_statusbar();
	++update_flag;
	message_number = 0;
	message_timer = 30;
	
	make_floaty();
}

void great_rating() {
	score[3] = score[3] + 1;
	dance_meter = dance_meter + 1;
	//render_statusbar();
	++update_flag;
	message_number = 1;
	message_timer = 30;
	
	make_floaty();
}

void good_rating() {
	score[4] = score[4] + 5;
	//render_statusbar();
	++update_flag;
	message_number = 2;
	message_timer = 30;
	
	make_floaty();
}

void boo_rating() {
	if(update_flag == 0){
		// don't do it double
		--dance_meter;
	}
	
	if (dance_meter == 0) {
		music_stop();
		for (i=0; i<MAX_ARROWS; i++) {
			enabled[i] = 0;
		}
	}
	//render_statusbar();
	++update_flag;
	message_number = 4;
	message_timer = 30;
	
	make_floaty();
}

void miss_rating() {
	--dance_meter; // this will be double if 2 arrows.
	
	if (dance_meter == 0) {
		music_stop();
		for (i=0; i<MAX_ARROWS; i++) {
			enabled[i] = 0;
		}
		arrow_spawn_timer = 0; // bug fix
	}
	render_statusbar();
	message_number = 3;
	message_timer = 30;
}

void pad_poll_single() {
	pad1 = pad_poll(0); // read the first controller
	pad1_new = get_pad_new(0);
}

void press_arrows() {
	pad_poll_single(); // only do this once a frame
	update_flag = 0; // prevent double update
	
	// i was causing bug, render score over writes it
	for (index2=0; index2<MAX_ARROWS; index2++) {
		if (enabled[index2] == 1) {
			
			y_pos_arrow = y_position[index2]; // cast to unsigned char
			
			//max is xf + 2*4 = x15
			//skip everything if arrow > target area
			if(y_pos_arrow > 0x18) continue;
			
			//precalculate some things, speeds it up
			tempo_mult_d2 = 1;
			tempo_mult_t2 = 5;
			tempo_mult_t3 = 9;
			tempo_mult_t4 = 13;
			
			//perfect
			if (y_pos_arrow >= 0x0F-tempo_mult_d2 && y_pos_arrow <= 0x0F+tempo_mult_d2) {
				//pad_poll_single();
				if (pad1_new & arrow_types[type[index2]]) { // pad1
					enabled[index2] = 0;
					perfect_rating();
				}
			}
			//great
			else if (y_pos_arrow >= 0x0F-tempo_mult_t2 && y_pos_arrow <= 0x0F+tempo_mult_t2) {
				//pad_poll_single();
				if (pad1_new & arrow_types[type[index2]]) { // pad1
					enabled[index2] = 0;
					great_rating();
				}
			}
			//good
			else if (y_pos_arrow >= 0x0F-tempo_mult_t3 && y_pos_arrow <= 0x0F+tempo_mult_t3) {
				//pad_poll_single();
				if (pad1_new & arrow_types[type[index2]]) { // pad1
					enabled[index2] = 0;
					good_rating();
				}
			}
			//boo
			else if (y_pos_arrow >= 0x0F-tempo_mult_t4 && y_pos_arrow <= 0x0F+tempo_mult_t4) {
				//pad_poll_single();
				if (pad1_new & arrow_types[type[index2]]) { // pad1
					enabled[index2] = 0;
					boo_rating();
				}
			}
		}
	}
	
	if(update_flag)
	{
		render_statusbar();
	}
	
}

void update_arrows() {
	for (index3=0; index3<MAX_ARROWS; index3++) {
		if (enabled[index3] == 1) {
			y_position[index3] = y_position[index3] - tempo_multiplier;
			if (y_position[index3] <= 0) {
				enabled[index3] = 0;
				miss_rating();
			}
		}
	}
}

void initialize_win() {
	clear_vram_buffer();
	ppu_off(); // screen off
	
	// unrle the screen
	vram_adr(NAMETABLE_A);

	vram_unrle(HotDance_Win);
	pal_bg(palette_bg_win);

	ppu_on_all(); // turn on screen
	game_mode = 2;
	render_statusbar();
}

void initialize_lose() {
	clear_vram_buffer();
	ppu_off(); // screen off
	
	// unrle the screen
	vram_adr(NAMETABLE_A);

	vram_unrle(HotDance_Lose);
	pal_bg(palette_bg_lose);

	ppu_on_all(); // turn on screen
	game_mode = 2;
	render_statusbar();
}

void check_none_enabled() {
	none_enabled = 1;
	for (i=0; i<MAX_ARROWS; i++) {
		if (enabled[i] == 1) {
			none_enabled = 0;
		}
	}
	if (!check_nothing_played() && none_enabled) {
		if (dance_meter == 0) {
			initialize_lose();
		}
		if (dance_meter > 0) {
			initialize_win();
		}
	}
}
	
void spawn_arrow() {
	spawned = 0;
	if (check_nothing_played() != 0) {
		for (i=0; i<MAX_ARROWS; i++) {
			if (enabled[i] == 0) {
				if (spawned == 0) {
					// so the arrows are more consistent / in line
					// spawn_two = rand8() % spawn_chance_regular;
					if (1) { //(spawn_two == 1)
						spawned = 1;
						if (enabled[i] == 0) {
							enabled[i] = 1;
							first_arrow = rand8() & 3; //% 4;
							type[i] = first_arrow;
							y_position[i] = 0xF0;
						}
						spawn_two = rand8() & 3; //% spawn_chance_double;
						if (spawn_two == 1) {
								if (i<MAX_ARROWS-1) {
									if (enabled[i+1] == 0) {
										enabled[i+1] = 1;
										second_arrow = rand8() & 3; //% 4;
										// make sure can't be the SAME arrow either
										//while (first_arrow == 0 && second_arrow == 3) {
										//	second_arrow = rand8() % 4;
										//}
										//while (first_arrow == 3 && second_arrow == 0) {
										//	second_arrow = rand8() % 4;
										//}
										//while (first_arrow == 1 && second_arrow == 2) {
										//	second_arrow = rand8() % 4;
										//}
										//while (first_arrow == 2 && second_arrow == 1) {
										//	second_arrow = rand8() % 4;
										//}
										
										// 0 left, 1 down, 2 up, 3 right
										if(first_arrow == 0)
										{
											while((second_arrow == 0) || (second_arrow == 3))
											{
												second_arrow = rand8() & 3;
											}
										}
										else if(first_arrow == 1)
										{
											while((second_arrow == 1) || (second_arrow == 2))
											{
												second_arrow = rand8() & 3;
											}
										}
										else if(first_arrow == 2)
										{
											while((second_arrow == 1) || (second_arrow == 2))
											{
												second_arrow = rand8() & 3;
											}
										}
										else if(first_arrow == 3)
										{
											while((second_arrow == 0) || (second_arrow == 3))
											{
												second_arrow = rand8() & 3;
											}
										}
										
										
										type[i+1] = second_arrow;
										y_position[i+1] = 0xF0;
									}
								}
						}
					}
				}
			}
		}
	}
	spawned = 1;
}

void draw_message() {
	if (message_timer > 0) {
		if (message_number == 0) {
			oam_meta_spr(12*8, 4*8, ratings_perfect);
		}
		if (message_number == 1) {
			oam_meta_spr(14*8, 4*8, ratings_great);
		}
		if (message_number == 2) {
			oam_meta_spr(14*8, 4*8, ratings_good);
		}
		if (message_number == 3) {
			oam_meta_spr(14*8, 4*8, ratings_miss);
		}
		if (message_number == 4) {
			oam_meta_spr(15*8, 4*8, ratings_boo);
		}
	}
}

void update_game() {
	arrow_timer++;
	arrow_spawn_timer++;
	if (message_timer > 0) {
		message_timer--;
	}
	if (arrow_timer > 9) {
		arrow_color++;
		arrow_timer = 0;
	}
	if (arrow_color > 3) {
		arrow_color = 0;
	}
	if (arrow_spawn_timer >= spawn_arrow_time)
	{
		spawn_arrow();
		arrow_spawn_timer = 0;
	}
}

void initialize_game() {
	ppu_off(); // screen off
	
	floaty_active = 0;
	floaty_active2 = 0;

	
	// unrle the screen
	vram_adr(NAMETABLE_A);
	
	if (song_select == 0) {
	vram_unrle(HotDance_Star);
	pal_bg(palette_bg_star);
	}
	if (song_select == 1) {
	vram_unrle(HotDance_China);
	pal_bg(palette_bg_china);
	}
	if (song_select == 2) {
	vram_unrle(HotDance);
	pal_bg(palette_bg_heart);
	}
	
	dance_meter = 8;
	for (i=0; i<MAX_ARROWS; i++) {
		enabled[i] = 0;
	}
	for (i=0; i<SCORE_DIGITS; i++) {
		score[i] = 0;
		last_score[i] = 0;
	}
	arrow_spawn_timer = 0;
	
	oam_clear(); // added, fix 1 frame wrong arrow
	ppu_on_all(); // turn on screen
	game_mode = 1;
}

void update_menu() {
	pad_poll_single();
	if (pad1_new & PAD_SELECT) {
		sfx_play(0, 0);
		song_select = song_select + 1;
		music_play(song_select);
	}
	if (pad1_new & PAD_START) {
		sfx_play(0, 0);
		initialize_game();
		music_play(song_select);
	}
}

void initialize_menu() {
	clear_vram_buffer();
	ppu_off(); // screen off
	
	// unrle the screen
	vram_adr(NAMETABLE_A);

	vram_unrle(HotDance_Title);
	pal_bg(palette_bg_title);

	ppu_on_all(); // turn on screen
	game_mode = 0;
}

void update_and_draw_winlose() {
	if (arrow_color == 0) {
		oam_meta_spr(11*8, (18*8)-1, metasprite4);
	}
	if (arrow_color == 1) {
		oam_meta_spr(11*8, (18*8)-1, metasprite8);
	}
	if (arrow_color == 2) {
		oam_meta_spr(11*8, (18*8)-1, metasprite12);
	}
	if (arrow_color == 3) {
		oam_meta_spr(11*8, (18*8)-1, metasprite16);
	}
	arrow_timer++;
	if (arrow_timer > 9) {
		arrow_color++;
		arrow_timer = 0;
	}
	if (arrow_color > 3) {
		arrow_color = 0;
	}
	pad_poll_single();
	if (pad1_new & PAD_START) {
		sfx_play(0, 0);
		initialize_menu();
		music_play(song_select);
	}
}

void draw_menu() {
	if (arrow_color == 0) {
		if (song_select == 0) {
			oam_meta_spr(11*8, (12*8)-1, metasprite4);
		}
		if (song_select == 1) {
			oam_meta_spr(11*8, (15*8)-1, metasprite4);
		}
		if (song_select == 2) {
			oam_meta_spr(11*8, (18*8)-1, metasprite4);
		}
	}
	if (arrow_color == 1) {
		if (song_select == 0) {
			oam_meta_spr(11*8, (12*8)-1, metasprite8);
		}
		if (song_select == 1) {
			oam_meta_spr(11*8, (15*8)-1, metasprite8);
		}
		if (song_select == 2) {
			oam_meta_spr(11*8, (18*8)-1, metasprite8);
		}
	}
	if (arrow_color == 2) {
		if (song_select == 0) {
			oam_meta_spr(11*8, (12*8)-1, metasprite12);
		}
		if (song_select == 1) {
			oam_meta_spr(11*8, (15*8)-1, metasprite12);
		}
		if (song_select == 2) {
			oam_meta_spr(11*8, (18*8)-1, metasprite12);
		}
	}
	if (arrow_color == 3) {
		if (song_select == 0) {
			oam_meta_spr(11*8, (12*8)-1, metasprite16);
		}
		if (song_select == 1) {
			oam_meta_spr(11*8, (15*8)-1, metasprite16);
		}
		if (song_select == 2) {
			oam_meta_spr(11*8, (18*8)-1, metasprite16);
		}
	}
	arrow_timer++;
	if (message_timer > 0) {
		message_timer--;
	}
	if (arrow_timer > 9) {
		arrow_color++;
		arrow_timer = 0;
	}
	if (arrow_color > 3) {
		arrow_color = 0;
	}
	if (song_select > 2) {
		song_select = 0;
		music_play(0);
	}
}

void main (void) {
	
	ppu_off(); // screen off
	
	// unrle the screen
	vram_adr(NAMETABLE_A);
	vram_unrle(HotDance_Title);
	
	// load the palettes
	pal_bg(palette_bg_title);
	pal_spr(palette_sp);

	// use the second set of tiles for sprites
	// both bg and sprite are set to 0 by default
	oam_size(1);
	bank_spr(1);
	
	music_play(0);
	
	
	//initialize zeropage variables
	dance_meter = 8;
	tempo_multiplier = 2;
	spawn_arrow_time = 30;
	spawn_chance_regular = 2;
	spawn_chance_double = 2;
	
	set_vram_buffer();
	
	ppu_on_all(); // turn on screen
	
	while (1){
		// infinite loop
		//clear_vram_buffer(); // clear the buffer at the start of each frame
		ppu_wait_nmi(); // wait till beginning of the frame
		clear_vram_buffer(); // clear the buffer at the start of each frame
		// the sprites are pushed from a buffer to the OAM during nmi
		
		// clear all sprites from sprite buffer
		oam_clear();
		
		if (game_mode == 0) {
		draw_menu();
		update_menu();
		
		}
		
		if (game_mode == 1) {
		update_arrows();
		press_arrows();
		
		
		// flip flop order so you can see the arrows.
		if(get_frame_count() & 1){
			draw_message(); // draw first
			draw_arrows();
		}
		else{
			draw_arrows();
			draw_message(); // draw last
		}
		
		
		
		
		update_game();
		check_none_enabled();
		}
		
		if (game_mode == 2) {
		update_and_draw_winlose();
		}
		// note, y positions 0xef-0xff will not appear on the screen
	}
}
	
	