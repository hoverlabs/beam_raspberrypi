/*
===========================================================================

  This is an example for Beam.

  Beam is a beautiful LED matrix — features 120 LEDs that displays scrolling text, animations, or custom lighting effects.
  Beam can be purchased here: http://www.hoverlabs.co

  Written by Emran Mahbub and Jonathan Li for Hover Labs.
  BSD license, all text above must be included in any redistribution


#  HISTORY
    v1.0     -  Initial Release
    v1.0RPi  -  Ported to RPi, Dec/16 Ryan Shuttleworth, rshuttle@gmail.com

#  INSTALLATION
    The 4 library files (beam.cpp, beam.h and charactermap.h and frames.h) are required to run Beam.
    Run the BeamDemo.ino file.

#  SUPPORT
    For questions and comments, email us at support@hoverlabs.co
===========================================================================
*/
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parse_frames.h"
#include "../../beam.h"


/* pin definitions for Beam */
#define RSTPIN 27        //use any digital pin
#define IRQPIN 22        //currently not used - leave unconnected
#define BEAMCOUNT 1     //number of beams daisy chained together

static int beamCount = 1;
static int speed = 2;
static int loops = 7;

static uint32_t frameIndex = 0;

extern uint8_t frameList[36][15];

/* Iniitialize an instance of Beam */
Beam b = Beam(RSTPIN, IRQPIN, BEAMCOUNT);
//Beam b;

/* Timer used by the demo loop */
unsigned long updateTimer = 0;
int demo = 0;

void setup(void) {

    // this doesn't work well with beam, timing?
    // wiringPiSetup();

    // fast and reliable beam functionality but requires root
    wiringPiSetupGpio();

    // does not work at all with beam
    // wiringPiSetupPhys();

    // this doesn't work well with beam, timing?  Unfortunate because it
    // doesn't require root access the way wiringPiSetupGpio does
    //wiringPiSetupSys();
}


void intro(void) {
    printf("Starting Beam example\n");

    b.begin();
    b.print("Hello World. This is Beam!");
    b.play();
}

void demo_loop() {

    /*
    The following cycles through two different demos every 20 seconds.
    Note that it is not necessary to print to Beam on every loop.
    */

    if (millis() - updateTimer > 20000){

        if (demo == 0){
            /*
            The print() command prints and scrolls text across Beam.
            */
            b.print("This is an example of fast scrolling text on Beam. ");
            b.setSpeed(3);
            b.setLoops(7);
            b.play();

            demo = 1;
            updateTimer = millis();

        } else if (demo == 1){
            /*
            The draw() command draws the frames defined in the frames.h header file,
            and animates them as if flipping through a book.
            */
            b.draw();
            b.setSpeed(2);
            b.setLoops(7);
            b.play();

            demo = 0;
            updateTimer = millis();
        }
    }
    // do something else here
}

void printHelp(void) {
    printf("Usage:\n");
    printf("sudo ./BeamDemo [options]\n");
    printf("Options:\n");
    printf("\t-n [1-4] set the number of beams (defaults to 1)\n");
    printf("\t-s [1-15] set the frame speed (defaults to 1, 15 is slowest)\n");
    printf("\t-l [1-7] set the number of frame loops (defaults to 7)\n");
    printf("\t-t \"some text\" display text on the beam\n");
    printf("\t-f \"some path\" load and display a frames file\n");
    printf("\t-h print this help\n");
    printf("To run a demo specify the -n [1-4] option or no options\n");
    printf("\n");
}

void parse_value(uint32_t line, uint32_t col, char *s) {
    uint32_t f;
    uint32_t i;
    int32_t v;

    f = (frameIndex / 15) % 36;
    i = frameIndex % 15;
    v = anytoi(s);

    if (i == 0) {
        printf("\nframe[%d]: ", (int) f);
    }

    printf("%02x ", (unsigned int) v);

    frameList[f][i] = (uint8_t) v;
    frameIndex++;
}

int parseFrameFile(const char *path) {
    frameIndex = 0;
    printf("Loading frame file: %s", path);
    parse_csv(path, ",", parse_value);
    printf("\n");
    return 1;
}

int main (int argc, char *argv[]) {
    char *tvalue = NULL;
    char *nvalue = NULL;
    char *fvalue = NULL;
    char *svalue = NULL;
    char *lvalue = NULL;
    int index = 0;
    int c = 0;

    while ((c = getopt(argc, argv, "hn:t:f:s:l:")) != -1) {
        switch (c) {
            case 'h':
                printHelp();
                exit(0);
            case 'n':
                // num beams
                nvalue = optarg;
                break;
            case 't':
                // display text
                tvalue = optarg;
                break;
            case 'f':
                fvalue = optarg;
                break;
            case 's':
                svalue = optarg;
                break;
            case 'l':
                lvalue = optarg;
                break;
            case '?':
                if (isAmoung(optopt, "ntp") == 1) {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                } else {
                  fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
                }
                return 1;
            default:
                abort();
        }
    }

    if (nvalue != NULL) {

        beamCount = atoi(nvalue);

        if (beamCount < 1) {
            beamCount = 1;
        } else if (beamCount > 4) {
            beamCount = 4;
        }
    }

    if (svalue != NULL) {
        speed = atoi(svalue);
        if (speed < 1) {
            speed = 1;
        } else if (speed > 15) {
            speed = 15;
        }
    }

    if (lvalue != NULL) {
        loops = atoi(svalue);
        if (loops < 1) {
            loops = 1;
        } else if (loops > 7) {
            loops = 7;
        }
    }

    setup();
    b = Beam(RSTPIN, IRQPIN, beamCount);

    if (tvalue != NULL) {
        b.begin();
        b.print(tvalue);
        b.play();
    } else if (fvalue != NULL) {
        // todo: load beam from file path in fvalue
        parseFrameFile(fvalue);
        b.begin();
        b.draw();
        b.setSpeed(speed);
        b.setLoops(loops);
        b.play();
    } else {
        // default activity if no 't' or 'f' options given
        intro();
        while(1) {
            demo_loop();
        }
    }

    return 0;
}
