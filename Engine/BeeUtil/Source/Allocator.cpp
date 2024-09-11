#include "BeeUtil.hpp"

#include "Allocator.hpp"

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN

#	include <Windows.h>
#endif // _WIN32

Bee::Utils::Memory::Impl::AllocatorImpl::AllocatorImpl(const uintmem& uAmount)
    : m_Capacity(uAmount)
{
#ifdef _DEBUG
    constexpr DWORD flags = HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY;
#else
    constexpr DWORD flags = 0;
#endif // _DEBUG

    m_Buffer = HeapAlloc(
        GetProcessHeap(),
        flags,
        uAmount);
}

Bee::Utils::Memory::Impl::AllocatorImpl::~AllocatorImpl()
{
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
    m_Buffer = HeapReAlloc(
        GetProcessHeap(),
        flags,
        m_Buffer,
        m_Capacity);
}

void Bee::Utils::Memory::Impl::AllocatorImpl::SetSize(const uintmem& uAmount)
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
