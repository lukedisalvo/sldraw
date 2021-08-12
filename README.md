# sldraw
This is the repo for a drawing application

## Pre-Checks
- Vagrant installed.
- Oracle Virtual Box installed.
- Virtualization is turned on in BIOS.

## How to Run
Clone this repository and open up a bash prompt.
Run the following commands
```sh
vagrant up
vagrant halt
vagrant up
```
If it is your first time running this command, this will take a long time. After it is done a gui window should appear. Right click and select "terminal" to open up the gui terminal.

Next run the following commands:
```sh
make
cmake --build .
```
Upon successful completion. The project should now be built.

## Test it!
Run the following command
```sh
./sldraw tests/test_car.slp
```
You will see a car being drawn in a window. Any other tests in the tests folder may be run with the same format.

## Make Your Own Drawing!
There are three different options to start drawing to your heart's desire.
Point with  a  value  of  two  coordinates  of  type  Number.  Points  are rendered as filled circles.Ex:
```sh
(point 0 0)
```
Line with a value of two Points.
Ex: 
```sh 
Line -> (line (point 0 0))
```
Arc with a value of two Points and a Number. The first Point is the center  of  the  arc,  the  second  is  the  starting  point  of  the  arc,  and  the  Number  is  the spanning angle in radians. Ex:
```sh
Arc -> (arc (point -300 -300) (point -275 -300) (/ pi 8))
```

To draw to the windows use the draw command:
```sh
(draw (arc (point -300 -300) (point -275 -300) (/ pi 8)))
```