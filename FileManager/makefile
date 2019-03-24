All: main.o dirobject.o directory.o \
	filesystem.o filemanager.o \
	LinuxPlatformLayer.o message.o \
	shortcut.o shortcutbar.o display.o
	g++ -o filemanager main.o dirobject.o directory.o \
	filesystem.o filemanager.o \
	LinuxPlatformLayer.o message.o \
	shortcut.o shortcutbar.o display.o \
	-lSDL2 -lSDL2_image -lSDL2_ttf -lstdc++fs -g

main.o: main.cpp display.h \
       	filesystem.h shortcutbar.h \
       	message.h platformlayer.h 
	g++ -c main.cpp -g
dirobject.o: dirobject.cpp dirobject.h
	g++ -c dirobject.cpp -g
directory.o: directory.cpp directory.h
	g++ -c directory.cpp -g
filesystem.o: filesystem.cpp filesystem.h
	g++ -c filesystem.cpp -g
filemanager.o: filemanager.cpp filemanager.h config.h
	g++ -c filemanager.cpp -g
LinuxPlatformLayer.o: LinuxPlatformLayer.cpp platformlayer.h
	g++ -c LinuxPlatformLayer.cpp -g
message.o: message.cpp message.h
	g++ -c message.cpp -g
shortcut.o: shortcut.cpp shortcut.h
	g++ -c shortcut.cpp -g
shortcutbar.o: shortcutbar.cpp shortcutbar.h \
	config.h platformlayer.h
	g++ -c shortcutbar.cpp -g
display.o: display.cpp display.h config.h
	g++ -c display.cpp -g
clean:
	rm -rf filemanager *.o
