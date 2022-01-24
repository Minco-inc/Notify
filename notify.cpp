#include <stdio.h>
#include <iostream>
#include <fstream>
#include "curl/curl.h"
#include "json/json.h"

using namespace std;

bool check_json(Json::Value);

int main(int argc, char **argv) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        Json::Value root;
        ifstream config("/usr/local/etc/notify/config.json", ifstream::binary);
        config >> root;
        bool json_valid = check_json(root);
        if (!json_valid) exit(1);
        string url = root["webhook_url"].asString();
        string raw_body = "";
        raw_body += root["prefix"].asString() + " ";
        string body = "";
        if (argc == 1) {
                raw_body.append("No Message");
        } else {
            for (int i = 0; i < argc -1; i++) {
                string arg = argv[i +1];
                raw_body += arg += " ";
            }
        }
        char *message = argv[1];
        body = "{\"content\":\"" + raw_body + "\"}";
        struct curl_slist* headers = nullptr;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6L);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            printf("Notification sent.\n");
        } else {
            cout << curl_easy_strerror(res) << endl;
        }   
        curl_slist_free_all(headers);
    }   
    curl_easy_cleanup(curl);
}

bool check_json(Json::Value root) {
    // webhook_url, prefix
    if (!root["webhook_url"]) {
        cerr << "Please add the webhook URL to config." << endl;
        return false;
    }
    if (!root["prefix"]) {
        cerr << "Please add the prefix to config. e.g. <@your_user_id>";
        return false;
    }
    return true;
}
