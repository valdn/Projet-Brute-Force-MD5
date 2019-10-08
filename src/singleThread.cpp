#include "cryptlib.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <time.h>
#include <iostream>

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

int main() {
	clock_t t1=clock();
	
	std::string msg = "zzzza", msg1 = "a", verif = "z";
	std::string digest, digest1;

	hash(msg, digest);
	hash(msg1, digest1);
	
	while((digest != digest1) && (msg1.size() <= msg.size())) {
		inc(msg1, verif, msg1.size() - 1);
		
		hash(msg1, digest1);
	}

	if(msg1.size() > msg.size())
		std::cerr << "Salut mon pote" << std::endl;
	else {
		clock_t t2= clock();
		float temps = (float)(t2-t1)/CLOCKS_PER_SEC;
		std::cout << temps << " s"<< std::endl;
	}

  	return 0;
}
