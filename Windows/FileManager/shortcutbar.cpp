#include "shortcutbar.h"
#include "config.h"
#include <windows.h>
#include <Lmcons.h>
ShortcutBar::ShortcutBar() {
	/* Query the OS for username */
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);

	std::string desktop = ROOT + "Users" + SLASH + username + SLASH + "Desktop";
	std::string documents = ROOT + "Users" + SLASH + username + SLASH + "Documents";
	shortcuts.emplace_back(Shortcut("Desktop", desktop, 0, 0, 0 , 0));
	shortcuts.emplace_back(Shortcut("Documents", documents, 0, 0 ,0 ,0));
}

std::vector<Shortcut>& ShortcutBar::get_s() {
	return shortcuts;
}

void ShortcutBar::clean() {
	for (auto n : shortcuts) {
		n.clean();
	}
}