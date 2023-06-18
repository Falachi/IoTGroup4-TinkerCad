#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

const int resetButton = 4;
const int nextButton = 3;
const int neoStrip = 13;

LiquidCrystal lcd(7,8,9,10,11,12);
Adafruit_NeoPixel strip(6, neoStrip, NEO_GRB + NEO_KHZ800);

const int ultraPin = 2;
const int pirPin = 5;
const int tiltPin = 6;
const int forcePin = A5;
const int tempPin = A4;

//safe = false, not safe = true
bool distanceStatus = false;
bool distancePirStatus = false;
bool breakStatus = false;
bool tempStatus = false;
bool tiltStatus = false;
bool trackStatus = false;
bool dangerStatus = false;

String fromArduino2;
bool tempStatus2 =false;
bool tiltStatus2 = false;
bool breakStatus2 = false;

bool wasInDanger = false;

void checkDanger()
{
	if (distanceStatus == true or distancePirStatus == true or breakStatus == true 
		or tempStatus == true or tiltStatus == true or tempStatus2 == true 
		or tiltStatus2 == true or breakStatus2 == true)
	{
		dangerStatus = true;
	}
	else
	{
		dangerStatus = false;
		wasInDanger = true;
	}
}

void readArduino()
{
	if (Serial.available())
	{
		fromArduino2 = Serial.readStringUntil('\n');
		delay(4);
		handleSensorStatus(fromArduino2);
	}
}

//some nonsense to split the received message by spaces so it can be processed
void handleSensorStatus(String message) 
{
	while (message.length() > 0) 
	{
		int spaceIndex = message.indexOf(' ');
		if (spaceIndex != -1)
		{
			String sensorLabel = message.substring(0, spaceIndex);
			processSensorLabel(sensorLabel);
			message = message.substring(spaceIndex + 1);
		} 
		else 
		{
			processSensorLabel(message);
			message = "";
		}
	}
}

//a handler to process if a string exist within the received string
void processSensorLabel(String sensorLabel) 
{
	if (sensorLabel.equals("breakSensor")) 
	{
		breakStatus2 = true;
	} 
	else if (sensorLabel.equals("tempSensor")) 
	{
		tempStatus2 = true;
	} 
	else if (sensorLabel.equals("tiltSensor")) 
	{
		tiltStatus2 = true;
	}
}

typedef void (*DisplayFunc)();

// Define the functions in the desired order
DisplayFunc displayFuncs[] = {
  displayUltrasonic,
  displayPir,
  displayTilt,
  displayForce,
  displayTemp
  // Add the other sensor information functions in the desired order
};

const int numFuncs = sizeof(displayFuncs) / sizeof(displayFuncs[0]);
int currentFuncIndex = 0;
int latestIndex = -1;

//ultrasonic functions
long convertToCentimeter(long duration)
{
	return duration / 29 / 2;
}

void checkUltrasonic(long distance)
{
	if(distance <= 212)
		distanceStatus = true;
	else
		distanceStatus = false;
}

void displayUltrasonic()
{
	if (distanceStatus == true)
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Ultsonic Status:");
		lcd.setCursor(0,1);
		lcd.print("OBJ INC!!! BREAK");
	}
	else
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Ultsonic Status:");
		lcd.setCursor(0,1);
		lcd.print("Good. No obj");
	}
}

//fucntions for pir sensor

void checkPir(int data)
{
	if (data == HIGH)
		distancePirStatus = false;
	else
		distancePirStatus = true;
}

void displayPir()
{
	if (distancePirStatus == true)
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("PIR Status:");
		lcd.setCursor(0,1);
		lcd.print("OBJ INC!! BREAK");
	}
	else
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("PIR Status:");
		lcd.setCursor(0,1);
		lcd.print("Good. No obj");
	}
}

//tilt sensor functions

void checkTilt(int tiltData)
{
	if (tiltData == 1)
		tiltStatus = true;
	else
		tiltStatus = false;
}

void displayTilt()
{
	if (tiltStatus == true or tiltStatus2 == true)
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Tilt Status:");
		lcd.setCursor(0,1);
		lcd.print("Tilt Detected!!!");
	}
	else
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Tilt Status:");
		lcd.setCursor(0,1);
		lcd.print("No tilt detected");
	}
}

//force sensor functions
void checkForce(int forceReading)
{
	if (forceReading >= 425)
		breakStatus = true;
	else
		breakStatus = false;
}

void displayForce()
{
	if (breakStatus == true or breakStatus2 == true)
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Break Status:");
		lcd.setCursor(0,1);
		lcd.print("FORCE TOO HIGH");
	}
	else
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Break Status:");
		lcd.setCursor(0,1);
		lcd.print("Good");
	}
}

// temperature sensor function
double covertToCelcius(double reading)
{
	double celcius = 0;
	celcius = ((reading * (5.0 / 1023.0)) - 0.5) * 100;
	return celcius;
}

void checkTemp(double tempData)
{
	if (tempData >= 100)
		tempStatus = true;
	else
		tempStatus = false;
}

void displayTemp()
{
	if (tempStatus == true or tempStatus2 == true)
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Temp Status:");
		lcd.setCursor(0,1);
		lcd.print("TEMP HIGH");
	}
	else
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Temp Status:");
		lcd.setCursor(0,1);
		lcd.print("Good");
	}
}

//updating the NeoPixel based on danger (red = danger, green = no danger)
void updateStrip()
{
	if (distanceStatus == true)
		strip.setPixelColor(0, 255, 0, 0);
	else
		strip.setPixelColor(0, 0, 255, 30);
	if (distancePirStatus == true)
		strip.setPixelColor(1, 255, 0, 0);
	else
		strip.setPixelColor(1, 0, 255, 30);
	if (tiltStatus == true or tiltStatus2 == true)
		strip.setPixelColor(2, 255, 0, 0);
	else
		strip.setPixelColor(2, 0, 255, 30);
	if (breakStatus == true or breakStatus2 == true)
		strip.setPixelColor(3, 255, 0, 0);
	else
		strip.setPixelColor(3, 0, 255, 30);
	if (tempStatus == true or tempStatus2 == true)
		strip.setPixelColor(4, 255, 0, 0);
	else
		strip.setPixelColor(4, 0, 255, 30);
	strip.show();
}

void currentFuncIndexCounter()
{
	currentFuncIndex += 1;
	if (currentFuncIndex == 6)
		currentFuncIndex = 0;
}

//next button stuff
void nextFunction()
{
	switch (currentFuncIndex)
	{
		case 0:
		displayUltrasonic();
		break;
		case 1:
		displayPir();
		break;
		case 2:
		displayTilt();
		break;
		case 3:
		displayForce();
		break;
		case 4:
		displayTemp();
		break;
	}
}

void lcdOverride()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("DANGER");
	lcd.setCursor(0,1);
	lcd.print("DETECTED");
}

int resetStatus = 0;

void changeResetStatus()
{
	if (resetStatus == 0)
		resetStatus = 1;
	else
		resetStatus = 0;
}
void setup()
{
	Serial.begin(9600);
	Serial.setTimeout(50);
	delay(100);
	lcd.begin(16,2);
	lcd.clear();
	strip.begin();
	strip.show();
	pinMode(resetButton, INPUT_PULLUP);
	pinMode(nextButton, INPUT_PULLUP);
	Serial.println("LCD, Strip and Button are set");
	
	pinMode(ultraPin, INPUT);
	pinMode(pirPin, INPUT);
	pinMode(tiltPin, INPUT);
	Serial.println("Input sensors are set");
	
	lcd.print("Starting");
	lcd.setCursor(0,1);
	lcd.print("System");
	delay(1000);
	lcd.clear();
	
	for (int pixelIndex = 0; pixelIndex < 6; pixelIndex++)
	{
		strip.setPixelColor(pixelIndex, 0, 255, 30);
		strip.show();
		delay(20);
		strip.setPixelColor(pixelIndex, 0, 0, 0);
		strip.show();
	}
	Serial.println("End Setup");
}

void loop()
{
	//configuring PIR sensor
	int pir;
	pir = digitalRead(pirPin);
	checkPir(pir);
	
	//configuring tilt sensor
	int tiltMotion;
	tiltMotion = digitalRead(tiltPin);
	checkTilt(tiltMotion);
	
	//configuring force sensor
	int forceReading;
	forceReading = analogRead(forcePin);
	checkForce(forceReading);
	Serial.println(breakStatus);
	
	//configuring temperature sensor
	double tempReading, tempCelcius;
	tempReading = analogRead(tempPin);
	tempCelcius = covertToCelcius(tempReading);
	checkTemp(tempCelcius);
	
	//configuring the ultrasonic sensor to send a pulse.
	long duration;
	pinMode(ultraPin, OUTPUT);
	digitalWrite(ultraPin, LOW);
	delayMicroseconds(2);
	digitalWrite(ultraPin, HIGH); //sending a pulse for 5ms
	delayMicroseconds(5);
	digitalWrite(ultraPin, LOW);
	//configure ultrasonic sensor to detect the echo
	pinMode(ultraPin, INPUT);
	duration = pulseIn(ultraPin, HIGH);
	checkUltrasonic(convertToCentimeter(duration));
	
	//read 2nd arduino
	tempStatus2 = false;
	tiltStatus2 = false;
	breakStatus2 = false;
	readArduino();
	delay(1000);
	//update strip
	updateStrip();
	//update danger status
	checkDanger();
	//lcd stuff
	int nextPress = digitalRead(nextButton);
	if (nextPress == LOW)
		currentFuncIndexCounter();
	if (latestIndex != currentFuncIndex or wasInDanger == true or dangerStatus == true)
	{
		nextFunction();
		wasInDanger = false;
	}
	//check for LCD override
	int resetPress = digitalRead(resetButton);
	if (resetPress == LOW)
		changeResetStatus();
	
	if (resetStatus == 0)
	{
		while (dangerStatus)
		{
			lcdOverride();
			resetPress = digitalRead(resetButton);
			if (resetPress == LOW)
				changeResetStatus();
			if (resetPress == 0){
				lcd.clear();
				currentFuncIndex = 0;
				nextFunction();
				dangerStatus = false;
			}
		}
	}
	else
		dangerStatus = false;
	Serial.println(breakStatus);
	
	latestIndex = currentFuncIndex;
}