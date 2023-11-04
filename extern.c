/*
 * global variable initializaton
 *
 * @(#)extern.c	4.82 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <curses.h>
#include "rogue.h"

bool after;				/* True if we want after daemons */
bool again;				/* Repeating the last command */
int  noscore;				/* Was a wizard sometime */
bool seenstairs;			/* Have seen the stairs (for lsd) */
bool amulet = FALSE;			/* He found the amulet */
bool door_stop = FALSE;			/* Stop running when we pass a door */
bool fight_flush = FALSE;		/* True if toilet input */
bool firstmove = FALSE;			/* First move after setting door_stop */
bool got_ltc = FALSE;			/* We have gotten the local tty chars */
bool has_hit = FALSE;			/* Has a "hit" message pending in msg */
bool in_shell = FALSE;			/* True if executing a shell */
bool inv_describe = TRUE;		/* Say which way items are being used */
bool jump = FALSE;			/* Show running as series of jumps */
bool kamikaze = FALSE;			/* to_death really to DEATH */
bool lower_msg = FALSE;			/* Messages should start w/lower case */
bool move_on = FALSE;			/* Next move shouldn't pick up items */
bool msg_esc = FALSE;			/* Check for ESC from msg's --More-- */
bool passgo = FALSE;			/* Follow passages */
bool playing = TRUE;			/* True until he quits */
bool q_comm = FALSE;			/* Are we executing a 'Q' command? */
bool running = FALSE;			/* True if player is running */
bool save_msg = TRUE;			/* Remember last msg */
bool see_floor = TRUE;			/* Show the lamp illuminated floor */
bool stat_msg = FALSE;			/* Should status() print as a msg() */
bool terse = FALSE;			/* True if we should be short */
bool to_death = FALSE;			/* Fighting is to the death! */
bool tombstone = TRUE;			/* Print out tombstone at end */
#ifdef MASTER
int wizard = FALSE;			/* True if allows wizard commands */
#endif
bool pack_used[26] = {			/* Is the character used in the pack? */
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE
};

char dir_ch;				/* Direction from last get_dir() call */
char file_name[MAXSTR];			/* Save file name */
char huh[MAXSTR];			/* The last message printed */
char *p_colors[MAXPOTIONS];		/* Colors of the potions */
char prbuf[2*MAXSTR];			/* buffer for sprintfs */
char *r_stones[MAXRINGS];		/* Stone settings of the rings */
char runch;				/* Direction player is running */
char *s_names[MAXSCROLLS];		/* Names of the scrolls */
char take;				/* Thing she is taking */
char whoami[MAXSTR];			/* Name of player */
char *ws_made[MAXSTICKS];		/* What sticks are made of */
char *ws_type[MAXSTICKS];		/* Is it a wand or a staff */
int  orig_dsusp;			/* Original dsusp char */
char fruit[MAXSTR] =			/* Favorite fruit */
		"coepotısa";
char home[MAXSTR] = { '\0' };		/* User's home directory */
char *inv_t_name[] = {
	"gao",
	"ruoq",
	"poe"
};
char l_last_comm = '\0';		/* Last last_comm */
char l_last_dir = '\0';			/* Last last_dir */
char last_comm = '\0';			/* Last command typed */
char last_dir = '\0';			/* Last direction given */
char *tr_name[] = {			/* Names of the traps */
	"sá deaqkıao",
	"sá heruokea",
	"sá zıa bâq nuosaı",
	"sá puejıaıchuo",
	"sá pıkafachuo",
	"sá kea hıeq bâq jıeqbeaq bẹhıaosaı",
	"sá kea ca lohagupu",
        "sá duajua cheduchuo"
};


int n_objs;				/* # items listed in inventory() call */
int ntraps;				/* Number of traps on this level */
int hungry_state = 0;			/* How hungry is he */
int inpack = 0;				/* Number of things in pack */
int inv_type = 0;			/* Type of inventory to use */
int level = 1;				/* What level she is on */
int max_hit;				/* Max damage done to her in to_death */
int max_level;				/* Deepest player has gone */
int mpos = 0;				/* Where cursor is on top line */
int no_food = 0;			/* Number of levels without food */
int a_class[MAXARMORS] = {		/* Armor class for each armor type */
	8,	/* LEATHER */
	7,	/* RING_MAIL */
	7,	/* STUDDED_LEATHER */
	6,	/* SCALE_MAIL */
	5,	/* CHAIN_MAIL */
	4,	/* SPLINT_MAIL */
	4,	/* BANDED_MAIL */
	3,	/* PLATE_MAIL */
};

int count = 0;				/* Number of times to repeat command */
FILE *scoreboard = NULL;	/* File descriptor for score file */
int food_left;				/* Amount of food in hero's stomach */
int lastscore = -1;			/* Score before this turn */
int no_command = 0;			/* Number of turns asleep */
int no_move = 0;			/* Number of turns held in place */
int purse = 0;				/* How much gold he has */
int quiet = 0;				/* Number of quiet turns */
int vf_hit = 0;				/* Number of time flytrap has hit */

int dnum;				/* Dungeon number */
int seed;				/* Random number seed */
int e_levels[] = {
        10L,
	20L,
	40L,
	80L,
       160L,
       320L,
       640L,
      1300L,
      2600L,
      5200L,
     13000L,
     26000L,
     50000L,
    100000L,
    200000L,
    400000L,
    800000L,
   2000000L,
   4000000L,
   8000000L,
	 0L
};

coord delta;				/* Change indicated to get_dir() */
coord oldpos;				/* Position before last look() call */
coord stairs;				/* Location of staircase */

PLACE places[MAXLINES*MAXCOLS];		/* level map */

THING *cur_armor;			/* What he is wearing */
THING *cur_ring[2];			/* Which rings are being worn */
THING *cur_weapon;			/* Which weapon he is weilding */
THING *l_last_pick = NULL;		/* Last last_pick */
THING *last_pick = NULL;		/* Last object picked in get_item() */
THING *lvl_obj = NULL;			/* List of objects on this level */
THING *mlist = NULL;			/* List of monsters on the level */
THING player;				/* His stats */
					/* restart of game */

WINDOW *hw = NULL;			/* used as a scratch window */

#define INIT_STATS { 16, 0, 1, 10, 12, "1x4", 12 }

struct stats max_stats = INIT_STATS;	/* The maximum for the player */

struct room *oldrp;			/* Roomin(&oldpos) */
struct room rooms[MAXROOMS];		/* One for each room -- A level */
struct room passages[MAXPASS] =		/* One for each passage */
{
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} }
};

#define ___ 1
#define XX 10
struct monster monsters[26] =
    {
/* Name		 CARRY	FLAG    str, exp, lvl, amr, hpt, dmg */
{ "akuatoro",	   0,	ISMEAN,	{ XX, 20,   5,   2, ___, "0x0/0x0" } },
{ "muqshoaı",	   0,	ISFLY,	{ XX,  1,   1,   3, ___, "1x2" } },
{ "ekureq",	  15,	0,	{ XX, 17,   4,   4, ___, "1x2/1x5/1x5" } },
{ "daragoq",	 100,	ISMEAN,	{ XX,5000, 10,  -1, ___, "1x8/1x8/3x10" } },
{ "suaıpue",	   0,	ISMEAN,	{ XX,  2,   1,   7, ___, "1x2" } },
{ "nıu nueqchuq", 0,	ISMEAN,	{ XX, 80,   8,   3, ___, "%%%x0" } },
	/* NOTE: the damage is %%% so that xstr won't merge this */
	/* string with others, since it is written on in the program */
{ "akuılabı",	  20,	ISMEAN|ISFLY|ISREGEN, { XX,2000, 13,   2, ___, "4x3/3x5" } },
{ "aıpo",	   0,	ISMEAN,	{ XX,  3,   1,   5, ___, "1x8" } },
{ "chuaı'ıoma",   0,	0,	{ XX,  5,   1,   9, ___, "0x0" } },
{ "jababokı",   70,	0,	{ XX,3000, 15,   6, ___, "2x12/2x4" } },
{ "peluka",	   0,	ISMEAN|ISFLY,	{ XX,  1,   1,   7, ___, "1x4" } },
{ "leperekaq",	   0,	0,	{ XX, 10,   3,   8, ___, "1x1" } },
{ "medusa",	  40,	ISMEAN,	{ XX,200,   8,   2, ___, "3x4/3x4/2x5" } },
{ "lıqpa",	 100,	0,	{ XX, 37,   3,   9, ___, "0x0" } },
{ "oroko",	  15,	ISGREED,{ XX,  5,   1,   6, ___, "1x8" } },
{ "aıpu",	   0,	ISINVIS,{ XX,120,   8,   3, ___, "4x4" } },
{ "gıa'eku",	   0,	ISMEAN,	{ XX, 15,   3,   3, ___, "1x5/1x5" } },
{ "korotaloq",   0,	ISMEAN,	{ XX,  9,   2,   3, ___, "1x6" } },
{ "oguı",	   0,	ISMEAN,	{ XX,  2,   1,   5, ___, "1x3" } },
{ "turula",	  50,	ISREGEN|ISMEAN,{ XX, 120, 6, 4, ___, "1x8/1x8/2x6" } },
{ "shıtuaq kuo", 0,	ISMEAN,	{ XX,190,   7,  -2, ___, "1x9/1x9/2x9" } },
{ "ꝡampırı",	  20,	ISREGEN|ISMEAN,{ XX,350,   8,   1, ___, "1x10" } },
{ "mıenua",	   0,	0,	{ XX, 55,   5,   4, ___, "1x6" } },
{ "aıkopı",	  30,	0,	{ XX,100,   7,   7, ___, "4x4" } },
{ "koahako",	  30,	0,	{ XX, 50,   4,   6, ___, "1x6/1x6" } },
{ "zombı",	   0,	ISMEAN,	{ XX,  6,   2,   8, ___, "1x8" } }
    };
#undef ___
#undef XX

struct obj_info things[NUMTHINGS] = {
    { 0,			26 },	/* potion */
    { 0,			36 },	/* scroll */
    { 0,			16 },	/* food */
    { 0,			 7 },	/* weapon */
    { 0,			 7 },	/* armor */
    { 0,			 4 },	/* ring */
    { 0,			 4 },	/* stick */
};

struct obj_info arm_info[MAXARMORS] = {
    { "leoqfuq puga",		 20,	 20, NULL, FALSE },
    { "lohakoaı",		 15,	 25, NULL, FALSE },
    { "leoqfuq puga caı",	 15,	 20, NULL, FALSE },
    { "leoqfuq tıqsa",		 13,	 30, NULL, FALSE },
    { "lohakoaı poaqnoa",		 12,	 75, NULL, FALSE },
    { "lohakoaı gıamıa",		 10,	 80, NULL, FALSE },
    { "lohakoaı tuoq",		 10,	 90, NULL, FALSE },
    { "lohakoaı suoıtuoq",		  5,	150, NULL, FALSE },
};
struct obj_info pot_info[MAXPOTIONS] = {
    { "ca bıao",		 7,   5, NULL, FALSE },
    { "ca gaıchıaı",		 8,   5, NULL, FALSE },
    { "hıaosaı",			 8,   5, NULL, FALSE },
    { "ca jeaq caı",		13, 150, NULL, FALSE },
    { "ca mıa kaqdeq",		 3, 100, NULL, FALSE },
    { "ca roe",		13, 130, NULL, FALSE },
    { "ca gaı bâq paıgıq",	 6, 130, NULL, FALSE },
    { "ca gaı bâq majı",	 6, 105, NULL, FALSE },
    { "ca jeaq joe",		 2, 250, NULL, FALSE },
    { "ca jaq roe",		 5, 200, NULL, FALSE },
    { "ca suaı",		 5, 190, NULL, FALSE },
    { "ca rıu caı",	13, 130, NULL, FALSE },
    { "ca kaqcıa",		 5,   5, NULL, FALSE },
    { "ca rıolıaı",		 6,  75, NULL, FALSE },
};
struct obj_info ring_info[MAXRINGS] = {
    { "leoq",		 9, 400, NULL, FALSE },
    { "ca jeaq caı",		 9, 400, NULL, FALSE },
    { "ca jou caı",	 5, 280, NULL, FALSE },
    { "ca chıaq",		10, 420, NULL, FALSE },
    { "ca mıa kaqdeq",		10, 310, NULL, FALSE },
    { "ca de",		 1,  10, NULL, FALSE },
    { "feıca bâq paıgıq",	10,  10, NULL, FALSE },
    { "ca muqjoe",		 8, 440, NULL, FALSE },
    { "ca hıaocaı",	 8, 400, NULL, FALSE },
    { "ca suaı roesho",		 4, 460, NULL, FALSE },
    { "ca chuqkuaımeoq",		 9, 240, NULL, FALSE },
    { "ca pıkafa",		 5,  30, NULL, FALSE },
    { "ca shuıruo",		 7, 470, NULL, FALSE },
    { "leoq bâq leoqfuq",		 5, 380, NULL, FALSE },
};
struct obj_info scr_info[MAXSCROLLS] = {
    { "bıaoca bâq paıgıq",		 7, 140, NULL, FALSE },
    { "guaqfuaq majı",			 4, 150, NULL, FALSE },
    { "jıaı bâq paıgıq",			 2, 180, NULL, FALSE },
    { "tua nuo",				 3,   5, NULL, FALSE },
    { "gıjeaqtua bâq leoqfuq",			 7, 160, NULL, FALSE },
    { "ca gaımuo bâq nuımchaı",		10,  80, NULL, FALSE },
    { "ca gaımuo bâq majıpeq",		10,  80, NULL, FALSE },
    { "ca gaımuo bâq hıaochuo",		 6,  80, NULL, FALSE },
    { "ca gaımuo bâq leoqfuq",		 	 7, 100, NULL, FALSE },
    { "ca gaımuo bâq majıraı",	10, 115, NULL, FALSE },
    { "teaca bâq paıgıq",			 3, 200, NULL, FALSE },
    { "ca gaı bâq haq",			 2,  60, NULL, FALSE },
    { "ca pıkafa",			 5, 165, NULL, FALSE },
    { "majıdo bâq hıaochuo",			 8, 150, NULL, FALSE },
    { "zaqtua bâq paıgıq",			 4,  75, NULL, FALSE },
    { "nua bâq huımajı",			 7, 105, NULL, FALSE },
    { "feıca bâq paıgıq",		 3,  20, NULL, FALSE },
    { "leoq bâq leoqfuq",			 2, 250, NULL, FALSE },
};
struct obj_info weap_info[MAXWEAPONS + 1] = {
    { "ujoq",				11,   8, NULL, FALSE },
    { "buaısheq",			11,  15, NULL, FALSE },
    { "hıeqnuao",			12,  15, NULL, FALSE },
    { "heruo",				12,   1, NULL, FALSE },
    { "nuısheq",				 8,   3, NULL, FALSE },
    { "gumuqsheq",		10,  75, NULL, FALSE },
    { "nuıjıeqbeaq",				12,   2, NULL, FALSE },
    { "shurıkeq",			12,   5, NULL, FALSE },
    { "jıeqbeaq",				12,   5, NULL, FALSE },
    { NULL, 0 },	/* DO NOT REMOVE: fake entry for dragon's breath */
};
struct obj_info ws_info[MAXSTICKS] = {
    { "gıozıa",			12, 250, NULL, FALSE },
    { "tua kaqkıubu",		 6,   5, NULL, FALSE },
    { "zıa bâq seoqceoq",		 3, 330, NULL, FALSE },
    { "zıa bâq loe",			 3, 330, NULL, FALSE },
    { "zıa bâq koesoaı",			 3, 330, NULL, FALSE },
    { "ca tuaıbeo",		15, 310, NULL, FALSE },
    { "zıa bâq majısoaı",		10, 170, NULL, FALSE },
    { "suaıca bâq paıgıq",		10,   5, NULL, FALSE },
    { "meoqca bâq paıgıq",		11, 350, NULL, FALSE },
    { "nua bâq mıene",		 9, 300, NULL, FALSE },
    { "doecıa",		 1,   5, NULL, FALSE },
    { "ca pıkafa",		 6, 340, NULL, FALSE },
    { "ca nıjuıfa",		 6,  50, NULL, FALSE },
    { "ca majıcıa",		 5, 280, NULL, FALSE },
};

struct h_list helpstr[] = {
    {'?',	"	lä noaq já sóakue",				TRUE},
    {'/',	"	lä sı já sá tıjao",			TRUE},
    {'h',	"	lä fa lô hore já",					TRUE},
    {'j',	"	lä fa lô namı já",					TRUE},
    {'k',	"	lä fa lô bero já",					TRUE},
    {'l',	"	lä fa lô hochu já",					TRUE},
    {'y',	"	lä fa lô berohore já",				TRUE},
    {'u',	"	lä fa lô berohochu já",				TRUE},
    {'b',	"	lä fa lô namıhore já",				TRUE},
    {'n',	"	lä fa lô namıhochu já",				TRUE},
    {'H',	"	lä buaıfa lô hore já",				FALSE},
    {'J',	"	lä buaıfa lô namı já",				FALSE},
    {'K',	"	lä buaıfa lô bero já",				FALSE},
    {'L',	"	lä buaıfa lô hochu já",				FALSE},
    {'Y',	"	lä buaıfa lô berohore já",				FALSE},
    {'U',	"	lä buaıfa lô berohochu já",			FALSE},
    {'B',	"	lä buaıfa lô namıhore já",			FALSE},
    {'N',	"	lä buaıfa lô namıhochu já",			FALSE},
    {CTRL('H'),	"	lä fa lô hore já chôe báq ẹleaq",		FALSE},
    {CTRL('J'),	"	lä fa lô namı já chôe báq ẹleaq",		FALSE},
    {CTRL('K'),	"	lä fa lô bero já chôe báq ẹleaq",			FALSE},
    {CTRL('L'),	"	lä fa lô right já chôe báq ẹleaq",		FALSE},
    {CTRL('Y'),	"	lä fa lô berohore já chôe báq ẹleaq",		FALSE},
    {CTRL('U'),	"	lä fa lô beroright já chôe báq ẹleaq",		FALSE},
    {CTRL('B'),	"	lä fa lô namıhore já chôe báq ẹleaq",	FALSE},
    {CTRL('N'),	"	lä fa lô namıhochu já chôe báq ẹleaq",	FALSE},
    {'\0',	"	SHIFT+feo: lä buaıfa já",		TRUE},
    {'\0',	"	CTRL+feo: lä fa já chôe báq ẹleaq",	TRUE},
    {'f',	"<feo>	lä soı já chôe ꝡä muaqjuı ra muaq já",	TRUE},
    {'t',	"<feo>	lä hıeq já sá",			TRUE},
    {'m',	"<feo>	lä koı já bujôı ꝡä heaqsho já sá",		TRUE},
    {'z',	"<feo>	lä choq já báq majıbeaq",		TRUE},
    {'^',	"<feo>	lä kaqgaı já báq cheduchuo",			TRUE},
    {'s',	"	lä joaı já báq cheduchuo ra kıao",		TRUE},
    {'>',	"	lä guqfa já chôq báq gaocoa",			TRUE},
    {'<',	"	lä geafa ja chôq báq gaocoa",			TRUE},
    {'.',	"	lá toaıtao sea",			TRUE},
    {',',	"	lá heaqsho",			TRUE},
    {'i',	"	lá mıeq heaqshuo",				TRUE},
    {'I',	"	lá sı bâq bọheaq shı",			TRUE},
    {'q',	"	lá pıe bâq nuımchaı",				TRUE},
    {'r',	"	lá noaq bâq peq",				TRUE},
    {'e',	"	lá chuq bâq haq",				TRUE},
    {'w',	"	lá jıaı bâq hıaochuo",			TRUE},
    {'W',	"	lá geısho bâq fuq",				TRUE},
    {'T',	"	lá geıshaı bâq fuq",			TRUE},
    {'P',	"	lá geısho bâq cheılıem",				TRUE},
    {'R',	"	lá geıshaı bâq cheılıem",				TRUE},
    {'d',	"	lá heaqshaı",				TRUE},
    {'c',	"	lá chuado",				TRUE},
    {'a',	"	lä nıqguo tao já híqsoq",			TRUE},
    {')',	"	lä naı jıaı ja hí",			TRUE},
    {']',	"	lä naı geı ja hí fuq",			TRUE},
    {'=',	"	lä naı geı ja hí cheılıem",			TRUE},
    {'@',	"	lä mıeq já báq raq sûq",			TRUE},
    {'D',	"	lä moaqtao já báq se duasho",		TRUE},
    {'o',	"	lä gaı ra beotua já báq beodaı",			TRUE},
    {CTRL('R'),	"	ꝡä nıqguo fuaqkaı jí túq",				TRUE},
    {CTRL('P'),	"	lä nıqguo sı já híqsoq kuqse",			TRUE},
    {ESCAPE,	"	lä bota já sá",			TRUE},
    {'S',	"	ꝡä shuotao jí jíaq",				TRUE},
    {'Q',	"	lá shaı luaq",					TRUE},
    {'!',	"	lä rıufa já shéll",				TRUE},
    {'F',	"<feo>	lä soı já chôe ꝡä muaqsho sá",		TRUE},
    {'v',	"	lä eq ní hí bashoq",			TRUE},
    {0,		NULL }
};
