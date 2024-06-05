#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c1294ncpdt.h>

// LCD special commands
#define LCD_CLEAR_DISPLAY 0xFE          // 0x01
#define LCD_CLEAR_BACKLIGHT_CTRL 0x80   // 0: Turn of, 255:  100%
#define LCD_SET_BAUD_RATE 0x81          // 4: 9600bps, 10:  115200bps
#define LCD_CARRIAGE_RETURN 0x0D        //to set the cursor to the start of the new line
#define LCD_LINE_FEED 0x0A              //the string to print

// Function prototypes
void configUART2(void);
void configPortD(void);
void UART2_SendChar(char data);
void UART2_SendString(char *str);
void UART2_SendCommand(char cmd, char value);
char* getThirdRowData();
char* getFourthRowData();
void UART2_PrintRows(char* row1, char* row2, char* row3, char* row4);


void main(void) {

    configPortD(void);
    configUART2(void);

    // Constant string for the first row
    char teamName[] = "GoT It!";
    // Empty string for the second row
    char emptyRow[] = "                    ";

    // Main loop
    while(1) {
        // Print all four rows
        UART2_PrintRows(teamName, emptyRow, getThirdRowData(), getFourthRowData());

        // Delay for some time before updating the data again
        for(int i = 0; i < 1000000; i++); // Simple delay loop
    }
}


// Configuring UART2
void configUART2(void) {
    SYSCTL_RCGCUART_R |= 0x00000004;    // Switch on clock for UART2
    while(!(SYSCTL_PRUART_R & 0x04));
    UART2_CTL_R &= ~0x0001;             // Disable UART2 for configuration
    UART2_IBRD_R = 8;                   // set DIVINT of BRD (bit-rate 115200bit/s)
    UART2_FBRD_R = 44;                  // set DIVFRAC of BRD
    UART2_LCRH_R = 0x00000060;          // serial format 8N1
    UART2_CTL_R |= 0x0101;              // Enable UART2 (EN, TX)

}

// Configuring PortD
void configPortD(void) {
    SYSCTL_RCGCGPIO_R |= (1<<3);        // Enable clock for GPIO Port D (0x8)
    while((SYSCTL_PRGPIO_R & (1<<3)) == 0);
    GPIO_PORTD_AHB_DEN_R |= 0x20;       // Enable PD(5) for TX
    GPIO_PORTD_AHB_AFSEL_R |= 0x20;
    GPIO_PORTD_AHB_PCTL_R = 0x00010000;

}

void UART2_SendChar(char data) {
    while(UART2_FR_R & 0x20);            // Wait until TXFF is 0 (Transmit FIFO not full)
    UART2_DR_R = data;                   // Write data to UART2 data register
}

void UART2_SendString(char *str) {
    while(*str != '\0') {                  // Continue until the end of the string
          UART2_SendChar(*str);            // Send each character
          str++;
      }
}

// Special commands
void UART2_SendCommand(char cmd, char value) {
    UART2_SendChar(cmd);                // Command must be transmitted first followed by the desired value
    UART2_SendChar(value);
}

char* getThirdRowData() {
    static char buffer[20];
    buffer = "samsom";      // Replace with motor speed function call
    return buffer;
}

char* getFourthRowData() {
    static char buffer[20];
    buffer = "sama";        // Replace with direction function call
    return buffer;
}

// Print the rows to the LCD
void UART2_PrintRows(char* row1, char* row2, char* row3, char* row4) {
    UART2_SendCommand(LCD_CMD_CLEAR_DISPLAY, 0x01);                // Clear the display

    // Print the first row
   UART2_SendChar(LCD_CMD_CARRIAGE_RETURN);  // Move to the start of the first row
   UART2_SendString(row1);                   // Send the first row string

   // Print the second row
   UART2_SendChar(LCD_CMD_CARRIAGE_RETURN);  // Move to the start of the second row
   UART2_SendString(row2);                   // Send the second row string

   // Print the third row
   UART2_SendChar(LCD_CMD_CARRIAGE_RETURN);  // Move to the start of the third row
   UART2_SendString(row3);                   // Send the third row string

   // Print the fourth row
   UART2_SendChar(LCD_CMD_CARRIAGE_RETURN);  // Move to the start of the fourth row
   UART2_SendString(row4);                   // Send the fourth row string
   }

}


