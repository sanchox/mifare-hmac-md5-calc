#include <stdio.h>

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

#include "hash-library/hmac.h"
#include "hash-library/md5.h"

std::string bytes_to_hex_string(unsigned const char* data, int size)
{
	std::stringstream ss;
	ss << std::hex;
	for (int i = 0; i < size; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];
	std::string s = ss.str();
	std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) -> unsigned char { return std::toupper(c); });

	return s;
}

std::vector<uint8_t> hex_string_to_vector_bytes(const std::string &hex)
{
	std::vector<uint8_t> bytes;

	for (unsigned int i = 0; i < hex.length(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		uint8_t byte = (uint8_t)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}

	return bytes;
}

int main(int argc, char* argv[])
{
        if (argc != 3) {
		printf("Use with 2 args {UID} and {HMAC-MD5-KEY}\n");
		printf("Example:\n");
		printf("	mifare-hmac-md5-calc 04451B22906580 test-key\n");
		exit(1);
	}
	auto uid_string = std::string(argv[1]);
	auto uid_length = uid_string.length() / 2;
	auto hmac_key = std::string(argv[2]);
	printf("UID %s\n", uid_string.c_str());
	printf("HMAC-MD5 key: %s\n", hmac_key.c_str());

	for (int sector = 0; sector < 16; ++sector) {
		for (int block = 1; block < 3; ++block) {
			uint8_t block_data[16];
			uint8_t tmp = (sector + block);

			std::string block_string = bytes_to_hex_string(block_data, sizeof(block_data));

			std::string hmac_msg_string = uid_string + bytes_to_hex_string(&tmp, 1);
			while (hmac_msg_string.length() < 16)
				hmac_msg_string = "0" + hmac_msg_string;

			std::string ref_block_string = hmac<MD5>(hmac_msg_string, hmac_key);
			std::transform(ref_block_string.begin(), ref_block_string.end(), ref_block_string.begin(),
				[](unsigned char c) -> unsigned char { return std::toupper(c); });

			printf("Sector %d | Block %d\t\tHMAC MSG: %s\t DATA: %s\n",
				sector,
				block,
				hmac_msg_string.c_str(), ref_block_string.c_str());
		}
	}
}
