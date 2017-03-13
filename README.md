# OneFuncButton
A simple Arduino button library modelled as a state machine

##Constructing a button object

Button constructor receives one mandatory and three optional arguments.

```c++
Button button1(pin [, activeHigh] [, firstHoldTime] [, subsqHoldTime]);
```

Examples:

```c++
Button button1(5);
Button button2(6, LOW);
Button button3(7, HIGH, 500, 300);
```

###Arguments

* **pin** - Arduino pin to which the button is connected
* **activeHigh** - HIGH for for active-high, LOW for active-low
* **firstHoldTime** - time required to trigger the first HOLD event
* **subsqHoldTime** - time required to trigger all subsequent HOLD events after the first one

##Getting the button state

The state() function needs to be called continuously in a loop. When the state() function is called and an event has occured since the previous call, it returns one of the following values:

```c++
byte Button.state();
```

Example:

```c++
byte s = button1.state();
```

###Button states

* **PRESS_SHORT** - The button has just been pushed down.
* **PRESS_LONG_FIRST** - The first hold event has just been triggered.
* **PRESS_LONG_SUBSQ** - A non-first hold event has just been triggered.
* **RELEASE_SHORT** - The button has just been released, and was not held long enough for any hold event to occur.
* **RELEASE_LONG_FIRST** - The button has just been released after one hold event has occured.
* **RELEASE_LONG_SUBSQ** - The button has just been released after two or more hold events have occured.

In all other cases, when there is no event to report, it returns one of the following states:

* **UP** - The button is currently up.
* **DOWN_SHORT** - The button has been pushed and is held down.
* **DOWN_LONG_FIRST** - The button is being held and one hold event has occured.
* **DOWN_LONG_SUBSQ** - The button is being held and two or more hold events have occured.

Three macros are provided which evaluate a stored button state:

* **ISDOWN(byte)** - Evaluates true for any state in which the button must be down
* **PRESSEVENT(byte)** - Evaluates true for any PRESS event
* **RELEASEEVENT(byte)** - Evaluates true for any RELEASE event

###User interaction example

In the following example the user makes one short press and one long press (which is held enough to trigger three long-press events). Black line represents the physical state of the button. Logical states and press-release events are marked below.

![OneFuncButton graphical example in time](https://raw.githubusercontent.com/athnix/OneFuncButton/master/onefuncbutton_example.png)

##Code examples

###Hold to increment

The following code increments the `minutes` variable by one, waits for a while (firstHoldTime), and then keeps increasing by one in shorter intervals (subsqHoldTime). This is a behavior similar to real-world digital clocks when their time is being set.

```c++
Button btn1(3);
byte s = btn1.state();
if (PRESSEVENT(s))
    minutes++;
```

###Short-press and long-press actions

The following code allows the user to perform two different actions by either short-pressing or long-pressing the button (without triggering the short-press action if they only intended to perform the long-press action).

```c++
Button btn1(3);
byte s = btn1.state();
switch (s) {
    case RELEASE_SHORT: shortPressAction(); break;
    case DOWN_LONG_FIRST: longPressAction(); break;
}
```

##Macros

Debouncing time can be changed by changing the DEBOUNCING_TIME macro in the function. By default it is 15 ms.
