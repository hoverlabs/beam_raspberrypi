#------------------------------------------------------------------------------

SOURCE=beam.cpp beam.h charactermap.h frames.h parse_frames.h parse_frames.cpp examples/BeamDemo/BeamDemo.cpp
MYPROGRAM=BeamDemo
MYINCLUDES=/usr/include/
FLAGS=permissive
MYLIBRARIES=wiringPi
CC=g++

#------------------------------------------------------------------------------



all: $(MYPROGRAM)



$(MYPROGRAM): $(SOURCE)

	$(CC) -I$(MYINCLUDES) -I./ -f$(FLAGS) $(SOURCE) -o$(MYPROGRAM) -l$(MYLIBRARIES)

clean:

	rm -f $(MYPROGRAM)
