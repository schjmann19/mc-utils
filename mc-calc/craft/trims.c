/*
armor trims crafting recipes
as of mc 1.21.10

note: jeo

note: they shouldn't be under '--craft' but eh
'obtain' more like but no for simplicity

*/

/* template

void _trim(void){
    printf("can be found in \n");
    printf("can be duped with 7 diamonds and 1 \n");
}

*/

// bless the minecraft wiki

#include <stdio.h>

void trims(void){
    printf("the true internal names are not used, instead, i will just use the unique names with `_trim` appended, so:\n");
    printf("sentry_trim\n");
    printf("vex_trim\n");
    printf("wild_trim\n");
    printf("coast_trim\n");
    printf("dune_trim\n");
    printf("wayfinder_trim\n");
    printf("raiser_trim\n");
    printf("shaper_trim\n");
    printf("host_trim\n");
    printf("ward_trim\n");
    printf("silence_trim\n");
    printf("tide_trim\n");
    printf("snout_trim\n");
    printf("rib_trim\n");
    printf("eye_trim\n");
    printf("spire_trim\n");
    printf("flow_trim\n");
    printf("bolt_trim\n");
    printf("netherite_upgrade\n");
}

void netherite_trim(void){ printf("no such trim, do you mean the netherite upgrade?"); } /* i'm not petty. */
void netherite_upgrade(void){
    printf("can be found in bastion remnants\n");
    printf("can be duped with 7 diamonds and 1 netherrack");
}

void sentry_trim(void){
    printf("can be found in pillager outposts\n");
    printf("can be duped with 7 diamonds and 1 cobblestone\n");
}

void vex_trim(void){
    printf("can be found in woodland mansions\n");
    printf("can be duped with 7 diamonds and 1 cobblestone\n");
}

void wild_trim(void){
    printf("can be found in jungle temples\n");
    printf("can be duped with 7 diamonds and 1 mossy cobblestone\n");
}

void coast_trim(void){
    printf("can be found in shipwrecks\n");
    printf("can be duped with 7 diamonds and 1 cobblestone\n");
}

void dune_trim(void){
    printf("can be found in desert pyramids\n");
    printf("can be duped with 7 diamonds and 1 sandstone\n");
}

void wayfinder_trim(void){
    printf("can be found in trail ruins\n");
    printf("can be duped with 7 diamonds and 1 terracota\n");
}

void raiser_trim(void){
    printf("can be found in trail ruins\n");
    printf("can be duped with 7 diamonds and 1 terracota\n");
}

void shaper_trim(void){
    printf("can be found in trail ruins\n");
    printf("can be duped with 7 diamonds and 1 terracota\n");
}

void host_trim(void){
    printf("can be found in trail ruins\n");
    printf("can be duped with 7 diamonds and 1 terracota\n");
}

void ward_trim(void){
    printf("can be found in ancient cities\n");
    printf("can be duped with 7 diamonds and 1 cobbled deepslate\n");
}

void silence_trim(void){
    printf("can be found in ancient cities\n");
    printf("can be duped with 7 diamonds and 1 cobbled deepslate\n");
}

void tide_trim(void){
    printf("can be obtained by killing elder guardians, with a chance of 0.20\n");
    printf("note: looting does not affect the 0.20 chance");
    printf("can be duped with 7 diamonds and 1 prismarine\n");
}

void snout_trim(void){
    printf("can be found in bastion remnants\n");
    printf("can be duped with 7 diamonds and 1 blackstone\n");
}

void rib_trim(void){
    printf("can be found in nether fortresses\n");
    printf("can be duped with 7 diamonds and 1 netherrack\n");
}

void eye_trim(void){
    printf("can be found in strongholds\n");
    printf("can be duped with 7 diamonds and 1 end stone\n");
}

void spire_trim(void){
    printf("can be found in end cities\n");
    printf("can be duped with 7 diamonds and 1 purpur block\n");
}

void flow_trim(void){
    printf("can be found in trial chambers\n");
    printf("can be duped with 7 diamonds and 1 breeze rod\n");
}

void bolt_trim(void){
    printf("can be found in trial chambers\n");
    printf("can be duped with 7 diamonds and 1 block of copper (waxed or not)\n");
}


// i feel like a nice person; so i may add "filtering by where you obtain"
// i.e. trail_ruins() to print all the ones that can be found in trail ruins, etc.
// not now though, maybe later



