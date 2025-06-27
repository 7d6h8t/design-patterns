#include "pch.h"
#include "cmdmanagerImpl.h"
#include "serialImpl.h"

using namespace BMS;

cmdmanagerImpl::cmdmanagerImpl() noexcept
{
}

cmdmanagerImpl::~cmdmanagerImpl() noexcept
{
}

cmdmanagerImpl& cmdmanagerImpl::getInstance()
{
    static serialImpl instance;
    return instance;
}
