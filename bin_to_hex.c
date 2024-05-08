#include <stdio.h>
#include <stdlib.h>
/* 
*   The stuff with reading from a bin file was chat gpt generated
*   so (thats why its redundant) I didnt want to do it myself
*   but the bin_array.h output was written by me 
*   
*   build: gcc .\bin_to_hex.c -o convert
*   usage: ./convert <bin_file>
*/

int main(int argc, char* argv[]) {
    
    if(argc != 2){
        printf("Usage ./convert <bin_file> \n");
        return 1;
    }
    
    FILE *read_ptr;
    FILE *write_ptr;
    unsigned char *buffer;
    long file_len;
    int i;

    // Open the file in binary mode
    read_ptr = fopen(argv[1], "rb"); 
    if (!read_ptr) {
        perror("Error opening read file");
        return 1;
    }

    write_ptr = fopen("bin_array.h", "w"); 
    if (!write_ptr) {
        perror("Error opening write file");
        return 1;
    }

    // Get the file length
    fseek(read_ptr, 0, SEEK_END);
    file_len = ftell(read_ptr);
    fseek(read_ptr, 0, SEEK_SET);

    // Allocate memory for the buffer
    buffer = (unsigned char *)malloc(file_len);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(read_ptr);
        return 1;
    }

    // Read the file into the buffer
    fread(buffer, file_len, 1, read_ptr);
    fclose(read_ptr);
    fprintf(write_ptr,"#ifndef __BINARY_DATA__\n#define __BINARY_DATA__\n");
    fprintf(write_ptr, "#include <stdint.h>\nconst int binary_length = %d;\nconst uint16_t binary_data[] = {\n",(file_len/2));
    for (i = 0; i < file_len-1; i+=2) {

        if(i != file_len-2)
            fprintf(write_ptr, "0x%02X%02X, ", buffer[i+1],buffer[i]);
        else
            fprintf(write_ptr, "0x%02X%02X ", buffer[i+1],buffer[i]);

        if(i % 4 == 0 && i != 0){
            fprintf(write_ptr, "\n");
        }
    }
    fprintf(write_ptr, "\n};\n#endif /* __BINARY_DATA__ */");
    
    // Free allocated memory
    free(buffer);

    return 0;
}