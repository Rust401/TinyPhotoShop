all: image

point: clean
	g++ -o point PointTest.cpp
	./point

layer: clean
	g++ -o layer LayerTest.cpp
	./layer

image: clean
	g++ -o image ImageTest.cpp
	./image

clean:
	rm -f point layer image