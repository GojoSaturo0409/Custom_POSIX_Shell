#include "iMan.h"

// skeleton taken by ChatGPT, breakdown into functions done by me for ease in tracking

// Function to resolve DNS and get the host information
struct hostent* resolve_dns(const char* hostname) {
    struct hostent* dns_info = gethostbyname(hostname);
    if (dns_info == NULL) {
        fprintf(stderr, "DNS resolution failed\n");
    }
    return dns_info;
}

// Function to create a socket and connect to the server
int connect_to_server(struct sockaddr_in* srv_addr, struct hostent* dns_info) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Copy the first resolved IP address into the server address structure
    memcpy(&srv_addr->sin_addr, dns_info->h_addr_list[0], dns_info->h_length);

    if (connect(socket_fd, (struct sockaddr*)srv_addr, sizeof(*srv_addr)) == -1) {
        perror("Connection to server failed");
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

// Function to construct and send the HTTP GET request
int send_http_request(int socket_fd, const char* search_term) {
    char http_request[2048];  // Use a reasonable buffer size for the request
    snprintf(http_request, sizeof(http_request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", search_term);

    if (send(socket_fd, http_request, strlen(http_request), 0) == -1) {
        perror("Failed to send HTTP request");
        return 0;
    }
    return 1;
}

// Function to strip HTML tags
void strip_html_tags(char* dest, const char* src) {
    int in_tag = 0;
    while (*src) {
        if (*src == '<') {
            in_tag = 1;
        } else if (*src == '>') {
            in_tag = 0;
        } else if (!in_tag) {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
}

// Function to receive and parse the HTTP response
void receive_and_parse_response(int socket_fd) {
    char* page_content = malloc(MAX_RESPONSE_LEN);  // Dynamically allocate buffer
    if (!page_content) {
        perror("Memory allocation failed");
        close(socket_fd);
        exit(1);
    }

    page_content[0] = '\0';  // Initialize buffer
    ssize_t recv_len;
    char recv_buffer[8192];  // Reasonable buffer for receiving chunks

    int body_started = 0;
    int no_match_found = 0;

    while ((recv_len = recv(socket_fd, recv_buffer, sizeof(recv_buffer) - 1, 0)) > 0) {
        recv_buffer[recv_len] = '\0';  // Null-terminate the buffer

        // Check if the body of the response has started
        if (!body_started) {
            char* content_start = strstr(recv_buffer, "<STRONG");
            if (content_start) {
                char* content_open = strstr(content_start, ">");
                if (content_open) {
                    content_open++;
                    strcat(page_content, content_open);
                    body_started = 1;
                }
            }
        } else {
            // Check for "No matches" in the response
            if (strstr(recv_buffer, "No matches")) {
                no_match_found = 1;
                break;
            }
            strcat(page_content, recv_buffer);
        }
    }

    if (no_match_found) {
        printf("No matching man page found for the command\n");
    } else {
        // Strip HTML tags from the received content
        char* parsed_content = malloc(MAX_RESPONSE_LEN);
        if (parsed_content) {
            strip_html_tags(parsed_content, page_content);
            printf("%s\n", parsed_content);
            free(parsed_content);
        }
    }

    free(page_content);  // Free dynamically allocated buffer
}

// Controller function to drive the program
void controller(char* argv[]) {
    char search_term[4096];  // Use a smaller buffer for the search term
    strncpy(search_term, argv[0], sizeof(search_term) - 1);
    search_term[sizeof(search_term) - 1] = '\0';

    struct hostent* dns_info = resolve_dns("man.he.net");
    if (dns_info == NULL) {
        exit(1);
    }

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(80);

    int socket_fd = connect_to_server(&srv_addr, dns_info);
    if (socket_fd == -1) {
        exit(1);
    }

    if (!send_http_request(socket_fd, search_term)) {
        close(socket_fd);
        exit(1);
    }

    receive_and_parse_response(socket_fd);
    close(socket_fd);
}
