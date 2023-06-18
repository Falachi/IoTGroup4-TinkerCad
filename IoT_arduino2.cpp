const int tempPin = A5;
const int forcePin = A4;
const int tiltPin = 6;

const int ledPin = 2;

bool breakStatus, tempStatus, tiltStatus = false;



void setup()
{
	Serial.begin(9600);
	delay(100);
	pinMode(tiltPin, INPUT);
	pinMode(ledPin, OUTPUT);
}

void loop()
{
	//configuring tilt sensor
	int tiltMotion;
	tiltMotion = digitalRead(tiltPin);
	checkTilt(tiltMotion);
	
	//configuring force sensor
	int forceReading;
	forceReading = analogRead(forcePin);
	checkForce(forceReading);
	
	//configuring temperature sensor
	double tempReading, tempCelcius;
	tempReading = analogRead(tempPin);
	tempCelcius = covertToCelcius(tempReading);
	checkTemp(tempCelcius);
	
	if (breakStatus or tempStatus or tiltStatus)
		digitalWrite(ledPin, HIGH);
	else
		digitalWrite(ledPin, LOW);
	
	String sendToMain = sensorStatus();
  if (sendToMain != "")
		Serial.println(sendToMain);
	
	delay(500);
}

String sensorStatus()
{
	String sensorStatus = "";
	if (breakStatus == true)
		sensorStatus += "breakSensor ";
	if (tempStatus == true)
		sensorStatus += "tempSensor ";
	if (tiltStatus == true)
		sensorStatus += "tiltSensor ";
	return sensorStatus;
}

void checkTilt(int tiltData)
{
	if (tiltData == 1)
		tiltStatus = true;
	else
		tiltStatus = false;
}

//force sensor functions
void checkForce(int forceReading)
{
	if (forceReading >= 425)
		breakStatus = true;
	else
		breakStatus = false;
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
	if (tempData >= 100.00)
		tempStatus = true;
	else
		tempStatus = false;
}