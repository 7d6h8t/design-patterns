#include "pch.h"
#include "include/device_manager.h"
#include <string>

std::list<uint32_t> device_manager::getCOMPortList()
{
    // buffer to store the path of the COM PORTS
    DWORD buffer_size = 100;
    std::wstring target_path;
    target_path.resize(buffer_size);

    std::list<uint32_t> port_list;
    //checking ports from COM0 to COM255
    for (int i = 0; i < 255; i++)
    {
        std::wstring str = L"COM" + std::to_wstring(i);
        DWORD res = QueryDosDevice(str.c_str(), const_cast<wchar_t*>(target_path.c_str()), buffer_size);

        //if it didn't find an object
        if (res != 0)
            port_list.push_back(i);
        else if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            buffer_size *= 2;
            target_path.resize(buffer_size);
            --i;
            continue;
        }
    }
    return port_list;
}
