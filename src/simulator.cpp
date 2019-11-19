#include "headers.hpp"

using namespace std;

int main(int argc /* argument count */, char *argv[] /* argument list */)
{
	try
	{
		bool DEBUG = 1;
		if (argc > 3)
		{
			cerr << "Error: Incorrect number of arguments: " << endl;
			return 1;
		}
		else if (argc == 3)
		{
			DEBUG = stoi(argv[2]);
			cerr << "+--------------------------------------------------------------------------------+" << endl;
			if (DEBUG)
			{
				cerr << ">> ENTERING DEBUGGING MODE: DEBUG CODE(" << DEBUG << ")" << endl;
			}
			else
			{
				cerr << ">> RUNNING WITHOUT DEBUGGING MODE: DEBUG CODE(" << DEBUG << ")" << endl;
			}
			cerr << "+--------------------------------------------------------------------------------+" << endl;
		}

		string binName = argv[1]; // Reading second argument from command line
		ifstream binStream;		  // Create binary stream object

		binStream.open(binName, ios::binary); // Load .bin file as a binary file
		if (!binStream.is_open())
		{
			cerr << "Error: Cannot open binary file." << endl;
			return 1;
		}

		char buffer[4];				//to hold 4 bytes
		
		vector<MEM_TYPE> instruction_mem; // vector of uint32_t holding instruction memory
		instruction_mem.resize(ADDR_INSTR_LENGTH);

		// Fill instruction memory
		ADDR_TYPE address = ADDR_INSTR_OFFSET; //starting from base 0x4000000
		while (!binStream.eof())
		{
			binStream.read(buffer, 4);
			binStream.peek(); // Check for eof on the next byte

			MEM_TYPE binNo = (static_cast<unsigned char>(buffer[0]) << 24) | (static_cast<unsigned char>(buffer[1]) << 16) | (static_cast<unsigned char>(buffer[2]) << 8) | static_cast<unsigned char>(buffer[3]);

			if (DEBUG)
			{
				cerr << hex << binNo << endl;
			}
			instruction_mem[addr_to_index(ADDR_INSTR_OFFSET, address)] = binNo;
			address += 1;
		}

		__print_memory_specific(instruction_mem, 0, 4);

		// Initialize state and instruction classes
		CPU C(instruction_mem);
		i_type_instructions i_instruction(C, DEBUG);
		r_type_instructions r_instruction(C, DEBUG);
		j_type_instructions j_instruction(C, DEBUG);

		// Executing instructions
		int executions = 0;
		ADDR_TYPE next_instruction;

		for (; C.pc >= ADDR_INSTR_OFFSET && C.pc < ADDR_INSTR_OFFSET + ADDR_INSTR_LENGTH; /*&& (instruction_mem[C.pc] != 0); address++*/)
		{

			C.reg[0] = 0;			  // $0 is always 0 on every clock cycle
			next_instruction = C.npc; //The preserved next instruction enables a branch delay

			C.instr = instruction_mem[addr_to_index(ADDR_INSTR_OFFSET, C.pc)];

			char instr_type = read_instruction(C.instr);

			switch (instr_type)
			{
			case 'r':
				r_instruction.execute();
				break;
			case 'i':
				i_instruction.execute();
				break;
			case 'j':
				j_instruction.execute();
				break;
			default:
				throw(static_cast<int>(INSTRUCTION_EXIT_CODE));
			}
			if (DEBUG)
			{
				C.display();
			}
			if (DEBUG)
			{
				C.view_regs();
			}

			executions++;
			C.pc = next_instruction;
		}

		//Successful termination/completion = return low 8-bits of the value in register $2.
		INSTR_TYPE SUCCESSFUL_EXIT_CODE = C.reg[2] & 0xFF;
		if (DEBUG)
		{
			cerr << "Executions: " << executions << "\n"
				 << ">> PROGRAM EXITED WITH RETURN CODE: " << SUCCESSFUL_EXIT_CODE << endl;
		}
		exit(static_cast<int>(SUCCESSFUL_EXIT_CODE));
	}
	catch (const int EXIT_CODE) // Exception handling
	{
		switch (EXIT_CODE)
		{
		case static_cast<int>(ARITHMETIC_EXIT_CODE):
			exit(static_cast<int>(ARITHMETIC_EXIT_CODE));
		case static_cast<int>(MEMORY_EXIT_CODE):
			exit(static_cast<int>(MEMORY_EXIT_CODE));
		case static_cast<int>(INSTRUCTION_EXIT_CODE):
			exit(static_cast<int>(INSTRUCTION_EXIT_CODE));
		case static_cast<int>(IO_EXIT_CODE):
			exit(static_cast<int>(IO_EXIT_CODE));
		default:;
		}
	}
	catch (...) // Unknown error
	{
		exit(static_cast<int>(INTERNAL_EXIT_CODE));
	}

	return 0;
}
