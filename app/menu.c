#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define path "/dev/stred"
#define STORAGE_SIZE 100 

void menu_print(){
	printf("Choose one of the following options:\n");
	printf("1: Read string\n");
	printf("2: Write new string\n");
	printf("3: Concat to string\n");
	printf("4: Delete string\n");
	printf("5: Clear spaces\n");
	printf("6: Remove substring\n");
	printf("7: Remove last n characters\n");
	printf("Note: When inputing strings, use \"quotes\"!\n");
}

FILE *fp;
char str[STORAGE_SIZE];

void read_str(){
	fp = fopen(path, "r");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}

	fscanf(fp, "%[^\n]", str);

	printf("Stored string:\n");
	printf("%s\n", str);	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void write_str(){
	printf("Insert string to be written:\n");
	scanf("\n");
	scanf("\"%[^\"]\"", str);
	fp = fopen(path, "w");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}
	fprintf(fp, "string=%s\n", str);//too many hours spent on \n..
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void concat_str(){
	printf("Insert string to be concated:\n");
	scanf("\n");
	scanf("\"%[^\"]\"", str);
	fp = fopen(path, "w");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}
	fprintf(fp, "append=%s\n", str);
	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void delete_str(){
	fp = fopen(path, "w");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}
	fprintf(fp, "clear\n");
	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void clear_str(){
	fp = fopen(path, "w");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}
	fprintf(fp, "shrink\n");
	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void remove_str(){
	printf("Insert substring to be removed:\n");
	scanf("\n");
	scanf("\"%[^\"]\"", str);
	fp = fopen(path, "w");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}	
	fprintf(fp, "remove=%s\n", str);

	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void truncate_str(){
	int trunc;
	printf("Insert number of truncated characters:\n");
	scanf("%d", &trunc);
	fp = fopen(path, "w");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}
	fprintf(fp, "truncate=%d\n", trunc);
	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void error_hand(){
	printf("Mode not valid.\n");
}

int main(){
	unsigned char mode;
	while(1){
		menu_print();
		scanf("%hhu", &mode);
		if (!(mode > 0 && mode <= 7)){
			error_hand();
		}
		else{
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
}
