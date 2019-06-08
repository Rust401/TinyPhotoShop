all: image

point: clean
	g++ -std=c++11 -o point PointTest.cpp
	./point

layer: clean
	g++ -std=c++11 -o layer LayerTest.cpp
	./layer

image: clean
	g++ -std=c++11 -o image ImageTest.cpp
	./image

clean:
	rm -f point layer image