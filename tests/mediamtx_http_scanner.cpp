#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <future>
#include <regex>
#include <curl/curl.h>

using namespace std;

const int HTTP_PORT = 8888;  // Default MediaMTX HTTP streaming port

// Function to execute shell commands and return output
string executeCommand(const string& command) {
    string result;
    char buffer[128];
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";
    
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    
    pclose(pipe);
    return result;
}

// Function to check and install missing dependencies
void installDependencies() {
    cout << "Checking dependencies...\n";

    // List of required packages
    // Note: The standard headers (<iostream>, <vector>, etc.) come with g++ and libstdc++-dev,
    // so checking for g++ is generally enough.
    vector<string> packages = {"g++", "libstdc++-dev", "libcurl4-openssl-dev"};
    vector<string> missing_packages;

    for (const string& pkg : packages) {
        string checkCmd = "dpkg -s " + pkg + " 2>/dev/null | grep Status";
        string result = executeCommand(checkCmd);
        if (result.find("install ok installed") == string::npos) {
            missing_packages.push_back(pkg);
        }
    }

    if (!missing_packages.empty()) {
        cout << "Missing packages detected. Installing...\n";
        string installCmd = "sudo apt update && sudo apt install -y ";
        for (const string& pkg : missing_packages) {
            installCmd += pkg + " ";
        }
        int installStatus = system(installCmd.c_str());
        if (installStatus != 0) {
            cerr << "Error: Failed to install required dependencies. Exiting...\n";
            exit(1);
        }
    } else {
        cout << "All dependencies are installed.\n";
    }
}

// Function to determine the local subnet dynamically
string getLocalSubnet() {
    string ip_output = executeCommand("ip -4 addr show | grep 'inet '");
    regex ip_regex(R"(inet\s+(\d+\.\d+\.\d+)\.\d+/\d+)");
    smatch match;
    if (regex_search(ip_output, match, ip_regex)) {
        return match[1].str() + ".";
    }
    cerr << "Failed to determine local subnet." << endl;
    return "";
}

// Function to check if an IP is reachable using ping
bool isIPReachable(const string& ip) {
    string command = "ping -c 1 -W 1 " + ip + " > /dev/null 2>&1";
    return system(command.c_str()) == 0;
}

// Callback function for handling HTTP responses
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    return size * nmemb;  // We don't need to process response data
}

// Function to check if MediaMTX HTTP stream is available
bool isHTTPStreamAvailable(const string& ip) {
    string http_url = "http://" + ip + ":" + to_string(HTTP_PORT) + "/live/index.m3u8";
    CURL* curl = curl_easy_init();
    if (!curl) return false;
    
    curl_easy_setopt(curl, CURLOPT_URL, http_url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // Only check response headers
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2); // 2-second timeout
    
    CURLcode res = curl_easy_perform(curl);
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    curl_easy_cleanup(curl);
    return (res == CURLE_OK && response_code == 200);
}

// Scan the local network for an active HTTP MediaMTX stream
string findMediaMTXHTTPStream() {
    string subnet = getLocalSubnet();
    if (subnet.empty()) return "";
    
    vector<future<bool>> futures;
    for (int i = 1; i < 255; ++i) {
        string ip = subnet + to_string(i);
        futures.push_back(async(launch::async, isIPReachable, ip));
    }
    
    for (int i = 1; i < 255; ++i) {
        if (futures[i - 1].get()) {
            string ip = subnet + to_string(i);
            if (isHTTPStreamAvailable(ip)) {
                return "http://" + ip + ":" + to_string(HTTP_PORT) + "/live/index.m3u8";
            }
        }
    }
    return "";
}

// Connect to the HTTP stream and print the URL
void connectToHTTPStream(const string& stream_url) {
    cout << "Attempting to open HTTP stream: " << stream_url << endl;
    // Open the stream in the default media player (Linux example: xdg-open)
    string command = "xdg-open " + stream_url + " > /dev/null 2>&1";
    system(command.c_str());
    cout << "Opened stream in default media player." << endl;
}

int main() {
    cout << "Checking system dependencies...\n";
    installDependencies();
    
    cout << "Detecting local subnet..." << endl;
    curl_global_init(CURL_GLOBAL_ALL);  // Initialize libcurl

    string stream_url = findMediaMTXHTTPStream();
    if (stream_url.empty()) {
        cerr << "No active MediaMTX HTTP stream found on the local network." << endl;
        curl_global_cleanup();
        return 1;
    }
    
    cout << "Found MediaMTX HTTP stream at: " << stream_url << endl;
    connectToHTTPStream(stream_url);
    
    curl_global_cleanup();  // Cleanup libcurl
    return 0;
}
