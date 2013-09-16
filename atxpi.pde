#define READY_LED 11
#define POWER_LED 3
#define PI_GPIO_23 1
#define PI_GPIO_24 0
#define POWER_SWITCH 2
#define RESET_SWITCH 5
#define PS_ON 4


void setup() {
	pinMode(READY_LED, OUTPUT);
	pinMode(POWER_LED, OUTPUT);
	pinMode(PI_GPIO_23, OUTPUT);
	pinMode(PS_ON, OUTPUT);

	pinMode(PI_GPIO_24, INPUT);
	pinMode(POWER_SWITCH, INPUT_PULLUP);
	pinMode(RESET_SWITCH, INPUT_PULLUP);

	digitalWrite(PI_GPIO_23, LOW);
	digitalWrite(PS_ON, HIGH);
	digitalWrite(POWER_LED, LOW);

	digitalWrite(READY_LED, HIGH);
}


void loop() {
	if(!digitalRead(POWER_SWITCH)) {
		digitalWrite(POWER_LED, HIGH);
		digitalWrite(PS_ON, LOW);
	}
}
