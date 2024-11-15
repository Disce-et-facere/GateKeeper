#include "timestamp.h"

char *getTimestamp(){

    time_t currentTime = time(NULL);

    struct tm *timeInfo = localtime(&currentTime);

    if (timeInfo == NULL) {
        fprintf(stderr, "Error getting local time\n");
        exit(EXIT_FAILURE);
    }

    char *timestamp = (char *)malloc(20 * sizeof(char)); // Adjust size based on your needs

    if (timestamp == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    if (strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", (const struct tm *)timeInfo) == 0) {
        fprintf(stderr, "Error formatting time\n");
        exit(EXIT_FAILURE);
    }
    
    return timestamp;
}
