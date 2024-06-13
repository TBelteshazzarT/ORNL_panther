#include <sdr_control/roboteq_interface.hpp>

namespace roboteq_interface
{
	RoboteqInterface::RoboteqInterface(std::string port, uint32_t baud)
	{
		si_ = std::make_shared<serial_interface::SerialInterface>(port, baud, 5);
		si_->open_port();
	}

	RoboteqInterface::~RoboteqInterface() { }

	void RoboteqInterface::diff_cmd(int32_t ch1, int32_t ch2, bool speed)
	{
		std::ostringstream cmd1;
		std::ostringstream cmd2;

		if(speed)
		{
			cmd1 << "!S 1 " << ch1 << "\r\n";
			cmd2 << "!S 2 " << ch2 << "\r\n";
		}
		else
		{
			cmd1 << "!G 1 " << ch1 << "\r\n";
			cmd2 << "!G 2 " << ch2 << "\r\n";
		}

		std::vector<uint8_t> v_cmd1(cmd1.str().begin(), cmd1.str().end());
		std::vector<uint8_t> v_cmd2(cmd2.str().begin(), cmd2.str().end());

		si_->write(v_cmd1);
		si_->write(v_cmd2);
	}

	void RoboteqInterface::skid_can_cmd(int32_t ch1, int32_t ch2, bool speed)
	{
		std::ostringstream fr_cmd;
		std::ostringstream fl_cmd;
		std::ostringstream rr_cmd;
		std::ostringstream rl_cmd;

		if(speed)
		{
			fr_cmd << "@01!S 1 " << ch1 << "\r\n";
			fl_cmd << "@01!S 2 " << ch2 << "\r\n";
			rr_cmd << "@02!S 1 " << ch1 << "\r\n";
			rl_cmd << "@02!S 2 " << ch2 << "\r\n";
		}
		else
		{
			fr_cmd << "@01!G 1 " << ch1 << "\r\n";
			fl_cmd << "@01!G 2 " << ch2 << "\r\n";
			rr_cmd << "@02!G 1 " << ch1 << "\r\n";
			rl_cmd << "@02!G 2 " << ch2 << "\r\n";
		}

		std::vector<uint8_t> v_fr_cmd(fr_cmd.str().begin(), fr_cmd.str().end());
		std::vector<uint8_t> v_fl_cmd(fl_cmd.str().begin(), fl_cmd.str().end());
		std::vector<uint8_t> v_rr_cmd(rr_cmd.str().begin(), rr_cmd.str().end());
		std::vector<uint8_t> v_rl_cmd(rl_cmd.str().begin(), rl_cmd.str().end());


		si_->write(v_fr_cmd);
		si_->write(v_fl_cmd);
		si_->write(v_rr_cmd);
		si_->write(v_rl_cmd);
	}

	void RoboteqInterface::mecanum_can_cmd(float drive, float turn, float strafe)
	{
		// TODO
	}

	void RoboteqInterface::set_open()
	{
		std::string ch1_open = "^MMOD 1 0\r\n";
		std::string ch2_open = "^MMOD 2 0\r\n";

		std::vector<uint8_t> v_ch1_open(ch1_open.begin(), ch1_open.end());
		std::vector<uint8_t> v_ch2_open(ch2_open.begin(), ch2_open.end());

		si_->write(v_ch1_open);
		si_->write(v_ch2_open);
		usleep(15000);
	}

	void RoboteqInterface::set_closed()
	{
		std::string ch1_closed = "^MMOD 1 1\r\n";
		std::string ch2_closed = "^MMOD 2 1\r\n";

		std::vector<uint8_t> v_ch1_closed(ch1_closed.begin(), ch1_closed.end());
		std::vector<uint8_t> v_ch2_closed(ch2_closed.begin(), ch2_closed.end());

		si_->write(v_ch1_closed);
		si_->write(v_ch2_closed);
		usleep(15000);
	}

	void RoboteqInterface::reset_encoders()
	{
		std::string ch1_reset = "!C 1 0\r\n";
		std::string ch2_reset = "!C 2 0\r\n";

		std::vector<uint8_t> v_ch1_reset(ch1_reset.begin(), ch1_reset.end());
		std::vector<uint8_t> v_ch2_reset(ch2_reset.begin(), ch2_reset.end());

		set_open();
		si_->write(v_ch1_reset);
		si_->write(v_ch2_reset);
		usleep(15000);
		set_closed();
	}

	void RoboteqInterface::can_set_open()
	{
		std::string fr_open = "@01^MMOD 1 0\r\n";
		std::string fl_open = "@01^MMOD 2 0\r\n";
		std::string rr_open = "@02^MMOD 1 0\r\n";
		std::string rl_open = "@02^MMOD 2 0\r\n";

		std::vector<uint8_t> v_fr_open(fr_open.begin(), fr_open.end());
		std::vector<uint8_t> v_fl_open(fl_open.begin(), fl_open.end());
		std::vector<uint8_t> v_rr_open(rr_open.begin(), rr_open.end());
		std::vector<uint8_t> v_rl_open(rl_open.begin(), rl_open.end());

		si_->write(v_fl_open);
		si_->write(v_fr_open);
		si_->write(v_rr_open);
		si_->write(v_rl_open);
		usleep(15000);
	}

	void RoboteqInterface::can_set_closed()
	{
		std::string fr_closed = "@01^MMOD 1 0\r\n";
		std::string fl_closed = "@01^MMOD 2 0\r\n";
		std::string rr_closed = "@02^MMOD 1 0\r\n";
		std::string rl_closed = "@02^MMOD 2 0\r\n";

		std::vector<uint8_t> v_fr_closed(fr_closed.begin(), fr_closed.end());
		std::vector<uint8_t> v_fl_closed(fl_closed.begin(), fl_closed.end());
		std::vector<uint8_t> v_rr_closed(rr_closed.begin(), rr_closed.end());
		std::vector<uint8_t> v_rl_closed(rl_closed.begin(), rl_closed.end());

		si_->write(v_fl_closed);
		si_->write(v_fr_closed);
		si_->write(v_rr_closed);
		si_->write(v_rl_closed);
		usleep(15000);
	}

	void RoboteqInterface::can_reset_encoders()
	{
		std::string fr_cmd = "@01!C 1 0\r\n";
		std::string fl_cmd = "@01!C 2 0\r\n";
		std::string rr_cmd = "@02!C 1 0\r\n";
		std::string rl_cmd = "@02!C 2 0\r\n";

		std::vector<uint8_t> v_fr_cmd(fr_cmd.begin(), fr_cmd.end());
		std::vector<uint8_t> v_fl_cmd(fl_cmd.begin(), fl_cmd.end());
		std::vector<uint8_t> v_rr_cmd(rr_cmd.begin(), rr_cmd.end());
		std::vector<uint8_t> v_rl_cmd(rl_cmd.begin(), rl_cmd.end());
	
		can_set_open();
		si_->write(v_fr_cmd);
		si_->write(v_fl_cmd);
		si_->write(v_rr_cmd);
		si_->write(v_rl_cmd);
		usleep(15000);
		can_set_closed();
	}

	bool RoboteqInterface::poll_packet(float &batt_v, float &amps, int64_t &enc1, int64_t &enc2)
	{
		const uint8_t size = 10;
		const float timeout = 0.05;
		float time = 0;
		int64_t check = 0;
		std::string cmd = "!r\r\n";
		std::vector<uint8_t> v_cmd(cmd.begin(), cmd.end());
		uint8_t c = 0;
		uint8_t state = 0;
		std::string result;
		std::string header("SDR");
		std::vector<std::string> result_array;

		while(si_->read_byte(c)) { }

		if(si_->write(v_cmd))
		{
			std::chrono::steady_clock::time_point start_clock = std::chrono::steady_clock::now();
			while(time <= timeout)
			{
				if(si_->read_byte(c))
				{
					result += c;
				}
				//std::cout << result << std::endl;
				switch(state)
				{
					case 0:
						if(result.find(header) != std::string::npos)
						{
							state++;
							// std::cout << "Header found" << std::endl;
						}
						break;
					case 1:
						if(result.find("\n") != std::string::npos)
						{
							std::stringstream s_stream(result);
							while(s_stream.good())
							{
								std::string substr;
								std::getline(s_stream, substr, ',');
								result_array.push_back(substr);
							}
							if(result_array.size() == size)
							{
								state++;
								// std::cout << "Newline found" << std::endl;
							}
						}
						break;
					case 2:
						for(uint16_t i=1; i<(result_array.size()-1); i++)
						{
							try
							{
								check += std::stol(result_array.at(i));	
							}
							catch(std::exception &e)
							{
								std::cout << "RoboteQ: " << e.what() << std::endl;
							}
						}
						if(check == std::stol(result_array.at(result_array.size()-1)))
						{
							try
							{
								batt_v = std::stof(result_array.at(3))/10.0;
								amps = fabs((std::stof(result_array.at(4))/10.0)) + fabs((std::stof(result_array.at(5))/10.0));
								enc1 = std::stol(result_array.at(1));
								enc2 = std::stol(result_array.at(2));
							}
							catch(std::exception &e)
							{
								std::cout << "RoboteQ: " << e.what() << std::endl;
							}

							return true;
						}
						break;
				}
				time = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_clock).count();
				//std::cout << time << std::endl;
			}
		}

		return false;
	}
}