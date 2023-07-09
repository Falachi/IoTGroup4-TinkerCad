#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
const int MAXSTACK = 5;

//defining the input pins
//track detection
const int trackDetectionPin = 6;
//preventive stuff
const int imuTiltPin = 5;
const int imuAccPin = A3;
const int forcePin = A5;
const int tempEnginePin = A4;
//track health
const int trackTiltPin = 4;
const int trainVibrationPin = A2;
const int wheelTempPin = A1;

//defining the output pins
const int neoStrip = 13;
LiquidCrystal lcd(7,8,9,10,11,12);
Adafruit_NeoPixel strip(8, neoStrip, NEO_GRB + NEO_KHZ800);

/*
the structure where the overall status of the train is kept. 
It'll be kept in a stack where MAX is 5.
*/
// struct allData {
//     //health stuff
//     int tilt; //"good" "bad" "okay"
//     double acceleration;
//     double weight;
//     double engineTemp;
//     double wheelTemp;
//     //track
//     int trackStatus; //"good" "bad" "okay"
//     //next data
//     allData* next;
// };
// //a function to remove the last node of the stack.
// void removeLast(allData* head) {
//     if (head->next == NULL)
//         Serial.print("Last node. Abort");
//     else {
//         while(head->next->next)
//             head = head->next;
//         delete head->next;
//         head->next = NULL;
//     }
// }
// //a function to check if the stack is full or not
// void checkFull(allData* head) {
//     int count = 0;
//     allData* tempHead = head;
//     while(tempHead != nullptr) {
//         count++;
//         tempHead = tempHead->next;
//     }
//     if (count >= MAXSTACK)
//         removeLast(head);
// }
// //a function to add a node, which will become the new head
// void addData(allData** headPtr, int tiltInput, double accelerationInput,
// double inputWeight, double inputEngineTemp, int inputTrackStatus, double inputWheelTemp) {
//     checkFull(*headPtr);
//     allData* newData = new allData;
//     newData->tilt = tiltInput;
//     newData->acceleration = accelerationInput;
//     newData->weight = inputWeight;
//     newData->engineTemp = inputEngineTemp;
//     newData->trackStatus = inputWheelTemp;
//     newData->wheelTemp = inputWheelTemp;
//     newData->next = *headPtr;
//     *headPtr = newData;
// }
// //a function to print the stack
// void printStack(allData* head, int index) {
//     allData* tmpHead = head;
//     Serial.println("-----------Train Data------------");
//     while(tmpHead) {
//         Serial.print("Train Tilt: ");
//         if (tmpHead->tilt == 0)
//             Serial.println("Good");
//         else if (tmpHead->tilt == 1)
//             Serial.println("Okay");
//         else if (tmpHead->tilt == 2)
//             Serial.println("Bad");
//         Serial.print("Train Acceleration: ");
//         Serial.println(tmpHead->acceleration);
//         Serial.print("Train Weight: ");
//         Serial.println(tmpHead->weight);
//         Serial.print("Engine Temperature: ");
//         Serial.println(tmpHead->engineTemp);
//         Serial.print("Track Status: ");
//         if (tmpHead->trackStatus == 0)
//             Serial.println("Good");
//         else if (tmpHead->trackStatus == 1)
//             Serial.println("Okay");
//         else if (tmpHead->trackStatus == 2)
//             Serial.println("Bad");
//         Serial.println("---------------------------------");
//         tmpHead = tmpHead->next;
//         index++;
//     }
    
// }

//track detection function - ultrasonic
int obstacleCheck = 0; //a 3 class variable where 0 = no obstacle, 1 = obstacle incoming, 2 = obstacle with auto brake
double convertToCentimeter(double duration) {
	return duration / 29 / 2;
}
void checkUltrasonic(double distance)
{
    if (distance <= 262)
        obstacleCheck = 1;
	else
		obstacleCheck = 0;
	if(distance <= 212)
		obstacleCheck = 2;
    
}

//preventive 
//tilt pin functions
int tiltCounter = 0;
int tiltStatus = 0;
int addToCounter(int number) {
    if (number == 5)
        return number;
    return number + 1;
}
void checkTilt(int tiltTime) {
    if (tiltCounter >= 5)
        tiltStatus = 2;
    else if (tiltCounter >= 3)
        tiltStatus = 1;
    if (tiltTime == 1)
        tiltCounter = addToCounter(tiltCounter);
    else
        tiltCounter = 0;
}
//acc pin function
int speedStatus = 0;
//classifying the speed as either bad (2), ok (1), good (0)
void checkSpeed(int trainSpeed) {
    if (trainSpeed >= 795)
        speedStatus = 1;
    if (trainSpeed >= 909)
        speedStatus = 2;
    if (trainSpeed < 795)
        speedStatus = 0;
}
//weight pin function
int weightStatus = 0;
//changing weight status based on 3 tier (0 = good, 1 = ok, 2 = bad)
void checkWeight(double weight) {
    if (weight >= 233)
        weightStatus = 1;
    if (weight >= 372)
        weightStatus = 2;
    if (weight < 233)
        weightStatus = 0;
}
//engine temperature functions
int engineTempStatus = 0;
void checkEngineTemp(double temperature) {
    if (temperature >= 65)
        engineTempStatus = 1;
    if (temperature >= 100)
        engineTempStatus = 2;
    if (temperature < 65)
        engineTempStatus = 0;
}
// track tilt pin functions
int trackTiltCounter = 0;
int trackTiltStatus = 0;
void checkTrackTilt(int tiltTime) {
    if (trackTiltCounter >= 5)
        trackTiltStatus = 2;
    else if (trackTiltCounter >= 3)
        trackTiltStatus = 1;
    if (tiltTime == 1)
        trackTiltCounter = addToCounter(trackTiltCounter);
    else
        trackTiltCounter = 0;
}
//track speed function
int trainVibrationStatus = 0;
//classifying the speed as either bad (2), ok (1), good (0)
void checkTrainVibration(int trainVibration) {
    if (trainVibration >= 795)
        trainVibrationStatus = 1;
    if (trainVibration >= 909)
        trainVibrationStatus = 2;
    if (trainVibration < 795)
        trainVibrationStatus = 0;
}
//engine temperature functions
int wheelTempStatus = 0;
void checkWheelTemp(double temperature) {
    if (temperature >= 65)
        wheelTempStatus = 1;
    if (temperature >= 100)
        wheelTempStatus = 2;
    if (temperature < 65)
        wheelTempStatus = 0;
}
//check track status based on vibration and tilt
int trackStatus = 0;
void checkTrack() {
    if (trackTiltStatus != 2 && trainVibrationStatus != 2)
        trackStatus = 0;
    else if (trackTiltStatus == 2 && trainVibrationStatus == 2)
        trackStatus = 2;
    else
        trackStatus = 1;
}
//an overall status of the train
int trainHealthOverall() {
    if ((tiltStatus == 1 || speedStatus == 1 || weightStatus == 1 || engineTempStatus == 1 || wheelTempStatus == 1) 
    && (tiltStatus != 2 || speedStatus != 2 || weightStatus != 2 || engineTempStatus != 2 || wheelTempStatus != 2)) {
        return 1;
    }
    else if (tiltStatus == 2 || speedStatus == 2 || weightStatus == 2 || engineTempStatus == 2 || wheelTempStatus == 2) {
        overrideLCD();
        return 2;
    }
    else
        return 0;

}
// //change string classicifaction to numerical
// int changeToNumerical(String classificationWord) {
//     if (classificationWord == "bad")
//         return 2;
//     else if (classificationWord == "okay")
//         return 1;
//     else
//         return 0;
// }
//override LCD button part for danger message
void overrideLCD() {
    lcd.setCursor(0, 1);
    int count = 0;
    char placeholder[20];

    if (strcmp(tiltStatus, "bad") == 0) {
        lcd.setCursor(0, 1);
        lcd.print("DANGER TILT");
        count++;
    }
    if (speedStatus == 2) {
        lcd.setCursor(0, 1);
        lcd.print("DANGER TOO FAST ");
        count++;
    }
    if (weightStatus == 2) {
        lcd.setCursor(0, 1);
        lcd.print("DANGER TOO HEAVY");
        count++;
    }
    if (engineTempStatus == 2) {
        lcd.setCursor(0, 1);
        lcd.print("DANGER ENGINEHOT");
        count++;
    }
    if (wheelTempStatus == 2) {
        lcd.setCursor(0, 1);
        lcd.print("DANGER WHEEL HOT");
        count++;
    }
    if (count >= 2){
        lcd.setCursor(0, 1);
        lcd.print("DANGER 2+ SENSOR");
    }
        
}
//updating the lcd with new status
void updateLCD() {
    if (obstacleCheck != 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WARNING OBSTACLE");
        lcd.setCursor(0,1);
        if (obstacleCheck == 1){
            lcd.print("HEADING TOWARDS");
        }
        else if (obstacleCheck == 2){
            lcd.print("ENGAGING BRAKE");
        }
            
    }
    else {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Track Stat:");
        lcd.setCursor(11,1);
        lcd.print(trackStatus);
        lcd.setCursor(0,0);
        lcd.print("Train Stat:");
        int overall = trainHealthOverall();
        lcd.setCursor(11,0);
        if(overall == 2)
            lcd.print("Bad");
        else if(overall == 1)
            lcd.print("Okay");
        else
            lcd.print("Good")
    }
    
}

//functions for the strip
//a general function
void generalStripUpdate(int placement, int status) {
    if (status == 0)
        strip.setPixelColor(placement, 0, 255, 30); //green for good
    else if (status == 1)
        strip.setPixelColor(placement, 242, 181, 39); //yellow for ok
    else if (status == 2)
        strip.setPixelColor(placement, 255, 0, 0); //red for bad
    else
        strip.setPixelColor(placement, 0, 0, 0); //off
}
void updateStrip() {
    generalStripUpdate(0, trainHealthOverall());
    generalStripUpdate(1, obstacleCheck);
    generalStripUpdate(2, tiltStatus);
    generalStripUpdate(3, speedStatus);
    generalStripUpdate(4, weightStatus);
    generalStripUpdate(5, engineTempStatus);
    generalStripUpdate(6, wheelTempStatus);
}

allData* dataStack = NULL;

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(50);
	delay(100);

    pinMode(trackDetectionPin, INPUT);
    pinMode(imuTiltPin, INPUT);
    pinMode(imuAccPin, INPUT);
    pinMode(forcePin, INPUT);
    pinMode(tempEnginePin, INPUT);
    pinMode(trackTiltPin, INPUT);
    pinMode(trainVibrationPin, INPUT);
    pinMode(wheelTempPin, INPUT);

    lcd.begin(16,2);
    lcd.clear();
    strip.begin();
    strip.show();

    Serial.println("Welcome. Arduino Command:");
    Serial.println("INPUT 'A' to view the default menu");
    Serial.println("INPUT 'B' to view last 5 entries");
    Serial.println("INPUT 'C' to view latest entry");
    Serial.println("PLEASE NOTE THAT COMMANDS OUTPUT ARE ON THE SERIAL MONITOR");
    delay(100);
}

void loop() {
    //trackDetection
    double duration;
    double distance;
    pinMode(trackDetectionPin, OUTPUT);
    digitalWrite(trackDetectionPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trackDetectionPin, HIGH); //sending a pulse for 5ms
	delayMicroseconds(5);
	digitalWrite(trackDetectionPin, LOW);
	//configure ultrasonic sensor to detect the echo
	pinMode(trackDetectionPin, INPUT);
	duration = pulseIn(trackDetectionPin, HIGH);
    distance = convertToCentimeter(duration);

    //preventive stuff
    //imu tilt sensor
    int imuTilt;
    imuTilt = digitalRead(imuTiltPin); // 0 or 1
    checkTilt(imuTilt); //
    //imu acc sensor
    int trainSpeed;
    trainSpeed = analogRead(imuAccPin); // 0 - 102
    checkSpeed(trainSpeed);
    //force pin
    double trainWeight;
    trainWeight = analogRead(forcePin); //reading 0 - 466
    checkWeight(trainWeight);
    //temp pin
    double engineTemp;
    engineTemp = (((analogRead(tempEnginePin) / 965.0) * 5) - 0.5) * 100; //-39.64 - 135.49
    checkEngineTemp(engineTemp);
    //track health stuff
    //tilt sensor
    int trackTilt;
    trackTilt = digitalRead(trackTiltPin); // 0 or 1
    checkTrackTilt(trackTilt);
    //track vibration sensor
    int trainVibration;
    trainVibration = analogRead(trainVibrationPin); // 0 - 1023
    checkTrainVibration(trainVibration);
    //wheel temp sensor
    double wheelTemp;
    wheelTemp = (((analogRead(wheelTempPin) / 965.0) * 5) - 0.5) * 100; //-39.64 - 135.49
    checkWheelTemp(wheelTemp);
    //compile findings into figuring if track is good or not

    //adding them to the stack
    //addData(&dataStack, tiltStatus, trainSpeed, trainWeight, engineTemp, trackStatus, wheelTemp);
    //update lcd and strip
    updateLCD();
    updateStrip();
    //check for user input
    // if (Serial.available()) {
    //     char input = Serial.read();
    //     switch (input) {
    //         case 'A':
    //             printStack(dataStack, 0);
    //             break;
    //         case 'B':
    //             printStack(dataStack, 4);
    //             break;
    //     }
    // }
    delay(500);
}