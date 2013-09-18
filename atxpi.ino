#include <Bounce.h>

#define READY_LED 11
#define POWER_LED 0
#define POWER_SWITCH 1
#define RESET_SWITCH 2
#define PI_GPIO_23 3
#define PI_GPIO_24 4
#define PI_GPIO_25 5
#define PS_ON 6


int power_state = 0;
int soft_shutdown_triggered = 0;
Bounce power_button = Bounce(POWER_SWITCH, 5);
Bounce reset_button = Bounce(RESET_SWITCH, 5);
elapsedMillis power_button_timer;
elapsedMillis reset_button_timer;


int powerOn() {
	digitalWrite(PS_ON, LOW);
	digitalWrite(POWER_LED, HIGH);
	return 1;
}


int powerOff() {
	digitalWrite(PS_ON, HIGH);
	digitalWrite(POWER_LED, LOW);
	return 0;
}


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
	power_button.update();
	if(power_button.fallingEdge()) {
		if(power_state == 0) { // activate psu
			power_state = powerOn();
		}
		else { // deactivate psu via power button
			power_button_timer = 0;
			for(; !power_button.risingEdge(); power_button.update()) {
				if(!soft_shutdown_triggered && power_button_timer >= 1000) {
					// power button held down for 1 second, tell rpi to shutdown
					soft_shutdown_triggered = 1; // set this so the soft shutdown will only be triggered once per power button press
					// tell rpi to shutdown here
				}
				if(power_button_timer >= 5000) {
					// power button held down for 5 seconds, force shutdown
					power_state = powerOff();
					break;
				}
			}
			soft_shutdown_triggered = 0;
		}
	}

	reset_button.update();
	if(power_state == 1 && reset_button.fallingEdge()) {
		reset_button_timer = 0;
		for(; !reset_button.risingEdge(); reset_button.update()) {
			if(reset_button_timer >= 1000) {
				// reset button held down for 1 second, cycle power
				power_state = powerOff();
				delay(1000);
				power_state = powerOn();
				break;
			}
		}
	}

}
