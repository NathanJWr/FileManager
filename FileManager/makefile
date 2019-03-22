All: main.o dirobject.o directory.o \
	filesystem.o filemanager.o \
	LinuxPlatformLayer.o message.o \
	shortcut.o shortcutbar.o display.o
	g++ -o filemanager main.o dirobject.o directory.o \
	filesystem.o filemanager.o \
	LinuxPlatformLayer.o message.o \
	shortcut.o shortcutbar.o display.o \
	-lSDL2 -lSDL2_image -lSDL2_ttf -lstdc++fs

main.o: main.cpp display.h \
       	filesystem.h shortcutbar.h \
       	message.h platformlayer.h 
	g++ -c main.cpp
dirobject.o: dirobject.cpp dirobject.h
	g++ -c dirobject.cpp
directory.o: directory.cpp directory.h
	g++ -c directory.cpp
filesystem.o: filesystem.cpp filesystem.h
	g++ -c filesystem.cpp
filemanager.o: filemanager.cpp filemanager.h config.h
	g++ -c filemanager.cpp
LinuxPlatformLayer.o: LinuxPlatformLayer.cpp platformlayer.h
	g++ -c LinuxPlatformLayer.cpp
message.o: message.cpp message.h
	g++ -c message.cpp
shortcut.o: shortcut.cpp shortcut.h
	g++ -c shortcut.cpp
shortcutbar.o: shortcutbar.cpp shortcutbar.h \
	config.h platformlayer.h
	g++ -c shortcutbar.cpp
display.o: display.cpp display.h config.h
	g++ -c display.cpp
clean:
	rm -rf filemanager *.o
