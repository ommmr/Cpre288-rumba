/**
 * @file movement.c
 * @brief this controls the movement for the entire CyBot
 * and handles the input/output of the bot.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */

#include "lcd.h"
#include "timer.h"
#include "open_interface.h"
#include "uart.h"
#include "string.h"
#include "scan.h"

void avoidright(oi_t *sensor_data);
void avoidleft(oi_t *sensor_data);
void movebackward(oi_t *sensor_data, int millimeters);
void turncounterclockwise(oi_t *sensor_data, int ang);
void turnclockwise(oi_t *sensor_data, int ang);
void moveforward(oi_t *sensor_data, int millimeters);
void playsong(void);

/**
 * Handles all commands, functions and movements of the cybot.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @param dist distance to be traveled by cybot every time move function is called
 * @param angle angle turned by cybot everytime turn function is called
 * @param sensor_data data from cybot sensors to be updated
 *
 * @date 7/3/2018
 */
void command(int dist, int angle, oi_t *sensor_data)
{

    int i = 0;
    char comment[21] = "";
    comment[20] = '\0';
    int holding = 0;

    while (1)
    {
        // waits for command
        holding = uart_receive();

        // if command is entered
        if (holding == '\r')
        {
            // if the last letter when entered is c, it clears the array
            if (comment[i - 1] == 'c')
            {
                comment[0] = 0;
                comment[i] = '\0';
                lcd_printf("clear");
                uart_sendChar('\n');

                break;
            }

            // if the fist letter is a w, drive forward
            else if (comment[0] == 'w')
            {
                comment[i] = '\0';
                lcd_printf("%s", comment);
                uart_sendChar('\n');
                moveforward(sensor_data, dist);
                break;
            }

            // if the fist letter is a s, drive backward
            else if (comment[0] == 's')
            {
                comment[i] = '\0';
                lcd_printf("%s", comment);
                uart_sendChar('\n');
                movebackward(sensor_data, dist);
                break;
            }

            // if the fist letter is an a, turn counterclockwise
            else if (comment[0] == 'a')
            {
                comment[i] = '\0';
                lcd_printf("%s", comment);
                uart_sendChar('\n');
                turncounterclockwise(sensor_data, angle);
                break;
            }

            // if the fist letter is a d, turn clockwise
            else if (comment[0] == 'd')
            {
                comment[i] = '\0';
                lcd_printf("%s", comment);
                uart_sendChar('\n');
                turnclockwise(sensor_data, angle);
                break;
            }

            // if the fist letter is a p, play the song
            else if (comment[0] == 'p')
            {
                comment[i] = '\0';
                lcd_printf("%s", comment);
                uart_sendChar('\n');
                playsong();
                break;
            }

            // if the fist letter is a p, play the song
            else if (comment[0] == 'e')
            {
                comment[i] = '\0';
                lcd_printf("%s", comment);
                uart_sendChar('\n');
                scan();
                break;
            }

            // else do nothing
            else
            {

            }
            i = 0;
        }

        // if enter is not hit, store the command
        else if (holding != 0)
        {

            comment[i] = holding;
            lcd_printf("char: %c\nindex: %d", holding, i);
            i++;
            holding = 0;
        }
    }
}


/**
 * Entire move_forward function from lab 2 modified so that it can stop
 * the bot when sensors detect cliff, line, object, or anything that might
 * obstruct the bot. Also returns some current action documentation for the user
 * since he will be remotely controlling it and would like to know what its doing.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @param millimeters distance to be moved forward by the cybot
 * @param sensor_data data from cybot sensors to be updated
 *
 * @date 7/3/2018
 */


void moveforward(oi_t *sensor_data, int millimeters)
{
    int sum = 0, j;
    int left = 0, frontleft = 0, frontright = 0, right = 0;
    char phrase[100] = "";

    while (sum < millimeters)

    {
        oi_setWheels(150, 150);
        oi_update(sensor_data);
        sum += sensor_data->distance;

        // if the right bumper is hit, send a warning to putty and move backward 100 cm
        if (sensor_data->bumpRight == 1)
        {
            strncpy(phrase,
                    "WARNING!!! Right bumper hit object, backing up 15 cm",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');
            movebackward(sensor_data, 100);
            break;

        }

        // if the left bumper is hit, send a warning to putty and move backward 100 cm
        if (sensor_data->bumpLeft == 1)
        {
            strncpy(phrase,
                    "WARNING!!! Left bumper hit object, backing up 15 cm", 100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');
            movebackward(sensor_data, 100);
            break;

        }

        left = sensor_data->cliffLeftSignal;
        frontleft = sensor_data->cliffFrontLeftSignal;
        frontright = sensor_data->cliffFrontRightSignal;
        right = sensor_data->cliffRightSignal;

        // if the left cliff sensor detects a hole, send a warning to putty and move back 100 cm
        if (left < 1000)
        {
            strncpy(phrase,
                    "WARNING!!! Left side hit hole, moving backward 5 cm, rotating 45 degrees to the right",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');
            movebackward(sensor_data, 50);
            turnclockwise(sensor_data, 45);

            break;
        }
        // if the left cliff sensor detects the boundry, send a warning to putty and turn clockwise 45 degrees
        if (left > 2650)
        {
            strncpy(phrase,
                    "WARNING!!! Left side hit boundy edge, moving backward 5 cm, rotating 45 degrees to the right",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');
            movebackward(sensor_data, 50);
            turnclockwise(sensor_data, 45);

            break;
        }

        // if the front left cliff sensor detects a hole, send a warning to putty and move back 100 cm
        if (frontleft < 1000)
        {
            // prints phrase
            strncpy(phrase,
                    "WARNING!!! Front Left side hit hole, moving backward 5 cm, rotating 90 degrees to the right",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {

                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');

            movebackward(sensor_data, 50);
            turnclockwise(sensor_data, 90);

            break;
        }

        // if the front left cliff sensor detects the boundry, send a warning to putty and turn clockwise 90 degrees
        if (frontleft > 2650)
        {
            strncpy(phrase,
                    "WARNING!!! Front Left side hit boundy edge, moving backward 5 cm, rotating 90 degrees to the right",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');

            movebackward(sensor_data, 50);
            turnclockwise(sensor_data, 90);

            break;
        }

        // if the front right cliff sensor detects a hole, send a warning to putty and move back 100 cm
        if (frontright < 1000)
        {
            strncpy(phrase,
                    "WARNING!!! Front Right side hit hole, moving backward 5 cm, rotating 90 degrees to the left",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');

            movebackward(sensor_data, 50);
            turncounterclockwise(sensor_data, 90);

            break;
        }

        // if the front right cliff sensor detects the boundry, send a warning to putty and turn counter clockwise 45 degrees
        if (frontright > 2650)
        {
            strncpy(phrase,
                    "WARNING!!! Front Right side hit boundy edge, moving backward 5 cm, rotating 90 degrees tho the right",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');

            movebackward(sensor_data, 50);
            turncounterclockwise(sensor_data, 90);

            break;
        }

        // if the right cliff sensor detects a hole, send a warning to putty and move back 100 cm
        if (right < 1000)
        {
            strncpy(phrase,
                    "WARNING!!! Right side hit hole, moving backward 5 cm, rotating 45 degrees to the left",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }
            uart_sendChar('\n');
            uart_sendChar('\r');

            movebackward(sensor_data, 50);
            turncounterclockwise(sensor_data, 45);

            break;
        }

        // if the right cliff sensor detects the boundry, send a warning to putty and turn counter clockwise 90 degrees
        if (right > 2650)
        {
            strncpy(phrase,
                    "WARNING!!! Right side hit boundy edge, moving backward 5 cm, rotating 45 degrees to the left",
                    100);
            for (j = 0; j < strlen(phrase); j++)
            {
                // prints phrase
                uart_sendChar(phrase[j]);
            }

            uart_sendChar('\n');
            uart_sendChar('\r');
            movebackward(sensor_data, 50);
            turncounterclockwise(sensor_data, 45);

            break;
        }

        lcd_printf("left: %d\nleftfront: %d\nrightfront: %d\nright: %d", left,
                   frontleft, frontright, right);

    }

    // sets wheels to zero and resets data
    oi_setWheels(0, 0);
    sum = 0;
    oi_update(sensor_data);
    oi_update(sensor_data);
}


/**
 * Entire move_backward function from lab 2 modified so that it can stop
 * the bot when sensors detect cliff, line, object, or anything that might
 * obstruct the bot. Also returns some current action documentation for the user
 * since he will be remotely controlling it and would like to know what its doing.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @param millimeters distance to be moved forward by the cybot
 * @param sensor_data data from cybot sensors to be updated
 *
 * @date 7/3/2018
 */
void movebackward(oi_t *sensor_data, int millimeters)
{
    int sum1 = 0;
    oi_setWheels(-150, -150);

    while (sum1 < millimeters)
    {
        oi_update(sensor_data);
        sum1 -= sensor_data->distance;
    }
    oi_setWheels(0, 0);

}



/**
 * Entire turncounterclockwise function that turns bot counterclock wise by specified angle
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @param angle angle to be turned counterclockwise by the cybot
 * @param sensor_data data from cybot sensors to be updated
 *
 * @date 7/3/2018
 */
void turncounterclockwise(oi_t *sensor_data, int ang)
{
    int sum1 = 0;
    oi_setWheels(150, -150);

    while (sum1 < ang)
    {
        oi_update(sensor_data);
        sum1 += sensor_data->angle;
    }
    oi_setWheels(0, 0);

}

/**
 * Entire turnclockwise function that turns bot counterclock wise by specified angle
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @param angle angle to be turned clockwise by the cybot
 * @param sensor_data data from cybot sensors to be updated
 *
 * @date 7/3/2018
 */
void turnclockwise(oi_t *sensor_data, int ang)
{
    int sum1 = 0;
    oi_setWheels(-150, 150);

    while (sum1 < ang)
    {
        oi_update(sensor_data);
        sum1 -= sensor_data->angle;
    }
    oi_setWheels(0, 0);

}


/**
 * plays song by pairing the arrays of the note period and the note frequencies based on the MIDI chart.
 * Uses the open interface to relay these arrays onto the board.
 *
 * @author Omar Abbas
 * @author Jack Temple
 * @author Josh Hall
 * @author Yue Chen
 *
 * @date 7/3/2018
 */
void playsong(void)
{

    int index = 0;
    int num_notes = 20;

    //                                          5                   10                  15
    unsigned char notes[30] = { 84, 88, 93, 84, 88, 93, 88, 83, 88, 93, 83, 88,
                                90, 88, 84, 88, 93, 84 };
    unsigned char period[30] = { 25, 25, 25, 40, 25, 25, 25, 25, 25, 25, 40, 25,
                                 25, 25, 25, 25, 25, 25 };

    oi_loadSong(index, num_notes, notes, period);
    oi_play_song(index);

}

