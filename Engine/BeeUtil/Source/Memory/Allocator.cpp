#include "BeeUtil.hpp"

#include "Memory/Memory.hpp"

using namespace Bee::Utils::Memory;

Details::AllocatorImpl::AllocatorImpl(b_usize uAmount)
    : m_uCapacity(uAmount)
{
#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY;
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    // BEE_LOG(Debug::Allocators, L"Allocation of %llu bytes on %p", m_uCapacity, this);

    m_pTmp = HeapAlloc(GetProcessHeap(),
                       flags,
                       uAmount);

    if (!m_pTmp)
    {
        throw Debug::BadAlloc(BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    m_pBlock = m_pTmp;
}

Details::AllocatorImpl::~AllocatorImpl()
{
    if (!m_pBlock)
    {
        return;
    }

    // BEE_LOG(Debug::Allocators, L"Free of %llu bytes on %p", m_uCapacity, this);

    if (!HeapFree(GetProcessHeap(),
                  0,
                  m_pBlock))
    {
        DWORD e = GetLastError();
        BEE_LOG(Debug::Error, L"Detected a leak %d", e);
    }

    m_pBlock = nullptr;
}

void Details::AllocatorImpl::Resize(const b_usize& uAmount)
{
    m_uCapacity += uAmount;

#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS;
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    BEE_LOG(Debug::Allocators, L"ReAllocation of %llu bytes on %p", m_uCapacity, this);

    m_pTmp = HeapReAlloc(GetProcessHeap(),
                         flags,
                         m_pBlock,
                         m_uCapacity);

    if (!m_pTmp)
    {
        throw Debug::BadAlloc(BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    m_pBlock = m_pTmp;
}

void Details::AllocatorImpl::SetSize(b_usize uAmount)
{
    if (uAmount < m_uCapacity)
    {
        return;
    }

    m_uCapacity = uAmount;

#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS;
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    // BEE_LOG(Debug::Allocators, L"Setting size on %p to %llu bytes", this, m_uCapacity);
    
    m_pTmp = HeapReAlloc(GetProcessHeap(),
                         flags,
                         m_pBlock,
                         m_uCapacity);

    if (!m_pTmp)
    {
        throw Debug::BadAlloc(BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    m_pBlock = m_pTmp;
}
