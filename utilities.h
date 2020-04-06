/*
===============================================================

                         Process Chat
Inter-process chat application for Operating Systems classes

                           ECE Paris
                          April 2020

===============================================================
*/

/**
 * \file utilities.h
 * \brief Process Chat application
 * \version 0.2
 * \date April 2020
*/

/*
File: utilities.h
Header for utilities functions
*/

#ifndef UTILITIES_H
#define UTILITIES_H

/**
 * \fn void search_and_clean(char str[], int len)
 * \brief Parse a string and terminate it properly with \c \n and \c \0
 *
 * \param str String containing the text to parse
 * \param len Lenght of str
 */
void search_and_clean(char str[], int len);

/**
 * \fn void check_error(const int err, const char str[])
 * \brief If err is -1, display error and terminate
 *
 * \param err error value
 * \param len string containing error location
 */
void check_error(const int err, const char str[]);

/**
 * \fn void display_intro(void)
 * \brief Display the intro message
 */
void display_intro(void);

/**
 * \fn void my_close(void)
 * \brief Properly close pipes
 */
void my_close(void);

/**
 * \fn void my_clean(void)
 * \brief Remove the fifo file and terminate
 */
void my_clean(void);

/**
 * \fn int find_file(char pipe_name[], int number_pos)
 * \brief Find an available name for the fifo file
 *
 * \param pipe_name generic name of the file
 * \param number_pos position of the character to be replace by a number
 */
int find_file(char pipe_name[], int number_pos);

/**
 * \fn void stdin_flush(void)
 * \brief Clean stdin
 */
void stdin_flush(void);

/**
 * \fn void terminate(int sucess)
 * \brief Close pipes, clean files and terminate
 * \param success Close on success or not (1 or 0)
 */
void terminate(int sucess);

#endif // UTILITIES_H