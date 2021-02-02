/*

File    : utility.h
Author  : Stefano Morgani
Date    : 2 March 2018
REV 1.0

Various utility functions
*/


#ifndef UTILITY_E_PUCK2_H
#define UTILTIY_E_PUCK2_H


/**
 * @brief Read the ID of the robot from the eFuse (BLOCK3) memory. Should be called once at the beginning.
 */
void robot_read_id(void);

/**
 * @brief Return the robot ID. Must be called after calling \ref robot_read_id.
 *
 * @return robot ID
 */
uint16_t robot_get_id(void);


#endif /* UTILITY_E_PUCK2_H */