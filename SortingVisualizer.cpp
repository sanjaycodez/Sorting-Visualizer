#include "raylib.h"
#include <vector>


#define ScreenWidth 1200
#define ScreenHeight 800

#define MinWindowWidth 800
#define MinWindowHeight 500

#define FPS 120

//Pillar States
#define NORMAL 0
#define SORTED 1
#define SELECTED 2


using namespace std;

//Size od Array;
int numberOfPillars = 50;

//Sorting Speed: 
int sortingSpeed = 61;

//Bools:
bool shouldRandomizeArray = true;
bool shouldShowMenuScreen = true;
bool shouldShowStartOptions = false; 
bool ShouldStartSorting  = false;

bool BubbleSortPressed = false;

bool addSpeed = false;
bool subSpeed = false;
bool addSize = false;
bool subSize = false;

bool NormalSize = false;
bool NormalSpeed = false;

bool isSorting = false;
bool isSorted = false;


//Screens:
void showMenuScreen();
void showStartOptions();

void Button(float x, float y, char *Text, Color color, bool &state);
void Bubble_Sort_Button(float tmp, char Bubble_Sort_Text[]);
void Start_Button(float size, float font, char startText[]);

//Function Definitions
void SortArray();
void DrawArray(vector<pair<int,int>> arr);
void RandomizeArray(vector<pair<int,int>> &arr);

void changeSize(char operation, int &value);
void changeSpeed(char operation, int &value);

Color FindColorForPillar(int PillarState);


//The actual Array
vector<pair<int , int>> arr(numberOfPillars);

int main(){

    //Window Configurations
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, "Sorting Algorithm Visualizer");
    SetWindowMinSize(MinWindowWidth, MinWindowHeight);
    //Set FPS
    SetTargetFPS(FPS);

    while(!WindowShouldClose()){
        
        if(shouldShowMenuScreen)
            showMenuScreen();

        shouldShowStartOptions = false;
        if(BubbleSortPressed)
            shouldShowStartOptions = true;

        if(shouldRandomizeArray) 
            RandomizeArray(arr);

        if(shouldShowStartOptions)
            showStartOptions();

        if (ShouldStartSorting && !isSorting && !isSorted) {
            isSorting = true;
            isSorted = false;
            // Call your sorting function here, e.g. BubbleSort(arr);
            isSorting = false;
            isSorted = true;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        
        EndDrawing();
    }
 
    CloseWindow();

    return 0;
}



//Fuction Declarations



//Drawing the pillars
void DrawArray(vector<pair<int,int>> arr){
    float Barwidth = (float) GetScreenWidth() / numberOfPillars;

    for(int i = 0 ; i < numberOfPillars; i++){
        Color color = FindColorForPillar(arr[i].second);

        DrawRectangleV (Vector2{(float) i*Barwidth, (float) GetScreenHeight() - arr[i].first},
                        Vector2{Barwidth, (float) arr[i].first},
                        color);

    }

}

//Randomizing the Array
void RandomizeArray(vector<pair<int,int>> &arr){
    for(int i = 0; i < numberOfPillars; i++){
        arr[i] = {GetRandomValue(30, MinWindowWidth - 140), NORMAL};

    shouldRandomizeArray = false;
    }
    return;
}


void showMenuScreen(){
    float font = (2.5 * GetScreenWidth()) / 100;
    char Bubble_sort_text[] = "BUBBLE SORT!";
    float tmp = MeasureText(Bubble_sort_text, font) / 2;
    Bubble_Sort_Button(tmp, Bubble_sort_text);

    DrawArray(arr);
}


void showStartOptions() {
    float font = (3.0 * GetScreenHeight()) / 100;


    char StartText[] = "Start Sorting!";
    float tmp = (27*GetScreenWidth()) / 100;
    Start_Button(tmp, font, StartText);


    tmp += MeasureText(StartText, font) + 75;
    char RandomizeArrayText[] = "Randomize Array!";
    Button(tmp, GetScreenHeight()/20 + font*2,
        RandomizeArrayText, DARKGRAY, shouldRandomizeArray);

    
    addSpeed = false;
    subSpeed = false;
    addSize = false;
    subSize = false;

    NormalSize = false;
    NormalSpeed = false;

    tmp = (84.2 * GetScreenWidth()) / 100;
    char TimeButtonText[] = "Speed";

 

    Button(tmp, GetScreenHeight()/20 + font*2,
        TimeButtonText, BLUE, NormalSpeed);


    if (NormalSpeed){
        changeSpeed('/', sortingSpeed);
        return;
    }


    tmp += MeasureText(TimeButtonText, font) + 20;
    char SpeedPlusButtonText[] = "+";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SpeedPlusButtonText, ORANGE, addSpeed); 


    if (addSpeed){
        changeSpeed('+', sortingSpeed);
        return;
    }

    tmp += MeasureText(SpeedPlusButtonText, font) + 20;
    char SpeedMinusButtonText[] = "-";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SpeedMinusButtonText, ORANGE, subSpeed);


    if (subSpeed){
        changeSpeed('-', sortingSpeed);
        return;
    }


    tmp = (5 * GetScreenWidth()) / 100;
    char SizeButtonText[] = "Size";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SizeButtonText, BLUE, NormalSize);
    

    if (NormalSize){
        changeSize('/', numberOfPillars);
        return;
    }



    tmp += MeasureText(SizeButtonText, font) + 30;
    char SizePlusButton[] = "+";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SizePlusButton, ORANGE, addSize);

    if (addSize){
        changeSize('+', numberOfPillars);
        return;
    }

    tmp += MeasureText(SizePlusButton, font) + 20;
    char SizeMinusButton[] = "-";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SizeMinusButton, ORANGE, subSize);


    if (subSize){
        changeSize('-', numberOfPillars);
        return;
    } 
}

void changeSpeed(char operation, int &value){
    switch (operation)
    {
    case  '-' :
        value = value  - 10 > 0 ? value - 10 : value;
        break;
    case '+' :
        value = value += 10 ;
    
    default:
        value = 61;
        break;
    }
    SetTargetFPS(sortingSpeed);
}

void changeSize(char operation, int &value){
    switch (operation)
    {
        case '-':
            if (value > 5){
                value -= 5;
                for (int i = 0; i < 5; i++)
                    arr.pop_back();
            }
            break;

        case '+':
            value += 5;
            for (int i = 0; i < 5; i++)
                arr.push_back({GetRandomValue(40, MinWindowWidth-80), NORMAL});
            break;
            
    default:
        while (numberOfPillars > 50){
            numberOfPillars--;
            arr.pop_back();
        }

        while (numberOfPillars < 50){
            numberOfPillars++;
               arr.push_back({GetRandomValue(40, MinWindowWidth-80), NORMAL});
        }
        break;
    }


    for (int i = 0; i < numberOfPillars; i++)
        arr[i].second = NORMAL;


    DrawArray(arr);
}

void Bubble_Sort_Button(float size, char Bubble_Sort_Text[]){
    Color color;
    if(BubbleSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(GetScreenWidth() / 2 - size, GetScreenHeight()/20, Bubble_Sort_Text, color, BubbleSortPressed);
    
}

void Start_Button(float size, float font, char Text[]) {
    Color color = ShouldStartSorting ? BLUE : GREEN;
    Button(size, GetScreenHeight()/20 + font * 2, Text, color, ShouldStartSorting);
}


Color FindColorForPillar(int pillarState){
    switch (pillarState)
    {
        case SELECTED :
            return LIGHTGRAY;

        case SORTED :
                return GREEN;

        default:
            return WHITE;
            break;
    }
} 


void Button(float x, float y, char *Text, Color color, bool &state){
    float font = (2.5 * GetScreenHeight()) / 100;
    Rectangle r1 = {
        .x = x,
        .y = y,
        .width = (float) MeasureText(Text, font),
        .height = (float) font
    };

    if(CheckCollisionPointRec(GetMousePosition(), r1)){
        DrawText(Text, x , y, font,  RED);

        if(IsMouseButtonPressed(0)){

            if(state == true)
                state = false; 
            else    
                state = true; 


            return;
        }      
    }else {
        DrawText(Text, x, y, font, color);
    }

    return;
}



