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


void setup() 
{
Serial.begin(9600);
//Initialize for RAK3172
Serial1.begin(9600, SERIAL_8N1);
delay(2000);
at_send_check_response("OK", 1000, "AT\r\n");
at_send_check_response("OK", 1000, "AT+NWM=0\r\n");
delay(500);
//Set P2P configurations
//Set the P2P frequency according to your country standards.
// I have used 865.5 MHz where India uses IN865-867 band. 
//SF can be set from 6 to 12
//Set the P2P bandwidth. IN uses 125. 
//Configure P2P code rate(4/5=0, 4/6=1, 4/7=2,4/8=3)
//<Freq>,<SF>,<Bandwidth>,<CR>,<Preamble>,<Power>  
at_send_check_response("OK", 1000, "AT+P2P=865500000:9:125:0:8:15\r\n");
delay(500);
}

void loop() 
{
//Send any data. Try to send data in Hex so that the size of the data is small.
at_send_check_response("OK", 1000, "AT+PSEND=11223344\r\n");
delay(10000);
}
