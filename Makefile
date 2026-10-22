CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
TARGETS  = exercice1_compte/compte exercice2_buffer/buffer exercice3_raii/raii

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	@for t in $(TARGETS); do echo "=== $$t ==="; ./$$t; echo; done

valgrind: all
	@for t in $(TARGETS); do valgrind --leak-check=full ./$$t; done

clean:
	rm -f $(TARGETS)
