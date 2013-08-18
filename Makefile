# TO DO: subdirectories and organisation, to prevent messiness

CC = g++

# Compiler options:
# -g      Enable debugging symbols
# -O3     Full compiler optimisations
# -Wall   Enable all warnings
# -I headers 	Look for header files in the 'headers' subdirectory
CFLAGS = -g -O3 -Wall -I headers

# libraries to link
LIBS = -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf

# The name of the program to build, set to the name of the directory we're working in
TARGET = $(shell basename $(CURDIR))

# directory wot cpp files are in
SOURCES := source
# list of the non-directory names of the .cpp files in the source folder
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
# list of .o files to build, made by replacing the .cpp suffix for each name in the CPPFILES list
OFILES := $(CPPFILES:.cpp=.o)
# directory wot .o files are in
OBJDIR := build
#
OBJECTS := $(addprefix $(OBJDIR)/, $(OFILES))

# The default target to make if none is specified.
all: $(TARGET).exe
	@echo All done!

# Link .o files together to produce an .exe file
$(TARGET).exe: $(OFILES)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

%.o: %.cpp
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@

# find .cpp files in the 'source' subdirectory
vpath %.cpp source
vpath %.o build

# Remove all the compiled files & muck.
.PHONY: clean	
clean:
	@echo Clean...
	rm -f *.exe $(OBJECTS) depend.mk

# Automatically extract dependencies from .cpp files.
# Invoking the compiler with -MM below makes it scan the source files and
# write out Makefile-style rules saying which other files they depend upon;
# we then include that file.
-include depend.mk
depend.mk: *.cpp *.h
	$(CC) -MM *.cpp >depend.mk
