FUN=client.o func.o
CC=gcc
TAR=CLI

$(TAR):$(FUN)
	$(CC) $(FUN) -o $(TAR) -lpthread

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:
clean:
	rm -rf $(FUNC)
	