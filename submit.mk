CC=gcc
CFLAGS := -O3 -lrt
DEPS := \
	Box.h \
	DsvUpdater_BoxTemperature.h \
	FormattedReader_Box.h \
	I_DsvUpdater.h \
	I_FormattedReader.h \
	I_List.h \
	I_Map.h \
	List_Fixed.h \
	Macro.h \
	Map_Box.h \

OBJ = \
	DsvUpdater_BoxTemperature.o \
	FormattedReader_Box.o \
	List_Fixed.o \
	main.o \
	Map_Box.o \

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

amr: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
	
.PHONY: clean
clean:
	@echo Cleaning build files...
	@rm -f *.o amr
