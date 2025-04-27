// Passgen, a simple password generator with optional UI
// General structure of the UI is as follows:
// ------------------------------------------
// |       	   Password Generator      		|
// |	                                    |
// |  [Length:  [ 12 ]]  (input or slider)  |
// |                                        |
// |  [ ] Include Uppercase Letters (A-Z)	|
// |  [ ] Include Lowercase Letters (a-z)	|
// |  [ ] Include Numbers (0-9)          	|
// |  [ ] Include Symbols (!@#$...)      	|
// |                                        |
// |  [ Generate Password ]  (button)       |
// |                                        |
// |  [ Your Password: [ ************ ] ]   |
// |                                        |
// |  [ Copy to Clipboard ] (button)        |
// ------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

const char* UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
const char* NUMBERS   = "0123456789";
const char* SYMBOLS   =    "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"; // Full symbol set
const char* SAFE_SYMBOLS = "!\"#$%&'()*+,-./:;<=>?@^_~"; // Excludes \ ` { } [ ] |

char pass_str[255] = "";

void shuffle_string(char* str, size_t length)
{
    
	char aux;
	size_t random;
	
	for (size_t i = 0; i < length; ++i) {
		random = rand() % length;
        
		aux = str[i];
		str[i] = str[random];
		str[random] = aux;
	}
}

char* generate_password(size_t length, bool has_upper, bool has_lower, bool has_num, bool has_sym, bool safe_sym)
{	
	size_t random;
	char* password = (char*)malloc(++length); // char is already one byte so we don't have to worry about sizeof

	size_t idx = 0;

	// this guarantees the password has at least one of the options selected
	if (has_upper) {
		strcat(pass_str, UPPERCASE);
		password[idx++] = UPPERCASE[rand()%26];
	} 

	if (has_lower) {
		strcat(pass_str, LOWERCASE);
		password[idx++] = LOWERCASE[rand()%26];
	}

	if (has_num) {
		strcat(pass_str, NUMBERS);
		password[idx++] = NUMBERS[rand()%10];
	}

	if (has_sym) {
		if (safe_sym) {
			strcat(pass_str, SAFE_SYMBOLS);
			password[idx++] = SAFE_SYMBOLS[rand()%25];

		} else {
			strcat(pass_str, SYMBOLS);
			password[idx++] = SYMBOLS[rand()%32];
		}
	}

	uint8_t len = strlen(pass_str);

	shuffle_string(pass_str, len);
	
 	for (size_t i = idx; i < length; ++i) {
		random = rand() % len;
		
		password[i] = pass_str[random];
 	}

	return password;
}

void flush_input_buffer() {
    // Flush the input buffer by reading and discarding characters until '\n'
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main(int argc, char const *argv[])
{
	if (argc > 1) {
		if (!strcmp(argv[1], "--noui") || !strcmp(argv[1], "-n")) {
			bool has_upper, has_lower, has_num, has_sym, safe_sym;
			char a;
			size_t length;

			printf("What's the length of the password? ");
			scanf("%zu", &length);
			getchar();

			printf("Would you like to include uppercase letters? [Y/n] ");
			a = getchar();
			if (a != '\n') getchar();
			has_upper = (a == 'y' || a == '\n') ? true : false;
			
			printf("Would you like to include lowercase letters? [Y/n] ");
			a = getchar();
			if (a != '\n') getchar();
			has_lower = (a == 'y' || a == '\n') ? true : false;
			
			printf("Would you like to include numbers? [Y/n] ");
			a = getchar();
			if (a != '\n') getchar();
			has_num = (a == 'y' || a == '\n') ? true : false;
			
			printf("Would you like to include symbols? [Y/n] ");
			a = getchar();
			if (a != '\n') getchar();
			has_sym = (a == 'y' || a == '\n') ? true : false;

			if (has_sym) {
				printf("Would you like to include ONLY safe symbols (excludes \\ ` { } [ ] |)? [Y/n] ");
				a = getchar();
				if (a != '\n') getchar();
				safe_sym = (a == 'y' || a == '\n') ? true : false;
			} else {
				safe_sym = false;
			}

			srand(time(NULL));

			char* password = generate_password(length, has_upper, has_lower, has_num, has_sym, safe_sym);

			printf("\n%s\n", password);
		
		} else {
			printf("Correct usage: \'./passgen\' or \'./passgen --noui\' or \'./passgen -n\'.\n");
		}
	
	} else {
		printf("Not yet implemented.\n");
	}


	return 0;
}
