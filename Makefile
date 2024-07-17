GCC=gcc
FLAGS=-Wall

OBJECTS=program.o parson.o buffer.o connection.o request.o helper.o

build: program

program: $(OBJECTS)
	$(GCC) -o $@ $^ -lm

program.o: program.c
	$(GCC) $(FLAGS) -c $^

parson.o: ./parson/parson.c ./parson/parson.h
	$(GCC) $(FLAGS) -c $<

buffer.o: ./buffer/buffer.c ./buffer/buffer.h
	$(GCC) $(FLAGS) -c $<

connection.o: ./connection/connection.c ./connection/connection.h
	$(GCC) $(FLAGS) -c $<

request.o: ./request/request.c ./request/request.h
	$(GCC) $(FLAGS) -c $<

helper.o: ./helper/helper.c ./helper/helper.h
	$(GCC) $(FLAGS) -c $<

clean:
	rm -rf program $(OBJECTS)
