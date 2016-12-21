# beam_rpi

Beam library for raspberry pi, ported from original code by
[Hover Labs](http://www.hoverlabs.co) in December(ish) 2016 by
Ryan Shuttleworth, @rshuttle.  Contact us at support@hoverlabs.co

## Requirements

- [WiringPi](https://github.com/WiringPi/WiringPi) must be installed

## Build

`make clean;make`

Output in

## Usage

`sudo ./BeamDemo [options]`

__Options:__

   `-n [1-4] set the number of beams (defaults to 1)`

   `-s [1-15] set the frame speed (defaults to 1, 15 is slowest)`

   `-l [1-7] set the number of frame loops (defaults to 7)`

   `-t "some text" display text on the beam`

   `-f "some path" load and display a frames file`

   `-h print this help`


To run a demo specify the -n [1-4] option or no options

## Examples

__Run Demo on 1 beam:__

   `sudo ./BeamDemo`

__Run Demo on 2 beams:__

   `sudo ./BeamDemo -n 2`

__Text on 1 beam:__

   `sudo ./BeamDemo -t "my text here"`

__Text on 3 beams:__

   `sudo ./BeamDemo -n 3 -t "my text here"`

__Custom frames:__

   `sudo ./BeamDemo -f "./sample-frames.txt"`

__Custom frames on 3 beams:__

   `sudo ./BeamDemo -n 3 -f "./sample-frames.txt"`





