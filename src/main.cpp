//sudo g++ -DNDEBUG -g2 -O2 -I /usr/local/include/cryptopp/ main.cpp -o test.exe /usr/local/lib/libcryptopp.a

#include "cryptlib.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <iostream>

int main (int argc, char* argv[])
{
    using namespace CryptoPP;
	HexEncoder encoder(new FileSink(std::cout));

	std::string msg = "MARC LE BON, LE BEUAU, LE BEUF, LE BEAUF";
	std::string digest;

	Weak::MD5 hash;
	hash.Update((const byte*)msg.data(), msg.size());
	digest.resize(hash.DigestSize());
	hash.Final((byte*)&digest[0]);

	std::cout << "Message: " << msg << std::endl;

	std::cout << "Digest: ";
	StringSource(digest, true, new Redirector(encoder));
	std::cout << std::endl;

	return 0;
}