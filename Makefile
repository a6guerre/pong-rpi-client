# Compiler and linker settings
CXX = g++
CC = gcc
CXXFLAGS = -Wall -I./libenjoy/src
CFLAGS = -Wall -I./libenjoy/src

# Directories
SRC_DIR = src
LIB_SRC_DIR = libenjoy/src

# Source files
CPP_SRCS = $(SRC_DIR)/pong-rpi-client.cpp
C_SRCS = $(LIB_SRC_DIR)/libenjoy.c $(LIB_SRC_DIR)/libenjoy_linux.c 
SRCS = $(CPP_SRCS) $(C_SRCS)

# Object files
CPP_OBJS = $(CPP_SRCS:.cpp=.o)
C_OBJS = $(C_SRCS:.c=.o)
OBJS = $(CPP_OBJS) $(C_OBJS)

# Dependency files
DEPS = $(OBJS:.o=.d)

# Output executable
TARGET = pong-rpi-client

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(TARGET) $(DEPS)

.PHONY: clean

