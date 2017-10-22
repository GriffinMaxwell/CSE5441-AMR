CC=gcc
LINKFLAGS := -lrt -pthread
CFLAGS := -O3
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
	Map_Double.h \

OBJ = \
	DsvUpdater_BoxTemperature.o \
	FormattedReader_Box.o \
	List_Fixed.o \
	Map_Box.o \
	Map_Double.o \
	maxwell_griffin_disposable.o \

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

amr: $(OBJ) 
	$(CC) -c -o maxwell_griffin_disposable.o maxwell_griffin_disposable.c $(CFLAGS)
	$(CC) -o disposable $^ $(CFLAGS) $(LINKFLAGS)
	
.PHONY: clean
clean:
	@echo Cleaning build files...
	@rm -f *.o disposable
