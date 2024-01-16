#g++ -c -pipe -O2 -std=gnu++1y -Wall -Wextra -fPIC  -I. -I../../../../Qt5.14.2/5.14.2/gcc_64/mkspecs/linux-g++ -o main.o main.cpp
#g++ -Wl,-O1 -o Revers main.o   -L/usr/lib/x86_64-linux-gnu/ -lPocoFoundation -lPocoNet


all: reverse
	
reverse: reverse.o	
	g++ -Wl,-O1 -L/usr/lib/x86_64-linux-gnu/ reverse.o -lPocoFoundation -lPocoNet -o reverse

obj: reverse.cpp
	g++ -c -pipe -O2 -std=gnu++1y -Wall -Wextra -fPIC reverse.cpp -o reverse.o
	

clean:
	rm reverse.o  reverse