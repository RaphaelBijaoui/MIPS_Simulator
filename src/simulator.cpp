// g++ -std=c++11 simulator_1.cpp -o simulator_1
#include "headers.hpp"

using namespace std;

// FUNCTION DECLARATIONS
template <typename T>
void __vertical_print_vector(const vector<T> &v);
void __print_memory(const vector<MEM_TYPE> &v);
char read_instruction(INSTR_TYPE &instruction);
int get_index_from_addr(const uint32_t &addr, vector<pair<uint32_t, uint32_t>> &v);

// MAIN
int main(int argc /* argument count */, char *argv[] /* argument list */)
{
	if (argc != 2)
	{
		cerr << "ERROR: Incorrect number of arguments: " << endl;
		cerr << argv[0] << ":" << argv[1] << endl;
		return 1;
	}

	string binName = argv[1]; // Reading second argument from command line
	// string binName = "src/test4.bin";				// Reading second argument from command line
	ifstream binStream;								// Create binary stream object
	binStream.open(binName, ios::binary | ios::in); // Load .bin file as a binary file
	if (!binStream.is_open())
	{
		cerr << "ERROR: Cannot open binary file." << endl;
		return 1;
	}
	vector<BUFFER_TYPE> buffer(BUFFER_SIZE, 0);
	vector<MEM_TYPE> imem; // Nx2 matrix of <i`nstruction, address> pairs

	// Initialize instruction memory
	ADDR_TYPE address = ADDR_INSTR;
	while (!binStream.eof())
	{
		binStream.read(buffer.data(), buffer.size()); // Reading 32 bits at a time, buffer.data() is a 32bit array
		streamsize s = binStream.gcount();			  // # of bits read
		if (s == 0)
		{
			break; // Ensures stream size 0 reads do not get converted to memory (reached end of bitStream)
		}
		INSTR_TYPE binNo = 0;
		int weight = s - 1;
		for (auto it = buffer.begin(); it != buffer.end(); ++it, --weight)
		{
			if (*it == '1') // IMPORTANT: iterator is a pointer to a char so it must be verfied with a char ''
			{
				binNo += (1 << weight);
			}
		}
		imem.emplace_back(make_pair(address, binNo)); // Inserting binary string to instruction memory
		address += 4;
	}

	// Initialize state and instruction classes
	State S(imem);
	// S.display();
	i_type_instructions i_instruction(S);
	r_type_instructions r_instruction(S);
	j_type_instructions j_instruction(S);

	// __print_memory(imem);

	// Executing instructions
	for (auto instr_ptr : imem)
	{

		char instr_type = read_instruction(S.instr);
		switch (instr_type)
		{
		case 'r':
			r_instruction.execute();
			// r_instruction.display();
			break;
		case 'i':
			i_instruction.execute();
			i_instruction.display();
			S.view_regs();
			S.view_imem();

			break;
		case 'j':
			j_instruction.execute();
			// j_instruction.display();
			break;
		default:
			cerr << "ERROR: FAILED TO READ INSTRUCTION." << endl;
			exit(-10);
		}
	}

	return 0;
} // END OF MAIN

// FUNCTION DEFINITIONS
char read_instruction(INSTR_TYPE &instruction)
{
	INSTR_TYPE opcode = (instruction & 0xFC000000) >> 26; // First 6 bits
	switch (opcode)
	{
	case 0:
		return 'r';
	case 0x2:
	case 0x3:
		return 'j';
	default:
		return 'i';
	}
}

int get_index_from_addr(const uint32_t &addr, vector<pair<uint32_t, uint32_t>> &v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		if (addr == v[i].first)
		{
			return i;
		}
	}
	cerr << "ERROR: Invalid memory address in memory block." << endl;
	exit(-10); // Could not find address
}

// HELPER FUNCTIONS
// Arbitrary type print vector function
template <typename T>
void __vertical_print_vector(const vector<T> &v)
{
	cerr << "Printing vector of size " << v.size() << ":\nSTART [" << endl;
	for (auto &elem : v)
	{
		cerr << hex << elem << "\n";
	}
	cerr << "] END" << endl;
}

void __print_memory(const vector<MEM_TYPE> &v)
{
	cerr << "Printing vector of size " << v.size() << ":\nSTART [" << endl;
	for (auto elem : v)
	{
		cerr << hex << elem.first << ":" << elem.second << "\n";
	}
	cerr << "] END" << endl;
}
