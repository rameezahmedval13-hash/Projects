/**
 * ==============================================
 * File Name: main.c
 * Author: Rameez Ahmed
 * Date: 10/11/2026
 * Description: This is the main file for GUI based 
 * modern calculator
 * ==============================================
 */

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    Rectangle rect; //button positions
    Color color; //button colors
    const char* text; //button text
} Button;

int main(){
    InitWindow(800, 600, "Modern Calculator");

    char displayText[256] = "\0";
    int textLength = 0;

    float firstOperand = 0.0f;
    char currentOperator = ' ';
    bool clearScreenNext = false; //to wipe screen when user enters second number

    //creating array for buttons using struct
    Button buttons[17] = {
        //Operators
        {{440, 200, 50, 50}, LIGHTGRAY, "+"},
        {{440, 260, 50, 50}, LIGHTGRAY, "-"},
        {{440, 320, 50, 50}, LIGHTGRAY, "*"},
        {{440, 388, 50, 50}, LIGHTGRAY, "/"},
        {{440, 440, 50, 50}, LIGHTGRAY, "="},
        //Numbers
        //Top Row 7-8-9
        {{200, 200, 60, 60}, LIGHTGRAY, "7"},
        {{280, 200, 60, 60}, LIGHTGRAY, "8"},
        {{360, 200, 60, 60}, LIGHTGRAY, "9"},
        //Middle Row 4-5-6
        {{200, 280, 60, 60}, LIGHTGRAY, "4"},
        {{280, 280, 60, 60}, LIGHTGRAY, "5"},
        {{360, 280, 60, 60}, LIGHTGRAY, "6"},
        //Bottom Row 1-2-3
        {{200, 360, 60, 60}, LIGHTGRAY, "1"},
        {{280, 360, 60, 60}, LIGHTGRAY, "2"},
        {{360, 360, 60, 60}, LIGHTGRAY, "3"},
        //Zero
        {{200, 440, 140, 60}, LIGHTGRAY, "0"},

        //Clear and decimal button
        {{100,440,60,60}, LIGHTGRAY, "C"},
        {{360,440,60,60}, LIGHTGRAY, "."}
    };


    while(WindowShouldClose() == false){
        //getting mouse position
        Vector2 mousePoint = GetMousePosition();

        //looping through all 17 buttons
        for (int i = 0; i<17; i++){
            //checking if mouse is over the current button
            if (CheckCollisionPointRec(mousePoint, buttons[i].rect)){
                buttons[i].color = GRAY; //hover effect

                            //check if actually clicked
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

                    //switch case to handle button clicks
                    switch(buttons[i].text[0]){

                        //number logic
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            //if an operator was pressed wipe screen for new num
                            if (clearScreenNext){
                                textLength = 0;
                                displayText[0] = '\0';
                                clearScreenNext = false;
                            }
                            //Append the number to screen
                            if (textLength < 255){
                                displayText[textLength] = buttons[i].text[0];
                                textLength++;
                                displayText[textLength] = '\0';
                            }
                            break;
                        
                        //decimal logic
                        case '.':
                            if (clearScreenNext){
                                displayText[0] = '0'; //start with 0 if decimal is first
                                displayText[1] = '.';
                                displayText[2] = '\0';
                                textLength = 2;
                                clearScreenNext = false;
                            }
                            //only add decimal if one doesn't exist
                            else if(strchr(displayText, '.') == NULL && textLength < 255){
                                displayText[textLength] = '.';
                                textLength++;
                                displayText[textLength] = '\0';
                            }
                            break;
                        
                        //clear logic
                        case 'C':
                            displayText[0] = '\0';
                            textLength = 0;
                            firstOperand = 0.0f;
                            currentOperator = ' ';
                            clearScreenNext = false;
                            break;

                        //Operator logic    
                        case '+': case '-': case '*': case '/':
                            if (textLength > 0){
                                 //Convert text to float and store it
                                 firstOperand = atof(displayText);
                                 //save the math symbol clicked
                                 currentOperator = buttons[i].text[0];
                                 //trigger flag so the next number typed wipes screen
                                clearScreenNext = true; 
                            }
                            break;
                        
                        //equals logic
                        case '=':

                            //convert the second num currently on screen to a float
                            float secondOperand = atof(displayText);
                            float result = 0.0f;

                            //performing mnath
                            if (currentOperator == '+'){
                                result = firstOperand + secondOperand;
                            }
                            else if (currentOperator == '-'){
                                result = firstOperand - secondOperand;
                            }
                            else if (currentOperator == '*'){
                                result = firstOperand * secondOperand;
                            }
                            else if (currentOperator == '/'){
                                //check for division by zero
                                if (secondOperand != 0.0f){
                                    result = firstOperand / secondOperand;
                                }
                                else{
                                    //display error message
                                    snprintf(displayText, sizeof(displayText), "Error: Division by zero");
                                    textLength = strlen(displayText);
                                    clearScreenNext = true; //clear screen on next input
                                    break; 
                                }//endif
                            }//endif

                            //convert float result back to a string to display
                            snprintf(displayText, sizeof(displayText), "%.2f", result);

                            //reset flags for the next calculation
                            textLength = 0;
                            clearScreenNext = true;
                            break;
                    }
                }
            }
            else{
                buttons[i].color = LIGHTGRAY; //resets color if not hovered
            }

        }

        //drawing the buttons
        BeginDrawing();
        ClearBackground(SKYBLUE);

        //Drawing the calculator screen at top
        DrawRectangle(100, 50, 600, 100, LIGHTGRAY);
        DrawRectangleLines(100, 50, 600, 100, BLACK);
        DrawText(displayText, 120, 80, 40, BLACK);

        //loop to draw buttons
        for (int i = 0; i<17; i++){
            DrawRectangleRec(buttons[i].rect, buttons[i].color);
            DrawText(buttons[i].text, buttons[i].rect.x + 20, buttons[i].rect.y + 15, 20, BLACK);
        }
        EndDrawing();

    }//endwhile
    CloseWindow();
    return 0;
}
