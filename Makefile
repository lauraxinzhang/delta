#
# Makefile for delta triangulation

# ----- Make Macros -----

CXX             = g++
CXXFLAGS        = -g -pedantic -w -Wall -Wextra -std=c++11 -O3

# OBJDIR = bin/
SRCDIR  = src/

TARGETS = basic
OBJECTS = basic.o Containers.o Mesh.o
DEPS    = $(SRCDIR)Containers.hpp $(SRCDIR)Mesh.hpp $(SRCDIR)delaunator.hpp

# ----- Make rules -----

all:	$(TARGETS)

clean:
	rm -rf $(TARGETS) $(OBJECTS)

basic:	$(OBJECTS) 
	$(CXX) $(CXXFLAGS) -o basic $(OBJECTS)

# basic.o: $(SRCDIR)basic.cpp $(SRCDIR)delaunator.hpp $(SRCDIR)Containers.h $(SRCDIR)Mesh.h
# 	$(CXX) -c $(CXXFLAGS) $(SRCDIR)basic.cpp

$(OBJECTS): %.o: $(SRCDIR)%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)