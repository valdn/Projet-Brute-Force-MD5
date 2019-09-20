//sudo g++ -DNDEBUG -g2 -O2 -I /usr/local/include/cryptopp/ main.cpp -o test.exe /usr/local/lib/libcryptopp.a

#include "cryptlib.h"
#include "main.hpp"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <iostream>

int main (int argc, char* argv[])
{
	std::string msg;
	std::string digest;
	crypto(msg, digest);
	return 0;
}

void crypto(std::string msg, std::string digest){
    using namespace CryptoPP;
	HexEncoder encoder(new FileSink(std::cout));

	std::cout << "Entrez votre message" << std::endl;
	getline(std::cin,msg);

	Weak::MD5 hash;
	hash.Update((const byte*)msg.data(), msg.size());
	digest.resize(hash.DigestSize());
	hash.Final((byte*)&digest[0]);

	std::cout << "Message: " << msg << std::endl;

	std::cout << "Digest: ";
	StringSource(digest, true, new Redirector(encoder));
	std::cout << std::endl;
}
