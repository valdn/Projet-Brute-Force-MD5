#include "cryptlib.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <time.h>
#include <iostream>

typedef std::chrono::high_resolution_clock Clock;

void reset (std::string &s) {
	for (int i = 0; i < s.size(); i++)
		s[i] = 'a';
}

void hash (std::string &s, std::string &digest){
	using namespace CryptoPP;
	HexEncoder encoder(new FileSink(std::cout));
	Weak::MD5 hash;
	hash.Update((const byte*)s.data(), s.size());
	digest.resize(hash.DigestSize());
	hash.Final((byte*)&digest[0]);
}

void inc (std::string &s, std::string &verif, int i) {
	if (s == verif) {
		reset(s);
		s += 'a';
		verif += 'z';
	}

	if (s[i] >= 'z') {
		s[i] = 'a';
		inc(s, verif, i-1);
	} else {
		s[i]++;
	}
}

int main(int argc, char *argv[]) {
	
	if(argc != 2) {
		std::cerr << "./singleThread.exe <Mot a chercher>" << std::endl;
		return 0;
	}

	std::string msg = argv[1], msg1 = "a", verif = "z";
	std::string digest, digest1;

	auto t1 = Clock::now();

	hash(msg, digest);
	hash(msg1, digest1);
	
	while((digest != digest1) && (msg1.size() <= msg.size())) {
		inc(msg1, verif, msg1.size() - 1);
		
		hash(msg1, digest1);
	}

	if(msg1.size() > msg.size())
		std::cerr << "ERREUR : erreur impossible" << std::endl;
	else {
		auto t2 = Clock::now();
		float temps = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		temps = temps/1000;
		std::cout << "Nombre Thread(s): 1";
		std::cout << " | Temps: " << temps << " seconds" << std::endl;
	}

  	return 0;
}
