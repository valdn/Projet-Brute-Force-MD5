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

	while((param[3] != param[1]) && (!trouve)) {
		//std::cerr << param[2] << std::endl;
		inc(param[2], param[4], param[2].size() - 1, param[5][0]);
		
		hash(param[2], param[3]);
	}

	if (param[3] == param[1])
		trouve = true;

	return(NULL);
}

int main() {
	auto t1 = Clock::now();

	std::string * param = new std::string[6];
	std::string * param2 = new std::string[6];

	param[0] = "zzzza";		// Mot voulant etre obtenu
	param[1]; 				// Digest du mot voulante etre obtenu
	param[2] = "a"; 		// Lettre depart thread1
	param[3]; 				// Digest mot depart thread1
	param[4] = "y";
	param[5] = 'a';

	param2[0] = "zzzza";	// Mot voulant etre obtenu
	param2[1];
	param2[2] = "b";
	param2[3];
	param2[4] = "z"; 		// Verification mot de fin thread1
	param2[5] = 'b';

	//std::cerr << param[0] << std::endl;

	hash(param[0], param[1]);
	hash(param[2], param[3]);

	hash(param2[0], param2[1]);
	hash(param2[2], param2[3]);

	pthread_t th1;
	pthread_t th2;

	pthread_create(&th1, NULL, dechiffre, (void *)param);
	pthread_create(&th2, NULL, dechiffre, (void *)param2);

	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	//std::cerr << param[0] << std::endl;


	if((param[2].size() && param[4].size()) > param[0].size())
		std::cerr << "Salut mon pote" << std::endl;
	else {
		auto t2 = Clock::now();
		float temps = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		temps = temps/1000;
		std::cout << "Temps: " << temps << " seconds" << std::endl;
	}
	
  	return 0;
}
