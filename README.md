# mc-utils

these are simple utilites to help forgetful people with simple minecraft related issues, including, as of now:
  forgetting how much wood, leather, and sugar cane is needed for a full level enchanting table,
  forgetting how much iron is needed for an anvil,
  keeping track of how much more netherite you need to complete a beacon (5904), and
  calculating N stacks + remainder from a quantity; or a total quantity from N stacks


they are not a bash script because i don't know bash :c

and they are in C because i want them to be portable across unix-like (or at least linux)

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
  -t, --total
for --total:
            use --stacks and --items (or -s and -i) to calculate the total amount

coming soon (or not very soon idk):
  -c, --craft <item_name> [--]       Show the crafting recipe (or where or how to obtain)
also intend to add --craft


# building:

for mc-calc, compile mc-calc.c     
for netherite-left, you must first, invent the universe.

<details>
<summary>note</summary>
wow look a click to expand note!
note:
questioning or judging of my possibly questionable #include practices will likely not be taken into account
</details>
