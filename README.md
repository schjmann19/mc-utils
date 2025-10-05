# mc-utils

these are simple utilites to help forgetful people with simple minecraft related issues, including, as of now:
  forgetting how much wood, leather, and sugar cane is needed for a full level enchanting table,
  forgetting how much iron is needed for an anvil,
  keeping track of how much more netherite you need to complete a beacon (5904), and
  calculating N stacks + remainder from a quantity

they are not a bash script because i don't know bash
and they are in C because i want them to be portable across unix-like, or at least linux

# netherite-left
options:
  -s, --scraps <N>     Number of scraps (integer)
  -i, --ingots <N>     Number of ingots (integer)
  -b, --blocks <N>     Number of blocks (integer)
  -h, --help           Show this help and exit
  -v, --version        Show version and exit
  -g, --gold           Show hint for gold and exit

(for gold, just pretend it's called gold-left)

# mc-calc
options:
      --goal <number> [--small]  Calculate stacks for given number
      --enchant                  Show enchanting table requirements
      --anvil                    Show anvil crafting requirements
  -v, --version                  Show version information
  -h, --help                     Show this help message


# building:

for mc-calc, compile mc-calc.c
for netherite-left, you must first, invent the universe.


