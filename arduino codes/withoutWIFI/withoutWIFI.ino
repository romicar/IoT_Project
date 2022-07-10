#define Linear_LDR_1 12
#define Linear_LDR_2 33 // 2
#define Linear_PIR 14   // 27
#define Table_LDR_1 32
#define Table_LDR_2 35
#define Table_PIR 34
#define Room_PIR 2 // 33
#define Room_US_1_trig 19
#define Room_US_1_echo 5
#define Room_US_2_trig 18
#define Room_US_2_echo 4
#define Room_US_3_trig 23
#define Room_US_3_echo 22

#define Linear_Lights 27 // 26
#define Room_Lights 26   // 21
#define Table_Lights 13

#define wait_room 6000
#define wait_linear 6000
#define wait_general 6000
#define us_threshold 5.00
#define ldr_threshold 400

/*char *ssid = "Khushe";
char *pass = "khushe1703";
String cse_ip = "192.168.206.199"; // YOUR IP from ipconfig/ifconfig
String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";
String ae = "Demonstration";
String cnt = "Trial_2";*/

int abs(int a)
{
    return a < 0 ? -a : a;
}
/*
void createCi(String val)
{
    HTTPClient http;
    http.begin(server + ae + "/" + cnt + "/");

    http.addHeader("X-M2M-Origin", "admin:admin");
    http.addHeader("Content-Type", "application/json;ty=4");
    int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");
    Serial.println(code);
    if (code == -1)
    {
        Serial.println("UNABLE TO CONNECT TO THE SERVER");
    }
    else
    {
        Serial.println("CONNECTED SUCCESSFULLY!");
    }

    http.end();
}*/

int last_detected_linear;
int last_detected_table;
int last_detected_room;

bool state_room = true;
bool state_linear = true;
bool state_table = true;

void switch_on_room();
void switch_off_room();
void switch_on_linear();
void switch_off_linear();
void switch_off_table();
void switch_on_table();
bool check_linear();
bool check_room();
bool check_table();

bool ultrasonic1(int trig, int echo);
bool ultrasonic2(int trig, int echo);
bool ultrasonic3(int trig, int echo);

int linear_off_time;
int room_off_time;
int table_off_time;

bool ldr1(int data);
bool ldr2(int data);
bool ldr3(int data);
bool ldr4(int data);

int last_sent = -1;

void setup()
{
    Serial.begin(9600);
    pinMode(Linear_LDR_1, INPUT);
    pinMode(Linear_LDR_2, INPUT);
    pinMode(Linear_PIR, INPUT_PULLUP);
    pinMode(Table_LDR_1, INPUT);
    pinMode(Table_LDR_2, INPUT);
    pinMode(Table_PIR, INPUT_PULLUP);
    pinMode(Room_PIR, INPUT_PULLUP);
    pinMode(Room_US_1_trig, OUTPUT);
    pinMode(Room_US_2_trig, OUTPUT);
    pinMode(Room_US_3_trig, OUTPUT);
    pinMode(Room_US_1_echo, INPUT);
    pinMode(Room_US_2_echo, INPUT);
    pinMode(Room_US_3_echo, INPUT);
    pinMode(Linear_Lights, OUTPUT);
    //    pinMode(General_Lights, OUTPUT);
    pinMode(Room_Lights, OUTPUT);
    pinMode(Table_Lights, OUTPUT);

    digitalWrite(Linear_Lights, HIGH);
    //    digitalWrite(General_Lights, HIGH);
    digitalWrite(Table_Lights, HIGH);
    digitalWrite(Room_Lights, HIGH);

    last_detected_linear = millis();
    last_detected_room = millis();
    last_detected_table = millis();

    /* WiFi.begin(ssid, pass);
     while (WiFi.status() != WL_CONNECTED)
     {
         Serial.println("Connecting to WiFi...");
         delay(1000);
     }
     Serial.println("WiFi Connected!");
     delay(12000);*/
}

void loop()
{
    int curr_time = millis();

    Serial.print("The current time is: ");
    Serial.println(curr_time);
    Serial.println();
    if (check_room() == true)
    {
        if (state_room == false)
        {
            Serial.print("The electricity was switched off for: ");
            Serial.print((float)(curr_time - room_off_time) / 1000);
            Serial.println(" seconds.");
            switch_on_room();
        }
        last_detected_room = curr_time;
    }
    else if (state_room == true && curr_time - last_detected_room >= wait_room)
    {
        switch_off_room();
        room_off_time = curr_time;
    }

    if (check_linear() == true)
    {
        if (state_linear == false)
        {
            Serial.print("The electricity was switched off for: ");
            Serial.print((float)(curr_time - linear_off_time) / 1000);
            Serial.println(" seconds.");
            switch_on_linear();
        }
        last_detected_linear = curr_time;
    }
    else if (state_linear == true && curr_time - last_detected_linear >= wait_linear)
    {
        switch_off_linear();
        linear_off_time = curr_time;
    }

    if (check_table() == true)
    {
        if (state_table == false)
        {
            Serial.print("The electricity was switched off for: ");
            Serial.print((float)(curr_time - table_off_time) / 1000);
            Serial.println(" seconds.");
            switch_on_table();
        }
        last_detected_table = curr_time;
    }
    else if (state_table == true && curr_time - last_detected_table >= wait_linear)
    {
        switch_off_table();
        table_off_time = curr_time;
    }

    /*int val = 0;
    if (state_table == true)
        val += 6;
    if (state_linear == true)
        val += 6;
    if (state_room == true)
        val += 12;

    if (last_sent == -1 || curr_time - last_sent > 5000)
    {
        createCi(String(val));
        last_sent = curr_time;
        for (int i = 0; i < 100; i++)
        {
          Serial.print("0");
          Serial.print(hour);
          Serial.print(":00");
          Serial.println();
        }
        hour++;
        delay(3000);
    }

    Serial.println();
    Serial.println();*/

    delay(1000);
}

void switch_on_room()
{
    digitalWrite(Room_Lights, HIGH);
    state_room = true;
}
void switch_off_room()
{
    digitalWrite(Room_Lights, LOW);
    state_room = false;
}

void switch_on_linear()
{
    digitalWrite(Linear_Lights, HIGH);
    state_linear = true;
}
void switch_off_linear()
{
    digitalWrite(Linear_Lights, LOW);
    state_linear = false;
}

void switch_on_table()
{
    digitalWrite(Table_Lights, HIGH);
    state_table = true;
}
void switch_off_table()
{
    digitalWrite(Table_Lights, LOW);
    state_table = false;
}
bool check_room()
{
    bool us1 = ultrasonic1(Room_US_1_trig, Room_US_1_echo);
    bool us2 = ultrasonic2(Room_US_2_trig, Room_US_2_echo);
    bool us3 = ultrasonic3(Room_US_3_trig, Room_US_3_echo);
    bool p = pir(Room_PIR);

    Serial.println();

    if (us1 || us2 || us3 || p)
    {
        return true;
    }
    return false;
}

bool check_linear()
{
    bool l1 = ldr1(Linear_LDR_1);
    bool l2 = ldr2(Linear_LDR_2);
    bool p = pir(Linear_PIR);

    if (l1 || l2 || p)
    {
        return true;
    }
    return false;
}

bool check_table()
{
    bool l1 = ldr3(Table_LDR_1);
    bool l2 = ldr4(Table_LDR_2);
    bool p = pir(Table_PIR);

    if (l1 || l2 || p)
    {
        return true;
    }
    return false;
}
bool ultrasonic1(int trig, int echo)
{
    static float prev_data = 0;
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    float duration = pulseIn(echo, HIGH);
    float new_data = 0.017 * duration;

    Serial.println(new_data);

    if (abs(new_data - prev_data) > us_threshold)
    {
        prev_data = new_data;
        return true;
    }

    prev_data = new_data;
    return false;
}

bool ultrasonic2(int trig, int echo)
{
    static float prev_data = 0;
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    float duration = pulseIn(echo, HIGH);
    float new_data = 0.017 * duration;

    Serial.println(new_data);

    if (abs(new_data - prev_data) > us_threshold)
    {
        prev_data = new_data;
        return true;
    }

    prev_data = new_data;
    return false;
}

bool ultrasonic3(int trig, int echo)
{
    static float prev_data = 0;
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    float duration = pulseIn(echo, HIGH);
    float new_data = 0.017 * duration;

    Serial.println(new_data);

    if (abs(new_data - prev_data) > us_threshold)
    {
        prev_data = new_data;
        return true;
    }

    prev_data = new_data;
    return false;
}

bool pir(int data)
{
    int new_data = digitalRead(data);
    if (new_data == RISING || new_data == HIGH)
    {
        Serial.print("MOTION DETECTED in region ");
        if (data == Linear_PIR)
            Serial.println("Linear Table");
        else if (data == Table_PIR)
            Serial.println("Table");
        else if (data == Room_PIR)
            Serial.println("Room");
        return true;
    }
    return false;
}

bool ldr1(int data)
{
    static int prev_data = 0;

    int new_data = analogRead(data);
    Serial.print("The LDR 1 Speaks: ");
    Serial.print(new_data);
    Serial.print(": The difference from previous is: ");
    Serial.println(abs(new_data - prev_data));
    if (abs(new_data - prev_data) > ldr_threshold)
    {
        prev_data = new_data;
        return true;
    }

    prev_data = new_data;
    return false;
}

bool ldr2(int data)
{
    static int prev_data = 0;

    int new_data = analogRead(data);
    Serial.print("The LDR 2 Speaks: ");
    Serial.print(new_data);
    Serial.print(": The difference from previous is: ");
    Serial.println(abs(new_data - prev_data));
    if (abs(new_data - prev_data) > ldr_threshold)
    {
        prev_data = new_data;
        return true;
    }

    prev_data = new_data;
    return false;
}

bool ldr3(int data)
{
    static int prev_data = 0;

    int new_data = analogRead(data);
    Serial.print("The LDR 3 Speaks: ");
    Serial.print(new_data);
    Serial.print(": The difference from previous is: ");
    Serial.println(abs(new_data - prev_data));
    if (abs(new_data - prev_data) > ldr_threshold)
    {
        prev_data = new_data;
        return true;
    }

    prev_data = new_data;
    return false;
}

bool ldr4(int data)
{
    static int prev_data = 0;

    int new_data = analogRead(data);
    Serial.print("The LDR 4 Speaks: ");
    Serial.print(new_data);
    Serial.print(": The difference from previous is: ");
    Serial.println(abs(new_data - prev_data));
    if (abs(new_data - prev_data) > ldr_threshold)
    {
        prev_data = new_data;
        return true;
    }

    prev_data = new_data;
    return false;
}
