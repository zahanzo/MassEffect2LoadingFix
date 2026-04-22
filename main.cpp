/* * CREATED BY: 0xRobert, 2021 | REFACTORED: 2026 (github.com/zahanzo)
 * DESCRIPTION: Fixes Mass Effect 2 long loading screens by forcing CPU Affinity.
 * * TECHNICAL BACKGROUND: THE UNREAL ENGINE 3 LOADING STALL
 * * THE BUG: 
 * Mass Effect 2 (built on Unreal Engine 3) features a notorious bug where loading screens 
 * take significantly longer on modern multi-core CPUs. This occurs because the engine's 
 * asset decompression and loading movie synchronization logic were designed for a different 
 * CPU era. When the OS attempts to distribute these tasks across multiple cores, the 
 * synchronization threads often "stall" due to thread-switching overhead.
 * * THE FIX (CPU AFFINITY):
 * By setting the Process Affinity Mask to 2 (binary 0010), we force the entire game 
 * process to run exclusively on "Core 1". Ironically, limiting the game to a single core 
 * during the loading phase eliminates the synchronization bottleneck, reducing loading 
 * times from minutes to mere seconds.
 */

#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <filesystem>

using std::cout;
using std::endl;
using std::string;

/**
 * Reads a string value from the Windows Registry with dynamic buffer sizing.
 */
string ReadRegistryPath(HKEY hKeyParent, const string& subkey, const string& valueName) {
    HKEY hKey;
    if (RegOpenKeyExA(hKeyParent, subkey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return "";
    }

    DWORD bufferSize = 0;
    // Get the required buffer size first
    RegQueryValueExA(hKey, valueName.c_str(), NULL, NULL, NULL, &bufferSize);

    std::vector<char> buffer(bufferSize);
    if (RegQueryValueExA(hKey, valueName.c_str(), NULL, NULL, (LPBYTE)buffer.data(), &bufferSize) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return "";
    }

    RegCloseKey(hKey);
    return string(buffer.data());
}

/**
 * Launches the game process and monitors for the F9 key to restore affinity.
 */
void LaunchAndMonitor(const string& gamePath) {
    system("Color B5"); 
    cout << "[+] Game path identified: " << gamePath << endl;

    std::filesystem::path binPath = std::filesystem::path(gamePath) / "Binaries" / "MassEffect2.exe";
    string exePath = binPath.string();

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    cout << "[+] Launching Mass Effect 2 with restricted affinity (Core 1)..." << endl;

    // Launching the process in SUSPENDED mode to set affinity before the first frame
    if (CreateProcessA(exePath.c_str(), NULL, NULL, NULL, FALSE, 
                       CREATE_SUSPENDED, NULL, 
                       binPath.parent_path().string().c_str(), &si, &pi)) {
        
        // Mask 2 = Core 1 (Binary 0010)
        SetProcessAffinityMask(pi.hProcess, 2); 
        ResumeThread(pi.hThread);

        cout << "\n==================================================" << endl;
        cout << "[!] LOADING BOOST ACTIVE!" << endl;
        cout << "[!] Wait for the loading screen to finish." << endl;
        cout << "[!] THEN, PRESS [F9] TO RESTORE FULL CPU POWER." << endl;
        cout << "==================================================\n" << endl;

        // Watchdog loop for F9 keypress
        while (true) {
            // Check if F9 is pressed (Async check works even with the game in focus)
            if (GetAsyncKeyState(VK_F9) & 0x8000) {
                DWORD_PTR systemAffinity, processAffinity;
                GetProcessAffinityMask(GetCurrentProcess(), &processAffinity, &systemAffinity);
                
                if (SetProcessAffinityMask(pi.hProcess, systemAffinity)) {
                    cout << "[+] SUCCESS: CPU Affinity restored to all cores!" << endl;
                    cout << "[+] You can now close this window and enjoy your game." << endl;
                    std::cin.get();
                    break; 
                }
            }
            Sleep(100); // Low CPU overhead for the launcher
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        cout << "[!] ERROR: Failed to launch the game executable." << endl;
        cout << "[!] Ensure the path in the .ini or Registry is correct." << endl;
        std::cin.get();
    }
}

int main() {
    // 1. First priority: Check for the local .ini file
    string configFileName = "me2lsf.ini";
    std::ifstream ifile(configFileName);

    if (ifile.is_open()) {
        cout << "[+] me2lsf.ini found. Using local path..." << endl;
        string gameLocation;
        std::getline(ifile, gameLocation);
        ifile.close();

        if (!gameLocation.empty()) {
            LaunchAndMonitor(gameLocation);
        } else {
            cout << "[!] Error: .ini file is empty." << endl;
            std::cin.get();
        }
    } 
    // 2. Second priority: Fallback to Windows Registry
    else {
        cout << "[*] me2lsf.ini not found. Checking Windows Registry..." << endl;
        
        string regPath = "Software\\BioWare\\Mass Effect 2";
        string gameLocation = ReadRegistryPath(HKEY_LOCAL_MACHINE, regPath, "Path");

        if (!gameLocation.empty()) {
            LaunchAndMonitor(gameLocation);
        } else {
            cout << "[!] ERROR: Mass Effect 2 path not found in Registry or .ini." << endl;
            cout << "[!] Please create me2lsf.ini with the game path or reinstall the game." << endl;
            std::cin.get();
        }
    }

    return 0;
}