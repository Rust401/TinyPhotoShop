all: ds

ds: clean
	clang++ -g -std=c++11 -o backend main.cpp basicDS.cpp utils.cpp BmpReader.cpp
	mv backend bin/
	bin/backend

clean:
	rm -f backend bmp test/* *.bmp