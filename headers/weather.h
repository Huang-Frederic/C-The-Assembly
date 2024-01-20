/* holder for curl fetch */
struct curl_fetch_st
{
    char *payload;
    size_t size;
};

struct Weather
{
    char description[100];
    int code; // 0: Bad, 1: Snow, 2: Good
};

CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch);
size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp);

void get_weather_code(struct Weather *weather);
void get_capital_by_country(char *capital, char *country);
int get_weather_desc(char *weather, char *country);
void parse_weather(char *weather, const char *json_string);
void get_weather_code(struct Weather *weather);
void get_weather(struct Weather *weather, char *country);