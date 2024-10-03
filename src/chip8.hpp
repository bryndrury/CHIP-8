// CHIP8 CLASS

#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <random>

#define FONTSET_START_ADDRESS 0x50

class Chip8
{
public:
    Chip8();
    ~Chip8();

    enum class State {
        STOPPED,
        RUNNING,
        PAUSED
    };

    std::array<uint8_t, 80> fontset = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0x70, 0x10, 0x20, 0x40, 0xF0, // 2
        0xF0, 0x10, 0x20, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    // 2048 bytes for display memory
    std::array<uint8_t, 64 * 32> display_buffer;    // Graphics buffer
    std::array<uint8_t, 16> keypad;                 // Keypad buffer
    bool drawFlag;                                  // Flag to indicate the screen state

    State currentState = State::RUNNING;
    bool awaitingKeyPress = false;

    bool LoadROM(const std::string &filename);
    void EmulateCycle();

    
private:
    // Memory (4096 bytes)
    std::array<uint8_t, 4096> memory;               // 4KB of memory
    
    // Program Counter
    uint16_t program_counter;                       // 16-bit register

    // Address Register
    uint16_t I;                                     // Index Register (12-bits)

    // The Stack
    std::array<uint8_t, 16> stack;
    uint16_t stack_pointer;
    
    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Registers
    std::array<uint8_t, 8> V;                      // 16 8-bit registers

    uint16_t opcode;                                // 16-bit opcode


    // MASKS
    // ===============================
    const uint16_t mask_NNN = 0x0FFF;
    const uint16_t mask_NN  = 0x00FF;
    const uint16_t mask_N   = 0x000F;
    const uint16_t mask_VF  = 0x000F; // Also used for V
    const uint16_t mask_VX  = 0x0F00;
    const uint16_t mask_VY  = 0x00F0;
    const uint16_t mask_OP  = 0xF000;

    // OPCODES
    // ===============================
    void OP_0NNN(); // Calls routine at NNN (Not really needed)
    void OP_00E0(); // Clear the screen (Set all vals in graphics buffer to 0)
    void OP_00EE(); 
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void OP_4XNN();
    void OP_5XY0();
    void OP_6XNN();
    void OP_7XNN();
    void OP_8XY0();	
    void OP_8XY1();	
    void OP_8XY2();	
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();
    void OP_9XY0();	
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXNN();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();
};

#endif // CHIP8_HPP