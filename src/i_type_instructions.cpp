// This file contains the function definitions for I type instructions
#include "i_type_instructions.hpp"
using namespace std;

int i_type_instructions::execute()
{
    /* Opcode - 6 bits, Source 1 - 5 bits, Dest - 5 bits, Immediate constant - 16 bits */

    cerr << ">> Executing I type instruction ";
    code = (C->instr & 0xFC000000) >> 26;
    src1 = (C->instr & 0x3E00000) >> 21;
    dest = (C->instr & 0x1F0000) >> 16;
    idata = (C->instr & 0xFFFF);
    // Sign extend 16bit immediate
    if (idata >> 15)
    {
        sx_idata = idata | 0xFFFF0000;
    }
    else
    {
        sx_idata = idata;
    }

    switch (code)
    {
    case 0X8:
        ADDI(C, src1, dest, sx_idata);
        return 1;
    case 0X9:
        ADDIU(C, src1, dest, sx_idata);
        return 1;
    case 0XC:
        ANDI(C, src1, dest, idata);
        return 1;
    case 0X4:
        BEQ(C, src1, dest, sx_idata);
        return 1;
    case 0X5:
        BNE(C, src1, dest, sx_idata);
        return 1;
    case 0X24:
        // LBU(C , src1, dest, sx_idata);
        return 1;
    case 0X20:
        // LB(C , src1, dest, sx_idata);
        return 1;
    case 0X25:
        // LHU(C , src1, dest, sx_idata);
        return 1;
    case 0X21:
        // LH(C , src1, dest, sx_idata);
        return 1;
    case 0XF:
        LUI(C, src1, dest, idata);
        return 1;
    case 0X23:
        LW(C, src1, dest, sx_idata);
        return 1;
    case 0X22:
        // LWL(C , src1, dest, sx_idata);
        return 1;
    case 0X26:
        // LWR(C , src1, dest, sx_idata);
        return 1;
    case 0X28:
        // SB(C , src1, dest, sx_idata);
        return 1;
    case 0X29:
        // SH(C, src1, dest, sx_idata);
        return 1;
    case 0X2B:
        // SW(C, src1, dest, sx_idata);
        return 1;
    case 0XA:
        // SLTI(C, src1, dest, sx_idata);
        return 1;
    case 0XB:
        // SLTIU(C, src1, dest, sx_idata);
        return 1;
    case 0XE:
        XORI(C, src1, dest, idata);
        return 1;
    case 0XD:
        ORI(C, src1, dest, idata);
        return 1;
    case 0X7:
        // BGTZ(C, src1, dest, sx_idata);
        return 1;
    case 0X6:
        // BLEZ(C, src1, dest, sx_idata);
        return 1;
    case 0X1:
        // BDECODER(C, src1, dest, sx_idata);
        return 1;
    default:
        return -12;
    }
}

void i_type_instructions::ADDI(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
{
    cerr << "ADDI" << endl;
    if (((C->reg[src1] < 0) && (sx_idata < 0) && (C->reg[src1] + sx_idata >= 0)) ||
        ((C->reg[src1] > 0) && (sx_idata > 0) && (C->reg[src1] + sx_idata <= 0)))
    {
        throw(static_cast<int>(ARITHMETIC_EXIT_CODE));
    }
    else
    {
        C->reg[dest] = C->reg[src1] + sx_idata;
        C->npc = C->npc + 1;
    }
}

void i_type_instructions::ADDIU(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
{
    cerr << "ADDIU" << endl;
    C->reg[dest] = static_cast<uint32_t>(static_cast<uint32_t>(C->reg[src1]) + sx_idata);
    C->npc = C->npc + 1;
}

void i_type_instructions::ANDI(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &idata)
{
    cerr << "ANDI" << endl;
    C->reg[dest] = C->reg[src1] & idata;
    C->npc = C->npc + 1;
}

void i_type_instructions::BEQ(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
{
    cerr << "BEQ" << endl;
    if (C->reg[src1] == C->reg[dest])
    {
        C->npc = C->npc + sx_idata;
    }
    else
    {
        C->npc = C->npc + 1;
    }
}
void i_type_instructions::BNE(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
{
    cerr << "BNE" << endl;
    if (C->reg[src1] != C->reg[dest])
    {
        C->npc = C->npc + sx_idata; // Supposedly 16bit shifted left (x4) but since our memory space is divided by 4, don't need to shift
    }
    else
    {
        C->npc = C->npc + 1;
    }
}
// void i_type_instructions::LBU(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "LBU" << endl; // Load byte unsigned LBU $1 16bOFFSET($2)
// }
// void i_type_instructions::LB(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "LB" << endl;
// }
// void i_type_instructions::LHU(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "LHU" << endl;
// }
// void i_type_instructions::LH(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "LH" << endl;
// }

void i_type_instructions::LUI(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &idata)
{
    cerr << "LUI" << endl;
    //Shift IDATA by 16 bits, load into RT (DEST). Exceptions: none.
    if (src1 == 0)
    {
        idata = idata << 16;
        C->reg[dest] = idata;
    }
    else
    {
        cerr << "src1 != 0. Making LUI invalid. Error not yet thrown.";
    }

    C->npc = C->npc + 1;
}
<<<<<<< HEAD

void i_type_instructions::LW(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
{
    cerr << "LW" << endl;
    ADDR_TYPE effective_address = src1+sx_idata;
    MEM_TYPE value;

    if(effective_address%4!=0){
        throw "Exception: Address Error Exception";
        exit(-11);
    }
    else{
        effective_address = effective_address >> 2;
        //if effective address = ADDR_GETC, call std::getchar (i.e. take first 8 bits of memory)
        if(effective_address==ADDR_GETC){
            //value = mem_block[effective_address]; //!!! MUST IMPLEMENT
            C->reg[dest] = value & 0x000000FF;
        }
        else{
            //C->reg[dest] = mem_block[effective_address]; //!! MUST IMPLEMENT
        }
    }

    C->npc = C->npc + 1;
}

=======
void i_type_instructions::LW(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
{
    cerr << "LW" << endl;
    INSTR_TYPE raw_mem_addr = sx_idata + C->reg[src1];
    INSTR_TYPE mem_addr = raw_mem_addr / 4;

    if ((mem_addr < 0) && ((raw_mem_addr & 0x3) != 0)) // If either of the two LSB of address are non-zero then throw exception
    {
        throw(static_cast<int>(MEMORY_EXIT_CODE));
    }
    else if (mem_addr == ADDR_GETC)
    {
        char input = read_char();
        uint32_t sx_input;
        if (input >> 15)
        {
            sx_input = 0xFFFFFF00 | input;
        }
        else
        {
            sx_input = input;
        }

        // C->[dest] =
    }
    else if (mem_addr == ADDR_PUTC)
    {
        char input;
        cin >> input;
        int32_t sx_input = static_cast<int32_t>(input);
        cout << sx_input << endl;
    }
    else
    {
    }
}
>>>>>>> develop
// void i_type_instructions::LWL(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "LWL" << endl;
// }
// void i_type_instructions::LWR(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "LWR" << endl;
// }
void i_type_instructions::ORI(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &idata)
{
    cerr << "ORI" << endl;
    C->reg[dest] = C->reg[src1] | idata;
    C->npc = C->npc + 1;
}
void i_type_instructions::XORI(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &idata)
{
    cerr << "XORI" << endl;
    C->reg[dest] = C->reg[src1] ^ idata;
    C->npc = C->npc + 1;
}
// void i_type_instructions::SLTI(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "SLTI" << endl;
// }
// void i_type_instructions::SLTIU(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "SLTIU" << endl;
// }
// void i_type_instructions::SB(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "SB" << endl;
// }
// void i_type_instructions::SH(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "SH" << endl;
// }
// void i_type_instructions::SW(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "SW" << endl;
// }
// void i_type_instructions::BGEZ(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &sx_idata)
// {
//     cerr << "BGEZ" << endl;
// }
// void i_type_instructions::BGEZAL(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &sx_idata)
// {
//     cerr << "BGEZAL" << endl;
// }
// void i_type_instructions::BGTZ(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "BGTZ" << endl;
// }
// void i_type_instructions::BLEZ(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "BLEZ" << endl;
// }
// void i_type_instructions::BLTZ(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &sx_idata)
// {
//     cerr << "BLTZ" << endl;
// }
// void i_type_instructions::BLTZAL(CPU *&C, INSTR_TYPE &src1, INSTR_TYPE &sx_idata)
// {
//     cerr << "BLTZAL" << endl;
// }
// void i_type_instructions::BDECODER(CPU *&S, INSTR_TYPE &src1, INSTR_TYPE &dest, INSTR_TYPE &sx_idata)
// {
//     cerr << "BDECODER" << endl;
// }
