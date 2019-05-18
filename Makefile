PROJECT_DIR := $(shell pwd)
PROM = getevent 
OBJ = main.o

$(PROM): $(OBJ)
	$(CXX) -o $(PROM) $(OBJ) $(CFLAGS) -lpthread

%.o: %.c
	$(CXX) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ) $(PROM)

install:
	sudo install -D -m 755 $(PROM) -t /usr/bin/
