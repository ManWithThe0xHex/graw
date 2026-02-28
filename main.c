#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <assert.h>

#define RAW_URL_BASE "https://raw.githubusercontent.com/"
#define GITHUB_URL   "https://github.com/"

char *str_replace(const char *src, const char *old, const char *new) {
    if (!src || !old || !new) return NULL;

    size_t old_len = strlen(old);
    size_t new_len = strlen(new);

    // nothing
    if (old_len == 0) return strdup(src);

    // Count occurrences of old
    size_t count = 0;
    const char *tmp = src;
    while ((tmp = strstr(tmp, old)) != NULL) {
        count++;
        tmp += old_len;
    }

    if ( count == 0 ) {
        fprintf(stderr, "ERROR: No occurrences was found\n");
        exit(1);
    }

    // Allocate result buffer
    size_t result_len = strlen(src) + count * (new_len - old_len) + 1;
    char *result = malloc(result_len);
    if (!result) return NULL;

    // Build the result string
    char *dest = result;
    tmp = src;
    const char *match;
    while ((match = strstr(tmp, old)) != NULL) {
        size_t prefix_len = match - tmp;
        memcpy(dest, tmp, prefix_len);   // copy part before match
        dest += prefix_len;
        memcpy(dest, new, new_len);      // copy replacement
        dest += new_len;
        tmp = match + old_len;           // skip past old match
    }
    strcpy(dest, tmp); // copy remaining string

    return result;
}

char* get_raw_url(char *url){
    char *raw_url = NULL;
    raw_url = str_replace(url, GITHUB_URL, RAW_URL_BASE);
    raw_url = str_replace(raw_url, "blob/", "");
    return raw_url;
}

void curl_request(char *raw_url){
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, raw_url);

        /* Perform the request, 'res' holds the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

void usage(FILE *file, char *program){
    fprintf(file, "Usage: '%s [url]' to make a curl request.\n", program);
}

int main(int argc, char *argv[]){
    (void) argc;
    assert(*argv != NULL);
    char *program = *argv++;

    if ( *argv == NULL ){
        usage(stderr, program);
        fprintf(stderr, "ERROR: not url is provided\n");
        exit(1);
    }

    char *url = *argv++;
    char *raw_url = NULL;

    raw_url = get_raw_url(url);
    curl_request(raw_url);

    return 0;
}
