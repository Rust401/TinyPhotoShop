all: t

point: clean
	clang++ -std=c++11 -o point PointTest.cpp
	./point

layer: clean
	clang++ -std=c++11 -o layer LayerTest.cpp
	./layer

image: clean
	clang++ -std=c++11 -o image ImageTest.cpp
	./image

t: clean
	clang++ -std=c++11 -o tt main.cpp basicDS.cpp
	./tt

clean:
	rm -f point layer image tt