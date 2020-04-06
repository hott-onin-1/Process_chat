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
 * \brief Header for Utilities fuctions
 * \version 1.0
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
 * \return return the ID for the fifo file (-1 if error)
 */
int find_file(char pipe_name[], int number_pos);

/**
 * \fn void stdin_flush(void)
 * \brief Clean stdin
 */
void stdin_flush(void);

/**
 * \fn void terminate(int context)
 * \brief Close pipes, clean files and terminate
 * \param context 0: failure. 1: success. 2: success and send SIGUSR1 to remote
 */
void terminate(int context);

#endif // UTILITIES_H
