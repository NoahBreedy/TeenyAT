#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 
*   The stuff with reading from a bin file was chat gpt generated
*   so (thats why its redundant) I didnt want to do it myself
*   but the default.h output was written by me 
*   
*   build: gcc .\bin_to_hex.c -o convert
*   usage: ./convert <bin_file> (optional)<bin_name>
*/

void string_toupper(char s[],char s_up[]){
    unsigned int s_len = strlen(s);
    int a;
    for ( a = 0; a < s_len; a++)
    { 
        s_up[a] = toupper(s[a]); 
    }
    s_up[a] = '\0';
}

int main(int argc, char* argv[]) {
    
    if(argc < 2 || argc > 3){
        printf("Usage ./convert <bin_file> (optional)<bin_name>\n");
        return 1;
    }
    
    FILE *read_ptr;
    FILE *write_ptr;
    unsigned char *buffer;
    long file_len;
    int i;

    read_ptr = fopen(argv[1], "rb"); 
    if (!read_ptr) {
        perror("Error opening read file");
        return 1;
    }

    char* name = "default";
    if(argc == 3){
        name = argv[2];
    }

    char name_upper[strlen(name)];
    string_toupper(name,name_upper);

    char name_buffer[strlen(name)+5];
    sprintf(name_buffer,"%s.h",name);

    write_ptr = fopen(name_buffer, "w"); 
    if (!write_ptr) {
        perror("Error opening write file");
        return 1;
    }

    /* Get the file length */
    fseek(read_ptr, 0, SEEK_END);
    file_len = ftell(read_ptr);
    fseek(read_ptr, 0, SEEK_SET);

    /* Allocate memory for the buffer */ 
    buffer = (unsigned char *)malloc(file_len);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(read_ptr);
        return 1;
    }

    fread(buffer, file_len, 1, read_ptr);
    fclose(read_ptr);
    fprintf(write_ptr,"#ifndef __%s_BINARY_DATA__\n#define __%s_BINARY_DATA__\n",name_upper,name_upper);
    fprintf(write_ptr, "#include <stdint.h>\nconst int %s_binary_length = %d;\nconst uint16_t %s_binary_data[] = {\n",name,(file_len/2),name);
    for (i = 0; i < file_len-1; i+=2) {

        if(i != file_len-2)
            fprintf(write_ptr, "0x%02X%02X, ", buffer[i+1],buffer[i]);
        else
            fprintf(write_ptr, "0x%02X%02X ", buffer[i+1],buffer[i]);

        if(i % 4 == 0 && i != 0){
            fprintf(write_ptr, "\n");
        }
    }
    fprintf(write_ptr, "\n};\n#endif /* __%s_BINARY_DATA__ */",name_upper);
    
    free(buffer);

    return 0;
}