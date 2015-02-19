ifndef NO_OPTIMIZE
	O_SYM = -O3
endif
ifndef NO_POPCNT
	P_SYM = -DPOPCNT
endif
ifdef FAIL_PERC
	F_SYM = -DCOUNT_FAIL_PERCENT
endif

all: main

clean: 
	rm *.o main

main: Main.o ACBuilder.o NodeQueue.o BitArray.o FastLookup.o HashMap.o PatternTable.o StateTable.o LinkedList.o PacketReader.o Scanner.o DumpReader.o StateMachine.o StateMachineGenerator.o TableStateMachine.o TableStateMachineGenerator.o StateGenerator.o StateMachineDumper.o LinearEncodedState.o BitmapEncodedState.o LookupTableState.o PathCompressedState.o SimpleLinearEncodedState.o
	gcc -Wall $(O_SYM) -o main -msse4.2 Main.o ACBuilder.o NodeQueue.o BitArray.o FastLookup.o HashMap.o PatternTable.o StateTable.o LinkedList.o PacketReader.o Scanner.o DumpReader.o StateMachine.o StateMachineGenerator.o TableStateMachine.o TableStateMachineGenerator.o StateGenerator.o StateMachineDumper.o LinearEncodedState.o BitmapEncodedState.o LookupTableState.o PathCompressedState.o SimpleLinearEncodedState.o  -pthread -lm

Main.o:  ../Main.c ACBuilder.o NodeQueue.o BitArray.o FastLookup.o HashMap.o PatternTable.o StateTable.o LinkedList.o PacketReader.o Scanner.o DumpReader.o StateMachine.o StateMachineGenerator.o TableStateMachine.o TableStateMachineGenerator.o StateGenerator.o StateMachineDumper.o LinearEncodedState.o BitmapEncodedState.o LookupTableState.o PathCompressedState.o SimpleLinearEncodedState.o
	gcc -Wall $(O_SYM) -c ../Main.c -I../

ACBuilder.o: ../AhoCorasick/ACBuilder.c ../AhoCorasick/ACBuilder.h
	gcc -Wall $(O_SYM) -c ../AhoCorasick/ACBuilder.c -I../

NodeQueue.o: ../AhoCorasick/NodeQueue.c ../AhoCorasick/NodeQueue.h
	gcc -Wall $(O_SYM) -c ../AhoCorasick/NodeQueue.c -I../

BitArray.o: ../Common/BitArray/BitArray.c ../Common/BitArray/BitArray.h
	gcc -Wall $(O_SYM) -c ../Common/BitArray/BitArray.c -I../

FastLookup.o: ../Common/FastLookup/FastLookup.c ../Common/FastLookup/FastLookup.h
	gcc -Wall $(O_SYM) -c ../Common/FastLookup/FastLookup.c -I../

HashMap.o: ../Common/HashMap/HashMap.c ../Common/HashMap/HashMap.h
	gcc -Wall $(O_SYM) -c ../Common/HashMap/HashMap.c -I../

PatternTable.o: ../Common/PatternTable.c ../Common/PatternTable.h
	gcc -Wall $(O_SYM) -c ../Common/PatternTable.c -I../

StateTable.o: ../Common/StateTable.c ../Common/StateTable.h
	gcc -Wall $(O_SYM) -c ../Common/StateTable.c -I../

LinkedList.o: ../DumpReader/BoundedBuffer/LinkedList.c ../DumpReader/BoundedBuffer/LinkedList.h
	gcc -Wall $(O_SYM) -c ../DumpReader/BoundedBuffer/LinkedList.c -I../

PacketReader.o: ../DumpReader/BoundedBuffer/PacketReader.c ../DumpReader/BoundedBuffer/PacketReader.h
	gcc -Wall $(O_SYM) -c ../DumpReader/BoundedBuffer/PacketReader.c -I../

Scanner.o: ../DumpReader/BoundedBuffer/Scanner.c ../DumpReader/BoundedBuffer/Scanner.h
	gcc -Wall $(O_SYM) $(F_SYM) -c ../DumpReader/BoundedBuffer/Scanner.c -I../

DumpReader.o: ../DumpReader/DumpReader.c ../DumpReader/DumpReader.h
	gcc -Wall $(O_SYM) $(F_SYM) -c ../DumpReader/DumpReader.c -I../

StateMachine.o: ../StateMachine/StateMachine.c ../StateMachine/StateMachine.h LinearEncodedState.o BitmapEncodedState.o LookupTableState.o PathCompressedState.o SimpleLinearEncodedState.o
	gcc -Wall $(O_SYM) $(F_SYM) -c ../StateMachine/StateMachine.c -I../

StateMachineGenerator.o: ../StateMachine/StateMachineGenerator.c ../StateMachine/StateMachineGenerator.h
	gcc -Wall -c ../StateMachine/StateMachineGenerator.c -I../

TableStateMachine.o: ../StateMachine/TableStateMachine.c ../StateMachine/TableStateMachine.h LinearEncodedState.o BitmapEncodedState.o LookupTableState.o PathCompressedState.o SimpleLinearEncodedState.o
	gcc -Wall $(O_SYM) $(F_SYM) -c ../StateMachine/TableStateMachine.c -I../

TableStateMachineGenerator.o: ../StateMachine/TableStateMachineGenerator.c ../StateMachine/TableStateMachineGenerator.h
	gcc -Wall $(O_SYM) -c ../StateMachine/TableStateMachineGenerator.c -I../

StateGenerator.o: ../StateMachine/StateGenerator.c ../StateMachine/StateGenerator.h LinearEncodedState.o BitmapEncodedState.o LookupTableState.o PathCompressedState.o SimpleLinearEncodedState.o
	gcc -Wall -c ../StateMachine/StateGenerator.c -I../

StateMachineDumper.o: ../StateMachine/StateMachineDumper.c ../StateMachine/StateMachineDumper.h
	gcc -Wall $(O_SYM) -c ../StateMachine/StateMachineDumper.c -I../

LinearEncodedState.o: ../States/LinearEncodedState.c ../States/LinearEncodedState.h
	gcc -Wall $(O_SYM) -c ../States/LinearEncodedState.c -I../

SimpleLinearEncodedState.o: ../States/SimpleLinearEncodedState.c ../States/SimpleLinearEncodedState.h
	gcc -Wall $(O_SYM) -c ../States/SimpleLinearEncodedState.c -I../

BitmapEncodedState.o: ../States/BitmapEncodedState.c ../States/BitmapEncodedState.h
	gcc -Wall $(O_SYM) $(P_SYM) -c ../States/BitmapEncodedState.c -I../ -msse4.2

LookupTableState.o: ../States/LookupTableState.c ../States/LookupTableState.h
	gcc -Wall $(O_SYM) -c ../States/LookupTableState.c -I../

PathCompressedState.o: ../States/PathCompressedState.c ../States/PathCompressedState.h
	gcc -Wall $(O_SYM) -c ../States/PathCompressedState.c -I../

