# XtUsb

Simple Sketch for Teensy boards for an XT to USB converter

## Building

The teensy needs to be set to to a Keyboard/Serial combo HID device.

Right now Scrollock is used as a special toggle and an alternative to numlock where the arrow keys send 
codes for the t-nav cluster of newer keyboards. I found that some apps simply don't recognize the arrow
navs of the numpad for some reason. 

I also swap the ctrl and capslock keys by default, edit the sketch to change this. I intend to make this
togglable eventually

