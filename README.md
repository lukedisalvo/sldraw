# sldraw
This is the repo for a drawing application.

## Pre-Checks
- Vagrant installed.
- Oracle Virtual Box installed.
- Virtualization is turned on in BIOS.

## How to Run
Clone this repository and open up a bash prompt.
Run the following commands, wait for completion following each command entered.
```sh
vagrant up
vagrant halt
vagrant up
```
If it is your first time running the vagrant up command, this will take a long time. After it is done a gui window should appear. Right click and select "terminal emulator" to open up the gui terminal.

Next run the following commands, wait for completion following each command entered:
```sh
cd /vagrant
make
cmake --build .
```
Upon successful completion. The project should now be built.

## Test it!
Run the following command
```sh
./sldraw tests/test_car.slp
```
You will see a car drawn in a popup window. Any other tests in the tests folder may be run with the same format.

## Make Your Own Drawing!
There are three different options to start drawing to your heart's desire.
Point with  a  value  of  two  coordinates  of  type  Number.  Points are rendered as filled circles.Ex:
```sh
(point 0 0)
```
Line with a value of two Points.
Ex: 
```sh 
(line (point 0 0))
```
Arc with a value of two Points and a Number. The first Point is the center  of  the  arc,  the  second  is  the  starting  point  of  the  arc,  and  the  Number  is  the spanning angle in radians. Ex:
```sh
(arc (point -300 -300) (point -275 -300) (/ pi 8))
```

To draw to the windows use the draw command:
```sh
(draw (arc (point -300 -300) (point -275 -300) (/ pi 8)))
```

All of these can be drawn one at a time by entering into the box at the bottom of the screen
next to the "slisp>" icon.

Using the special command begin in a .slp file allows you to draw multiple points/lines/arcs. I suggest looking
at the test files for formatting. 
Happy Drawing!