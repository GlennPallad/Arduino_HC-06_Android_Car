package com.bitwitcher.bwbogie;

/**
	* This class defines the format of message between Android devices and
	* Bluetooth SPP devices.
	* The message format is composed of 14 bytes at most, 3 bytes at least as shown below:
	* 		Most:
	* 			|M|C|[-]|X|[.]|[X]|[X]|,|[-]|Y|[.]|[Y]|[Y]|"\n"|
	* 		Least:
	* 			|M|C|"\n"|
	* Where:
	* M = Mode Code, which can be
	* 		0 = Tracking Mode
	* 		1 = Avoidance Mode
	* 		2 = Free Mode
	* C = Command Code, which can be
	* 		0 = start
	* 		1 = stop
	* 		2 = disconnect
	* 		3 = factor
	* [-] = Minus sign, which only used under Free Mode(|M|C| = |2|3|).
	* , = Separator, separate X factor and Y factor.
	* X[.XX] and Y[.YY] are factors which only used under Free Mode(|M|C| = |2|3|),
	* 	the range of them is from -1.00 to 1.00.
	* 		X.XX = X factor
	* 		Y.YY = Y factor
	*/

public class Protocol {
	public static final String TRACKING = "0";
	public static final String AVOIDANCE = "1";
	public static final String FREE = "2";

	public static final String START = "0";
	public static final String STOP = "1";
	// public static final String DISCONNECT = "2"; /* Reserve for future development. */
	public static final String FACTOR = "3";

	public static final String T_STAR = TRACKING + START;
	public static final String T_STOP = TRACKING + STOP;
	// public static final String T_DISC = TRACKING + DISCONNECT;
	public static final String A_STAR = AVOIDANCE + START;
	public static final String A_STOP = AVOIDANCE + STOP;
	// public static final String A_DISC = AVOIDANCE + DISCONNECT;
	public static final String F_FACT = FREE + FACTOR;
	public static final String F_STOP = FREE + STOP;
	// public static final String F_DISC = FREE + DISCONNECT;
}
