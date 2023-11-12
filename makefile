CC = g++
CFLAGS = -Wall -g

SERVER_SRC_DIR = server/src
SERVER_INC_DIR = server/inc
SERVER_OBJ_DIR = server/obj

CLIENT_SRC_DIR = client/src
CLIENT_INC_DIR = client/inc
CLIENT_OBJ_DIR = client/obj

SHARED_SRC_DIR = shared/src
SHARED_INC_DIR = shared/inc
SHARED_OBJ_DIR = shared/obj

SERVER_SOURCES = $(wildcard server/src/*.cpp)
SERVER_HEADERS = $(wildcard server/inc/*.hpp)
SERVER_OBJECTS = $(patsubst server/src/%.cpp, $(SERVER_OBJ_DIR)/%.o, $(SERVER_SOURCES))

CLIENT_SOURCES = $(wildcard client/src/*.cpp)
CLIENT_HEADERS = $(wildcard client/inc/*.hpp)
CLIENT_OBJECTS = $(patsubst client/src/%.cpp, $(CLIENT_OBJ_DIR)/%.o, $(CLIENT_SOURCES))

SHARED_SOURCES = $(wildcard shared/src/*.cpp)
SHARED_HEADERS = $(wildcard shared/inc/*.hpp)
SHARED_OBJECTS = $(patsubst shared/src/%.cpp, $(SHARED_OBJ_DIR)/%.o, $(SHARED_SOURCES))

SERVER_TARGET = server.a
CLIENT_TARGET = client.a

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SHARED_OBJECTS) $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) $(SHARED_OBJECTS) $(SERVER_OBJECTS) -o $(SERVER_TARGET)

$(CLIENT_TARGET): $(SHARED_OBJECTS) $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) $(SHARED_OBJECTS) $(CLIENT_OBJECTS) -o $(CLIENT_TARGET)

$(SERVER_OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -I$(SERVER_INC_DIR) -I$(SHARED_INC_DIR) -o $@

$(CLIENT_OBJ_DIR)/%.o: $(CLIENT_SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -I$(CLIENT_INC_DIR) -I$(SHARED_INC_DIR) -o $@

$(SHARED_OBJ_DIR)/%.o: $(SHARED_SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -I$(SHARED_INC_DIR) -I$(CLIENT_INC_DIR) -I$(SERVER_INC_DIR) -o $@

clean:
	rm -rf $(SERVER_TARGET) $(CLIENT_TARGET) $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SHARED_OBJECTS)

.PHONY: all server client shared clean