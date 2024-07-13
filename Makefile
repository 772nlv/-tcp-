FUN=server.o func.o
CC=gcc
TAR=SER

$(TAR):$(FUN)
	$(CC) $(FUN) -o $(TAR) -lpthread

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:
clean:
	rm -rf $(FUN)
	