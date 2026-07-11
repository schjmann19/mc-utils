# a "hardcoded coord to relative to ~ ~ ~ coord -converter"
## _(crconvert (Coordinate Relative Convert))_
if a command block is placed at `100 100 100`, and it does "`setblock 101 101 101 air`",
convert that to "`setblock ~1 ~1 ~1 air`"
### more example:
_(given the command block is at 100 100 100)_
`fill 250 250 250 50 50 50 air` -> `fill ~150 ~150 ~150 ~-50 ~-50 ~-50 air`

### why?
because we may not know the coordinates of the /fill and /setblock targets. of course,
we do, but maybe if only the build is exported to another world, i want it to be pasteable anywhere,
not specifically at the same coords we're building it in a superflat world

### Plan
its only prerequisite would be that the user gives the coordinates, so like:
```sh
mc-util --crconvert 100 100 100 "fill 250 250 250 50 50 50 air"
fill ~150 ~150 ~150 ~-50 ~-50 ~-50 air
```
argument 1 -> the coords of the command block argument 2 -> (quoted in the shell) the command to convert

### Notes

# It's a little more complicated than it first appears

The converter has to know which arguments are coordinates, because commands all have different syntax.

For example:
```
fill x1 y1 z1 x2 y2 z2 block
```
vs.
```
setblock x y z block
```
vs.
```
tp @s x y z
```
vs.
```
particle dust 1 0 0 1 x y z dx dy dz speed count
```
The converter can't just replace every number, it needs a parser for Minecraft commands.

### Fortunately...

Mojang already publishes the command grammar in the game's data. Libraries exist that can parse Minecraft commands properly (Brigadier-based parsers).

If we only care about a subset (fill, setblock, clone, summon, tp, etc.), it's much easier.

^ for now, only support /fill and /setblock