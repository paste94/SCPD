output: ms_fun.o merge_sort_seq.o merge_sort_mio.o
	g++ ms_fun.o merge_sort_seq.o -o main_seq.o
	g++ ms_fun.o merge_sort_mio.o -o main_mio.o

ms_fun.o: ms_fun.cpp
	g++ -Wall -c ms_fun.cpp

merge_sort_seq.o: merge_sort_seq.cpp
	g++ -Wall -c merge_sort_seq.cpp

merge_sort_mio.o: merge_sort_mio.cpp
	g++ -Wall -c merge_sort_mio.cpp

clean:
	rm -f merge_sort_mio.o merge_sort_seq.o ms_fun.o