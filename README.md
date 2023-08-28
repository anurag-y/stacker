# Stacker Game
Stacker (arcade game) implemented on Arduino using MAX7219 LED Display ( 4-in1).
This Stacker game project was made for a course project at my college.
## Demo video
A video of game-play can be saan at: https://youtube.com/shorts/A0G4Um-1eNA?si=xUB6_Y98Zq_mo6qK
# Hardware used and their specifications:
1. Arduino Uno
2. MAX7219 LED Matrix (4-in-1)
3. Breadboard
4. Pushbutton
5. Buzzer (not essential)
6. A few M2M wires

# Game description:
1. An array of ON LEDs moves from one side to another, changing directions on hitting the edge.
2. When the user gives an input through the pushbutton array stops there.
3. At height==0 the place where LEDs stop serve as reference. From now on, the goal of player is to align array of light moving at next height.
4. In case of imperfect alignment size of array for the next level is reduced to the no of LEDs aligned. In case of total miss game is over.
5. When height increases by 8, size of ON array decreases if it has already not decresed due to misalignment.
6. Reaching the top is WIN.

# Extra features added
1. Players can choose level
2. Display of score and heighest score at a given level
3. Music by buzzer for clicks, WINs and losses.



Credits: Music notes used from https://github.com/xitanggg/-Pirates-of-the-Caribbean-Theme-Song
