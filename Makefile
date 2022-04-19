build:
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c -o scheduler.out
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out

clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out

test:
	gcc ./Test/Dynamic_Array_Test.c ./DataStructures/Dynamic_Array.c -o Dynamic_Array_Test.out
	./Dynamic_Array_Test.out
	gcc ./Test/Priority_Queue_Test.c ./DataStructures/Priority_Queue.c ./DataStructures/Dynamic_Array.c -o Priority_Queue_Test.out
	./Priority_Queue_Test.out