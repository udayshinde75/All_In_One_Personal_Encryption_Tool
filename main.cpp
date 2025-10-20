#include <iostream>
#include <filesystem>
#include <string>
#include "Utils/Utils.hpp"

// Entry point for the FileCrypt CLI prototype.
// This version focuses on input handling and path validation.
// Actual encryption/decryption will be implemented in later iterations.

using namespace std;
namespace fs = std::filesystem;

// Prints the main menu to stdout and prompts the user for an action.
void displayMenu() {
    cout << "=============================\n";
    cout << "   FileCrypt - Encryption Tool\n";
    cout << "=============================\n";
    cout << "1. Encrypt File\n";
    cout << "2. Decrypt File\n";
    cout << "3. Encrypt Folder\n";
    cout << "4. Decrypt Folder\n";
    cout << "5. Exit\n";
    cout << "-----------------------------\n";
    cout << "Enter your choice: ";
}

// Main REPL loop handling user choices and basic validation.
int main() {
    int choice;
    string path, password;

    while (true)
    {
        // Show menu and read user choice
        displayMenu();
        cin >> choice;
        cin.ignore();

        if ( choice == 5 )
        {
            cout << "Exiting FileCrypt. Goodbye!\n";
            break;
        }

        // Ask for a file or folder depending on the operation
        if (choice == 1 || choice == 2)
        {
            cout << "Enter the full path of file: ";
        }
        else if (choice == 3 || choice == 4)
        {
            cout << "Enter the full path of folder: ";
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        getline(cin, path);

        // Validate that the given path exists on disk
        if (!Utils::pathExists(path)) {
            cout << "❌ Error: The specified path does not exist.\n\n";
            continue;
        }
        
        // Read password as a plain string (masking only for display)
        cout << "Enter your password: ";
        getline(cin, password);

        // For now, just echo a summary of the provided inputs
        cout << "\n----------------------------------------\n";
        cout << "You selected option: " << choice << endl;
        cout << "Target path: " << path << endl;
        cout << "Password: " << string(password.size(), '*') << endl;
        cout << "----------------------------------------\n\n";

        // Placeholder: future work will call encryption/decryption routines here
        cout << "✅ (Simulation) Operation ready to start...\n\n";
        
    }
    
}