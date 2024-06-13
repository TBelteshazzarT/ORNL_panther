#include <sdr_control/serial_interface.hpp>
#include <iostream>

namespace serial_interface
{
    SerialInterface::SerialInterface(std::string port, uint32_t baud, uint32_t read_timeout) 
        : port_(port), baud_(baud), port_opened_(false), read_timeout_(read_timeout), read_error_(false)
    {
        io_service_ = std::make_shared<asio::io_service>();
        serial_port_ = std::make_shared<asio::serial_port>(*io_service_);
        timer_ = std::make_shared<asio::deadline_timer>(serial_port_->get_io_service());
    }

    SerialInterface::~SerialInterface()
    {

    }

    bool SerialInterface::open_port()
    {
        try
        {
            serial_port_->open(port_);
            serial_port_->set_option(asio::serial_port_base::baud_rate(baud_));
            serial_port_->set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));
            serial_port_->set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
            serial_port_->set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
        }
        catch(asio::system_error& er)
        {
            std::cout << "Error opening " << port_ << ": " << er.what() << "\n";
            return false;
        }

        port_opened_ = true;
        std::cout << "Opened port " << port_ << "\n";
        return port_opened_;
    }

    void SerialInterface::close_port()
    {
        serial_port_->close();
        port_opened_ = false;
    }

    bool SerialInterface::write(std::vector<uint8_t> buffer)
    {
        if(!port_opened_)
        {
            open_port();
            return false;
        }
        
        try
        {
            // std::cout << "Write value: " << (uint)buffer[0] << " ";
            asio::write(*serial_port_, asio::buffer(buffer));      // Blocking write that could hang
        }
        catch(asio::system_error& er)
        {
            std::cout << "Error writing to port " << port_ << ": " << er.what() << "\n";
            return false;
        }

        return true;
    }

    bool SerialInterface::read_byte(uint8_t& byte)
    {
        if(!port_opened_)
        {
            open_port();
            return false;
        }

        int8_t value = 0;

        try
        {
            serial_port_->get_io_service().reset();
            asio::async_read(*serial_port_, asio::buffer(&value, 1), std::bind(&SerialInterface::read_handler,
                this, std::placeholders::_1, std::placeholders::_2));

            timer_->expires_from_now(boost::posix_time::milliseconds(read_timeout_));     // TODO: Remove Boost depend
            timer_->async_wait(std::bind(&SerialInterface::timed_out, this, std::placeholders::_1));

            serial_port_->get_io_service().run();
        }
        catch(asio::system_error& er)
        {
            std::cout << "Error reading from port " << port_ << ": " << er.what() << "\n";
            return false;
        }
        byte = value;
        // std::cout << "Read value: " << (uint)byte << "\n";

        if(!read_error_) { return true; }
        else { return false; }
    }

    bool SerialInterface::block_read_byte(uint8_t& byte)
    {
        uint8_t value = 0;
        uint8_t n_bytes = 0;

        try
        {
            n_bytes = asio::read(*serial_port_, asio::buffer(&value, 1));
        }
        catch(asio::system_error& er)
        {
            std::cout << "Error reading from port " << port_ << ": " << er.what() << "\n";
            return false;
        }

        if(!n_bytes) 
        {
            std::cout << "No bytes read\n";
            return false; 
        }

        std::cout << "Read value: " << (uint)value << "\n";

        byte = value;
        return true;
    }

    void SerialInterface::read_handler(const asio::error_code& ec, std::size_t bytes_transferred)
    {
        // std::cout << "Error code: " << ec << " Bytes: " << bytes_transferred << "\n";
        read_error_ = (ec || (bytes_transferred == 0));
        timer_->cancel();
    }

    void SerialInterface::timed_out(const asio::error_code& ec)
    {
        if(ec) { return; }
        serial_port_->cancel();
    }
}