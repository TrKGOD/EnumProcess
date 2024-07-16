#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "psapi.lib")

void PrintProcessNameAndID(DWORD processID) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Get the process name.
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Print the process name and identifier.
    std::wcout << szProcessName << " (PID: " << processID << ")" << std::endl;

    // Release the handle to the process.
    CloseHandle(hProcess);
}

int main() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;

    while (true) {
        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
            std::cerr << "Failed to enumerate processes." << std::endl;
            return 1;
        }

        // Calculate how many process identifiers were returned.
        cProcesses = cbNeeded / sizeof(DWORD);

        // Print the name and process identifier for each process.
        for (unsigned int i = 0; i < cProcesses; i++) {
            if (aProcesses[i] != 0) {
                PrintProcessNameAndID(aProcesses[i]);
            }
        }

        // Wait for user input before enumerating again
        std::wcout << "Press Enter to enumerate processes again or type 'exit' to quit: ";
        std::string userInput; // Mudamos para std::string
        std::getline(std::cin, userInput); // Usamos std::cin
        if (userInput == "exit") {
            break;
        }
    }

    return 0;
}
