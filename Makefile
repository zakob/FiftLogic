
EXE = fiftgltest
SOURCE = fiftgltest.cpp fiftgl.cpp
CXXFLAGS = -g -Wall -Wformat

all: $(EXE)
	@echo Build complete.

fiftgl.o: fiftgl.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

fiftgltest.o: fiftgltest.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): fiftgl.o fiftgltest.o
	$(CXX) -o $@ $^ $(CXXFLAGS)
clean:
	rm -f $(EXE) fiftgl.o fiftgltest.o
