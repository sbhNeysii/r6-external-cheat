#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <WinUser.h>
#include <Psapi.h>
#include <stdint.h>
#include <string>

class Memory
{
public:
    uint64_t base = 0x0;

    inline bool intialize()
    {
        if (GetPid())
        {
            if (GetModuleBaseAddr())
            {
                return true;
            }
        }
        return false;
    }

    inline uint32_t GetPid()
    {
        HWND hWnd = FindWindowA(NULL, "Rainbow Six");
        DWORD PID;
        GetWindowThreadProcessId(hWnd, &PID);
        pid = (uint32_t)PID;
        return pid;
    }

    inline uint64_t GetModuleBaseAddr()
    {
        HMODULE h_modules[1024];
        DWORD pcb_needed;

        handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

        if (K32EnumProcessModules(handle, h_modules, sizeof(h_modules), &pcb_needed))
        {
            for (unsigned int i = 0; i < (pcb_needed / sizeof(HMODULE)); i++)
            {
                TCHAR module_name[MAX_PATH];
                if (GetModuleFileNameEx(handle, h_modules[i], module_name, sizeof(module_name) / sizeof(TCHAR)))
                {
                    std::string str_module_name = module_name;
                    if (str_module_name.find(name) != std::string::npos)
                    {
                        base = (uint64_t)h_modules[i];
                        return base;
                    }
                }
            }
        }

        return 0;
    }

    template<typename T>
    T read(uint64_t address)
    {
        T buffer;
        ReadProcessMemory(handle, (LPCVOID)address, &buffer, sizeof(T), NULL);
        return buffer;
    }

    template<typename T>
    void write(uint64_t address, T buffer)
    {
        WriteProcessMemory(handle, (LPVOID)address, &buffer, sizeof(buffer), NULL);
    }

private:
    uint32_t pid = 0x0;
    HANDLE handle = 0x0;
    std::string name = "RainbowSix.exe";
};

inline Memory memory;