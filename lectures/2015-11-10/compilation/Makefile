CXX = c++
CXXFLAGS = -std=c++14 -pedantic -Wall -Wextra -Werror -O2

CLEAN = h f.o g.o h.o libfg.a

h: h.o libfg.a
	$(CXX) $(CXXFLAGS) -o h $< -lfg -L.

libfg.a: f.o g.o
	libtool -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

g.o: g.hpp
f.o: f.hpp g.hpp
h.o: f.hpp

clean:
	$(RM) $(CLEAN)
