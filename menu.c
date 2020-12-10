#include <stdio.h>
#include <stdlib.h>
#define path "/dev/stred"

void menu_print(){
	printf("Choose one of the following options:\n");
	printf("1: Read string\n");
	printf("2: Write new string\n");
	printf("3: Concat to string\n");
	printf("4: Delete string\n");
	printf("5: Clear spaces\n");
	printf("6: Remove substring\n");
	printf("7: Remove last n characters\n");
}

FILE *fp;
char *str;
size_t byte_size = 100;

void read_str(){
	fp = fopen(path, "r");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}
	str = (char *)malloc(byte_size + 1);
	getline(&str, &byte_size, fp);
	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
	printf("%s", str);
}

void write_str(){

}

void concat_str(){

}

void delete_str(){

}

void clear_str(){

}

void remove_str(){

}

void truncate_str(){

}

void error_hand(){

}

int main(){
	unsigned char mode;
	while(1){
		menu_print();
		scanf("%hhu", &mode);
		switch(mode) {
			case 1:
				read_str();
				break;
			case 2:
				write_str();
				break;
			case 3:
				concat_str();
				break;
			case 4:
				delete_str();
				break;
			case 5:
				clear_str();
				break;
			case 6:
				remove_str();
				break;
			case 7:
				truncate_str();
				break;
			default:
				error_hand();
				break;
		}
	}
}
