#include "cryptlib.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <time.h>
#include <iostream>

typedef std::chrono::high_resolution_clock Clock;

bool trouve = false;

void reset (std::string &s, char l) {

	s[0] = l;

	for (int i = 1; i < s.size(); i++)
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

void inc (std::string &s, std::string &verif, int i, char l) {
	
	if (i < 0 || i > s.size()) {
		std::cerr << "ERREUR : indice impossible" << std::endl;
		return;
	}

	if (s == verif) {
		reset(s, l);
		s += 'a';
		verif += 'z';
	} else {
		if (i == 0) {
			s[i]++;
		}	
	
		if (s[i] >= 'z') {
			s[i] = 'a';
			inc(s, verif, i-1, l);
		} else {
			s[i]++;
		}
	}
}

void * dechiffre (void * arg) {

	std::string * param = (std::string*)arg;

	std::string result = param[0];
	std::string digestResult;

	std::string composition = param[1];
	std::string digestComposition;

	std::string endWord = param[2];

	hash(result, digestResult);
	hash(composition, digestComposition);

	while((digestComposition != digestResult) && (!trouve)) {
		inc(composition, endWord, composition.size() - 1, param[1][0]);
		
		hash(composition, digestComposition);
	}

	if (digestComposition == digestResult) {
		trouve = true;
	}

	return(NULL);
}

int main() {
	auto t1 = Clock::now();

	std::string result = "zzzza";
	int nbThread = 2;

	std::string ** tab = new std::string * [2];

	for (int i = 0; i < 2; i++)
		tab[i] = new std::string[3];

	tab[0][0] = result;
	tab[0][1] = "a";
	tab[0][2] = "y";

	tab[1][0] = result;
	tab[1][1] = "b";
	tab[1][2] = "z";

	pthread_t th1;
	pthread_t th2;

	pthread_create(&th1, NULL, dechiffre, (void *)tab[0]);
	pthread_create(&th2, NULL, dechiffre, (void *)tab[1]);

	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	auto t2 = Clock::now();
	float temps = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	temps = temps / 1000;
	std::cerr << "Temps: " << temps << " seconds" << std::endl;

  	return 0;
}
