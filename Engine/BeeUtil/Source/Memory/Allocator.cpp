#include "BeeUtil.hpp"

#include "Memory/Allocator.hpp"

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN

#	include <Windows.h>
#endif // _WIN32

Bee::Utils::Memory::Impl::AllocatorImpl::AllocatorImpl(uintmem uAmount)
    : m_Capacity(uAmount)
{
#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY;
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG
    B_LOG(Problems::Allocators, L"Allocation on %p of %llu bytes", this, m_Capacity);

    m_Buffer = HeapAlloc(
        GetProcessHeap(),
        flags,
        uAmount);
}

Bee::Utils::Memory::Impl::AllocatorImpl::~AllocatorImpl()
{
    B_LOG(Problems::Allocators, L"Free on %p of %llu bytes", this, m_Capacity);

    HeapFree(
        GetProcessHeap(),
        0,
        m_Buffer);
}

void Bee::Utils::Memory::Impl::AllocatorImpl::Resize(const uintmem& uAmount)
{
#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS;
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    m_Capacity += uAmount;
    B_LOG(Problems::Allocators, L"ReAllocation on %p of %llu bytes", this, m_Capacity);

    m_Buffer = HeapReAlloc(
        GetProcessHeap(),
        flags,
        m_Buffer,
        m_Capacity);
}

void Bee::Utils::Memory::Impl::AllocatorImpl::SetSize(uintmem uAmount)
{
#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS;
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    m_Capacity = uAmount;
    m_Buffer = HeapReAlloc(
        GetProcessHeap(),
        flags,
        m_Buffer,
        m_Capacity);
}
