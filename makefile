CC = g++

SERVER_SRC_DIR = server/src
CLIENT_SRC_DIR = client/src
SHARED_SRC_DIR = shared/src

SERVER_INC_DIR = server/inc
CLIENT_INC_DIR = client/inc
SHARED_INC_DIR = shared/inc

SERVER_OBJ_DIR = server/obj
CLIENT_OBJ_DIR = client/obj
SHARED_OBJ_DIR = shared/obj

SERVER_SOURCES = $(wildcard server/src/*.cpp)
CLIENT_SOURCES = $(wildcard client/src/*.cpp)
SHARED_SOURCES = $(wildcard shared/src/*.cpp)

SERVER_HEADERS = $(wildcard server/inc/*.hpp)
CLIENT_HEADERS = $(wildcard client/inc/*.hpp)
SHARED_HEADERS = $(wildcard shared/inc/*.hpp)

SERVER_OBJECTS = $(patsubst server/src/%.cpp, $(SERVER_OBJ_DIR)/%.o, $(SERVER_SOURCES))
CLIENT_OBJECTS = $(patsubst client/src/%.cpp, $(CLIENT_OBJ_DIR)/%.o, $(CLIENT_SOURCES))
SHARED_OBJECTS = $(patsubst shared/src/%.cpp, $(SHARED_OBJ_DIR)/%.o, $(SHARED_SOURCES))

SERVER_TARGET = server.a
CLIENT_TARGET = client.a

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SHARED_OBJECTS) $(SERVER_OBJECTS)
	$(CC) -Wall $(SERVER_OBJECTS) $(SHARED_OBJECTS) -o $(SERVER_TARGET)

$(CLIENT_TARGET): $(SHARED_OBJECTS) $(CLIENT_OBJECTS)
	$(CC) -Wall $(CLIENT_OBJECTS) $(SHARED_OBJECTS) -o $(CLIENT_TARGET)

$(SERVER_OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp
	$(CC) -Wall -c $< -I$(SERVER_INC_DIR) -I$(SHARED_INC_DIR) -o $@

$(CLIENT_OBJ_DIR)/%.o: $(CLIENT_SRC_DIR)/%.cpp
	$(CC) -Wall -c $< -I$(CLIENT_INC_DIR) -I$(SHARED_INC_DIR) -o $@

$(SHARED_OBJ_DIR)/%.o: $(SHARED_SRC_DIR)/%.cpp
	$(CC) -Wall -c $< -I$(SHARED_INC_DIR) -o $@

clean:
	rm -rf $(SERVER_TARGET) $(CLIENT_TARGET) $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SHARED_OBJECTS)

.PHONY: all server client shared clean