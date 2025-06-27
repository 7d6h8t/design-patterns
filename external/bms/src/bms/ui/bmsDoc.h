#pragma once

class bmsDoc final : public CDocument
{
    DECLARE_DYNCREATE(bmsDoc)

public:
    bmsDoc() noexcept;
    virtual ~bmsDoc() noexcept;
};
