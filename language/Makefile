CFLAGS = -g -c -Wshadow -Winit-self -Wredundant-decls -Wundef -Wfloat-equal -Winline \
		 -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default \
 		 -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy \
		 -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
		 -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing \
		 -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

EXE_FLAG_VALGRIND = valgrind -s --leak-check=yes --show-leak-kinds=all --track-origins=yes --log-file="valgrind_log.txt" 

CC = g++
TARGET = Language

PREF_OBJ = ./obj/
PREF_SRC = ./src/
PREF_GRV= ../graphviz/src/

LANG_SRC = $(wildcard $(PREF_SRC)*.cpp)
LANG_OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(LANG_SRC))

GRV_CPP = $(wildcard $(PREF_GRV)*.cpp)
GRV_OBJ = $(patsubst $(PREF_GRV)%.cpp, $(PREF_OBJ)%.o, $(GRV_CPP))

OBJ = $(GRV_OBJ) $(LANG_OBJ)

all : $(TARGET) run

$(TARGET) : $(OBJ) 
	$(CC) $(OBJ) -o $(TARGET).exe

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(CC)  -c $< -o $@

$(PREF_OBJ)%.o : $(PREF_GRV)%.cpp
	$(CC)  -c $< -o $@

clean: 
	rm $(TARGET).exe $(PREF_OBJ)*.o

run:
	$(EXE_FLAG_VALGRIND) ./$(TARGET).exe
