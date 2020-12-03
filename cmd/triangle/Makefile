CFLAGS = -std=c++20 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTriangle: main.cpp
	g++ $(CFLAGS) -o VulkanTriangle main.cpp $(LDFLAGS)

.PHONY: clean test

test: VulkanTriangle
	./VulkanTriangle

clean:
	rm -f VulkanTriangle
