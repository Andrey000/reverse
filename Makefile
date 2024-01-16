
all: reverse
	
reverse: reverse.o	
	g++ -Wl,-O1 -L/usr/lib/x86_64-linux-gnu/ reverse.o -lPocoFoundation -lPocoNet -o reverse

obj: reverse.cpp
	g++ -c -pipe -O2 -std=gnu++1y -Wall -Wextra -fPIC reverse.cpp -o reverse.o
	

clean:
	rm reverse.o  reverse