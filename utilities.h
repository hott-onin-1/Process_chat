#ifndef UTILITIES_H
#define UTILITIES_H

void search_and_clean(char str[], int len);
void check_error(const int err, const char str[]);
void display_intro(void);
void my_close(void);
void my_clean(void);
int find_file(char pipe_name[], int number_pos);
void stdin_flush(void);

#endif // UTILITIES_H