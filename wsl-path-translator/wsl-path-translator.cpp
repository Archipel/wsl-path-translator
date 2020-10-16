// wsl-path-translator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <filesystem>

using namespace std;

int main(int argc, char *argv[]) {

	string cmd;
	size_t len = 0;
	for(int i = 1; i < argc; i++) {
		len += 1 + strlen(argv[i]) + 5; // space + normal length + lengt of "/mnt"
	}
	cmd.reserve(len);
	
	for(int i = 1; i < argc; i++) {
		filesystem::path argpath(argv[i]);
		if(exists(argpath)) {
			argpath = canonical(argpath);
			if(argpath.has_root_name()) {
				string root_name = argpath.root_name().string();
				if(root_name[0] != '/') {
					string newarg = "'";
					if(root_name[0] == '\\') { //UNC root
						for(const char* c = root_name.c_str(); *c != L'\0'; c++) {
							if(*c == '\\') {
								newarg += '/';
							}
							else {
								newarg += *c;
							}
						}
					}
					else { // e.g. C:
						newarg += "/mnt/";
						for(const char* c = root_name.c_str(); *c != L':' && *c != L'\0'; c++) {
							newarg += tolower(*c);
						}
					}

					string relativepath = argpath.relative_path().string();
					if(relativepath.length()) {
						newarg += '/';
						for(const char* c = relativepath.c_str(); *c != L'\0'; c++) {
							switch(*c) {
							case '\\':
								newarg += '/'; break;
							case '\'':
								newarg += "\\'"; break;
							default:
								newarg += *c;
							}
						}
					}
					newarg += '\'';
					cmd += ' ';
					cmd += newarg;
					continue;
				}
			}
		}
		if(cmd.length() > 0) {
			cmd += ' ';
		}
		cmd += argv[i];
		
	}
	return system(cmd.c_str());
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
