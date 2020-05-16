# 2020-pd2-sokoban

## Video link

https://youtu.be/z6SEDZxqcVw

## How to play

Use `W` `S` `A` `D` to control the character.  
The Blue character is player.  
The pink character is box.  
The green character is storage point.  

## Features

I did features below.  
1. Main menu  
2. Save Game (Save to `build-*/saves` directory)  
3. Show number of steps  

## Bonus

I use file I/O to load level so if you want to add some map into this game,  
you can add file named `level_<number>.dat` to `datas` directory and add to resource.  
The `<number>` of level should be continuous increasing.  
The size of map is at most 20*20.  

It will play sound when clear a level or replay.  
  
And I change Window name and icon, too.

### About Map Format

0 Box on goal  
1 Wall  
2 Goal  
3 Ground  
4 Player  
5 Box  

Becareful that the player can only appear once, or the game will crash.
