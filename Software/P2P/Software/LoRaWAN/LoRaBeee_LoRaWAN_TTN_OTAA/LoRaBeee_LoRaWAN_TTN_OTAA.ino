/*

This firmware is written for LoRaBeee board to work in LoRaWAN mode.
Works with TTN in OTAA Class A. I suggest using OTAA because it doesn't 
have fixed keys unlike ABP.  

*/




int join_stat;
int recv_stat;

static char recv_buf[512];
static int at_send_check_response(char *p_ack, int timeout_ms, char *p_cmd, ...)
{
int ch;
int num = 0;
int index = 0;
int startMillis = 0;
va_list args;
memset(recv_buf, 0, sizeof(recv_buf));
va_start(args, p_cmd);
Serial1.printf(p_cmd, args);
Serial.printf(p_cmd, args);
va_end(args);
delay(200);
startMillis = millis();
if (p_ack == NULL)
{
return 0;
}
do
{
while (Serial1.available() > 0)
{
ch = Serial1.read();
recv_buf[index++] = ch;
Serial.print((char)ch);
delay(2);
}
if (strstr(recv_buf, p_ack) != NULL)
{
return 1;
}
} while (millis() - startMillis < timeout_ms);
return 0;
}

static char recv_buf1[512];
static int at_check_response(char *p_ack1, int timeout_ms1, ...)
{
int ch;
int num = 0;
int index = 0;
int startMillis = 0;
memset(recv_buf1, 0, sizeof(recv_buf1));
delay(200);
startMillis = millis();
if (p_ack1 == NULL)
{
return 0;
}
do
{
while (Serial1.available() > 0)
{
ch = Serial1.read();
recv_buf1[index++] = ch;
Serial.print((char)ch);
delay(2);
}
if (strstr(recv_buf1, p_ack1) != NULL)
{
return 1;
}
} while (millis() - startMillis < timeout_ms1);
return 0;
}


void setup() 
{
Serial.begin(9600);
//Initialize for RAK3172
Serial1.begin(9600, SERIAL_8N1);
delay(2000);
at_send_check_response("OK", 1000, "AT\r\n");
at_send_check_response("OK", 1000, "AT+NWM=1\r\n");
delay(500);
//This command is used configure the activation method of the device either OTAA or ABP.
at_send_check_response("OK", 1000, "AT+NJM=1\r\n");
//This command is used to configure the the LoRaWAN® class of the module.
at_send_check_response("OK", 1000, "AT+CLASS=A\r\n");
//This command is used to configure the regional frequency band.

// Setting up Keys
at_send_check_response("OK", 1000, "AT+BAND=3r\n");
at_send_check_response("OK", 1000, "AT+DEVEUI=YOUR DEVICE EUI HERE r\n");
at_send_check_response("OK", 1000, "AT+APPEUI=YOUR APP EUI HERE r\n");
at_send_check_response("OK", 1000, "AT+APPKEY=YOUR APP KEY HERE\r\n");

//Joining the Network
join();

//Check if the module has joined the TTN Server
int join_stat = at_check_response("+EVT:JOINED", 1000);  
if (join_stat == 1)
{
Serial.println("Join Success"); 
}
else
{
Serial.print("Failed, Joining Again");  
//Reset RAK3172
Serial1.print("ATZ\r\n"); 
join(); 
}

}

void loop() 
{
//Sending data to TTN  
at_send_check_response("OK", 1000, "SEND=12345678\r\n");

//Check if the Data is sent to the TTN Server
int recv_stat = at_check_response("+EVT:SEND CONFIRMED OK", 1000);  
if (recv_stat == 1)
{
Serial.println("Sent Success"); 
}
else
{
Serial.print("Failed, Send Again");    
}

delay(10000);
}

void join()
{
at_send_check_response("OK", 1000, "AT+JOIN=1:0:10:8\r\n");
}
