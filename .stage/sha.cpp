#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

std::string computeSHA256(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    // Initialize SHA-256 context
    SHA256_CTX sha256Context;
    SHA256_Init(&sha256Context);

    char buffer[BUFSIZ];
    while (file.read(buffer, sizeof(buffer))) {
        SHA256_Update(&sha256Context, buffer, file.gcount());
    }

    // Finalize the hash and get the digest
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_Final(digest, &sha256Context);

    // Convert the digest to a hexadecimal string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(digest[i]);
    }

    return ss.str();
}



