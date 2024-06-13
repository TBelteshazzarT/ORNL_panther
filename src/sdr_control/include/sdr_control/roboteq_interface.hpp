#ifndef ROBOTEQ_H
#define ROBOTEQ_H

#include <iostream>
#include <sstream>
#include <chrono>
#include <memory>
#include <exception>
#include <sdr_control/serial_interface.hpp>

namespace roboteq_interface
{
    /**
     * @brief Class used to interface with RoboteQ motor controllers
     */
    class RoboteqInterface
    {
        public:
            /**
             * @brief Construct a new Roboteq Interface object
             * @param port Name of the port the Roboteq is connected to
             * @param baud Serial baud rate
             */
            RoboteqInterface(std::string port, uint32_t baud);
            /**
             * @brief Destroy the Roboteq Interface object
             */
            ~RoboteqInterface();
            /**
             * @brief Sends Roboteq commands for a differential drive robot
             * @param ch1 Command value for channel 1
             * @param ch2 Command value for channel 2
             * @param speed True for closed looop speed control and false for open loop
             */
            void diff_cmd(int32_t ch1, int32_t ch2, bool speed=false);     
            /**
             * @brief Sends Roboteq commands for a skid steer robot using RoboCAN
             * @param ch1 Command value for all motors on channel 1
             * @param ch2 Command value for all motors on channel 2
             * @param speed True for closed looop speed control and false for open loop
             */
            void skid_can_cmd(int32_t ch1, int32_t ch2, bool speed=false); 
            /**
             * @brief FUNCTION IS NOT IMPLEMENTED. DO NOT USE!
             * @param drive 
             * @param turn 
             * @param strafe 
             */
            void mecanum_can_cmd(float drive, float turn, float strafe);    // TODO    
            /**
             * @brief Puts the controller in open loop mode
             */
            void set_open();
            /**
             * @brief Puts the controller in closed loop speed control mode
             */
            void set_closed();  
            /**
             * @brief Resets encoder counts
             */
            void reset_encoders();
            /**
             * @brief Puts controllers in open loop mode through RoboCAN
             */
            void can_set_open();
            /**
             * @brief Puts controllers in closed loop speed control mode through RoboCAN
             */
            void can_set_closed();
            /**
             * @brief Resets encoder counts through RoboCAN
             */
            void can_reset_encoders();
            /**
             * @brief Sends a !r command to the Roboteq which consolidates data into a single 
             * packet and responds
             * @param batt_v Battery voltage in Volts
             * @param amps Current draw of the motors in Amps
             * @param enc1 Raw encoder counts of channel 1
             * @param enc2 Raw encoder counts of channel 2
             * @return true Packet successfully read
             * @return false Packet read failed
             */
            bool poll_packet(float &batt_v, float &amps, int64_t &enc1, int64_t &enc2);
            
        private:
            // Serial interface object pointer
            std::shared_ptr<serial_interface::SerialInterface> si_;
    };
}

#endif

