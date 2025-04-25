all: part1 part2 part3 part4 part5 iobound cpubound

part1: part1.c
	gcc -g -o part1 part1.c

part2: part2.c
	gcc -g -o part2 part2.c

part3: part3.c
	gcc -g -o part3 part3.c

part4: part4.c
	gcc -g -o part4 part4.c

part5: part5.c
	gcc -g -o part5 part5.c

clean:
	rm -f *.o part1 part2 part3 part4 part5 iobound cpubound

iobound: iobound.c
	gcc iobound.c -o iobound

cpubound: cpubound.c
	gcc cpubound.c -o cpubound
