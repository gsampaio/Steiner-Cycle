/******************************************************************************
 * timer.hpp: Interface/Implementation for system clock time class.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : 09/09/2011 by andrade
 *  Last update: 09/09/2011 by andrade
 *
 * This software is licensed under the Common Public License. Please see
 * accompanying file for terms.
 *****************************************************************************/

#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>

#include <ctime>
#include <unistd.h>
#include <sys/time.h>

/**
 * \brief System clock timer class.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains of a Timer based on system clock. It follows the same
 * interface of Boost::Timer, but it measures the real time. Resolution in
 * seconds.
 */
class Timer {
    public:
        /** \name Constructors and Destructor*/
        //@{
        /// Default constructor.
        Timer() {
            gettimeofday(&start_time, 0);
        }
        //@}

        /** \name Member methods. */
        //@{
        /// Restarts the timer.
        void restart() {
            gettimeofday(&start_time, 0);
        }

        /// Return elapsed time in seconds.
        double elapsed() const {
            timeval end_time;
            gettimeofday(&end_time, 0);

            return (end_time.tv_sec + end_time.tv_usec/1000000.0) -
                   (start_time.tv_sec + start_time.tv_usec/1000000.0);
        }
        //@}

    protected:
        timeval start_time;
};
#endif // TIMER_HPP
