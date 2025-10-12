### mc-calc:
add reverse goal mode: 'total'
--total --stacks or -s + --items -i (possible --stacks-small)
to output total quantity


# example, you have two stacks and 27 iron ingots:

    $ mc-calc --total --stacks 2 --items 27
    155.

    #or
    $ mc-calc -t -s 2 -i 27
    155.

# or using small stacks (16), say you have 3 stacks and 2 ender pearls:

    $ mc-calc --total --stacks-small 3 --items 2 
    50.

    #or
    $ mc-calc -t --stacks-small 3 -i 2
    50.

                                            # let me know if you want -ss instead of --stacks-small


# non-passive-aggresive
#or
    $ mc-calc --total --stacks 2 --items 27 --non-passive-aggresive
    155

    $ mc-calc -t -s 2 -i 27 --non-passive-aggresive
    155

    $ mc-calc -t --stacks-small 3 -i 2 --non-passive-aggresive
    50

    $ mc-calc -t --stacks-small 3 -i 2 --non-passive-aggresive
    50

