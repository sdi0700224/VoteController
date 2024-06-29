#paths
MODULES = 
INCLUDE = 

# compiler
CC = g++

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CPPFLAGS = -Wall -Werror -g

# Αρχεία .o
OBJS = MVote.o CsvReader.o LinearHashMapOF.o VoteController.o Voter.o VotersCatalogue.o

# Το εκτελέσιμο πρόγραμμα
EXEC = MVote

# Παράμετροι για δοκιμαστική εκτέλεση
ARGS =  -f voters5000.csv -b 2

$(EXEC): $(OBJS) 
	$(CC) $(OBJS) -o $(EXEC) $(CPPFLAGS)

run: $(EXEC)
	./$(EXEC) $(ARGS)

clean:
	rm -f $(OBJS) $(EXEC)

debug: $(EXEC)
	valgrind ./$(EXEC) $(ARGS)