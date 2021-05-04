# neo_trinkey_macro
Arduino macro sketch for adafruits neo trinkey device

What is a Neo Trinkey:

A neo trinkey is an adafruit device. It has a microprocessor, 4 full rgb leds, and the side has 2 capacitive touch keys and fits into a usb port. 
https://www.adafruit.com/product/4870

What this sketch does:

This sketch sets up the neo trinkey device as a macro keyboard. It does not keep macros encrypted and the read fuses are not set (yet), so just be aware someone skilled could pull the bitstream and decompile to get the macros out. I am fine with this as mine won't leave my possession.

Usage: 

To set up the macros, edit the macros array, compile and write to device.

It lights the led that corresponds to the slot. So you touch button 2, and it rotates through the macros in the layer, and you know which one you are on by which one of the 4 leds is lit. then you touch the other button to write the macro (as a usb keyboard).

Options:

By default, layers are disabled (cause I don't need them yet) but by uncommenting (aka defining EN_LAYERS) you can have 3 layers with 4 macros each.
If you enable layers, each layer has a different primary color on the selection RGB led.
Layers are switched when you touch both buttons at the same time. I have not done as much fine tuning with the layers selection as i did with the touch buttons, so your mileage might vary. 


Planned:
I think it would be really cool to be able to set one of the macro slots to output FIDO 2FA, or mimic some of the other hardware keys' modes (like yubikey)
