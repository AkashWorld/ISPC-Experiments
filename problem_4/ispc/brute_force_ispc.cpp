// The main recursive method to print all possible strings of length "length"

export void compare_hashes(uniform const int8 phrases[], uniform const unsigned int64 hashes_low[], uniform const unsigned int64 hashes_up[], uniform const unsigned int64 digest[], uniform unsigned int8 output[], uniform size_t length, uniform int count){
            uniform unsigned int64 dig_up = digest[1];
            uniform unsigned int64 dig_low = digest[0];
            foreach(i = 0 ... count){
              if((hashes_low[i] == dig_low) && (hashes_up[i] == dig_up)){
                memcpy(output,(unsigned int8 *)&phrases[i * length],length);
              }
            }
}