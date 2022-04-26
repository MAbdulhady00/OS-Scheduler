build:
	gcc process_generator.c ./DataStructures/LinkedQueue.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c IO.c process_data.c ./SchedulingAlgorithms/HPF.c ./SchedulingAlgorithms/SRTN.c ./SchedulingAlgorithms/RR.c ./DataStructures/Circular_Queue.c ./DataStructures/Priority_Queue.c ./DataStructures/Dynamic_Array.c -o scheduler.out
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out

clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out

test:
	#gcc ./Test/Dynamic_Array_Test.c ./DataStructures/Dynamic_Array.c -o Dynamic_Array_Test.out
	#./Dynamic_Array_Test.out
	#gcc ./Test/Priority_Queue_Test.c ./DataStructures/Priority_Queue.c ./DataStructures/Dynamic_Array.c -o Priority_Queue_Test.out
	#./Priority_Queue_Test.out
	#gcc -g ./Test/HPF_test.c ./SchedulingAlgorithms/HPF.c ./DataStructures/Priority_Queue.c ./DataStructures/Dynamic_Array.c -o HPF_test.out
	#./HPF_test.out 1
	#gcc ./Test/Circular_Queue_Test.c ./DataStructures/Circular_Queue.c -o Circular_Queue_Test.out
	#./Circular_Queue_Test.out
	#gcc -g ./Test/SRTN_test.c ./SchedulingAlgorithms/SRTN.c ./DataStructures/Priority_Queue.c ./DataStructures/Dynamic_Array.c -o SRTN_test.out
	#./SRTN_test.out 1
	gcc -g ./Test/SRTN_test_clk.c ./SchedulingAlgorithms/SRTN.c ./DataStructures/Priority_Queue.c ./DataStructures/Dynamic_Array.c -o SRTN_test_clk.out
	gcc process.c -o process.out
	
	./SRTN_test_clk.out 1
testIO:
	gcc ./Test/IO_Test.c ./DataStructures/LinkedQueue.c -o IO_Test.out
	./IO_Test.out

