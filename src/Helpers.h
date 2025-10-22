#pragma once

#include <Arduino.h>
#include <vector>
#include <stdexcept>
#include <ErriezCRC32.h>

namespace Helpers {

    //Reverse byte order of vector
    std::vector<uint8_t> switchEndian(const std::vector<uint8_t>& bytes);

    //Reverse bits in a 16-bit word
    uint16_t reverseBits16(uint16_t n);

    //Reverse bits in each 16-bit chunk of vector
    std::vector<uint8_t> logicReverseBitsOrder(const std::vector<uint8_t>& data);

    //Reverse order of 2-byte frames
    std::vector<uint8_t> invertFrames(const std::vector<uint8_t>& data);

    //Compute CRC32 as 4-byte vector, reversed
    std::vector<uint8_t> calculateCRC32Bytes(const std::vector<uint8_t>& data);

}