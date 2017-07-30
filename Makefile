// Add by yaukey at 2017-07-30.
// Make file.

CPP = clang++
CPPFLAGS = -O3

ALL_R = smallrt
ALL_MAIN_O = main.o
ALL_BASE_O = 
ALL_TEMP = *.o *.out
RM = rm -f

ifdef DEBUG
CPPFLAGS=-g
endif

default: &(ALL_R)

&(ALL_R) : $(ALL_MAIN_O) $(ALL_BASE_O)
	CPP -o $(ALL_R) $(ALL_MAIN_O) $(ALL_BASE_O)

clean:
	RM $(ALL_R) $(ALL_MAIN_O) $(ALL_BASE_O) $(ALL_TEMP)

main.o: main.c
	CPP CPPFLAGS -c main.c
