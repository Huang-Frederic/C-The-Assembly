// Using https://www.weatherapi.com/ API

#include "weather.h"

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;                             /* calculate buffer size */
    struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

    /* expand buffer */
    p->payload = (char *) realloc(p->payload, p->size + realsize + 1);

    /* check buffer */
    if (p->payload == NULL) {
      /* this isn't good */
      fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
      /* free buffer */
      free(p->payload);
      /* return */
      return -1;
    }

    /* copy contents to buffer */
    memcpy(&(p->payload[p->size]), contents, realsize);

    /* set new buffer size */
    p->size += realsize;

    /* ensure null termination */
    p->payload[p->size] = 0;

    /* return size */
    return realsize;
}

/* fetch and return url body via curl */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
    CURLcode rcode;                   /* curl result code */

    /* init payload */
    fetch->payload = (char *) calloc(1, sizeof(fetch->payload));

    /* check payload */
    if (fetch->payload == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        /* return error */
        return CURLE_FAILED_INIT;
    }

    /* init size */
    fetch->size = 0;

    /* set url to fetch */
    curl_easy_setopt(ch, CURLOPT_URL, url);

    /* set calback function */
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);

    /* pass fetch struct pointer */
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);

    /* set default user agent */
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* set timeout */
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);

    /* enable location redirects */
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);

    /* set maximum allowed redirects */
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    /* fetch the url */
    rcode = curl_easy_perform(ch);

    /* return */
    return rcode;
}

void get_capital_by_country (char *capital, char *country) {
    if (strcmp(country, "France") == 0)
        strcat(capital, "Paris");

    else if (strcmp(country, "USA") == 0)
        strcat(capital, "Washington");
    
    else if (strcmp(country, "China")  == 0)
        strcat(capital, "Pekin");
    
    else if (strcmp(country, "Korea")  == 0)
        strcat(capital, "Seoul");
    
    else
        strcat(capital, "Tokyo");
}   


int get_weather_desc(char *weather, char *country) {
    CURL *ch;                                               /* curl handle */
    CURLcode rcode;                                         /* curl result code */

    json_object *json;                                      /* json post body */
    enum json_tokener_error jerr = json_tokener_success;    /* json parse error */

    struct curl_fetch_st curl_fetch;                        /* curl fetch struct */
    struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */
    struct curl_slist *headers = NULL;                      /* http headers to send with request */

    /* url to test site */
    char url[200] = "http://api.weatherapi.com/v1/current.json?key=f576b532ad1e4a86a54173925241901&q=\0";
    char capital[20] = {"\0"};
    get_capital_by_country(capital, country);

    strcat(url, capital);

    /* init curl handle */
    if ((ch = curl_easy_init()) == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
        /* return error */
        return 1;
    }

    /* set content type */
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    /* create json object for post */
    json = json_object_new_object();

    /* set curl options */
    curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);


    /* fetch page and capture return code */
    rcode = curl_fetch_url(ch, url, cf);

    /* cleanup curl handle */
    curl_easy_cleanup(ch);

    /* free headers */
    curl_slist_free_all(headers);

    /* free json object */
    json_object_put(json);

    /* check return code */
    if (rcode != CURLE_OK || cf->size < 1) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s",
            url, curl_easy_strerror(rcode));
        /* return error */
        return 2;
    }

    /* check payload */
    if (cf->payload != NULL) {
        /* print result */
        // printf("CURL Returned: \n%s\n", cf->payload);
        /* parse return */
        json = json_tokener_parse_verbose(cf->payload, &jerr);
        /* free payload */
        free(cf->payload);
    } else {
        /* error */
        fprintf(stderr, "ERROR: Failed to populate payload");
        /* free payload */
        free(cf->payload);
        /* return */
        return 3;
    }

    /* check error */
    if (jerr != json_tokener_success) {
        /* error */
        fprintf(stderr, "ERROR: Failed to parse json string");
        /* free json object */
        json_object_put(json);
        /* return */
        return 4;
    }

    printf("JSON: %s\n", json_object_to_json_string(json));

    parse_weather(weather, json_object_to_json_string(json));
    
    // printf("Weather : %s", weather);
    /* exit */
    return 0;
}

void parse_weather(char *weather, const char *json_string) {
    char *res;
    int weather_len;
    char *end_index; 

    res = strstr(json_string, "\"text\":");
    end_index = strchr(res + 9, '"'); 
    weather_len = end_index - (res + 9);
    strncpy(weather, res + 9, weather_len);
    weather[weather_len] = '\0';
}

void get_weather_code(struct Weather *weather) {
    if (strstr(weather->description, "Sunny") != NULL || strstr(weather->description, "Clear") != NULL)
        weather->code = 2;
    else if (strstr(weather->description, "Snow") != NULL)
        weather->code = 1;
    else
        weather->code = 0;
}

void get_weather(struct Weather *weather, char *country) {
    get_weather_desc(weather->description, country);
    get_weather_code(weather);
}



int main(int argc, char argv[]) {
    struct Weather weather;
    get_weather(&weather, "Japan");
    printf("Weather: %s | Code: %d", weather.description, weather.code);
}