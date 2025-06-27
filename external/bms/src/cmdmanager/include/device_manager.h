/*****************************************************************//**
 * @file   device_manager.h
 * @brief  getting com port list form device manager.
 * 
 * @author aslkdj1
 * @date   January 2023
 *********************************************************************/

#pragma once
#include "export.h"
#include <list>

/**
 * @brief class for get com port list.
 */
class DLL_EXPORT device_manager
{
private:
    device_manager() noexcept = delete;
    ~device_manager() noexcept = delete;

public:
    /**
     * @brief get com port list from device manager.
     * 
     * @return com port list
     */
    static std::list<uint32_t> getCOMPortList();
}; //class device_manager
