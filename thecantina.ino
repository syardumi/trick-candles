// This #include statement was automatically added by the Particle IDE.
#include <RCSwitch.h>
#include <string.h>

#define LIVINGROOM_Light_1_On 5330227
#define LIVINGROOM_Light_1_Off 5330236
#define LIVINGROOM_Light_2_On 5330371
#define LIVINGROOM_Light_2_Off 5330380
#define LIVINGROOM_Light_3_On 5330691
#define LIVINGROOM_Light_3_Off 5330700
#define LIVINGROOM_Light_4_On 267715
#define LIVINGROOM_Light_4_Off 267724
#define LIVINGROOM_Light_5_On 268035
#define LIVINGROOM_Light_5_Off 268044

#define BEDROOM_Light_1_On 5332227
#define BEDROOM_Light_1_Off 5332236
#define BEDROOM_Light_2_On 5338371
#define BEDROOM_Light_2_Off 5338380
#define BEDROOM_Light_3_On 267571
#define BEDROOM_Light_3_Off 267580

RCSwitch switchTx = RCSwitch();
RCSwitch switchRx = RCSwitch();

void smartHomeHandler(const char *event, const char *data)
{
    //parse data for switch position & room location & maybe light ID
    const char comma[2] = ",";
    char *mutable_data = strdup(data);
    Particle.publish("mutable-data", mutable_data);
    char *switch_pos = strtok(mutable_data, comma);
    char *room_location = strtok(NULL, comma);
    char *light_number = strtok(NULL, comma);
    int light_num_val;
    if (light_number != NULL)
        light_num_val = atoi(light_number);

    Particle.publish("room-location", room_location);
    Particle.publish("switch-position", switch_pos);

    //send TX based on 2 (maybe 3) params
    //this logic portion should be off-loaded to the cloud
    //and light codes stored to a JSON db for reference so
    //that this code doesn't need to keep scaling proportionally
    //with more outlets/lights.
    if (strcmp(room_location, "livingroom") == 0){
        if (strcmp(switch_pos, "on") == 0){
            if (light_number == NULL) {
                switchTx.send(LIVINGROOM_Light_1_On, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_2_On, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_3_On, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_4_On, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_5_On, 24);
            }else if (light_num_val == 1){
                switchTx.send(LIVINGROOM_Light_1_On, 24);
            }else if (light_num_val == 2){
                switchTx.send(LIVINGROOM_Light_2_On, 24);
            }else if (light_num_val == 3){
                switchTx.send(LIVINGROOM_Light_3_On, 24);
            }else if (light_num_val == 4){
                switchTx.send(LIVINGROOM_Light_4_On, 24);
            }else if (light_num_val == 5){
                switchTx.send(LIVINGROOM_Light_5_On, 24);
            }
        }else if (strcmp(switch_pos, "off") == 0){
            if (light_number == NULL) {
                switchTx.send(LIVINGROOM_Light_1_Off, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_2_Off, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_3_Off, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_4_Off, 24);
                delay(300);
                switchTx.send(LIVINGROOM_Light_5_Off, 24);
            }else if (light_num_val == 1){
                switchTx.send(LIVINGROOM_Light_1_Off, 24);
            }else if (light_num_val == 2){
                switchTx.send(LIVINGROOM_Light_2_Off, 24);
            }else if (light_num_val == 3){
                switchTx.send(LIVINGROOM_Light_3_Off, 24);
            }else if (light_num_val == 4){
                switchTx.send(LIVINGROOM_Light_4_Off, 24);
            }else if (light_num_val == 5){
                switchTx.send(LIVINGROOM_Light_5_Off, 24);
            }
        }
    }else if (strcmp(room_location, "bedroom") == 0){
        if (strcmp(switch_pos, "on") == 0){
            if (light_number == NULL) {
                switchTx.send(BEDROOM_Light_1_On, 24);
                delay(300);
                switchTx.send(BEDROOM_Light_2_On, 24);
                delay(300);
                switchTx.send(BEDROOM_Light_3_On, 24);
            }else if (light_num_val == 1){
                switchTx.send(BEDROOM_Light_1_On, 24);
            }else if (light_num_val == 2){
                switchTx.send(BEDROOM_Light_2_On, 24);
            }else if (light_num_val == 3){
                switchTx.send(BEDROOM_Light_3_On, 24);
            }
        }else if (strcmp(switch_pos, "off") == 0){
            if (light_number == NULL) {
                switchTx.send(BEDROOM_Light_1_Off, 24);
                delay(300);
                switchTx.send(BEDROOM_Light_2_Off, 24);
                delay(300);
                switchTx.send(BEDROOM_Light_3_Off, 24);
            }else if (light_num_val == 999) {
                switchTx.send(BEDROOM_Light_1_Off, 24);
                delay(300);
                switchTx.send(BEDROOM_Light_3_Off, 24);
            }else if (light_num_val == 1){
                switchTx.send(BEDROOM_Light_1_Off, 24);
            }else if (light_num_val == 2){
                switchTx.send(BEDROOM_Light_2_Off, 24);
            }else if (light_num_val == 3){
                switchTx.send(BEDROOM_Light_3_Off, 24);
            }
        }
    }
}

void setup() {

    //set pin modes
    pinMode(D3, OUTPUT);
    pinMode(D1, INPUT_PULLDOWN);

    //set transmitter pulse length and enable
    switchTx.setPulseLength(180);
    switchTx.enableTransmit(D3);

    //enable reception
    switchRx.enableReceive(D1);

    //subscribe to cloud events for switchLightsState
    Particle.subscribe("switchLightsState", smartHomeHandler);
}

void loop() {

    //if reception happens
    if (switchRx.available()) {

        //get reception value (the RX code in decimal)
        int value = switchRx.getReceivedValue();

        //log the value for use later
        if (value == 0) {
            Particle.publish("unknown-encoding", "NULL");
        } else {
            char buf[10];
            itoa(value, buf, 10);
            Particle.publish("RF-received", buf);
        }

        //reset the receiver
        switchRx.resetAvailable();

    }
}
