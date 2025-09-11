all: build
	

build:
	g++ src/main.cpp -o clist -l sqlite3
	echo "Build complited"
