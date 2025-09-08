#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compress_string_gzip(char *input, char **output, int *out_len) {
    uLong source_len = strlen(input);  // don't include null terminator
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    // Initialize with gzip encoding (15 window bits + 16 = gzip)
    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        fprintf(stderr, "deflateInit2 failed\n");
        *output = NULL;
        *out_len = 0;
        return;
    }

    zs.next_in = (Bytef *)input;
    zs.avail_in = source_len;

    uLong bound = deflateBound(&zs, source_len);
    *output = malloc(bound);
    if (!*output) {
        perror("malloc");
        deflateEnd(&zs);
        *out_len = 0;
        return;
    }

    zs.next_out = (Bytef *)*output;
    zs.avail_out = bound;

    int ret = deflate(&zs, Z_FINISH);
    if (ret != Z_STREAM_END) {
        fprintf(stderr, "deflate failed: %d\n", ret);
        free(*output);
        *output = NULL;
        *out_len = 0;
        deflateEnd(&zs);
        return;
    }

    *out_len = zs.total_out;

    deflateEnd(&zs);

    printf("Compression successful: original size %lu, compressed size %d\n",
           source_len, *out_len);
}




