#include "brute_force.h"

unsigned char digest[MD5_DIGEST_LENGTH];
unsigned char digest2[MD5_DIGEST_LENGTH];
bool cracked = 0;

int main(){
    const char md5[]= "b1f4f9a523e36fd969f4573e25af4540"; 
    str_to_md5(md5);
    int lenght = 4;
    char str[94];
    gen_arr(str);
    int n = sizeof str;
    gen_strs(str, "", n, lenght);  //Note: this function works on all cases and not just the case above
    return 0;

}

// The main recursive method to print all possible strings of length "length"
void gen_strs(const char str[],string prefix,const int n, const int length){
    if(cracked){
        return;
    } else{
        MD5((unsigned char*)prefix.c_str(), prefix.length(), (unsigned char*)&digest); 
        if(memcmp(digest,digest2,MD5_DIGEST_LENGTH) == 0){
            cout<< "PASSWORD: " << prefix << endl;
            cracked = 1;
            return;
        }
        if (prefix.length() < length){
            for (int i = 0; i < n; i++){
              gen_strs(str, prefix + str[i], n, length);
            }
        } else{
            return;
        }

    }
}

void gen_arr(char arr[]){
    for(char i = 0;i < 95;i++){
      arr[(int)i] = i + 32;
    }
}

void str_to_md5(const char* str){
    int n = strlen(str);
    char * endptr;
    for(int i = 0; i < n;i= i + 2){
        char lett[3];
        lett[2] = 0;
        strncpy(lett,str + i,2);
        long num = strtol(lett, &endptr, 16);
        digest2[i / 2] = num;
    }
    printf("\n");
}

void md5_to_str(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
    printf("\n");
}
