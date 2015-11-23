#include "stdafx.h"
#include "SerialPort.h"

int main(int argc, char* argv[])
{
	bool rv;

	SerialPort::print_devices();
	std::string name = SerialPort::get_port_name(0);

	SerialPort serial;
	rv = serial.start(name.c_str(), 115200);	
	if (rv == false) {
		return -1;
	}

	// initialize
	serial.end_of_line_char(0x0d);
	serial.write_some("BI010\r\n");
	serial.write_some("PE011\r\n");
	serial.write_some("RA01000\r\n");

	// wait
	Sleep(5 * 1000);

	serial.stop();

	return 0;
}
