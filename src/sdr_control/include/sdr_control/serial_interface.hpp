#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <string>
#include <memory>
#include <chrono>
#include <asio.hpp>

namespace serial_interface
{
    /**
     * @brief Class used to interface through a serial port
     */
    class SerialInterface
    {
        public:
            /**
             * @brief Construct a new Serial Interface object
             * @param port Name of the port to open
             * @param baud Baud rate for serial communication
             * @param read_timeout Timeout in milliseconds for async read
             */
            SerialInterface(std::string port, uint32_t baud, uint32_t read_timeout);
            /**
             * @brief Destroy the Serial Interface object
             */
            ~SerialInterface();
            /**
             * @brief Attempts to open a serial port
             * @return true Port opened
             * @return false Failed to open port
             */
            bool open_port();
            /**
             * @brief Closes serial port
             */
            void close_port();
            /**
             * @brief Writes a series of characters to a serial port
             * @param buffer Vector that contains the data to be written
             * @return true Serial write successful
             * @return false Serial write failed
             */
            bool write(std::vector<uint8_t> buffer);
            /**
             * @brief Attempt to read a single byte in the serial buffer
             * @param byte Value of the byte read
             * @return true Read successful
             * @return false Read failed
             */
            bool read_byte(uint8_t& byte);

            bool block_read_byte(uint8_t& byte);

        private:
            /**
             * @brief Handler to be called when the async read operation completes
             * @param ec Result of operation
             * @param bytes_transferred Number of bytes copied into the buffer
             */
            void read_handler(const asio::error_code& ec, std::size_t bytes_transferred);
            /**
             * @brief To be called when an async function times out
             * @param ec Result of operation
             */
            void timed_out(const asio::error_code& ec);

            // Port name
            std::string port_;
            // Serial baud rate
            uint32_t baud_;
            // Flag to see if the port is opened
            bool port_opened_;
            // Serial read timeout in milliseconds
            uint32_t read_timeout_;
            // Flag to see if there was a read error
            bool read_error_;
            // ASIO object pointers
            std::shared_ptr<asio::serial_port> serial_port_;
            std::shared_ptr<asio::io_service> io_service_;
            std::shared_ptr<asio::deadline_timer> timer_;
            
    };
}

#endif