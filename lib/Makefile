C_CRC = $(sort $(wildcard *.c))
OBJ   = $(patsubst %.c,%.o,$(C_CRC))

file = print.o
all:$(OBJ)
	
%.o:%.c
	${GCC} ${CFLAG} -c $<
clean:
	$(shell rm *.o 2>/dev/null)
