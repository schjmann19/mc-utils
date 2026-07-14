#define USAGE_TEXT \
    "\
    mc-util --status, --srvcheck         Check the status of a Minecraft server (online?, players?, MOTD, etc)\n\
    mc-util --goal <number> [--small]    Calculates stacks for a given number\n\
    mc-util --enchant                    Show enchanting table requirements (wip)\n\
    mc-util --anvil                      Show anvil crafting requirements\n\
    mc-util -v, --version                Show version information\n\
    mc-util -h, --help                   Show this help message\n\
    mc-util -t, --total                  Calculate total items from stacks and items\n\
        Options for --total:\n\
            -s, --stacks <number>        Number of full stacks\n\
        -i, --items <number>             Number of individual items\n\
    mc-util --download                   Download latest Minecraft JAR to ~/mc-util/data/\n\
    mc-util --recipe [<item>]            Show crafting recipe for <item> (uses ~/mc-util/data/recipes.json), \n\
                                         or interactively let the user choose the item with fzf.\n\
        mc-utl --recipe --recipes=<path> Use a custom recipes file\n\
    mc-util --generate-list              List all crafteable items (one per line, for fuzzy finding)\n"

#define ENCHANTMENT_TABLE_TEXT \
    "\
    For a max level (30) table, you need 15 bookshelves.\n\
    For which you need 45 books.\n\
    For which you need 45 leather and 135 paper.\n\
    For which you need 2 stacks and 7 sugar cane.\n\
    Also, 1 stack and 26 planks.\n\
"