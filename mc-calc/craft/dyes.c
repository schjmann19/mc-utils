#include <stdio.h>

/*
dyes crafting recipes
as of mc 1.21.10

note: jeo
*/

// color helper func
void ccolor(void){  // use this with '--colors' or sm like that idrk
    printf( "the available colors in minecraft are:\n");
    printf("white\n");
    printf("light_gray\n");
    printf("gray\n");
    printf("black\n");
    printf("brown\n");
    printf("red\n");
    printf("orange\n");
    printf("yellow\n");
    printf("lime [note: not called `light_green`]\n");
    printf("green\n");
    printf("cyan\n");
    printf("light_blue\n");
    printf("blue\n");
    printf("purple\n");
    printf("magenta\n");
    printf("pink\n");
}

void white_die(void){
    printf("craftable in inventory; shapeless\n");
    printf("ingredients: \n");
    printf("1x bone_meal or\n");
    printf("1x lily_of_the_valley\n");
    printf("gives 1 white_dye\n");
} 

void light_gray_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("options: \n");
    printf("white_tulip = 1 light_gray_dye\n");
    printf("oxeye_daise = 1 light_gray_dye\n");
    printf("white_dye + white_dye + black_dye = 3 light_gray_dye");
    printf("gray_dye + white_dye = 2 light_gray_dye");
} 

void gray_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("options: \n");
    printf("closed_eyeblossom = 1 gray_dye\n");
    printf("white_dye + black_dye = 2 gray_dye\n");
} 

void black_dye(void){
    printf("craftable in inventory\n");
    printf("options: \n");
    printf("ink_sac = 1 black_dye\n");
    printf("wither_rose = 1 black_dye\n");
} 

void brown_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("only option:\n");
    printf("cocoa_beans = 1 brown_dye\n");
} 

void red_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("options: \n");
    printf("poppy = 1 red_dye\n");
    printf("red_tulip = 1 red_dye\n");
    printf("beetroot = 1 red_dye\n");
    printf("rose_bush = 2 red_dye\n");
} 

void orange_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("options: \n");
    printf("red_dye + yellow_dye = 2 orange_dye\n");
    printf("orange_tulip = 1 orange_dye\n");
    printf("torchflower = 1 orange_dye\n");
    printf("open_eyeblossom = 1 orange_dye\n");
} 

void yellow_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("options: \n");
    printf("dandelion = 1 yellow_dye\n");
    printf("sunflower = 2 yellow_dye\n");
    printf("wildflowers =1 yellolw_dye\n");
} 

void lime_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("single option: \n");
    printf("green_dye + white_dye = 2 lime_dye\n");
} 

void cyan_dye(void){
    printf("craftable in inventory; shapeless:\n");
    printf("options: \n");
    printf("blue_dye + green_dye = 2 cyan_dye\n");
    printf("pitcher_plant = 2 cyan_dye\n");
} 