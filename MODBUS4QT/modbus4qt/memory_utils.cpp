#include "memory_utils.h"

//-----------------------------------------------------------------------------

uint8_t
hi(uint16_t word)
{
    uint8_t* bytes = (uint8_t*)&word;

    uint16_t test = 1; // * 0x0001 *
    if (*((uint8_t*)&test) == 0)
    {
        return bytes[0];	// big endian (network/motorolla)
    }
    else
    {
        return bytes[1];	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

uint16_t
host2net(uint16_t word)
{
    uint16_t test = 1; // * 0x0001 *
    if (*((uint8_t*)&test) == 0)
    {
        return word;		// big endian (network/motorolla)
    }
    else
    {
        return swap(word);	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

uint8_t
lo(uint16_t word)
{
    uint8_t* bytes = (uint8_t*)&word;

    uint16_t test = 1; // * 0x0001 *
    if (*((uint8_t*)&test) == 0)
    {
        return bytes[1];	// big endian (network/motorolla)
    }
    else
    {
        return bytes[0];	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

uint16_t
net2host(uint16_t word)
{
    uint16_t test = 1; // * 0x0001 *
    if (*((uint8_t*)&test) == 0)
    {
        return word;		// big endian (network/motorolla)
    }
    else
    {
        return swap(word);	// little endian (intel)
    }
}

//-----------------------------------------------------------------------------

uint16_t
swap(uint16_t word)
{
    uint16_t result = lo(word);
    result = result << 8;
    result += hi(word);

    return result;
}

//-----------------------------------------------------------------------------
// EOF
