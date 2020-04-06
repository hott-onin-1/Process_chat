/*
===============================================================

                         Process Chat
Inter-process chat application for Operating Systems classes

                           ECE Paris
                          April 2020

===============================================================
*/

/**
 * \file core.h
 * \brief Header for Core fuctions
 * \version 1.0
 * \date April 2020
*/

/*
File: core.h
Header for core functions
*/

#ifndef CORE_H
#define CORE_H

/**
 * \fn void handle_signals(int sig)
 * \brief Signal handler, calle when process receive SIGINT or SIGUR1
 *
 * \param sig ID of the signal received
 */
void handle_signals(int sig);

/**
 * \fn ask_id(int forbidden_id)
 * \brief Ask the user the ID of remote client to connect to
 *
 * \param forbidden_id ID that can't be used (usually local ID)
 * \return valid ID for the remote
 */
int ask_id(int forbidden_id);

/**
 * \fn etablish_connexion(t_client * remote, t_client * local)
 * \brief etablish the connexion between client and remote
 *
 * \param remote client structure containing remote client information
 * \param local client structure containing local client information
 */
void etablish_connexion(t_client * remote, t_client * local);

/**
 * \fn void chat_loop(t_client * remote, t_client * local)
 * \brief Provide chat services between remote and local
 *
 * \param remote client structure containing remote client information
 * \param local client structure containing local client information
 */
void chat_loop(t_client * remote, t_client * local);

#endif // CORE_H
