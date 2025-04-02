#include <iostream>
#include <windows.h>
#include <thread>

#include "teenyat.h"

#define SHM_SIZE sizeof(teenyat)
const char SHM_NAME[] = "sharedmemory";
teenyat* data;

void run_process() {
    system("lcd test.bin");
}

void monitor_process(){
    while(1){
        std::cout << "Debugger PC: " << data->reg[TNY_REG_PC].u << std::endl;
        std::cout << "Debugger rA: " << data->reg[TNY_REG_A].s << std::endl;
        std::cout << "Debugger rB: " << data->reg[TNY_REG_B].s << std::endl;
        std::cout << "Debugger rC: " << data->reg[TNY_REG_C].s << std::endl;
        std::cout << "Debugger rD: " << data->reg[TNY_REG_D].s << std::endl;
        std::cout << "Debugger rE: " << data->reg[TNY_REG_E].s << std::endl;
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(void) {
    
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, SHM_SIZE, SHM_NAME);
    if (!hMapFile) {
        std::cerr << "Could not create file mapping." << std::endl;
        return 1;
    }

    data = (teenyat*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE);
    if (!data) {
        std::cerr << "Could not map view of file." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    std::thread process_thread(run_process); 
    std::thread monitor_thread(monitor_process); 
    
    process_thread.join(); 
    monitor_thread.join();

    UnmapViewOfFile(data);
    CloseHandle(hMapFile);
    return 0;
}
