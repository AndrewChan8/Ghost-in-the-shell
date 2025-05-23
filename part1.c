#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int count_lines(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
      perror("Error opening file");
      exit(EXIT_FAILURE);
  }
  int lines = 0;
  char line[1024];
  while (fgets(line, sizeof(line), file) != NULL) {
      lines++;
  }
  fclose(file);
  return lines;
}

int main(int argc, char *argv[]){
  if(argc != 3){
    fprintf(stderr, "Invalid use: incorrect number of parameters\n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "-f") != 0) {
    fprintf(stderr, "Error: Missing '-f' flag\n");
    exit(EXIT_FAILURE);
  }

  int num_lines = count_lines(argv[2]);

  pid_t *pid_array = (pid_t*)malloc(num_lines * sizeof(pid_t));
  if (!pid_array) {
    perror("Failed to allocate memory for pid_array");
    exit(EXIT_FAILURE);
  }

  FILE *file = fopen(argv[2], "r");
  if (!file) {
    perror("Error opening file");
    free(pid_array);
    exit(EXIT_FAILURE);
  }

  int max_args = 10;
  char line[1024];
  int num_processes = 0;

  while(fgets(line, sizeof(line), file) != NULL){
    line[strcspn(line, "\n")] = '\0';

    char *args[max_args];
    char *token = strtok(line, " ");
    int j = 0;

    while (token != NULL && j < max_args) {
      args[j++] = token;
      token = strtok(NULL, " ");
    }
    args[j] = NULL; // Null terminate for execvp

    pid_t pid = fork();
    if(pid < 0){
      perror("Failed to fork process");
      fclose(file);
      free(pid_array);
      exit(EXIT_FAILURE);
    }else if(pid == 0){
      fclose(file);
      if(execvp(args[0], args) == -1) {
        perror("Execvp failed");
        free(pid_array);
        exit(EXIT_FAILURE);
      }
    }else{
      pid_array[num_processes++] = pid;
    }
  }
  fclose(file);
  for (int i = 0; i < num_processes; i++) {
    if (waitpid(pid_array[i], NULL, 0) < 0) {
      perror("Waitpid failed");
    }
  }

  free(pid_array);

  return 0;
}