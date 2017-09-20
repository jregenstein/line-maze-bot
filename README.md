# line-maze-bot
Basic logic:

For solving the maze, our mBot is programmed to first follow a straight line and make slight adjustments unless both of the middle two sensors are reading the black line. When the side sensors detect the line, we determine that we have reached an intersection and call the evaluateIntersection() method. This method determines whether we should turn left, turn right, go straight or celebrate because we’ve reached the goal. It does this by first checking the leftmost sensor to see if a left turn is available, checking the rightmost sensor to see if a right turn is available, and then moving forward and checking the middle sensors to see if a straight line is available. Since we use the left-hand rule, we prioritize turning left. If turning left isn’t an option, the mBot will prioritize going forwards and if there are no other options it will turn right. If it reaches a dead end, the bot will move forward a bit and then make a u-turn. We determine that the mBot has reached the goal if we move forward and the leftmost and rightmost sensors still detect the line.

With regards to turns, to make sure the robot ends up on the line, we manually turn roughly 80 degrees and then have the mBot keep turning until it detects the line with its middle sensors. The 80 degrees is a roughly calibrated number that helps us avoid the situation where the mBot falsely assumes it has completed its turn when it discovers another line while turning. 

How we solved the loops:

Since we do not have enough time, we did not actually implement an advanced algorithm. They way we solved it is by setting up a counter which counts the number of left turns it made. Here is how the counter works:
Left turn: +1
Right turn: -1
U turn: -2 (because we use the left hand rule, the robot always need to turn left twice to get in and outside the deadend)
When it exits the loop, reset the counter to 0.
We originally want to count the consecutive left/right turns, but there is no way for us to know when the robot is getting into the loop, so we just try to counteract the leftturns with the rightturns. When the counter exceeds the threshold(hard-coded, might be incrementing for better performance. For our case it is 8, since we have a small maze), it will start to seek another way out (aka. When seeing an intersection, blind the left option, if there is a straight line, go straight; else if there is a right option, turn right). One problem we might experience is, the counter might have super large count before it enters a loop (like a super large spiral).

Other known issue:

The one other issue we know about occurs when we hit an intersection at an off angle. This may confuse the robot into making a turn that it shouldn’t make and going off of the line.  
