# aos
# authors: team-810

First commit: 11.26

Button functions finalized, ready for debugging.

Left button: move left 1 unit with a 10 ms delay; can move while jumping or falling; should not move when hitting a wall.

Right button: move right 1 unit with a 10 ms delay; can move while jumping or falling; should not move when hitting a wall.

Jump button: jump to as high as 100 unit at most; move up 1 unit with a 10 ms delay; cannot jump while falling.

Early release of the jump button causes the character to fall. Falling is 1 unit down with a 10 ms delay.

Falling should occur even if no button is pressed. Should not fall below the ground.

Current setting: ground level: 0, left wall: -1000, right wall: 1000.