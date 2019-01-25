#include "shortcutbar.h"
#include "config.h"

#ifdef _WIN32
#include <windows.h>
#include <Lmcons.h>
#define USERNAME() \
	TCHAR username[UNLEN + 1]; \
	DWORD size = UNLEN + 1; \
	GetUserName((TCHAR*)username, &size);
#endif

#ifdef __unix__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#define USERNAME() \
	std::string username; uid_t uid = geteuid(); \
	struct passwd *pw = getpwuid(uid); \
	if (pw) username = pw->pw_name;
#endif

ShortcutBar::ShortcutBar()
{
	/* Query the OS for username */
	USERNAME();

	std::string desktop = ROOT + HOME + SLASH  + username + SLASH + "Desktop";
	std::string documents = ROOT + HOME + SLASH + username + SLASH + "Documents";
	std::string downloads = ROOT + HOME + SLASH + username + SLASH + "Downloads";
	std::string music = ROOT + HOME + SLASH + username + SLASH + "Music";
	std::string pictures = ROOT + HOME + SLASH + username + SLASH + "Pictures";
	std::string videos = ROOT + HOME + SLASH + username + SLASH + "Videos";

	shortcuts.emplace_back(Shortcut("Desktop", desktop));
	shortcuts.emplace_back(Shortcut("Documents", documents));
	shortcuts.emplace_back(Shortcut("Downloads", downloads));
	shortcuts.emplace_back(Shortcut("Music", music));
	shortcuts.emplace_back(Shortcut("Pictures", pictures));
	shortcuts.emplace_back(Shortcut("Videos", videos));
}

std::vector<Shortcut>& ShortcutBar::get_s()
{
	return shortcuts;
}

bool ShortcutBar::checkClicks(int x, int y, std::string& path)
{
	for (Shortcut& n : shortcuts)
	{
		if (n.clicked(x, y))
		{
			path = n.path();
			return true;
		}
	}
	return false;
}
