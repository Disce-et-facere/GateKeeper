#include "randomPass.h"

char *randomPass(){

    int length = 16;

    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()";

    char *password = malloc((size_t)(length + 1) * sizeof(char));

    if (password == NULL) {
        fprintf(stderr, "Memory allocation failed. / randomPass\n");
        exit(EXIT_FAILURE);
    }

     srand((unsigned int)time(0));

     for(int i = 0; i < length; i++) {
      int randomIndex = (int)rand() % (sizeof(chars) - 1);
      password[i] = chars[randomIndex];
   }

   password[length] = '\0';

   return password;
}