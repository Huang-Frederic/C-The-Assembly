// Using https://www.weatherapi.com/ API
#include "../headers/include.h"
#include "../headers/weather.h"


size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;                         
    struct curl_fetch_st *p = (struct curl_fetch_st *)userp;

    p->payload = (char *)realloc(p->payload, p->size + realsize + 1);

    if (p->payload == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
        free(p->payload);
        return -1;
    }

    memcpy(&(p->payload[p->size]), contents, realsize);
    p->size += realsize;
    p->payload[p->size] = 0;
    return realsize;
}


CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch)
{
    CURLcode rcode; 
    fetch->payload = (char *)calloc(1, sizeof(fetch->payload));

    if (fetch->payload == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        return CURLE_FAILED_INIT;
    }

    fetch->size = 0;
    curl_easy_setopt(ch, CURLOPT_URL, url);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)fetch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    rcode = curl_easy_perform(ch);

    return rcode;
}

void get_capital_by_country(char *capital, char *country)
{
    if (strcmp(country, "France") == 0)
        strcat(capital, "Paris");

    else if (strcmp(country, "USA") == 0)
        strcat(capital, "Washington");

    else if (strcmp(country, "China") == 0)
        strcat(capital, "Pekin");

    else if (strcmp(country, "Korea") == 0)
        strcat(capital, "Seoul");

    else
        strcat(capital, "Tokyo");
}

int get_weather_desc(char *weather, char *country)
{
    CURL *ch;       
    CURLcode rcode; 

    json_object *json;                                   
    enum json_tokener_error jerr = json_tokener_success; 

    struct curl_fetch_st curl_fetch;        
    struct curl_fetch_st *cf = &curl_fetch; 
    struct curl_slist *headers = NULL;      

    char url[200] = "http://api.weatherapi.com/v1/current.json?key=f576b532ad1e4a86a54173925241901&q=\0";
    char capital[20] = {"\0"};

    get_capital_by_country(capital, country);
    strcat(url, capital);

    if ((ch = curl_easy_init()) == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
        return 1;
    }

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    json = json_object_new_object();

    curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);

    rcode = curl_fetch_url(ch, url, cf);

    curl_easy_cleanup(ch);

    curl_slist_free_all(headers);

    json_object_put(json);

    if (rcode != CURLE_OK || cf->size < 1)
    {
        if (display_errors_on)
            fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s", url, curl_easy_strerror(rcode));
        return 2;
    }


    if (cf->payload != NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "\nCURL Returned: \n%s\n", cf->payload);
        json = json_tokener_parse_verbose(cf->payload, &jerr);
        free(cf->payload);
    }
    else
    {
        if (display_errors_on)
            fprintf(stderr, "ERROR: Failed to populate payload");
        free(cf->payload);
        return 3;
    }

    if (jerr != json_tokener_success)
    {
        if (display_errors_on)
            fprintf(stderr, "ERROR: Failed to parse json string");
        json_object_put(json);
        return 4;
    }

    parse_weather(weather, json_object_to_json_string(json));
    return 0;
}

void parse_weather(char *weather, const char *json_string)
{
    char *res;
    int weather_len;
    char *end_index;

    res = strstr(json_string, "\"text\":");
    end_index = strchr(res + 9, '"');
    weather_len = end_index - (res + 9);
    strncpy(weather, res + 9, weather_len);
    weather[weather_len] = '\0';
}

void get_weather_code(struct Weather *weather)
{
    if (strstr(weather->description, "Sunny") != NULL || strstr(weather->description, "Clear") != NULL)
        weather->code = 2;
    else if (strstr(weather->description, "Snow") != NULL)
        weather->code = 1;
    else
        weather->code = 0;
}

void get_weather(struct Weather *weather, char *country)
{
    get_weather_desc(weather->description, country);
    get_weather_code(weather);
}
