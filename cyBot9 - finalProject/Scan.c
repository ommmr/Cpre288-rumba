/**
 * @file Scan.c
 * @brief controls the servo, IR sensor and Ping sensor, scans the field and returns a number of objects.
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
#include "Scan.h"


//All of my variables that I do not know where to put

 //objects array
int numObjects = 0;


int get_numObjects(){

    return numObjects;
}
float linear_width(object_t obj)
{
    float angle_inDegrees = ((obj.radial_size) * (3.142) / 180.0);
    float lw = obj.distance * tan(angle_inDegrees / 2);
    obj.linear_width = lw;
    return lw;
}


/**
 * Sets up the data so a human is able to read it on putty.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 *  @param objects the variable to count the number of objects in the field of view.
 *
 * @date 7/3/2018
 */
void send_Data_Putty(object_t* objects){


    int i=0;

    uart_sendStr("Degree \tSonar Distance (cm) \t linear width\n\r");
    char str1[200] = "";

    while(i<numObjects){

//        sprintf(str1, "  %d \t          %.2f \t            %.2f \n\r", total,
//                        get_distance(), adc_cm);

        sprintf(str1, "%d      %.2f                      %.2d\n\r", objects[i].degrees, objects[i].distance, (2*objects[i].linear_width));
        uart_sendStr(str1);
        i++;
    }
    numObjects = 0;
}

/**
 * Moves the servo in 2 degree increments, returns ping and sonar data along with the servo moving as well.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @Josh Hall
 * @Yue Chen
 *
 * @date 7/3/2018
 */
void scan(void)
{
    object_t objects[100];

    int total = 0, measure = 2, object = 0;
    int pa = 0; // j = 0, k = 0;

    float adc_cm, initial, final;// small;
//    char str1[200] = "";

    move_servo0(); //reset servo
    timer_waitMillis(500); //wait till reset is done

//   uart_sendStr("Degrees \tSonar Distance (cm) \tIR Distance (cm) \n\r"); //send stuff to putty
    uart_sendStr("Scanning....\n\r");
    while (total != 180)
    {
        //IR SENSOR
        //adc_cm = 253399 * pow(adc_read, -1.285);//Cybot 6
        //adc_cm = 122594 * (pow(adc_read, -1.181)); //CyBot 9

        //GET IR DISTANCE
        adc_cm = convert_toCM(ADC_read(10));

        //CAPS ADC AT 150 TO LIMIT VIEW DIESTNACE
        if (adc_cm >= 150)
        {
            adc_cm = 150; //filters out past 1.5 m
        }

        //GET SONAR DISTANCE
        pulse(); //Sends and receives the signal
        timer_waitMillis(10); // wait to get distance reading from timer 3b handler

        //MOVE SERVO, FLIP DIRECTION INCASE REACH MAX
        total += move_servo_by(measure); //Moves servo by measurement(2 degrees)

        //IF OBJECT IS IN RANGE
        if (adc_cm <= 80)
        {
            //OBJECT IS A "SEE OBJECT VARIABLE"
            //IF ITS VALUE, IS 0 ITS NOT LOOKING AT AN OBJECT WITHIN RANGE
            // IF ITS VALUE IS 1, ITS LOOKING AT AN OBJECT WITH IN RANGE
            if (!object)
            {
                object = 1; //see object variable SET TO 1
                pa = total; //marks first point for angle, SAME AS CURRENT ANGLE OF SERVO
                initial = get_distance(); //marks first distance SAME AS ACCURATE READING
                //FOR CURRENT SONAR DISTANCE
            }
        }//ELSE, IF CURRENT IR READING IS MORE THAN 80, AND PREVIOUS READING WAS AT 1
        //THEN THAT MEANS WE WERE SEEING AN OBJECT AND NOW WE ARE NOT, SO
        else if (adc_cm > 80 && object)
        {
            //SAY WE ARE NOT SEEING AN OBJECT ANYMORE
            object = 0;

            //THIS IS TO ELIMINATE NOISE, OBJECT MUST BE ATLEAST 2 DEGREES WIDE
            if ((total - pa) > 2)
            {
                final = get_distance();
                object_t temp = { (initial + final) / 2, (total - pa), (total
                        + pa) / 2 }; //CREAT THE OBJECT FINALLY AND PUT IT IN AREA, AND GIVE ITS ATTRIBUTES
                temp.linear_width = linear_width(temp);
                objects[numObjects] = temp;
                numObjects++;

            }
        }


    }

    send_Data_Putty(objects);


    move_servo0();
    timer_waitMillis(1000);


}

