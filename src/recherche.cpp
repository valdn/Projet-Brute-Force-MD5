#include <iostream>

void set (std::string &s, int max) {
	for (int i = 0; i < max; i++)
		s = s + ' ';
}

void inc (std::string &s, int i) {
	if (s[i] >= '~') {
		s[i] = ' ';
		inc(s, i-1);
	} else {
		s[i]++;
	}
}

std::string bruteForce (std::string msg, std::string result) {

	int size = 0, nbM = 0;
	std::string l, Stmp;

	for (int i = 0; i < msg.size(); i++) {
		if (msg[i] == '_')
			size++;
	}

	set(l, size);

	int i = 0;

	while (Stmp != result) {
		i++;
		Stmp = "";
		nbM = 0;
		for (int i = 0; i < msg.size(); i++) {
			if (msg[i] == '_') {
				Stmp += l[nbM];
				nbM++;
			} else {
				Stmp += msg[i];
			}
		}
		std::cout << Stmp << std::endl;
		inc (l, size - 1);
	}

	return Stmp;
}

int main() {

	std::string msg;
	std::string result;

	std::cout << "Mot avec les lettres manquantes : ";
	getline(std::cin, msg);

	std::cout << "Mot voulus : ";
	getline(std::cin, result);


	result = bruteForce(msg, result);
	//std::cout << bruteForce(msg, result) << std::endl;

  	return 0;
}