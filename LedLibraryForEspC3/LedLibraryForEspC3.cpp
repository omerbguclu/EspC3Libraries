// LedLibraryForEspC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


#define DCON_SIZE 4
#define SEGMENT_SIZE 8
#define DIGIT_NUMBER 4
#define HIGH 1
#define LOW 0

#define ClearBit(a,b) (a &= ~(0x01 << b))
#define GetBit(a,b) ((a & (0x01 << b)) == (0x01 << b))
#define SetBit(a,b) (a |= (0x01 << b))

typedef struct LedPin {
    uint8_t Pin : 4;
    uint8_t State : 1;
    uint8_t BlinkState : 1;
    uint8_t Reserved : 2;
}LedPin;

typedef struct SevenSegment {
    LedPin Led[8];
}SevenSegment;

/* ----------- BEGIN SEVEN SEGMENT SYMBOLS  ---------- */
typedef enum ata_SevenSegmentSymbols {
    SEV_SEG_DIGIT_0,
    SEV_SEG_DIGIT_1,
    SEV_SEG_DIGIT_2,
    SEV_SEG_DIGIT_3,
    SEV_SEG_DIGIT_4,
    SEV_SEG_DIGIT_5,
    SEV_SEG_DIGIT_6,
    SEV_SEG_DIGIT_7,
    SEV_SEG_DIGIT_8,
    SEV_SEG_DIGIT_9,
    SEV_SEG_BLANK_SEGMENT,
    SEV_SEG_LETTER_S,
    SEV_SEG_LETTER_C,
    SEV_SEG_LETTER_F,
    SEV_SEG_LETTER_d,
    SEV_SEG_LETTER_c,
    SEV_SEG_LETTER_E,
    SEV_SEG_LETTER_H,
    SEV_SEG_LETTER_L,
    SEV_SEG_LETTER_r,
    SEV_SEG_LETTER_P,
    SEV_SEG_LETTER_o,
    SEV_SEG_LETTER_0,
    SEV_SEG_LETTER_U,
    SEV_SEG_LETTER_n,
    SEV_SEG_LETTER_t,
    SEV_SEG_LETTER_I,
    SEV_SEG_LETTER_u,
    SEV_SEG_LETTER_G,
    SEV_SEG_LETTER_Y,
    SEV_SEG_LETTER_A,
    SEV_SEG_LETTER_b,
    SEV_SEG_SIGN_DASH,
    SEV_SEG_SEMI_PLUS_LEFT,
    SEV_SEG_SEMI_PLUS_RIGHT,
    SEV_SEG_LAST
}ata_SevenSegmentSymbols;
/* ------------ END SEVEN SEGMENT SYMBOLS  ----------- */

const unsigned char ata_SevenSegmentSymbolArray[SEV_SEG_LAST] = {
    0b0111111, // 0               -- SEV_SEG_DIGIT_0
    0b0000110, // 1               -- SEV_SEG_DIGIT_1
    0b1011011, // 2               -- SEV_SEG_DIGIT_2
    0b1001111, // 3               -- SEV_SEG_DIGIT_3
    0b1100110, // 4               -- SEV_SEG_DIGIT_4
    0b1101101, // 5               -- SEV_SEG_DIGIT_5
    0b1111101, // 6               -- SEV_SEG_DIGIT_6
    0b0000111, // 7               -- SEV_SEG_DIGIT_7
    0b1111111, // 8               -- SEV_SEG_DIGIT_8
    0b1101111, // 9               -- SEV_SEG_DIGIT_9
    0b0000000, // Blank segment   -- SEV_SEG_BLANK_SEGMENT
    0b1101101, // s letter        -- SEV_SEG_LETTER_S
    0b0111001, // C letter        -- SEV_SEG_LETTER_C
    0b1110001, // F letter        -- SEV_SEG_LETTER_F
    0b1011110, // d letter        -- SEV_SEG_LETTER_d
    0b1011000, // c letter        -- SEV_SEG_LETTER_c
    0b1111001, // E letter        -- SEV_SEG_LETTER_E
    0b1110110, // H letter        -- SEV_SEG_LETTER_H
    0b0111000, // L letter        -- SEV_SEG_LETTER_L
    0b1010000, // r letter        -- SEV_SEG_LETTER_r
    0b1110011, // P letter        -- SEV_SEG_LETTER_P
    0b1011100, // o letter        -- SEV_SEG_LETTER_o
    0b0111111, // 0 letter        -- SEV_SEG_LETTER_0
    0b0111110, // U letter        -- SEV_SEG_LETTER_U
    0b1010100, // n letter        -- SEV_SEG_LETTER_n
    0b1111000, // t letter        -- SEV_SEG_LETTER_t
    0b0110000, // I letter        -- SEV_SEG_LETTER_I
    0b0011100, // u letter        -- SEV_SEG_LETTER_u
    0b1111101, // G letter        -- SEV_SEG_LETTER_G
    0b1101110, // Y letter        -- SEV_SEG_LETTER_Y
    0b1110111, // A letter        -- SEV_SEG_LETTER_A
    0b1111100, // b letter        -- SEV_SEG_LETTER_b
    0b1000000, // dash            -- SEV_SEG_SIGN_DASH
    0b1110000, // semi plus left  -- SEV_SEG_SEMI_PLUS_LEFT
    0b1000110  // semi plus right -- SEV_SEG_SEMI_PLUS_RIGHT
};

// class for driving 4 digit 7 segment display
class Led {
    SevenSegment m_digits[DIGIT_NUMBER];
    uint8_t m_dcons[DCON_SIZE];
    uint8_t m_segments[SEGMENT_SIZE];
public:
    Led() {}
    Led(uint8_t dcon1, uint8_t dcon2, uint8_t dcon3, uint8_t dcon4,
        uint8_t seg1, uint8_t seg2, uint8_t seg3, uint8_t seg4,
        uint8_t seg5, uint8_t seg6, uint8_t seg7, uint8_t seg8)
    {
        m_dcons[0] = dcon1;
        m_dcons[1] = dcon2;
        m_dcons[2] = dcon3;
        m_dcons[3] = dcon4;
        m_segments[0] = seg1;
        m_segments[1] = seg2;
        m_segments[2] = seg3;
        m_segments[3] = seg4;
        m_segments[4] = seg5;
        m_segments[5] = seg6;
        m_segments[6] = seg7;
        m_segments[7] = seg8;
    }
    void Init();
    void Set(uint8_t index, uint8_t value);
    void Blink(uint8_t index, uint8_t value);
    void WriteToSevenSegment(SevenSegment* strptr, ata_SevenSegmentSymbols symbol);
    SevenSegment* GetDigit(uint8_t digit);
    void Scan();
private:
    void ResetDcons();
};


void Led::ResetDcons()
{
    memset(m_dcons, HIGH, DCON_SIZE);
}

void Led::Scan()
{
    for (size_t dcon = 0; dcon < DCON_SIZE; dcon++)
    {
        ResetDcons();
        m_dcons[dcon] = LOW;
        for (size_t segment = 0; segment < SEGMENT_SIZE; segment++)
        {
            m_segments[segment] = m_digits[dcon].Led[segment].State;
        }
    }
}

void Led::WriteToSevenSegment(SevenSegment* strptr, ata_SevenSegmentSymbols symbol)
{
    unsigned char state = ata_SevenSegmentSymbolArray[symbol];
    strptr->Led[0].State = GetBit(state, 0);
    strptr->Led[1].State = GetBit(state, 1);
    strptr->Led[2].State = GetBit(state, 2);
    strptr->Led[3].State = GetBit(state, 3);
    strptr->Led[4].State = GetBit(state, 4);
    strptr->Led[5].State = GetBit(state, 5);
    strptr->Led[6].State = GetBit(state, 6);
}

SevenSegment* Led::GetDigit(uint8_t digit)
{
    if (digit >= 0 && digit < DIGIT_NUMBER)
        return &m_digits[digit];
    return nullptr;
}

int main()
{
    Led ss(1,2,3,4,1,2,3,4,5,6,7,8);
    ss.WriteToSevenSegment(ss.GetDigit(0), SEV_SEG_DIGIT_1);
    ss.Scan();
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
