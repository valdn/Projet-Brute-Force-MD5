#include <iostream>

void set (std::string &s, int max) {
	for (int i = 0; i < max; i++)
		s = s + 'a';
}

void inc (std::string &s, int i) {
	if (s[i] >= 'z') {
		s[i] = 'a';
		inc(s, i-1);
	} else {
		s[i]++;
	}
}

int main() {

	int size = 0;
	int nbT = 0;

	std::string l;
	std::string msg = "______";
	std::string Stmp;

	for (int i = 0; i < msg.size(); i++) {
		if (msg[i] == '_')
			size++;
	}

	set(l, size);

	std::cout << l << std::endl;

	while (Stmp != "poison") {
		Stmp = "";
		nbT = 0;
		for (int i = 0; i < msg.size(); i++) {
			if (msg[i] == '_') {
				Stmp += l[nbT];
				nbT++;
			} else {
				Stmp += msg[i];
			}
		}
		std::cout << Stmp << std::endl;
		inc (l, size - 1);
	}


  	return 0;
}