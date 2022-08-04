/*
 * Read a scroll and let it happen
 *
 * @(#)scrolls.c	4.44 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <curses.h>
#include <ctype.h>
#include "rogue.h"

/*
 * read_scroll:
 *	Read a scroll from the pack and do the appropriate thing
 */

void
read_scroll()
{
    THING *obj;
    PLACE *pp;
    int y, x;
    char ch;
    int i;
    bool discardit = FALSE;
    struct room *cur_room;
    THING *orig_obj;
    static coord mp;

    obj = get_item("noaq", SCROLL);
    if (obj == NULL)
	return;
    if (obj->o_type != SCROLL)
    {
	msg("Kaıte kóu sıa da.");
	return;
    }
    /*
     * Calculate the effect it has on the poor guy.
     */
    if (obj == cur_weapon)
	cur_weapon = NULL;
    /*
     * Get rid of the thing
     */
    discardit = (bool)(obj->o_count == 1);
    leave_pack(obj, FALSE, FALSE);
    orig_obj = obj;

    switch (obj->o_which)
    {
	case S_CONFUSE:
	    /*
	     * Scroll of monster confusion.  Give him that power.
	     */
	    player.t_flags |= CANHUH;
	    msg("Ceo gıozıa súqbo gumuq sa %s da.", pick_color("kıa"));
	when S_ARMOR:
	    if (cur_armor != NULL)
	    {
		cur_armor->o_arm--;
		cur_armor->o_flags &= ~ISCURSED;
		msg("Pıka gıozıa súqbo leoqfuq sa %s da.", pick_color("mıalareo"));
	    }
	when S_HOLD:
	    /*
	     * Hold monster scroll.  Stop all monsters within two spaces
	     * from chasing after the hero.
	     */

	    ch = 0;
	    for (x = hero.x - 2; x <= hero.x + 2; x++)
		if (x >= 0 && x < NUMCOLS)
		    for (y = hero.y - 2; y <= hero.y + 2; y++)
			if (y >= 0 && y <= NUMLINES - 1)
			    if ((obj = moat(y, x)) != NULL && on(*obj, ISRUN))
			    {
				obj->t_flags &= ~ISRUN;
				obj->t_flags |= ISHELD;
				ch++;
			    }
	    if (ch)
	    {
		msg("Shaı duy ke ıoma rëı hóa súq da.");
		scr_info[S_HOLD].oi_know = TRUE;
	    }
	    else
		msg("Moe súq lûı buaq ja sa da.");
	when S_SLEEP:
	    /*
	     * Scroll which makes you fall asleep
	     */
	    scr_info[S_SLEEP].oi_know = TRUE;
	    no_command += rnd(SLEEPTIME) + 4;
	    player.t_flags &= ~ISRUN;
	    msg("Nuosho súq da.");
	when S_CREATE:
	    /*
	     * Create a monster:
	     * First look in a circle around him, next try his room
	     * otherwise give up
	     */
	    i = 0;
	    for (y = hero.y - 1; y <= hero.y + 1; y++)
		for (x = hero.x - 1; x <= hero.x + 1; x++)
		    /*
		     * Don't put a monster in top of the player.
		     */
		    if (y == hero.y && x == hero.x)
			continue;
		    /*
		     * Or anything else nasty
		     */
		    else if (step_ok(ch = winat(y, x)))
		    {
			if (ch == SCROLL
			    && find_obj(y, x)->o_which == S_SCARE)
				continue;
			else if (rnd(++i) == 0)
			{
			    mp.y = y;
			    mp.x = x;
			}
		    }
	    if (i == 0)
		msg("Huogaı súq sa rue shoıse müıbo hóa jâq noı sa da.");
	    else
	    {
		obj = new_item();
		new_monster(obj, randmonster(FALSE), &mp);
	    }
	when S_ID_POTION:
	case S_ID_SCROLL:
	case S_ID_WEAPON:
	case S_ID_ARMOR:
	case S_ID_R_OR_S:
	{
	    static char id_type[S_ID_R_OR_S + 1] =
		{ 0, 0, 0, 0, 0, POTION, SCROLL, WEAPON, ARMOR, R_OR_S };
	    /*
	     * Identify, let him figure something out
	     */
	    scr_info[obj->o_which].oi_know = TRUE;
	    msg("Majıpeq %s ní peq da.", scr_info[obj->o_which].oi_name);
	    whatis(TRUE, id_type[obj->o_which]);
	}
	when S_MAP:
	    /*
	     * Scroll of magic mapping.
	     */
	    scr_info[S_MAP].oi_know = TRUE;
	    msg("Obe, sha kaıte ní peq sa guaqfuaq da.");
	    /*
	     * take all the things we want to keep hidden out of the window
	     */
	    for (y = 1; y < NUMLINES - 1; y++)
		for (x = 0; x < NUMCOLS; x++)
		{
		    pp = INDEX(y, x);
		    switch (ch = pp->p_ch)
		    {
			case DOOR:
			case STAIRS:
			    break;

			case '-':
			case '|':
			    if (!(pp->p_flags & F_REAL))
			    {
				ch = pp->p_ch = DOOR;
				pp->p_flags |= F_REAL;
			    }
			    break;

			case ' ':
			    if (pp->p_flags & F_REAL)
				goto def;
			    pp->p_flags |= F_REAL;
			    ch = pp->p_ch = PASSAGE;
			    /* FALLTHROUGH */

			case PASSAGE:
pass:
			    if (!(pp->p_flags & F_REAL))
				pp->p_ch = PASSAGE;
			    pp->p_flags |= (F_SEEN|F_REAL);
			    ch = PASSAGE;
			    break;

			case FLOOR:
			    if (pp->p_flags & F_REAL)
				ch = ' ';
			    else
			    {
				ch = TRAP;
				pp->p_ch = TRAP;
				pp->p_flags |= (F_SEEN|F_REAL);
			    }
			    break;

			default:
def:
			    if (pp->p_flags & F_PASS)
				goto pass;
			    ch = ' ';
			    break;
		    }
		    if (ch != ' ')
		    {
			if ((obj = pp->p_monst) != NULL)
			    obj->t_oldch = ch;
			if (obj == NULL || !on(player, SEEMONST))
			    mvaddch(y, x, ch);
		    }
		}
	when S_FDET:
	    /*
	     * Potion of gold detection
	     */
	    ch = FALSE;
	    wclear(hw);
	    for (obj = lvl_obj; obj != NULL; obj = next(obj))
		if (obj->o_type == FOOD)
		{
		    ch = TRUE;
		    wmove(hw, obj->o_pos.y, obj->o_pos.x);
		    waddch(hw, FOOD);
		}
	    if (ch)
	    {
		scr_info[S_FDET].oi_know = TRUE;
			show_win("Uru súqbo shıqhua jòı shîqgaı súq tî baq haq hı da.--Sıe--");
	    }
	    else
		msg("Uru súqbo shıqhua da.");
	when S_TELEP:
	    /*
	     * Scroll of teleportation:
	     * Make him dissapear and reappear
	     */
	    {
		cur_room = proom;
		teleport();
		if (cur_room != proom)
		    scr_info[S_TELEP].oi_know = TRUE;
	    }
	when S_ENCH:
	    if (cur_weapon == NULL || cur_weapon->o_type != WEAPON)
		msg("Moe súq lûı buaq ja sa da.");
	    else
	    {
		cur_weapon->o_flags &= ~ISCURSED;
		if (rnd(2) == 0)
		    cur_weapon->o_hplus++;
		else
		    cur_weapon->o_dplus++;
		msg("Pıka gıozıa súqbo %s sa %s da.",
		    weap_info[cur_weapon->o_which].oi_name, pick_color("mıo"));
	    }
	when S_SCARE:
	    /*
	     * Reading it is a mistake and produces laughter at her
	     * poor boo boo.
	     */
	    msg("Huogaı súq hîaı tĩjao sa shaohuı da.");
	when S_REMOVE:
	    uncurse(cur_armor);
	    uncurse(cur_weapon);
	    uncurse(cur_ring[LEFT]);
	    uncurse(cur_ring[RIGHT]);
	    msg("Moe súq kîaı sa paıruo súq da.");
	when S_AGGR:
	    /*
	     * This scroll aggravates all the monsters on the current
	     * level and sets them running towards the hero
	     */
	    aggravate();
	    msg("Huogaı súq sa beocıa laqgea.");
	when S_PROTECT:
	    if (cur_armor != NULL)
	    {
		cur_armor->o_flags |= ISPROT;
		msg("Tıe sa zıazıa %s leoqboe súqbo leoqfuq da.", pick_color("eloareo"));
	    }
	    else
		msg("Moe súq lûı buaq ja sa da.");
#ifdef MASTER
	otherwise:
	    msg("Jua ní majıpeq da.");
	    return;
#endif
    }
    obj = orig_obj;
    look(TRUE);	/* put the result of the scroll on the screen */
    status();

    call_it(&scr_info[obj->o_which]);

    if (discardit)
	discard(obj);
}

/*
 * uncurse:
 *	Uncurse an item
 */

void
uncurse(THING *obj)
{
    if (obj != NULL)
	obj->o_flags &= ~ISCURSED;
}
