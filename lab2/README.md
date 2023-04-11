# Quick summary

## Task A
You'll have configured D0, D1, D2 and D3 as outputs.  
D0 will blink using `SysCtlDelay`, same as in lab1, kinda ez  

NB: `SysCtlDelay` is a 3 cycle loop (RTFM xd),  
ie: if you want 3\*x cycles, pass it x

## Task B
You'll have configured D1 to blink using interrupts  
D0 should still be blinking, they will likely be quickly out of sync even if you try to set them to the same theoretical frequency.  
That's normal.  

NB: the half-width counter is 16-bit, the full-width is 32-bit, the max sets all bits to 1s

## Task C1
You'll have configured D2 to "blink" (the frequency is fast enough that the visible effect is on the brightness instead) using PWM.  
Be careful to set everything up carefully.  
I messed up in a subtle way during the lab and the previous diodes _also_ stopped blinking (the code in this repo is fully working now though).  
I skimmed the Driver Library and I think this could've been a bus fault, anyway check every line of setup if things get weird.  

Make sure to check the diagram for the details of which PWM thingy to set up, depends on which GPIO you've got for a certain LED:  
![Pin Configurations](../Page2-DK-TM4C123G%20Getting%20Started%20Guide.png)

## Task C2
You'll have configured D3 (in the lab/repo I accidentally did this on D2, you'll need to try separate the two) to change brightness (using PWM again).  
The PWM setup is almost the same as in C1, the interrupt setup is almost the same as in B.  
The main gotcha for this task is probably if you're assigned to set "0% brightness" somewhere in the loop, then remember that you shouldn't actually set the pulse width to 0...

## Final state
You'll have two diodes blinking out of sync, a third with lowered brightness, and the fourth cycling through brightness steps

## Note
Useful lectures are 4 and 7 imo, mayybe 3 also?

