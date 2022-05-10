/*
 * global variable initializaton
 *
 * @(#)init.c	4.31 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <stdlib.h>
#include <curses.h>
#include <ctype.h>
#include <string.h>
#include "rogue.h"

/*
 * init_player:
 *	Roll her up
 */
void
init_player()
{
    register THING *obj;

    pstats = max_stats;
    food_left = HUNGERTIME;
    /*
     * Give him some food
     */
    obj = new_item();
    obj->o_type = FOOD;
    obj->o_count = 1;
    add_pack(obj, TRUE);
    /*
     * And his suit of armor
     */
    obj = new_item();
    obj->o_type = ARMOR;
    obj->o_which = RING_MAIL;
    obj->o_arm = a_class[RING_MAIL] - 1;
    obj->o_flags |= ISKNOW;
    obj->o_count = 1;
    cur_armor = obj;
    add_pack(obj, TRUE);
    /*
     * Give him his weaponry.  First a mace.
     */
    obj = new_item();
    init_weapon(obj, MACE);
    obj->o_hplus = 1;
    obj->o_dplus = 1;
    obj->o_flags |= ISKNOW;
    add_pack(obj, TRUE);
    cur_weapon = obj;
    /*
     * Now a +1 bow
     */
    obj = new_item();
    init_weapon(obj, BOW);
    obj->o_hplus = 1;
    obj->o_flags |= ISKNOW;
    add_pack(obj, TRUE);
    /*
     * Now some arrows
     */
    obj = new_item();
    init_weapon(obj, ARROW);
    obj->o_count = rnd(15) + 25;
    obj->o_flags |= ISKNOW;
    add_pack(obj, TRUE);
}

/*
 * Contains defintions and functions for dealing with things like
 * potions and scrolls
 */

char *rainbow[] = {
    "anıtıaq",
    "bao",
    "baokıa",
    "boaqreo",
    "chaıtıaq",
    "cuı",
    "gıorıa",
    "hoelohareo",
    "hoereo",
    "ıqserıq",
    "kıa",
    "koaloa",
    "kuao",
    "kuo",
    "loa",
    "loekıa",
    "lue",
    "mıo",
    "moykuo",
    "naraq",
    "noaıbao",
    "reomıa",
    "rıaoreo",
    "rıq",
    "ruı",
    "seoqreo",
    "sıaıkıa",
    "tıaq",
};

#define NCOLORS (sizeof rainbow / sizeof (char *))
int cNCOLORS = NCOLORS;

static char *gismu_mid[] = {"ct","cp","cf","ck","cm","cn","cl","cr","jd","jb","jv","jg","jm","jn","jl","jr","st","sp","sf","sk","sx","sm","sn","sl","sr","zd","zb","zv","zg","zm","zn","zl","zr","tc","ts","tp","tf","tk","tx","tm","tn","tl","tr","dj","dz","db","dv","dg","dm","dn","dl","dr","pc","ps","pt","pf","pk","px","pm","pn","pl","pr","bj","bz","bd","bv","bg","bm","bn","bl","br","fc","fs","ft","fp","fk","fx","fm","fn","fl","fr","vj","vz","vd","vb","vg","vm","vn","vl","vr","kc","ks","kt","kp","kf","km","kn","kl","kr","gj","gz","gd","gb","gv","gm","gn","gl","gr","xs","xt","xp","xf","xm","xn","xl","xr","mc","mj","ms","mt","md","mp","mb","mf","mv","mk","mg","mx","mn","ml","mr","nc","nj","ns","nz","nt","nd","np","nb","nf","nv","nk","ng","nx","nm","nl","nr","lc","lj","ls","lz","lt","ld","lp","lb","lf","lv","lk","lg","lx","lm","ln","lr","rc","rj","rs","rz","rt","rd","rp","rb","rf","rv","rk","rg","rx","rm","rn","rl"};
static char *gismu_init[] = {"bl","br","cf","ck","cl","cm","cn","cp","cr","ct","dj","dr","dz","fl","fr","gl","gr","jb","jd","jg","jm","jv","kl","kr","ml","mr","pl","pr","sf","sk","sl","sm","sn","sp","sr","st","tc","tr","ts","vl","vr","xl","xr","zb","zd","zg","zm","zv"};

static char *sylls[] = {
"by","bay","bey","boy","boeq","buao","bueq","bıaı","bıeq","bıoq",
"pay","pey","pou","poy","pıq","peaq","peaı","poeq","puoı","pıeq","pıoq",
"fy","foe","foy","fıq","feaq","foeq","fuao","fuoq","fuoı","fıao","fıaq","fıaı","fıoq",
"nay","nou","neaı",
"do","day","dey","dıy","deoq","doeq","duoq",
"tay","tey","tıy",
"zoe","zou","zoy","zoı","zuı","zeaq","zeaı","zeoq","zoaq","zoeq","zuao","zuaı","zueq","zuoq","zuoı","zıao","zıaq","zıaı","zıeq","zıoq",
"cay","ceq","coy","cuq","cuoq","cuoı","cıaq","cıaı","cıeq",
"sueq",
"reaı",
"lay","ley","luy","leaı","loeq","lıoq",
"nhy","nhay","nhaı","nhea","nheq","nhey","nheı","nhoa","nhoy","nhue","nhuq","nhuı","nheaq","nheaı","nheoq","nhoaq","nhoeq","nhuao","nhuaq","nhuaı","nhueq","nhuoq","nhuoı",
"jy","joeq",
"chy","chay","choy","chuy","chıy","cheaı","choeq","chueq","chuoq","chıoq",
"shay","shey","shoq","shoy","shuy","sheaı","sheoq","shoaq","shoeq","shuoq","shuoı","shıao","shıaı","shıeq","shıoq",
"gy","gey","goq","goy","geaı","geoq","goeq","gueq","guoı","gıoq",
"key","keoq","koeq",
"hay","hoy","huy","hıq","hıy","heaı","hoeq","huoq",
};

STONE stones[] = {
    { "manaopıo",		 25},
    { "hoerıqpıo",	 40},
    { "loahıupıo",	 50},
    { "nueqkıapıo",	 40},
    { "choıpıo",	300},
    { "rıqpıo",	300},
    { "gerymanıumy",	225},
    { "ruameıpıo",	  5},
    { "zaqlopıo",		 50},
    { "ıqse",		150},
    { "kırıpytonaıty",	300},
    { "lazulıpıo",	 50},
    { "mıaopıo",	 50},
    { "loeshıa",	 15},
    { "ceaqpıo",		 60},
    { "mırupıo",		200},
    { "mıqly",		220},
    { "magopıo",	 63},
    { "rubı",		350},
    { "safıra",	285},
    { "pıoloha",	200},
    { "katopıo",	 50},
    { "topazy",		 60},
    { "kuaopıo",	 70},
    { "toaqpıo",	300},
    { "zırykoq",	 	 80},
};

#define NSTONES (sizeof stones / sizeof (STONE))
int cNSTONES = NSTONES;

char *wood[] = {
    "avocado wood",
    "balsa",
    "bamboo",
    "banyan",
    "birch",
    "cedar",
    "cherry",
    "cinnibar",
    "cypress",
    "dogwood",
    "driftwood",
    "ebony",
    "elm",
    "eucalyptus",
    "fall",
    "hemlock",
    "holly",
    "ironwood",
    "kukui wood",
    "mahogany",
    "manzanita",
    "maple",
    "oaken",
    "persimmon wood",
    "pecan",
    "pine",
    "poplar",
    "redwood",
    "rosewood",
    "spruce",
    "teak",
    "walnut",
    "zebrawood",
};

#define NWOOD (sizeof wood / sizeof (char *))
int cNWOOD = NWOOD;

char *metal[] = {
    "lıumıaoloha",
    "berılysaı",
    "kuoqsaı",
    "mesıq",
    "raqsu",
    "kıaloha",
    "ceoqloha",
    "hoeloha",
    "sıaıloha",
    "tuoqloha",
    "maqneloha",
    "naoloha",
    "nıkyloha",
    "lıuloha",
    "palatına",
    "sheqloha",
    "mıaoloha",
    "sılıkoq",
    "tıqloha",
    "caıloha",
    "tuqsyteq",
    "zıqky",
};

#define NMETAL (sizeof metal / sizeof (char *))
int cNMETAL = NMETAL;
#define MAX3(a,b,c)	(a > b ? (a > c ? a : c) : (b > c ? b : c))

static bool used[MAX3(NCOLORS, NSTONES, NWOOD)];

/*
 * init_colors:
 *	Initialize the potion color scheme for this time
 */
void
init_colors()
{
    register int i, j;

    for (i = 0; i < NCOLORS; i++)
	used[i] = FALSE;
    for (i = 0; i < MAXPOTIONS; i++)
    {
	do
	    j = rnd(NCOLORS);
	until (!used[j]);
	used[j] = TRUE;
	p_colors[i] = rainbow[j];
    }
}

/*
 * init_names:
 *	Generate the names of the various scrolls
 */
#define MAXNAME	40	/* Max number of characters in a name */
#define RANDOM_VOWEL "aeiou"[rnd(5)]
#define RANDOM_CONSONANT "bcdfgjklmnprstvxz"[rnd(17)]

void
init_names()
{
    register int nsyl;
    register char *cp, *sp;
    register int i, nwords;

    for (i = 0; i < MAXSCROLLS; i++)
    {
	cp = prbuf;
	nwords = rnd(2) + 1;
	while (nwords--)
	{
        // Make a random Lojban gismu.
        if (rnd(2)) {
            sp = gismu_init[rnd((sizeof gismu_init) / (sizeof (char*)))];
            while (*sp) *cp++ = *sp++;
            *cp++ = RANDOM_VOWEL;
            *cp++ = RANDOM_CONSONANT;
            *cp++ = RANDOM_VOWEL;
            *cp++ = ' ';
        } else {
            *cp++ = RANDOM_CONSONANT;
            *cp++ = RANDOM_VOWEL;
            sp = gismu_mid[rnd((sizeof gismu_init) / (sizeof (char*)))];
            while (*sp) *cp++ = *sp++;
            *cp++ = RANDOM_VOWEL;
            *cp++ = ' ';
        }
	    // nsyl = rnd(3) + 1;
	    // while (nsyl--)
	    // {
		// sp = sylls[rnd((sizeof sylls) / (sizeof (char *)))];
		// if (&cp[strlen(sp)] > &prbuf[MAXNAME])
		// 	break;
		// while (*sp)
		//     *cp++ = *sp++;
	    // }
	    // *cp++ = ' ';
	}
	*--cp = '\0';
	s_names[i] = (char *) malloc((unsigned) strlen(prbuf)+1);
	strcpy(s_names[i], prbuf);
    }
}

/*
 * init_stones:
 *	Initialize the ring stone setting scheme for this time
 */
void
init_stones()
{
    register int i, j;

    for (i = 0; i < NSTONES; i++)
	used[i] = FALSE;
    for (i = 0; i < MAXRINGS; i++)
    {
	do
	    j = rnd(NSTONES);
	until (!used[j]);
	used[j] = TRUE;
	r_stones[i] = stones[j].st_name;
	ring_info[i].oi_worth += stones[j].st_value;
    }
}

/*
 * init_materials:
 *	Initialize the construction materials for wands and staffs
 */
void
init_materials()
{
    register int i, j;
    register char *str;
    static bool metused[NMETAL];

    for (i = 0; i < NWOOD; i++)
	used[i] = FALSE;
    for (i = 0; i < NMETAL; i++)
	metused[i] = FALSE;
    for (i = 0; i < MAXSTICKS; i++)
    {
	for (;;)
	    if (rnd(2) == 0)
	    {
		j = rnd(NMETAL);
		if (!metused[j])
		{
		    ws_type[i] = "wand";
		    str = metal[j];
		    metused[j] = TRUE;
		    break;
		}
	    }
	    else
	    {
		j = rnd(NWOOD);
		if (!used[j])
		{
		    ws_type[i] = "staff";
		    str = wood[j];
		    used[j] = TRUE;
		    break;
		}
	    }
	ws_made[i] = str;
    }
}

#ifdef MASTER
# define	NT	NUMTHINGS, "things"
# define	MP	MAXPOTIONS, "potions"
# define	MS	MAXSCROLLS, "scrolls"
# define	MR	MAXRINGS, "rings"
# define	MWS	MAXSTICKS, "sticks"
# define	MW	MAXWEAPONS, "weapons"
# define	MA	MAXARMORS, "armor"
#else
# define	NT	NUMTHINGS
# define	MP	MAXPOTIONS
# define	MS	MAXSCROLLS
# define	MR	MAXRINGS
# define	MWS	MAXSTICKS
# define	MW	MAXWEAPONS
# define	MA	MAXARMORS
#endif

/*
 * sumprobs:
 *	Sum up the probabilities for items appearing
 */
void
sumprobs(struct obj_info *info, int bound
#ifdef MASTER
	, char *name
#endif
)
{
#ifdef MASTER
	struct obj_info *start = info;
#endif
    struct obj_info *endp;

    endp = info + bound;
    while (++info < endp)
	info->oi_prob += (info - 1)->oi_prob;
#ifdef MASTER
    badcheck(name, start, bound);
#endif
}

/*
 * init_probs:
 *	Initialize the probabilities for the various items
 */
void
init_probs()
{
    sumprobs(things, NT);
    sumprobs(pot_info, MP);
    sumprobs(scr_info, MS);
    sumprobs(ring_info, MR);
    sumprobs(ws_info, MWS);
    sumprobs(weap_info, MW);
    sumprobs(arm_info, MA);
}

#ifdef MASTER
/*
 * badcheck:
 *	Check to see if a series of probabilities sums to 100
 */
void
badcheck(char *name, struct obj_info *info, int bound)
{
    register struct obj_info *end;

    if (info[bound - 1].oi_prob == 100)
	return;
    printf("\nBad percentages for %s (bound = %d):\n", name, bound);
    for (end = &info[bound]; info < end; info++)
	printf("%3d%% %s\n", info->oi_prob, info->oi_name);
    printf("[hit RETURN to continue]");
    fflush(stdout);
    while (getchar() != '\n')
	continue;
}
#endif

/*
 * pick_color:
 *	If he is halucinating, pick a random color name and return it,
 *	otherwise return the given color.
 */
char *
pick_color(char *col)
{
    return (on(player, ISHALU) ? rainbow[rnd(NCOLORS)] : col);
}
