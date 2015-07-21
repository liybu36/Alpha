TARGET = SelfAwayMain
CC = g++
ROOTCINT = $(ROOTSYS)/bin/rootcint
DICTNAME = SelfAwaySelector_dict
SRCS = $(addsuffix .C, $(TARGET))
DIR = .
SOURCES = $(DIR)/SelfAwaySelector.C
HEADERS = $(DIR)/SelfAwaySelector.h
OBJS = $(addsuffix .o, $(notdir $(basename $(SRCS))))
SOBJS = $(addsuffix .o, $(notdir $(basename $(SOURCES))))
ROOTCFLAGS = $(shell $(ROOTSYS)/bin/root-config --cflags) 
ROOTLIBS   = $(shell $(ROOTSYS)/bin/root-config --libs) -lProof -lProofPlayer
ROOTLDFLAGS = $(shell $(ROOTSYS)/bin/root-config --ldflags)

all: $(DICTNAME).C
	$(CC) $(ROOTCFLAGS) $(ROOTLIBS) $(SRCS) $(SOURCES) $^ -o $(TARGET) 

$(DICTNAME).C: $(HEADERS)
	$(ROOTCINT) -f $@ -c $^

.PHONY: clean

clean: 
	rm -rf ./*_dict.h ./*_dict.C