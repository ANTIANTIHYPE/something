#pragma once

#include <cstdint>

namespace nstd {
    class endianness {
    public:
        uint16_t htons(uint16_t value) const {
            uint16_t result;
            uint8_t* p = reinterpret_cast<uint8_t*>(&value);
            result = (p[0] << 8) | p[1];
            return result;
        }

        uint32_t htonl(uint32_t value) const {
            uint32_t result;
            uint8_t* p = reinterpret_cast<uint8_t*>(&value);
            result = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
            return result;
        }

        uint64_t htonll(uint64_t value) const {
            uint64_t result;
            uint8_t* p = reinterpret_cast<uint8_t*>(&value);
            result = (static_cast<uint64_t>(p[0]) << 56) | (static_cast<uint64_t>(p[1]) << 48) |
                (static_cast<uint64_t>(p[2]) << 40) | (static_cast<uint64_t>(p[3]) << 32) |
                (static_cast<uint64_t>(p[4]) << 24) | (static_cast<uint64_t>(p[5]) << 16) |
                (static_cast<uint64_t>(p[6]) << 8) | p[7];
            return result;
        }

        uint16_t ntohs(uint16_t value) const {
            return htons(value);
        }

        uint32_t ntohl(uint32_t value) const {
            return htonl(value);
        }

        uint64_t ntohll(uint64_t value) const {
            return htonll(value);
        }
    }; // how did i even miss this
}