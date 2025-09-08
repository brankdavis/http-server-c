#ifndef COMPRESS_H
#define COMPRESS_H

// #ifdef __cplusplus
// extern "C" {
// #endif

// // Macro for exporting functions (for shared libraries)
// #ifndef COMPRESS_API
// #define COMPRESS_API
// #endif

// // Compress function prototype
// COMPRESS_API int compress(const char* input, char* output, int output_size);

// #ifdef __cplusplus
// }
// #endif
void compress_string_gzip(char *input, char **output, int *out_len);

#endif // COMPRESS_H