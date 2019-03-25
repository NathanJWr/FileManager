All: main.o dirobject.o directory.o \
	filesystem.o filemanager.o \
	LinuxPlatformLayer.o message.o \
	shortcut.o shortcutbar.o display.o \
    input.o
	g++ -o filemanager main.o dirobject.o directory.o \
	filesystem.o filemanager.o \
	LinuxPlatformLayer.o message.o \
	shortcut.o shortcutbar.o display.o input.o \
	-lSDL2 -lSDL2_image -lSDL2_ttf -lstdc++fs -g

main.o: main.cpp
	g++ -c main.cpp -g
dirobject.o: dirobject.cpp
	g++ -c dirobject.cpp -g
directory.o: directory.cpp
	g++ -c directory.cpp -g
filesystem.o: filesystem.cpp
	g++ -c filesystem.cpp -g
filemanager.o: filemanager.cpp
	g++ -c filemanager.cpp -g
LinuxPlatformLayer.o: LinuxPlatformLayer.cpp
	g++ -c LinuxPlatformLayer.cpp -g
message.o: message.cpp
	g++ -c message.cpp -g
shortcut.o: shortcut.cpp
	g++ -c shortcut.cpp -g
shortcutbar.o: shortcutbar.cpp
	g++ -c shortcutbar.cpp -g
display.o: display.cpp
	g++ -c display.cpp -g
input.o: input.cpp
	g++ -c input.cpp -g
clean:
	rm -rf filemanager *.o
