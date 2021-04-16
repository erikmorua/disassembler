# makefile for Program 2 Disassembler
# g++ compiler
CXX=g++
# flags
CXXFLAGS=-std=c++11 -g
# target with output of dis executable
dis: dis.cpp
	$(CXX) $(CXXFLAGS) -o dis dis.cpp

# call clean to remove executable
clean:
	rm disassembler