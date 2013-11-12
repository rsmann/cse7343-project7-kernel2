/* Kernel.c
 * Scott Mann
 * CSE 7343 - Project 7
 * A simple kernel to support printing a string to video, 
 * reading a line from the keyboard, and reading a sector from disk.
 */

/* Function Prototypes */
int getRegisterValue(char ah, char al);
void printString(char* message);
void readString(char* buffer);

/* Some pseudo-constants (since I'm unsure of K&R C requirements)
 * for use in calculations in other functions involving memory offsets
 */
char videoInterrupt = 0x10;
char inputInterrupt = 0x16;

char printCommand = 0xE;

char charBackspace = 0x8;
char charEnter = 0xD;
char charLineFeed = 0xA;
char charNull = 0x0;

/* The main entry point of the program */
int main()
{
	char inputBuffer[80];

	/* Step 1 requirement */
	printString("Hello World\r\n\0");
	
	/* Step 2 requirement */
	printString("Enter a line: \0");
	readString(inputBuffer);
	printString("\r\n\0");
	printString(inputBuffer);

	while (1) {}
}

/* Reusable function to calculate an integer value from a high byte and low byte*/
int getRegisterValue(char ah, char al)
{
	return ah * 256 + al;
}

/* Print a character array to the screen */
void printString(char* message)
{
	/* Grab the length of the message */
	int len = sizeof(message);
	char currentChar = '\0';
	int i = 0;

	/* If the message is empty, we're done. */
	if (len == 0) return;

	/* Read the first character to set up the loop */
	currentChar = message[i];

	/* Read each character and output it until we encounter \0 */
	while (currentChar != charNull)
	{
		interrupt(videoInterrupt, getRegisterValue(printCommand, currentChar), 0, 0, 0);
		i++;
		currentChar = message[i];
	}

	return;
}

/* Read a line from the keyboard */
void readString(char* buffer)
{
	char currentChar = '\0';
	int charIndex = 0;

	/* Ensure that the input buffer is not zero-length */
	if (sizeof(buffer) ==0)
	{
		printString("Zero-length buffer exception.\r\0");
		return;
	}

	do
	{
		/* Read the first character from the keyboard to set up the loop */
		/* Interrupt returns the ASCII code for the key pressed. */
		currentChar = interrupt(inputInterrupt, 0, 0, 0, 0);

		if (currentChar != charBackspace)
		{
			/* Output it to the screen */
			interrupt(videoInterrupt, getRegisterValue(printCommand, currentChar), 0, 0, 0);

			buffer[charIndex] = currentChar;
			charIndex++;
		}
		else if (currentChar == charBackspace)
		{
			if (charIndex > 0) charIndex--;
			{
				/* Clear the character from the screen instead of just backing the cursor up */
				interrupt(videoInterrupt, getRegisterValue(printCommand, charBackspace), 0, 0, 0);
				interrupt(videoInterrupt, getRegisterValue(printCommand, charNull), 0, 0, 0);
				interrupt(videoInterrupt, getRegisterValue(printCommand, charBackspace), 0, 0, 0);

				/* Clear the character from the buffer as well. */				
				buffer[charIndex] = charNull;
			}
		}

	} while (currentChar != charEnter);

	/* Append line feed and null to end of input */
	buffer[charIndex] = charLineFeed;
	buffer[charIndex + 1] = charNull;
}
