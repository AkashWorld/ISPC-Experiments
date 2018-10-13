#include "brute_force.h"

unsigned char digest[MD5_DIGEST_LENGTH];
// bool cracked = 0;

int main(int argc, char * argv[]){
    if(argc < 2){
        cout << "Correct usage: brute_forcer <md5 hash>\n";
        exit(1);
    } else if(strlen(argv[1]) != 32){
        cout << "MD5 not correct length\n";
        exit(1);
    }
    // const char md5[]= "0fdc3cbc9a749efcaf083440a794fae4"; 
    char * md5 = argv[1];
    int length = 4;
    unsigned char  * digest2 = (unsigned char * )calloc(1,MD5_DIGEST_LENGTH);
    // gen_strs(str, "", n, length);  //Note: this function works on all cases and not just the case above
    uint8_t * output = new uint8_t[length]();
    int count = pow(62,length);
    char * phrases;
    unsigned char * hashes_low;
    unsigned char * hashes_up;
    // for(int i = 0; i < count;i++){
    //     phrases[i] = (char*)calloc(length, sizeof(char));
    //     hashes[i] = (unsigned char*)calloc(16, sizeof(char));
    // }
    
    str_to_md5(md5, digest2);
    md5_to_str(digest2);
    clock_t start, end;
    long elapsed = 0;
    for(int i = 1; i <= length;i++){
        count = pow(62,i);
        phrases = (char*) calloc(count * length, sizeof(char ));
        hashes_low = (unsigned char*)calloc(count, MD5_DIGEST_LENGTH / 2);
        hashes_up = (unsigned char*)calloc(count, MD5_DIGEST_LENGTH / 2);
        char filename[10];
        printf("[AVX]Starting passwords of size %d\n",i);
        sprintf(filename, "table%d.txt",i);
        load_hashes(phrases,hashes_low,hashes_up,length,filename);
        start = clock();
        ispc::compare_hashes((const int8_t *)phrases,(const uint64_t *)hashes_low,(const uint64_t *)hashes_up,(const uint64_t *)digest2,output,length, count);
        end = clock();
        free(phrases);
        free(hashes_low);
        free(hashes_up);
        elapsed += end - start;
        if(strlen((char*)output) > 0){
            printf("[AVX]Password: %s\n",output);
            break;
        }
    }

    printf("[AVX]MD5 hash comparison of %d "
           "entries took %ld cycles\n", count, end - start);
    memset(output,0,length);
    elapsed = 0;
    for(int i = 1; i <= length;i++){
        count = pow(62,i);
        phrases = (char*) calloc(count * length, sizeof(char ));
        hashes_low = (unsigned char*)calloc(count, MD5_DIGEST_LENGTH / 2);
        hashes_up = (unsigned char*)calloc(count, MD5_DIGEST_LENGTH / 2);
        char filename[10];
        printf("[SERIAL]Starting passwords of size %d\n",i);
        sprintf(filename, "table%d.txt",i);
        load_hashes(phrases,hashes_low,hashes_up,length,filename);
        start = clock();
        compare_hashes_serial((const int8_t *)phrases,(const uint64_t *)hashes_low,(const uint64_t *)hashes_up,(const uint64_t *)digest2,output,length, count);
        end = clock();
        free(phrases);
        free(hashes_low);
        free(hashes_up);
        elapsed += end - start;
        if(strlen((char*)output) > 0){
            printf("[SERIAL]Password: %s\n",output);
            break;
        }
    }

    printf("[SERIAL]MD5 hash comparison of %d "
           "entries took %ld cycles\n", count, end - start);
    return 0;

}

void compare_hashes_serial(const int8_t * phrases,const uint64_t * hashes_low,const uint64_t * hashes_up, const uint64_t * digest,uint8_t * output,size_t length, int count){
            uint64_t dig_up = digest[1];
            uint64_t dig_low = digest[0];
            for(int i = 0; i < count;i++){
              if(((uint64_t)hashes_low[i] == dig_low) && ((uint64_t)hashes_up[i] == dig_up)){
                memcpy(output,(uint8_t *)&phrases[i * length],length);
              }
            }
}

int load_hashes(char * phrases, unsigned char * hashes_low, unsigned char * hashes_up, const size_t phrase_size, const char* filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        fprintf(stderr, "File %s could not be opened!\n", filename);
        fclose(file);
        return 0;
    }
    char * phrase_line = (char*)calloc(phrase_size, sizeof(char));
    unsigned char * md5_hash = (unsigned char*)calloc(16, sizeof(char));
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int flipper = 1;
    int counter = 0;
    unsigned char  * digest2 = (unsigned char * )calloc(1,MD5_DIGEST_LENGTH);
while ((read = getline(&line, &len, file)) != -1) {
        if(flipper){
            memcpy(&phrases[counter * phrase_size], line, phrase_size);
            flipper = 1 - flipper;
        } else{
            str_to_md5(line, digest2);
            memcpy(&hashes_low[counter * MD5_DIGEST_LENGTH / 2], digest2, 8);
            memcpy(&hashes_up[counter * MD5_DIGEST_LENGTH / 2], &digest2[8], 8);
            flipper = 1 - flipper;
            counter++;
        }
    }
    cout << "Done loading hashes... " << counter << endl;
    fclose(file);
    free(phrase_line);
    free(md5_hash);
    return 1;
}


void gen_arr(char arr[]){
    for(char i = 0;i < 95;i++){
      arr[(int)i] = i + 32;
    }
}

void str_to_md5(const char* str, unsigned char  * digest2){
    int n = strlen(str);
    char * endptr;
    for(int i = 0; i < n;i= i + 2){
        char lett[3];
        lett[2] = 0;
        strncpy(lett,str + i,2);
        long num = strtol(lett, &endptr, 16);
        digest2[i / 2] = num;
    }
    // printf("\n");
}

void md5_to_str(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
    printf("\n");
}
