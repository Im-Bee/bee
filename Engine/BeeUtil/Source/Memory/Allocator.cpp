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
    : m_Capacity(uAmount)
{
#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY;
    BEE_LOG(Problems::Allocators, L"Allocation of %llu bytes on %p", m_Capacity, this);
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    m_Buffer = HeapAlloc(
        GetProcessHeap(),
        flags,
        uAmount);
}

Details::AllocatorImpl::~AllocatorImpl()
{
    BEE_LOG(Problems::Allocators, L"Free of %llu bytes on %p", m_Capacity, this);

    HeapFree(
        GetProcessHeap(),
        0,
        m_Buffer);
}

void Details::AllocatorImpl::Resize(const uintmem& uAmount)
{
    m_Capacity += uAmount;

#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS;
    BEE_LOG(Problems::Allocators, L"ReAllocation of %llu bytes on %p", m_Capacity, this);
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    m_Buffer = HeapReAlloc(
        GetProcessHeap(),
        flags,
        m_Buffer,
        m_Capacity);
}

void Details::AllocatorImpl::SetSize(uintmem uAmount)
{
    m_Capacity = uAmount;

#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS;
    BEE_LOG(Problems::Allocators, L"Setting size on %p to %llu bytes", this, m_Capacity);
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    m_Buffer = HeapReAlloc(
        GetProcessHeap(),
        flags,
        m_Buffer,
        m_Capacity);
}
