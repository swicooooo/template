#pragma once

#include <string>

// class Base64
// {
// public:
//     static Base64& instance()
//     {
//         static Base64 b;
//         return b;
//     }
    std::string base64_encode     (std::string const& s, bool url = false);
    std::string base64_encode_pem (std::string const& s);
    std::string base64_encode_mime(std::string const& s);

    std::string base64_decode(std::string const& s, bool remove_linebreaks = false);
    std::string base64_encode(unsigned char const*, size_t len, bool url = false);

// };




