CC=gcc
CXX=g++
RM=rm -f

TARGET=./histograma_mt


INCLUDES=-I./include  -I.
LDFLAGS=
LDLIBS=-lpthread

CXXFLAGS=-std=c++17 -Wall

DIR_OBJ=objs

SRCS=$(wildcard *.cc)

OBJS=$(patsubst %.cc,$(DIR_OBJ)/%.o,$(SRCS))


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) 

$(DIR_OBJ)/%.o: %.cc
	@mkdir -p $(DIR_OBJ)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCLUDES)

clean:
	@$(RM) -r $(DIR_OBJ)
	@$(RM) $(TARGET)
	@$(RM) *~ core


.PHONY: all clean 
	