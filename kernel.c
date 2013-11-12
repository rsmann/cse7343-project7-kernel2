/* Kernel.c
 * Scott Mann
 * CSE 7343 - Project 7
 * A simple kernel to support printing a string to video, 
 * reading a line from the keyboard, and reading a sector from disk.
 */

/* Function Prototypes */
void printString(char* message);
int getRegisterValue(char ah, char al);

/* Some pseudo-constants (since I'm unsure of K&R C requirements)
 * for use in calculations in other functions involving memory offsets
 */
char videoInterrupt = 0x10;
char printCommand = 0xE;

/* The main entry point of the program */
int main()
{
	printString("Hello World\r\n\0");
	
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
	while (currentChar != '\0')
	{
		interrupt(videoInterrupt, getRegisterValue(printCommand, currentChar), 0, 0, 0);
		i++;
		currentChar = message[i];
	}

	return;
}
