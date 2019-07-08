all: ds

ds: clean
	clang++ -std=c++11 -o backend main.cpp basicDS.cpp utils.cpp BmpReader.cpp
	./backend

png: clean
	clang++ -std=c++11 -o  bmp utils.cpp BmpReader.cpp main.cpp
	./bmp

clean:
	rm -f backend bmp