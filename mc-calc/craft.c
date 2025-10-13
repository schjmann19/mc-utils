#include "aux.c"

// WIP

/*
    NOTE:
    THE ITEM'S AND BLOCK'S INTERNAL NAMES ARE USED, NOT THEIR DISPLAYNAMES.
    E.G.: `gold_block` instead of "Block of Gold"

    NOTE 2:
    THE SOURCE FOR THIS INFORMATION IS THE MINECRAFT WIKI.
    (https://minecraft.wiki/w/Crafting#Complete_recipe_list)
    IF YOU SPOT A MISTAKE, KINDLY LET ME KNOW.

    NOTE 3:
    at the time of this commit, the latest version is 1.21.9.
*/


/* 
templates

void namec(void){
    printf("crafting table required\n");
    printf("ingredients: \n");
    printf("s1, s2, s3\n");
    printf("s4, s5, s6\n");
    printf("s7, s8, s9\n");
    printf("gives ?\n");
}

void namei(void){
    printf("craftable in inventory\n");
    printf("ingredients: \n");
    printf("s1, s2\n");
    printf("s3, s4\n");
    printf("gives ?\n");
} */

// loop through argv, call the function of the same name
// if no matches, `no such block or item; or it has no crafting recipe`

void nope(void){
    printf("no such block or item; or it doesn't have a crafting recipe.");
}


/* - building blocks - */

void gold_block(void){
    printf("crafting table required\n");
    printf("ingredients: gold_ingot x9\n");
    printf("gold_ingot, gold_ingot, gold_ingot\n");
    printf("gold_ingot, gold_ingot, gold_ingot\n");
    printf("gold_ingot, gold_ingot, gold_ingot\n");
    printf("gives 1 gold_block\n");
}

void iron_block(void){
    printf("crafting table required\n");
    printf("ingredients: iron_ingot x9\n");
    printf("iron_ingot, iron_ingot, iron_ingot\n");
    printf("iron_ingot, iron_ingot, iron_ingot\n");
    printf("iron_ingot, iron_ingot, iron_ingot\n");
    printf("gives 1 iron_block\n");
}

void bricks(void){
    printf("craftable in inventory\n");
    printf("ingredients: brick x4\n");
    printf("brick, brick\n");
    printf("brick, brick\n");
    printf("gives 1 bricks\n");
} 

void diamond_block(void){
    printf("crafting table required\n");
    printf("ingredients: diamond x9\n");
    printf("diamond, diamond, diamond\n");
    printf("diamond, diamond, diamond\n");
    printf("diamond, diamond, diamond\n");
    printf("gives 1 diamond_block\n");
}

void stone_slab(void){
    printf("crafting table required\n");
    printf("ingredients: stone x3\n");
    printf("-----, -----, -----\n");
    printf("-----, -----, -----\n");
    printf("stone, stone, stone\n");
    printf("gives 6 stone_slab\n");
}

void bookshelf(void){
    printf("crafting table required\n");
    printf("ingredients: planks(any) x6, book x3\n");
    printf("plank, plank, plank\n");
    printf("book , book , book \n");
    printf("plank, plank, plank\n");
    printf("gives ?\n");
}

void snow_block(void){
    printf("craftable in inventory\n");
    printf("ingredients: snowball x4 \n");
    printf("snowball, snowball\n");
    printf("snowball, snowball\n");
    printf("gives 1 snow_block\n");
}

void clay(void){
    printf("craftable in inventory\n");
    printf("ingredients: clay_ball x4\n");
    printf("clay_ball, clay_ball\n");
    printf("clay_ball, clay_ball\n");
    printf("gives 1 clay\n");
}

/* i'm tired okay? */

// mossy_cobblestone

// jack_o_lantern

// glowstone

// white_wool

// sandstone

// lapis_block

// stone_stairs

// (wooden)_slab

// (wooden)_stairs

// sandstone_slab

// cobblestone_slab

// cobblestone_stairs

// stone_bricks

// melon

// brick_slab

// brick_stairs

// stone_brick_stairs

// nether_bricks

// nether_bricks_stairs

// stone_bricks_slab

// chiseled_sandstone

// sandstone_stairs

// emerald_block

// nether_brick_stairs

/* pickle with planks, bamboo is a weirdo like me 
   i'll see how i put these */

// quartz_block

// quartz_pillar [note to self: jeo]

// quartz_slab [note to self: jeo]

// quartz_stairs [note to self: jeo]

// chiseled_quartz_block [note to self: jeo]

// hay_block [note to self: called bale not block]

// coal_block

// (color)_terracota [note to self: called stained?]

// packed_ice

// (color)_stained_glass

// granite [shapeless!]

// diorite

// andesite [shapeless!]

// polished_diorite

// polished_granite

// polished_andesite

// chiseled_stone_bricks

// mossy_stone_bricks [shapeless!] [multiple options!]

// prismarine

// sea_lantern

// coarse_dirt

// prismarine_bricks

// dark_prismarine [note to self: jeo]

// red_sandstone

// chiseled_red_sandstone

// red_sandstone_slab

// red_sandstone_stairs

// end_stone_bricks

// purpur_block

// purpur_pillar

// purpur_stairs

// purpur_stairs

// magma_block

// bone_block

// nether_wart_block

// concrete_powder [shapeless!] 

// wood

// cut_sandstone

// cut_red_sandstone

// quartz_pillar

// red_nether_bricks

// dried_kelp_block

// prismarine_stairs

// prismarine_brick_stairs

// dark_prismarine_stairs

// prismarine_slab

// prismarine_brick_slab

// dark_prismarine_slab

// blue_ice

// stripped_wood

// scaffolding

// andesite_stairs

// polished_andesite_stairs

// diorite_stairs

// polished_diorite_stairs

// granite_stairs

// polished_granite_stairs

// smooth_sandstone_stairs

// smooth_red_sandstone_stairs

// smooth_quartz_stairs

// red_nether_brick_stairs

// mossy_stone_brick_stairs

// mossy_cobblestone_stairs

// end_stone_brick_stairs

// smooth_stone_slab

// andesite_slab

// polished_andesite_slab

// polished_diorite_slab

// diorite_slab

// end_stone_brick_slab

// granite_slab

// polished_granite_slab

// mossy_stone_brick_slab

// red_nether_brick_slab

// smooth_quartz_slab

// smooth_red_sandstone_slab

// smooth_sandstone_slab

// mossy_cobblestone_slab

// cut_red_sandstone_slab

// cut_sandstone_slab

// netherite_block

// polished_basalt

// polished_blackstone

// polished_blackstone_slab

// polished_blackstone_stairs

// polished_blackstone_bricks

// blackstone_stairs

// polished_blackstone_brick_slab

// polished_blackstone_brick_stairs

// blackstone_slab

// chiseled_nether_bricks

// quartz_bricks

// chiseled_polished_blackstone

// copper_block

// waxed_copper_block [shapeless!]

// amethyst_block

// [waxed_]cut_copper [shapeless!]

// tinted_glass

// [waxed_]cut_copper_slab

// [waxed_]cut_copper_stairs

// dripstone_blocks

// polished_deepslate

// deepslate_bricks

// deepslate_tiles

// chiseled_deepslate

// polished_deepslate_slab

// polished_deepslate_stairs

// deepslate_tile_slab

// deepslate_tile_stairs

// deepslate_brick_slab

// deepslate_brick_stairs

// cobbled_deepslate_slab

// cobbled_deepslate_stairs

// raw_gold_block

// raw_copper_block

// raw_iron_block

// mud_bricks

// mud_brick_slab

// chiseled_bookshelf

// bamboo_mosaic

// bamboo_mosaic_stairs

// bamboo_mosaic_slab

// bamboo_block

// copper_grate

// tuff_bricks

// polished_tuff

// tuff_bricks

// tuff_brick_stairs

// polished_tuff_stairs

// chiseled_tuff_bricks

// !chiseled_copper

// tuff_stairs

// tuff_brick_slab

// chiseled_tuff

// polished_tuff_slab

// tuff_slab

// resing_block

// resin_bricks

// chiseled_resin_bricks

// resin_brick_slab

// resin_brick_stairs

/* - decoration blocks - */

// torch

// chest

// painting

// crafting_table

// ladder

// sign

// snow

// jukebox

// [wooden]_fence

// iron_bars

// glass_pane

// nether_brick_fence

// enchanting_table

// end_crystal

// item_frame

// flower_pot

// mossy_cobblestone_wall

// furnace

// [!fireworks]

// (color_)bed

// stonecutter [note to self: jeo]

// (color_)carpet

// anvil

// stained_glass_pane [!multiple options]

// color_(banner)

// armor_stand

// end_rod

// ((color_))shulker_box

// campfire

// loom

// andesite_wall 

// brick_wall

// diorite_wall

// end_stone_brick_wall

// granite_wall

// mossy_stone_brick_wall

// nether_brick_wall

// prismarine_wall

// red_nether_brick_wall

// red_sandstone_wall

// sandstone_wall

// stone_brick_wall

// barrel 

// smoker

// blast_furnace

// cartography_table

// fletching_table

// grindstone

// smithing_table

// cobblestone_wall

// lantern

// composter

// beehive

// honeycomb_block

// respawn_anchor

// lodestone

// blackstone_wall

// polished_blackstone_wall

// polished_blackstone_brick_wall

// soul_campfire

// iron_chain

// soul_torch

// (color_)candle

// glow_item_frame

// moss_carpet

// polished_deepslate_wall

// deepslate_tile_wall

// deepslate_brick_wall

// cobbled_deepslate_wall

// packed_mud

// mud_brick_wall

// muddy_mangrove_roots

// [wooden_]hanging_sign

// decorated_pot

// tuff_wall

// polished_tuff_wall

// tuff_brick_wall

// [!]shelf

// creaking_heart

// resin_brick_wall

// dried_ghast

// copper_chest

// copper_golem_statue

// copper_torch

// copper_chain

// copper_bars

// copper_lantern


/* - redstone - */

// lever

// iron_door

// redsonte_torch

// redstone_dust

// note_bock

// [wooden_]pressure_plate

// stone_pressure_plate

// stone_button

// sticky_piston

// []fence_gate

// tripwire_hook

// lectern

// [wooden_]button

// redstone_block

// trapped_chest

// hopper

// [wooden_]door

// dropper

// dispenser

// redstone_lamp

// daylight_sensor

// piston

// repeater

// comparator

// tnt

// slime_block

// iron_trapdoor

// [wooden_]trapdoor

// observer

// light_weighted_pressure_plate [not called gold]

// heavy_weighted_pressure_plate [not called iron]

// honey_block

// target

// polished_blackstone_button

// polished_blackstone_pressure_plate

// lightning_rod

// calibrated_sculk_sensor

// crafter 

// copper_bulb

// copper_door

// copper_trapdoor [note to self: jeo]


/* - transportation - */

// minecart

// rail

// furnace_minecart

// chest_minecart

// powered_minecart

// tnt_minecart

// activator_rail

// hopper_minecart

// [wood_]boat

// [wood_]chest_boat

/* [bamboo is the same but called raft] */


/* - foodstuffs - */

// bread

// mushroom_stew [shapeless!]

// golden_apple

// cookie

// golden_carrot

// pumpkin_pie [shapeless!]

// cake

// beetroot_soup [shapeless!]

// rabbit_stew [shapeless!]

// dried_kelp

// suspicious_stew [multiple options!] [shapeless!]

// honey_bottle [shapeless!]


/* - tools - */

// flint_and_steel

// [material_]pickaxe

// [material_]hoe

// [material_]axe

// [material_]shovel

// fishing_rod

// shears

// carrot_on_a_stick

// warped_fungus_on_a_stick

// brush


/* - utilities - */

// saddle

// bucket

// compass

// clock

// (color_)bundle

// spyglass

// recovery_compass

// (color_)harness


/* - combat - */

// bow

// [material_]helmet

// turtle_shell

// [material_]leggings

// [material_]boots

// arrow

// [material_]chestplate

// [material_]sword

// spectral_arrow

// tipped_arrow

// shield

// crossbow

// mace

// wolf_armor

// wind_charge


/* - brewing - */

// glass_bottle

// blaze_powder

// magma_cream

// fermented_spider_eye

// cauldron

// brewing_stand

// glistering_melon_slice


/* - materials - */

// wheat

// stick

// bowl

// leather

// slimeball

// [!]ingot(also coal, emerald, diamond, and lapis lazuli)

//// netherite ingot opts!

//// same for ingots as for raw 

///// and resin clumps!

// [!]nugget(s

// sugar [x2 opts]

/// [!!]dye

/// [!!] seeds


/* - miscellaneous - */

// book

// map

// ender_eye

// fire_charge

// writable_book (not called book and quill)

// beacon

// [material_]horse_armor

// paper

// conduit

// [!] banner_pattern

// music_disc_5

///// !trims


