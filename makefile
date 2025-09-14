all: build
	
build:
	g++ src/main.cpp -o clist -l sqlite3
	sudo ln -s ~/clist/clist /usr/local/bin/clist
	echo "Build complited"

clean:
	rm ./clist
	rm db.sqlite3
	sudo rm /usr/local/bin/clist
	echo "Cleaning complete"

rebuild: clean build
	echo "Rebuild complited"
