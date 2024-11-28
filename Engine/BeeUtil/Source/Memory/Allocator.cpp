#include "BeeUtil.hpp"

#include "Memory/Allocator.hpp"

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN

#	include <Windows.h>
#endif // _WIN32

using namespace Bee::Utils::Memory;

Details::AllocatorImpl::AllocatorImpl(uintmem uAmount)
    : m_uCapacity(uAmount)
{
#ifdef _DEBUG
    constexpr DWORD flags = HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY;
#else
    constexpr DWORD flags = HEAP_NO_SERIALIZE;
#endif // _DEBUG

    // BEE_LOG(Problems::Allocators, L"Allocation of %llu bytes on %p", m_uCapacity, this);

    m_pTmp = HeapAlloc(
        GetProcessHeap(),
        flags,
        uAmount);

    if (!m_pTmp)
        throw Problems::BadAlloc(BEE_COLLECT_DATA());

    m_pBlock = m_pTmp;
}

Details::AllocatorImpl::~AllocatorImpl()
{
    if (!m_pBlock)
        return;

    // BEE_LOG(Problems::Allocators, L"Free of %llu bytes on %p", m_uCapacity, this);

    if (!HeapFree(
        GetProcessHeap(),
        0,
        m_pBlock))
    {
        DWORD e = GetLastError();
        BEE_LOG(Problems::Error, L"Detected a leak %d", e);
    }

    m_pBlock = nullptr;
}

void Details::AllocatorImpl::Resize(const uintmem& uAmount)
{
    m_uCapacity += uAmount;

#ifdef _DEBUG
    constexpr DWORD flags = HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS;
#else
    constexpr DWORD flags = HEAP_NO_SERIALIZE;
#endif // _DEBUG

    // BEE_LOG(Problems::Allocators, L"ReAllocation of %llu bytes on %p", m_uCapacity, this);

    m_pTmp = HeapReAlloc(
        GetProcessHeap(),
        flags,
        m_pBlock,
        m_uCapacity);

    if (!m_pTmp)
        throw Problems::BadAlloc(BEE_COLLECT_DATA());

    m_pBlock = m_pTmp;
}

void Details::AllocatorImpl::SetSize(uintmem uAmount)
{
    m_uCapacity = uAmount;

#ifdef _DEBUG
    constexpr DWORD flags = HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS;
#else
    constexpr DWORD flags = HEAP_NO_SERIALIZE;
#endif // _DEBUG

    // BEE_LOG(Problems::Allocators, L"Setting size on %p to %llu bytes", this, m_uCapacity);
    
    m_pTmp = HeapReAlloc(
        GetProcessHeap(),
        flags,
        m_pBlock,
        m_uCapacity);

    if (!m_pTmp)
        throw Problems::BadAlloc(BEE_COLLECT_DATA());

    m_pBlock = m_pTmp;
}
