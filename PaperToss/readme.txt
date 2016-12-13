/* 
 Borna Kalantar kalantb 001207031
 Rebecca Tran tranr5 001425611
 Neil Robichaud robichne 001425566
 Sean McKay mckaysm 001423885
 */

 Extra Features:
 	- Advanced Camera Control
 		- Can switch between using polar coordinates and regular coordinates
 	- Textures
 		- Different texture placed on the ball for each level
 	- Ray Picking
 		- Can pick up the ball after it is thrown
 	- Lighting
 		- Lighting on the whole game (material added as well)
 	- We tried implementing the animated character, snowman, where it is an obstacle. We came across the issue of bounding the snowman and not knowing the coordinates of the moving snowman (it was supposed to go back and forth on the x axis to block the ball being shot). Since we couldn't figure out the coordinates, the ball just goes through the snowman rather than bouncing off it after "hitting" it, and we removed it from the game. 

Rest of implementation:
	- Three levels in total, 5 stages per level
	- At each stage, the basket is in a different position
	- With each level, there is a different ball (different texture). Level 1: Wood, level 2: rock and level 3: bouncy. 