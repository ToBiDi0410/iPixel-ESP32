#include "Helpers.h"

namespace Helpers {

    std::vector<uint8_t> switchEndian(const std::vector<uint8_t>& bytes) {
        std::vector<uint8_t> result(bytes.rbegin(), bytes.rend());
        return result;
    }

    uint16_t reverseBits16(uint16_t n) {
        n = ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
        n = ((n & 0xF0F0) >> 4) | ((n & 0x0F0F) << 4);
        n = ((n & 0xCCCC) >> 2) | ((n & 0x3333) << 2);
        n = ((n & 0xAAAA) >> 1) | ((n & 0x5555) << 1);
        return n;
    }

    std::vector<uint8_t> logicReverseBitsOrder(const std::vector<uint8_t>& data) {
        if (data.size() % 2 != 0)
            throw std::invalid_argument("Input size must be a multiple of 2 bytes.");

        std::vector<uint8_t> result;
        result.reserve(data.size());

        for (size_t i = 0; i < data.size(); i += 2) {
            uint16_t value = ((uint16_t)(data[i]) << 8) | data[i + 1];
            uint16_t reversed_value = reverseBits16(value);
            result.push_back((uint8_t)((reversed_value >> 8) & 0xFF));
            result.push_back((uint8_t)(reversed_value & 0xFF));
        }

        return result;
    }

    std::vector<uint8_t> invertFrames(const std::vector<uint8_t>& data) {
        if (data.size() % 2 != 0)
            throw std::invalid_argument("invertFrames: Input size must be multiple of 2 bytes.");

        std::vector<uint8_t> result;
        result.reserve(data.size());

        // Process in 2-byte frames (like original 4-char hex)
        for (int i = (int)(data.size()) - 2; i >= 0; i -= 2) {
            result.push_back(data[i]);
            result.push_back(data[i + 1]);
        }
        return result;
    }

    std::vector<uint8_t> calculateCRC32Bytes(const std::vector<uint8_t>& data) {
        uint32_t crc = crc32Update(data.data(), data.size(), CRC32_INITIAL);
        crc = crc32Final(crc);
        std::vector<uint8_t> crc_bytes = {
            (uint8_t)((crc >> 24) & 0xFF),
            (uint8_t)((crc >> 16) & 0xFF),
            (uint8_t)((crc >> 8) & 0xFF),
            (uint8_t)(crc & 0xFF)
        };
        return switchEndian(crc_bytes);
    }

}