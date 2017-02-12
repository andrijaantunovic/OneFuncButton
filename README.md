# OneFuncButton
A simple Arduino button library modelled as a state machine

##Button states

A button can be in a number of possible states. The return value of the `state()` function can be one of the internal states (UP, DOWN\_\*) or a one-time event which is not actually stored as an internal state (PRESS\_\*, RELEASE\_\*). Additionaly, two internal states (DEBOUNCING\_\*) are abstracted inside the class and as such are never returned from the function.

Internal states, which are also returned as return values inbetween events, are:  
```c++
#define UP                 0b000000 // The button is currently up.
#define DOWN_SHORT         0b000001 // The button has been pushed and is held down.
#define DOWN_LONG_FIRST    0b000011 // The button has been held and one hold event has occured.
#define DOWN_LONG_SUBSQ    0b000111 // The button has been held and two or more hold events have occured.
```

In most cases the internal states aren't very useful and you're better off just using the button events. Button events are:  
```c++
#define PRESS_SHORT        0b001001 // The button has just been pushed down.
#define PRESS_LONG_FIRST   0b001011 // The first hold event has just been triggered.
#define PRESS_LONG_SUBSQ   0b001111 // The second or other hold event has just been triggered.
#define RELEASE_SHORT      0b010000 // The button has just been released, and was not held long enough for a hold event to occur.
#define RELEASE_LONG_FIRST 0b010010 // The button has just been released after one hold event occured.
#define RELEASE_LONG_SUBSQ 0b010110 // The button has just been released after two or more hold events occured.
```	

The following two states are internal-only and are intentionally abstracted inside the class:
```c++
#define DEBOUNCING_UP      0b100000
#define DEBOUNCING_DOWN    0b100001
```
When the button is in the `DEBOUNCING_UP` state, `UP` is returned, and when it's in the `DEBOUNCING_DOWN` state, `DOWN_SHORT` is returned.

Three macro functions are also provided to simplify some common use-cases:
```c++
#define ISDOWN(X)          (X & 1)         //Evaluates true while the button is down
#define PRESSEVENT(X)      (X & 0b001000)  //Evaluates true when any PRESS event occurs
#define RELEASEEVENT(X)    (X & 0b010000)  //Evaluates true when any RELEASE event occurs
```

These macros are enabled by the fact that each bit in the states' binary values has a specific meaning:
```
0b001011
  ||||||
  |||||`- Button is down?
  ||||`-- Short press (0) or long press (1)?
  |||`--- If long-press phase, first longpress phase (0) or subsequent longpress phase (1)?
  ||`---- Press event?
  |`----- Release event?
  `------ Debouncing?
```

##How the function works

(Put image here.)

##Constructing a button object

Button constructor takes one mandatory and three optional arguments. The mandatory argument is the pin to which the button is connected.

```c++
Button btn1(3);
```

This will construct a button with the default values (active-high, firstHoldTime = 800, subsqHoldTime = 200). You can provide additional arguments to have the button active-low, or to have different HoldTime delays:

```c++
Button btn1(3, LOW, 500, 300);
```
		
##Examples of use

Here is an example of a simple use case:

###Hold to increment

```c++
byte s = btn1.state();
if (PRESSEVENT(s))
    minutes++;
```

This will increment the minutes variable similarly to how incrementing the time while setting a real digital clock works. When the button is pressed and held, it will increase by one immediately, wait for a while (firstHoldTime), and then keep increasing by one in shorter intervals (subsqHoldTime).

###Short-press and long-press actions

```c++
byte s = btn1.state();
switch (s) {
    case RELEASE_SHORT: shortPressAction(); break;
    case DOWN_LONG_FIRST: longPressAction(); break;
}
```

This allows the user to perform two different actions by either short-pressing or long-pressing the button (without triggering the short-press action if they only intended to perform the long-press action).

###Short-press and long-press actions with cancel

```c++
Button btn1(3, HIGH, 800, 2000); //We have the subsqHoldTime set to 2000 so that the user has enough time to release the button inbetween the moments when the long action tone is played and when the action would be cancelled
byte s = btn1.state();
switch (s) {
    case RELEASE_SHORT: shortPressAction(); break;
    case PRESS_LONG_FIRST: playLongActionTone(); break;
    case RELEASE_LONG_FIRST: longPressAction(); break;
    case PRESS_LONG_SUBSQ: playCancelTone(); break;
}
```
In this example the user is able to change their mind and not execute any action even after they started holding the button down by holding it for an additional 2000 ms after the long-action tone. If the user wishes to initiate a long-press action, they need to release the button after the long-action tone but before the cancel tone. This is similar to a feature found on some UPS devices which only have one button for both powering the equipment on and off.

##Other notes

Debouncing time can be changed by changing the DEBOUNCING_TIME macro in the function. By default it is 15 ms.
