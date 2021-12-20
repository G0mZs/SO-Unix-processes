CC		:= gcc
C_FLAGS := -Wall -Wextra

OUT		:= ../out
SRC		:= src
INCLUDE	:= ../include
LDFLAGS= -pthread

LIBRARIES	:= -pthread

EXECUTABLE	:= arbitro

all: $(OUT)/$(EXECUTABLE)

clean:
	find .. -type p -exec rm -f {} \;
	-$(RM) $(OUT)/$(EXECUTABLE)

run: all
	./$(OUT)/$(EXECUTABLE)

$(OUT)/$(EXECUTABLE): $(SRC)/*.c
	$(CC) $(C_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)
