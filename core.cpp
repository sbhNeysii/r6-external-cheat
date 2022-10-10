#include <Windows.h>
#include <iostream>

#include "memory.h" 
#include "offsets.h"

// If you need any help or have questions feel free to add me on discord: neysii#5229

int main()
{
    SetConsoleTitleA("");
    std::cout << "[+] Clientmode launched." << std::endl;
    std::cout << "[!] Make sure Battleye is turned off!" << std::endl; 
    std::cout << "[/] Searching for RainbowSix.exe" << std::endl;
    while (!FindWindowA("R6Game", "Rainbow Six")) { // Find Rainbow Six Siege Class and Window
        Sleep(1000); // Wait 1 second 
    }
    std::cout << "[+] Found RainbowSix.exe" << std::endl;
    memory.intialize(); // Getting pid and base address of the game
    std::cout << "[>] Baseaddress: " << memory.base << std::endl; // Print the modulebase of RainbowSix.exe
    std::cout << "[>] Gamemanager: " << offsets::gamemanager() << std::endl;
    std::cout << "[>] Entitylist: " << offsets::entitymanager(offsets::gamemanager()).first << std::endl;
    std::cout << "[>] Entitycount: " << offsets::entitymanager(offsets::gamemanager()).second << std::endl;
    std::cout << "[>] Roundstate: " << offsets::roundstate() << std::endl;
    while (true) {
        if (offsets::roundstate() == 2 || offsets::roundstate() == 3) { // check if entity is in game
            for (int i = 1; i < offsets::entitymanager(offsets::gamemanager()).second; i++) { // loop through entitycount
                std::uint64_t controller = memory.read<std::uint64_t>(offsets::entitymanager(offsets::gamemanager()).first + (i * sizeof(std::uint64_t))); // get controller by entitylist
                if (!controller) continue; // check pointer
                std::uint64_t pawn = offsets::player::pawn(controller); // get player pawn through controller
                if (!pawn) continue; // check pointer
                std::uint64_t actor = offsets::player::actor(pawn); // get actor through pawn
                if (!actor) continue; // check pointer
                offsets::player::outlines(actor); // write outlines to player
            }
        }
        Sleep(10);
    }
    Sleep(-1);
}