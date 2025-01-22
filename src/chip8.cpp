#include "chip8.hpp"

Chip8::Chip8()
{
    program_counter = 0x200; 
    opcode          = 0;
    I               = 0;
    stack_pointer   = 0;

    // Clear graphics buffer
    OP_0NNN();
    drawFlag = false;

    // Clear stack, keypad, V registers, memory
    stack.fill(0);
    keypad.fill(0);
    V.fill(0);
    memory.fill(0);

    // Load fontset into first 80 bytes of memory
    for (int i = FONTSET_START_ADDRESS; i < 80; i++)
    {
        memory[i] = fontset[i];
    }

    // Start timers
    delay_timer = 0;
    sound_timer = 0;

    currentState = State::RUNNING;
}

Chip8::~Chip8()
{
    currentState = State::STOPPED;
}

bool Chip8::LoadROM(const std::string &filename)
{
    // Open the file
    // ...
    (void) filename;
    return true;
}

void Chip8::EmulateCycle()
{
    if (currentState == State::PAUSED | currentState == State::PAUSED)
        return; 

    // As the opcode will be two bytes shift by another 8 bits
    opcode = memory[program_counter] << 8 | memory[program_counter + 1] ;

    // std::cout << program_counter << std::endl;
    // std::cout << opcode << std::endl;

    switch (opcode)
    {
    case 0x0000:
        OP_0NNN();
        break;
    case 0x00E0:
        OP_00E0();
        break;
    case 0x00EE:
        OP_00EE();
        break;
    case 0x1000:
        OP_1NNN();
        break;
    case 0x2000:
        OP_2NNN();
        break;
    case 0x3000:
        OP_3XNN();
        break;
    case 0x4000:
        OP_4XNN();
        break;
    case 0x5000:
        OP_5XY0();
        break;
    case 0x6000:
        OP_6XNN();
        break;
    case 0x7000:
        OP_7XNN();
        break;
    case 0x8000:
        OP_8XY0();
        break;
    case 0x8001:
        OP_8XY1();
        break;
    case 0x8002:
        OP_8XY2();
        break;
    case 0x8003:
        OP_8XY3();
        break;
    case 0x8004:
        OP_8XY4();
        break;
    case 0x8005:
        OP_8XY5();
        break;
    case 0x8006:
        OP_8XY6();
        break;
    case 0x8007:
        OP_8XY7();
        break;
    case 0x800E:
        OP_8XYE();
        break;
    case 0x9000:
        OP_9XY0();
        break;
    case 0xA000:
        OP_ANNN();
        break;
    case 0xB000:
        OP_BNNN();
        break;
    case 0xC000:
        OP_CXNN();
        break;
    case 0xD000:
        OP_DXYN();
        break;
    case 0xE09E:
        OP_EX9E();
        break;
    case 0xE0A1:
        OP_EXA1();
        break;
    case 0xF007:
        OP_FX07();
        break;
    case 0xF00A:
        OP_FX0A();
        break;
    case 0xF015:
        OP_FX15();
        break;
    case 0xF018:
        OP_FX18();
        break;
    case 0xF01E:
        OP_FX1E();
        break;
    case 0xF029:
        OP_FX29();
        break;
    case 0xF033:
        OP_FX33();
        break;
    case 0xF055:
        OP_FX55();
        break;
    case 0xF065:
        OP_FX65();
        break;
    default:
        std::cout << "Invalid Opcode\n"; 
        break;
    }
    sound_timer = 1;
    
    // Manage Timers
    if (delay_timer > 0) --delay_timer;     // Decrement delay timer (if zero stop)
    if (sound_timer > 0)
    {
        if (sound_timer == 1)
        {
            soundFlag = true;
        }
        --sound_timer;
    }
}

void Chip8::OP_0NNN()
{
    // std::cout << "Hi there, you shouldn't be here!\n\n";
    program_counter += 2;
}

void Chip8::OP_00E0() // Clear the display buffer
{
    display_buffer.fill(0);
    drawFlag = true;
    program_counter += 2;
}

void Chip8::OP_00EE() // Return from Subroutine
{
    --stack_pointer; 
    program_counter = stack[stack_pointer];

    program_counter += 2;
} 

void Chip8::OP_1NNN() // Jump to address NNN
{
    program_counter = opcode & mask_NNN;
}

void Chip8::OP_2NNN() // Call the subroutine at NNN
{
    stack[ stack_pointer ] = program_counter;
    ++stack_pointer;
    OP_1NNN();
}

void Chip8::OP_3XNN() // If VX == NN skip next instruction
{
    program_counter += ( V[ (opcode & mask_VX) >> 8 ] == (opcode & mask_NN) ) ? 4 : 2;
}

void Chip8::OP_4XNN() // If Vx != NN skip next instruction
{
    program_counter += (V[ (opcode & mask_VX) >> 8 ] != (opcode & mask_NN)) ? 4 : 2;
}

void Chip8::OP_5XY0() // If Vx == Vy skip next instruction
{   
    program_counter += (V[ (opcode & mask_VX) >> 8 ] == V[ (opcode & mask_VY) >> 4 ]) ? 4 : 2;
}

void Chip8::OP_6XNN() // Set VX to NN
{
    V[ (opcode & mask_VX) >> 8 ] = (opcode & mask_NN);

    program_counter += 2;
}

void Chip8::OP_7XNN() // Add NN to VX (carrying flag is not changed)
{
    V[ (opcode & mask_VX) >> 8 ] += (opcode & mask_NN);

    program_counter += 2;
}

void Chip8::OP_8XY0() // Set VX to the value of VY
{
    V[ (opcode & mask_VX) >> 8 ] = V[ (opcode & mask_VY) >> 4 ];

    program_counter += 2;
}	

void Chip8::OP_8XY1() // Set VX to VX OR VY (bitwise OR)
{
    V[ (opcode & mask_VX) >> 8 ] |= V[ (opcode & mask_VY) >> 4 ];

    program_counter += 2; 
}	

void Chip8::OP_8XY2() // Set VX to VX AND VY (bitwise AND)
{
    V[ (opcode & mask_VX) >> 8 ] &= V[ (opcode & mask_VY) >> 4 ];

    program_counter += 2;
}	

void Chip8::OP_8XY3() // Set VX to VX XOR VY (bitwise XOR)
{
    V[ (opcode & mask_VX) >> 8 ] ^= V[ (opcode & mask_VY) >> 4 ];

    program_counter += 2;
}

void Chip8::OP_8XY4() // Adds VY to VX. VF is the overflow flag
{
    uint8_t VX = (opcode & mask_VX) >> 8;
    uint8_t VY = (opcode & mask_VY) >> 4;

    uint16_t sum = V[VX] + V[VY];

    V[mask_VF] = (sum > mask_NN) ? 1 : 0;

    V[VX] = sum & mask_NN;

    program_counter += 2;
}

void Chip8::OP_8XY5() // VY is sub-ed fom VX. VF is the underflow flag
{
    uint8_t VX = (opcode & mask_VX) >> 8;
    uint8_t VY = (opcode & mask_VY) >> 4;

    V[mask_VF] = (V[VX] >= V[VY]) ? 1 : 0;
    V[VX] -= V[VY];

    program_counter += 2;
}

void Chip8::OP_8XY6() // Shifts VX right by 1, then stores the least sig bit into VF
{
    uint8_t VX = (opcode & mask_VX) >> 8;
    V[mask_VF] = V[VX] & 0x1; // least sig bit in VF
    V[VX] >>= 1;

    program_counter += 2;
}

void Chip8::OP_8XY7() // Set VX to VY - VX. VF is the underflow flag
{
    uint8_t VX = (opcode & mask_VX) >> 8;
    uint8_t VY = (opcode & mask_VY) >> 4;

    V[mask_VF] = (V[VY] > V[VX]) ? 1 : 0;
    V[VX] = V[VY] - V[VX]; 

    program_counter += 2;
}

void Chip8::OP_8XYE() // Shift VX left 1, then stores the most sig bit into VF
{
    uint8_t VX = (opcode & mask_VX) >> 8;
    V[mask_VF] = (V[VX] & 0x80) >> 7; // most sig bit in VF
    V[VX] <<= 1;

    program_counter += 2;
}

void Chip8::OP_9XY0() // If VX != VY skip next instruction 
{
    program_counter += (V[ (opcode & mask_VX) >> 8 ] != V[ (opcode & mask_VY) >> 4 ]) ? 4 : 2;
}	

void Chip8::OP_ANNN() // Set I to NNN
{
    I = opcode & mask_NNN;

    program_counter += 2;
}

void Chip8::OP_BNNN() // Jump to NNN plus V0
{
    program_counter = (opcode & mask_NNN) + V[0];
}

void Chip8::OP_CXNN() // Sets VX to rand
{
    V[ (opcode & mask_VX) >> 8 ] = rand() & (opcode & mask_NN);

    program_counter += 2;
}

void Chip8::OP_DXYN() // Draw Sprite at (VX, VY)
{
    uint8_t x = V[ (opcode & mask_VX) >> 8 ];
    uint8_t y = V[ (opcode & mask_VY) >> 4 ];
    uint8_t height = opcode & mask_N;
    uint8_t pixel;

    V[mask_VF] = 0;

    for (int y_line = 0; y_line < height; y_line++)
    {
        pixel = memory[I + y_line];
        for (int x_line = 0; x_line < 8; x_line++)
        {
            if (display_buffer[(x + x_line + ((y+ y_line) * 64))] == 1)
            {
                V[mask_VF] = 1;
            }
            display_buffer[x + x_line + ((y + y_line) * 64)] ^= 1;
        }
    }

    drawFlag = true;
    program_counter += 2;
}

void Chip8::OP_EX9E() // If Key stored in VX is pressed, skip next instruction
{
    uint8_t key = V[ (opcode & mask_VX) >> 8 ];
    program_counter += (keypad[key] != 0) ? 4 : 2;
}

void Chip8::OP_EXA1() // If Key stored in VX is not pressed, skip next instruction
{
    uint8_t key = V[ (opcode & mask_VX) >> 8 ];
    program_counter += (keypad[key] == 0) ? 4 : 2;
}

void Chip8::OP_FX07() // Set VX to the delay timer
{
    V[ (opcode & mask_VX) >> 8 ] = delay_timer;
    program_counter += 2;
}

void Chip8::OP_FX0A() // Wait until keypresss then store in VX
{
    // Check if keypad has a press
    bool key_pressed = false;
    for (uint8_t i = 0; i < 16; i++)
    {
        if (keypad[i] != 0)
        {
            V[ (opcode & mask_VX) >> 8 ] = i;
            key_pressed = true;
            break;
        }
    }

    if (!key_pressed)
        return;

    program_counter += 2;
}

void Chip8::OP_FX15() // Set the delay timer to VX
{
    delay_timer = V[ (opcode & mask_VX) >> 8 ];

    program_counter += 2;
}

void Chip8::OP_FX18() // Set the sound timer to VX
{
    sound_timer = V[ (opcode & mask_VX) >> 8 ];

    program_counter += 2;
}

void Chip8::OP_FX1E() // Add VX to I
{
    I += V[ (opcode & mask_VX) >> 8 ];

    program_counter += 2;
}

void Chip8::OP_FX29() // Set I to location for sprite for char VX.
{
    uint8_t char_ = V[ (opcode & mask_VX) >> 8 ];
    I = FONTSET_START_ADDRESS + (char_ * 5);

    program_counter += 2;
}

void Chip8::OP_FX33() 
{
    // Stores the binary-coded decimal representation of VX, 
    // with the hundreds digit in memory at location in I, 
    // the tens digit at location I+1, 
    // and the ones digit at location I+2.
    uint8_t value = V[ (opcode & mask_VX) >> 8 ];
    memory[I] = value / 100;
    memory[I + 1] = (value / 10) % 10;
    memory[I + 2] = value % 10;

    program_counter += 2;
}

void Chip8::OP_FX55() // Store V0 to VX in memory starting at adress I
{
    uint8_t VX = (opcode & mask_VX) >> 8;
    for (uint8_t i = 0; i <= VX; i++)
    {
        memory[I + i] = V[i];
    }
    program_counter += 2;
}

void Chip8::OP_FX65() // Fill V0 to VX will values from memory from address I 
{
    uint8_t VX = (opcode & mask_VX) >> 8;
    for (uint8_t i = 0; i <= VX; i++)
    {
        V[i] = memory[I + i];
    }
    program_counter += 2;
}