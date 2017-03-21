# I am a comment, and I want to say that the variable CXX will be
# the compiler to use.
CXX=g++
# Hey!, I am comment number 2. I want to say that CXXFLAGS will be the
# options I'll pass to the compiler.
CXX_FLAGS=-Wall -std=c++0x


EXEC_SRC = test_entity.cc test_boss.cc test_battle.cc
EXEC = test_entity test_boss test_battle

SRCS := $(wildcard *.cc)
SRCS := $(filter-out $(EXEC_SRC), $(SRCS))
OBJECTS = $(SRCS:.cc=.o)


all: $(EXEC)

test_entity: $(OBJECTS) test_entity.cc
	$(CXX) $(CXX_FLAGS) $(OBJECTS) test_entity.cc -o test_entity

test_boss: $(OBJECTS) test_boss.cc
	$(CXX) $(CXX_FLAGS) $(OBJECTS) test_boss.cc -o test_boss

test_battle: $(OBJECTS) test_battle.cc
	$(CXX) $(CXX_FLAGS) $(OBJECTS) test_battle.cc -o test_battle

# To obtain object files
%.o: %.cc $(SRCS) $(wildcard *.h)
	$(CXX) $(CXX_FLAGS) -c  $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
