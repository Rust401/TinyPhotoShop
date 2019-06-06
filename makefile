all: point

point:
	g++ -o point PointTest.cpp
	./point

layer:
	g++ -o layer LayerTest.cpp
	./layer

image:
	g++ -o image ImageTest.cpp
	./image

clean:
	rm -f point