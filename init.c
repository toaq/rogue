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
    "eloareo",
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

/*
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
*/

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
    /* I don't want to translate a bunch of obscure woods so let's just describe staffs with shape adjectives */
    "buaı",
    "choı",
    "de",
    "doq",
    "fıe",
    "goaq",
    "gue",
    "jua",
    "koq",
    "lıu",
    "nea",
    "ruoq",
    "sao",
    "shıa",
    "sıq",
    "tuoq",
};

#define NWOOD (sizeof wood / sizeof (char *))
int cNWOOD = NWOOD;

char *metal[] = {
    "lıumıala",
    "berılysaı",
    "kuoqsaı",
    "mesıq",
    "raqsu",
    "kıdo",
    "ceoqloha",
    "eloa",
    "ceke",
    "tuoqloha",
    "maqnesıoq",
    "naoloha",
    "nıkyloha",
    "lıuloha",
    "palatına",
    "zouku",
    "mıala",
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


static char *gismu_mid[] = {"ct","cp","cf","ck","cm","cn","cl","cr","jd","jb","jv","jg","jm","jn","jl","jr","st","sp","sf","sk","sx","sm","sn","sl","sr","zd","zb","zv","zg","zm","zn","zl","zr","tc","ts","tp","tf","tk","tx","tm","tn","tl","tr","dj","dz","db","dv","dg","dm","dn","dl","dr","pc","ps","pt","pf","pk","px","pm","pn","pl","pr","bj","bz","bd","bv","bg","bm","bn","bl","br","fc","fs","ft","fp","fk","fx","fm","fn","fl","fr","vj","vz","vd","vb","vg","vm","vn","vl","vr","kc","ks","kt","kp","kf","km","kn","kl","kr","gj","gz","gd","gb","gv","gm","gn","gl","gr","xs","xt","xp","xf","xm","xn","xl","xr","mc","mj","ms","mt","md","mp","mb","mf","mv","mk","mg","mx","mn","ml","mr","nc","nj","ns","nz","nt","nd","np","nb","nf","nv","nk","ng","nx","nm","nl","nr","lc","lj","ls","lz","lt","ld","lp","lb","lf","lv","lk","lg","lx","lm","ln","lr","rc","rj","rs","rz","rt","rd","rp","rb","rf","rv","rk","rg","rx","rm","rn","rl"};
static char *gismu_init[] = {"bl","br","cf","ck","cl","cm","cn","cp","cr","ct","dj","dr","dz","fl","fr","gl","gr","jb","jd","jg","jm","jv","kl","kr","ml","mr","pl","pr","sf","sk","sl","sm","sn","sp","sr","st","tc","tr","ts","vl","vr","xl","xr","zb","zd","zg","zm","zv"};
#define rndstr(X) X[rnd((sizeof X) / (sizeof (char*)))] 
#define JBO_V "aeiou"[rnd(5)]
#define JBO_C "bcdfgjklmnprstvxz"[rnd(17)]
char *lojban_name() {
	int nwords = rnd(3) ? 2 : 1;
	prbuf[0] = '\0';
	while (nwords--) {
		if (rnd(2)) {
			sprintf(prbuf + strlen(prbuf), "%s%s%c%c%c",
			prbuf[0] ? " " : "", rndstr(gismu_init), JBO_V, JBO_C, JBO_V);
		} else {
			sprintf(prbuf + strlen(prbuf), "%s%c%c%s%c",
			prbuf[0] ? " " : "", JBO_C, JBO_V, rndstr(gismu_mid), JBO_V);
		}
	}
	return prbuf;
}

static char *toki_pona_cv[] = {"a","e","i","o","u","ja","je","jo","ju","ka","ke","ki","ko","ku","la","le","li","lo","lu","ma","me","mi","mo","mu","na","ne","ni","no","nu","pa","pe","pi","po","pu","sa","se","si","so","su","ta","te","to","tu","wa","we","wi"};
#define TP_CV toki_pona_cv[rnd((sizeof toki_pona_cv) / (sizeof (char*)))]
#define TP_C
char *toki_pona_name() {
	int nwords = rnd(3) ? 2 : 1;
	char *syll;
	prbuf[0] = '\0';
	for (int i = 0; i < nwords; i++) {
		if (i > 0) sprintf(prbuf + strlen(prbuf), " ");
		int nsyll = (nwords == 1) ? 3 : rnd(2)+2;
		int n_coda = FALSE;
		for (int j = 0; j < nsyll; j++) {
			do { syll = rndstr(toki_pona_cv); }
			while (((syll[0] == 'n' || syll[0] == 'm') && n_coda)
				|| (strlen(syll) == 1 && j > 0));
			n_coda = rnd(3) ? FALSE : TRUE;
			sprintf(prbuf + strlen(prbuf), "%s%s", syll, n_coda ? "n" : "");
		}
	}
	return prbuf;
}

static char *ithkuil_v[] = {"a","e","u","o","ö","ü","î","û","â","ê","ô","i","ai","ei","ui","au"};
static char *ithkuil_cx[] = {"kt","q’","qh","rţ","ňs","p","č’","čh","nn","nš","c’","ň","ňň","nz","th","rč","ll","z","kk","pţ","nž","nt","kţ","rl","lz","ss","ns","mt","d","b","rd","zb","g","f","fs","x","xh","ç","č","c","rň","kh","q","rg","xht","tļ","ż","j","ňx","mž","ļ","r","rr","ţ","pt","rk","v","çq","nţ","kç","sk","st","lt","dh","šq","ph","ňz","kš","mš","pç","ch","lk","çt","rt","ňž","sq","št","çk","řb","n","m","pš","ž","t","rx","mp","ld","ňq","ndh","šš","ps","ňt","sp","mz","ls","ms","rz","t’","rm","s","mm","ňš","rs","š","qt","k","k’","rq","p’","šk","mb","ňk","xt","ks","ln","šp","lm","mļ","lţ","lf","lv","lb","lš","nļ","lc","lq","ff","rc","rf","rtļ","mx","lč","lg","rç","rp","lp","rn","rš","bz","gz","rb"};
static char *ithkuil_ca[] = {"l", "t", "s", "c", "ţ", "rm", "dh", "šq", "rk", "r", "tļ", "ļ", "rt", "tt", "šk", "ll", "p", "mt", "ż", "n", "m", "k", "xht", "rp", "d"};
static char *ithkuil_cg[] = {"y", "w", "hw", "hm", "hn", "hr", "lw", "ly","rw","ry","řw","řy"};
static char *ithkuil_cr[] = {"b", "bb", "bbr", "bd", "bdh", "bg", "bj", "bk", "bkw", "bk'", "bkh", "bl", "blw", "bm", "bn", "bq", "bqw", "bq'", "bqh", "br", "bř", "bt", "bt'", "bth", "btw", "bv", "bw", "by", "bz", "bzd", "bzdw", "bzg", "bzgr", "bž", "c", "cc", "ck", "ckh", "cl", "cm", "cn", "cp", "cp'", "cq", "cr", "ct", "ct'", "ctw", "cw", "cx", "cy", "c'", "ch", "ç", "çc", "çcw", "çç", "çč", "çčw", "çf", "çfl", "çk", "çkç", "çkf", "çkl", "çkr", "çkţ", "çkv", "çkw", "çky", "çk'", "çkh", "çkhw", "çl", "çm", "çmw", "çn", "çnw", "çp", "çpç", "çpf", "çpl", "çpr", "çpř", "çpţ", "çpw", "çph", "çphw", "çq", "çqf", "çql", "çqr", "çqţ", "çq'", "çqh", "çqhw", "çr", "çt", "çtl", "çtr", "çtř", "çtw", "çt'", "çth", "çthw", "çv", "çw", "č", "čk", "čkh", "čl", "čm", "čn", "čp", "čp'", "čq", "čq'", "čr", "čt", "čthw", "čv", "čw", "č'", "čh",
	"d", "dbr", "dby", "dd", "ddl", "ddr", "ddř", "ddw", "ddy", "dg", "dgl", "dgr", "dgv", "dgw", "dgy", "dh", "dhb", "dhbl", "dhbv", "dhbw", "dhbz", "dhg", "dhgl", "dhgr", "dhgv", "dhgw", "dhgy", "dhgz", "dhl", "dhm", "dhmw", "dhmy", "dhn", "dhnw", "dhr", "dhř", "dhv", "dhw", "dhy", "dk", "dl", "dlw", "dly", "dm", "dmy", "dn", "dnw", "dr", "dř", "dv", "dw", "dy", "f", "fc", "fč", "ff", "ffm", "ffr", "ffy", "fk", "fk'", "fkç", "fkh", "fkhw", "fkl", "fkr", "fkř", "fkţ", "fkw", "fky", "fl", "fly", "fn", "fpç", "fpl", "fpţ", "fq", "fqh", "fql", "fqr", "fqţ", "fr", "fř", "fs", "fsk", "fsk'", "fskh", "fsm", "fsn", "fsp", "fsp'", "fsq", "fsq'", "fsqh", "fsqw", "fst", "fsy", "fš", "fšk", "fšk'", "fškh", "fšp", "fšp'", "fšqh", "fšth", "ft", "fţ", "fth", "fw", "fy", "g", "gb", "gd", "gdh", "gdr", "gg", "gl", "gm", "gn", "gr", "gř", "gv", "gw", "gy", "gz", "gzb", "gzbl", "gzd", "gzdr", "gzdw", "gzl", "gzv", "gž",
	"j", "jgw", "jj", "jk", "jm", "jph", "jq'", "jr", "jth", "jv", "jw", "k", "k'", "kc", "kç", "kč", "kčhw", "kd", "kf", "kh", "khw", "kk", "kk'", "kkr", "kl", "klw", "km", "kn", "kp", "kpl", "kr", "ks", "kš", "ksf", "ksl", "ksn", "ksp", "kšp", "ksr", "kst", "kšt", "ksw", "ksţ", "ksv", "kt", "kth", "ktr", "kţ", "kt'", "kţkl", "kţr", "kv", "kw", "ky", "l", "ļ", "lb", "lbw", "lc", "lč", "lç", "lc'", "lč'", "lch", "ld", "ldh", "ldr", "ldw", "lf", "lfq", "lfw", "lg", "lgw", "lk", "lk'", "lkh", "lkhw", "lkw", "ll", "lm", "lmz", "ln", "lntw", "lp", "lp'", "lphw", "lq", "lq'", "lqh", "lr", "lř", "ls", "lš", "lskw", "lt", "lt'", "lth", "ltw", "lţ", "lţr", "lv", "lx", "lxh", "lxw", "ly", "lz", "lż", "lž", "lzw", "lżw", "ļkh", "ļļ", "ļm", "ļn", "ļp", "ļp'", "ļph", "ļq", "ļq'", "ļqh", "ļw", "m", "mb", "mbr", "mç", "mf", "mft", "mg", "mj", "mk", "mk'", "mkh", "mkhw", "ml", "mm", "mn", "mp", "mpr", "mp'", "mph", "mpy",
	"mq", "mqhw", "mr", "mř", "ms", "msk", "mš", "mt", "mţ", "mt'", "mth", "mv", "mvy", "mw", "mx", "mxh", "mxz", "my", "mz", "mž", "n", "ň", "nç", "nc'", "nč'", "nch", "nčh", "nchw", "nçw", "nd", "ndh", "ndr", "ndw", "nl", "nļ", "nm", "nn", "nr", "nř", "ns", "nš", "nst", "nsw", "nt", "ntr", "nţ", "nţw", "nt'", "nth", "nv", "nw", "ny", "nz", "nž", "ňc", "ňč", "ňç", "ňčh", "ňčhw", "ňf", "ňg", "ňgr", "ňgw", "ňgy", "ňk", "ňkh", "ňkr", "ňkw", "ňky", "ňn", "ňqh", "ňř", "ňs", "ňsp", "ňšw", "ňk'", "ňsk", "ňth", "ňv", "ňw", "ňx", "ňxh", "ňz", "ňž", "p", "p'", "pc", "pč", "pç", "pçk", "pc'", "pč'", "pch", "pčh", "pčhw", "pçt", "pçw", "pf", "pg", "ph", "phw", "pj", "pk", "pkl", "pkw", "pky", "pk'", "pkh", "pl", "pļ", "plw", "pm", "pn", "pny", "pp", "pp'", "pph", "pq", "pq'", "pqh", "pr", "př", "ps", "psb", "psf", "psl", "psk", "psp'", "psq", "psq'", "psr", "pst", "pstw", "psw", "psy", "pš", "pšk", "pškh", "pšq'", "pšqh",
	"pšt", "pt", "pţ", "pt'", "pth", "pw", "px", "pxh", "py", "q", "qc", "qč", "qch", "qf", "qfr", "qfř", "qfw", "qh", "qj", "ql", "qly", "qm", "qn", "qp", "qq", "qr", "qt", "qth", "qt'", "qţ", "qţl", "qţř", "qv", "qw", "q'", "r", "ř", "rb", "rc", "rc'", "rč", "rç", "rch", "rčp'", "rčt'", "rd", "rdh", "rdhř", "rdhv", "rdhw", "rf", "rfq", "rfž", "rg", "rgy", "rgzd", "rgzw", "rj", "rk", "rk'", "rkh", "rkr", "rl", "rļ", "rm", "rmkh", "rmq'", "rmst", "rmxh", "rn", "rnr", "rntr", "rp", "rp'", "rpc'", "rpč'", "rph", "rpţ", "rpth", "rpv", "rpx", "rq", "rq'", "rqhw", "rqw", "rr", "rs", "rš", "rsč", "ršč", "rsf", "ršf", "rsk", "ršk", "rsl", "ršl", "rsm", "ršm", "rsn", "ršn", "rsp", "ršp", "rsph", "rsq", "ršq", "rsqh", "ršqh", "rst", "rst'", "ršt", "ršţ", "ršt'", "ršth", "rstr", "rsv", "ršv", "rsx", "rt", "rţ", "rt'", "rth", "rthw", "rtk", "rţk", "rtļ", "rţl", "rţm", "rtn", "rţn", "rtp", "rţq", "rţq'", "rţt'", "rţv",
	"rtw", "rţw", "rv", "rvp'", "rw", "rx", "rxh", "rz", "rż", "rž", "rzd", "rzw", "rżw", "řb", "řbg", "rbw", "řc", "řč", "řc'", "řč'", "řch", "řdh", "řd", "řdř", "řdw", "řf", "řg", "řgl", "řgr", "řgř", "řgw", "řgz", "řj", "řjw", "řk", "řk'", "řkh", "řkw", "řl", "řļ", "řm", "řmn", "řn", "řqr", "řř", "řš", "řsw", "řthw", "řzw", "s", "sc", "sč", "sch", "sf", "sk", "sk'", "skh", "skhw", "skl", "skr", "sky", "sl", "sm", "sn", "sp", "sp'", "sph", "sphw", "spl", "sq", "sq'", "sqh", "sqhw", "sqw", "sr", "sř", "ss", "st", "sţ", "st'", "sth", "sthw", "stl", "str", "stv", "stw", "sty", "sv", "sw", "sx", "sxh", "sy", "š", "šč", "šč'", "ščh", "šf", "šk", "šk'", "škç", "škh", "škhw", "škţ", "šky", "šl", "šm", "šny", "šp", "šp'", "šph", "šphw", "špr", "šq", "šq'", "šqh", "šr", "šř", "šš", "št", "šţ", "št'", "šth", "štl", "šv", "šw", "šx", "šxh", "šy", "t", "t'", "tç", "tf", "tfř", "th", "tk", "tk'", "tkç", "tkh", "tkr",
	"tl", "tļ", "tlw", "tm", "tn", "tp", "tq", "tr", "tř", "tt", "tv", "tw", "tx", "txh", "ty", "ţ", "ţb", "ţc", "ţc'", "ţč", "ţg", "ţk", "ţk'", "ţkh", "ţkhw", "ţkl", "ţkř", "ţky", "ţl", "ţlw", "ţly", "ţm", "ţmw", "ţmy", "ţn", "ţny", "ţp", "ţpf", "ţph", "ţphw", "ţpļ", "ţpř", "ţpţ", "ţpw", "ţq", "ţq'", "ţqf", "ţqh", "ţqhw", "ţqţ", "ţr", "ţř", "ţt", "ţt'", "ţth", "ţtl", "ţtř", "ţv", "ţw", "ţx", "ţxh", "ţxhw", "ţy", "v", "vbr", "vdr", "vg", "vgl", "vgr", "vgř", "vgw", "vgy", "vkr", "vkř", "vkw", "vky", "vl", "vm", "vn", "vpţ", "vr", "vř", "vtr", "vty", "vv", "vw", "vy", "vzm", "vžř", "vžv", "vzy", "x", "xč", "xchw", "xčw", "xč'", "xh", "xhč'", "xhchw", "xhčw", "xhf", "xhl", "xhly", "xhm", "xhmy", "xhn", "xhnw", "xhny", "xhp", "xhp'", "xhpř", "xhpw", "xht", "xht'", "xhth", "xhtr", "xhtř", "xhtw", "xhw", "xl", "xlw", "xm", "xmy", "xn", "xp", "xphw", "xpw", "xp'", "xr", "xt", "xt'", "xth", "xthw", "xtļ", "xw", "xx",
	"z", "zb", "zbr", "zbř", "zbw", "zd", "zdl", "zdr", "zdř", "zdw", "zdy", "zg", "zgr", "zgř", "zgw", "zj", "zk", "zl", "zm", "zmy", "zn", "zň", "zp", "zp'", "zph", "zq", "zq'", "zqh", "zr", "zt", "zv", "zvw", "zvy", "zw", "zy", "zz", "ż", "żb", "żg", "żk", "żk'", "żn", "żp'", "żph", "żq", "żq'", "żqh", "żr", "żř", "żt", "żv", "ž", "žb", "žbř", "žd", "ždr", "ždw", "žg", "žgl", "žgr", "žgy", "žk'", "žl", "žlw", "žm", "žmy", "žny", "žph", "žq", "žq'", "žqh", "žr", "žv", "žvw", "žw", "žy"};
static char *ithkuil_bias[] = { "n","nn", "m","mm", "ň","ňň", "ţ","ţţ", "ç","çç", "s","ss", "z","zz", "š","šš", "l","ll", "r","rr", "ř","řř", "ļ","ļļ", "kç","kçç", "pļ","pļļ", "pç","pçç", "x","xx", "xh","xxh", "ks","kss", "f","ff", "kš","kšš", "kf","kff", "pš","pšš", "ps","pss", "pf","pff", };

char *ithkuil_name() {
do {
	prbuf[0] = '\0';
	// Vr
	if (1) {
		if (rnd(3) == 0) {
			sprintf(prbuf+strlen(prbuf), "%s", rndstr(ithkuil_cg));
		}
		sprintf(prbuf+strlen(prbuf), "%s", rndstr(ithkuil_v));
	}
	// Cr
	sprintf(prbuf+strlen(prbuf), "%s", rndstr(ithkuil_cr));
	// Vc
	sprintf(prbuf+strlen(prbuf), "%s", rndstr(ithkuil_v));
	if (rnd(4) == 0) {
		// Ci+Vi
		sprintf(prbuf+strlen(prbuf), "%c%c", "wyh"[rnd(3)], "aeiou"[rnd(5)]);
	}
	// Ca
	sprintf(prbuf+strlen(prbuf), "%s", rndstr(ithkuil_ca));
	if (rnd(3) == 0) {
		// VxC
		sprintf(prbuf+strlen(prbuf), "%s%s", rndstr(ithkuil_v), rndstr(ithkuil_cx));
		if (rnd(2) == 0) {
			// another VxC
			sprintf(prbuf+strlen(prbuf), "%s%s", rndstr(ithkuil_v), rndstr(ithkuil_cx));
		}
	}
	if (rnd(2) == 0) {
		// Vf
		sprintf(prbuf+strlen(prbuf), "%c", "aieu"[rnd(3)]);
		if (rnd(3) == 0) {
			sprintf(prbuf+strlen(prbuf), "’%s", rndstr(ithkuil_bias));
		}
	}
 } while (utf8_strlen(prbuf) < 7);
	return prbuf;
}


void
init_names()
{
    register int i;
    for (i = 0; i < MAXSCROLLS; i++)
    {
	switch (rnd(3)) {
		case 0: lojban_name(); break;
		case 1: toki_pona_name(); break;
		case 2: ithkuil_name(); break;
	}
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
		    ws_type[i] = "nuıbeaq";
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
		    ws_type[i] = "beaq";
		    str = wood[j];
		    used[j] = TRUE;
		    break;
		}
	    }
	ws_made[i] = str;
    }
}

#ifdef MASTER
# define	NT	NUMTHINGS, "raı"
# define	MP	MAXPOTIONS, "majınao"
# define	MS	MAXSCROLLS, "majıpeq"
# define	MR	MAXRINGS, "cheıbıu"
# define	MWS	MAXSTICKS, "majıbeaq"
# define	MW	MAXWEAPONS, "hıaochuo"
# define	MA	MAXARMORS, "leoqfuq"
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
