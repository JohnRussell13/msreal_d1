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
int nbytes = 100;
int bytes_read;
char *str = 0; 
char str_w[STORAGE_SIZE];

void read_str(){
	fp = fopen(path, "r");
	
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}

	fscanf(fp, "%[^\n]", str_w);

	printf("Stored string:\n");
	printf("%s\n", str_w);	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void write_str(){
	printf("Insert string to be written:\n");
	str = (char *)malloc(nbytes + 1);
	bytes_read = getline(&str, &nbytes, stdin);
	str[bytes_read - 1] = 0;
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
	free(str);
}

void concat_str(){
	printf("Insert string to be concated:\n");
	str = (char *)malloc(nbytes + 1);
	bytes_read = getline(&str, &nbytes, stdin);
	str[bytes_read - 1] = 0;
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
	free(str);
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
	str = (char *)malloc(nbytes + 1);
	bytes_read = getline(&str, &nbytes, stdin);
	str[bytes_read - 1] = 0;
	
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
	free(str);
}

void truncate_str(){
	char trunc[10];
	printf("Insert number of truncated characters:\n");
	scanf("%s", trunc);
	fp = fopen(path, "w");
	if (fp == NULL){
		printf("Error: Invalid path. File not opened.\n");
		return;
	}
	fprintf(fp, "truncate=%s\n", trunc);
	
	if (fclose(fp)){
		printf("Error: File can't be closed.\n");
		return;
	}
}

void error_hand(){
	printf("Mode not valid.\n");
}

int main(){
	int mode;
	while(1){
		menu_print();
		str = (char *)malloc(nbytes + 1);
		bytes_read = getline(&str, &nbytes, stdin);
		str[bytes_read - 1] = 0;
		
		mode = atoi(str);
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
		free(str);
	}
}
