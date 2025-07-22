#include "raylib.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

std::thread sortingThread;

#define ScreenWidth 1200
#define ScreenHeight 800
#define MinWindowWidth 800
#define MinWindowHeight 500
#define FPS 120

#define NORMAL 0
#define SORTED 1
#define SELECTED 2

using namespace std;

int numberOfPillars = 50;
int sortingSpeed = 61;
bool stopSorting = false;

bool shouldRandomizeArray = true;
bool shouldShowMenuScreen = true;
bool shouldShowStartOptions = false;
bool ShouldStartSorting = false;
bool sortingInProgress = false;

bool sortingMenuOpen = false;
bool selectedBubble = false;
bool selectedInsertion = false;
bool selectedQuick = false;
bool selectedMerge = false;

bool addSpeed = false;
bool subSpeed = false;
bool addSize = false;
bool subSize = false;

bool NormalSize = false;
bool NormalSpeed = false;

void showMenuScreen();
void showStartOptions();
void Button(float x, float y, char *Text, Color color, bool &state);
void Sorting_Options_Button(float tmp, char SortText[]);
void Sorting_Selection_Buttons();
void Start_Button(float size, float font, char startText[]);
void Stop_Button(float size, float font, char stopText[]);

void BubbleSort();
void InsertionSort();
void QuickSort(int low, int high);
int Partition(int low, int high);
void MergeSort(int l, int r);
void Merge(int l, int m, int r);

void DrawArray(vector<pair<int,int>> arr);
void RandomizeArray(vector<pair<int,int>> &arr);
void changeSize(char operation, int &value);
void changeSpeed(char operation, int &value);
Color FindColorForPillar(int PillarState);
void Highlight(int index, int state);

vector<pair<int , int>> arr(numberOfPillars);

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, "Sorting Algorithm Visualizer");
    SetWindowMinSize(MinWindowWidth, MinWindowHeight);
    SetTargetFPS(FPS);
    SetWindowState(FLAG_VSYNC_HINT);  // ✅ VSync for smooth drawing

    while(!WindowShouldClose()) {
        if (ShouldStartSorting && !sortingInProgress) {
            sortingInProgress = true;
            stopSorting = false;

            // Optional debug log
            TraceLog(LOG_INFO, "Sorting started!");

            // Start sorting in a separate thread
            sortingThread = std::thread([](){
                if (selectedBubble) BubbleSort();
                else if (selectedInsertion) InsertionSort();
                else if (selectedQuick) QuickSort(0, arr.size() - 1);
                else if (selectedMerge) MergeSort(0, arr.size() - 1);

                sortingInProgress = false;
                ShouldStartSorting = false;
                // Do NOT auto-open or auto-close the menu here
                // User should open the menu by clicking the menu button
            });
            sortingThread.detach();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if(shouldShowMenuScreen)
            showMenuScreen();

        // Show the sorting menu if the user has opened it and not currently sorting
        if (!sortingInProgress && sortingMenuOpen) {
            Sorting_Selection_Buttons();
        }

        // Only show the other menu options if the sorting menu is not open
        shouldShowStartOptions = (!sortingMenuOpen && (selectedBubble || selectedInsertion || selectedQuick || selectedMerge || sortingInProgress));
        if (shouldShowStartOptions)
            showStartOptions();

        if(shouldRandomizeArray)
            RandomizeArray(arr);

        DrawArray(arr);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawArray(vector<pair<int,int>> arr) {
    float Barwidth = (float) GetScreenWidth() / numberOfPillars;
    for(int i = 0 ; i < numberOfPillars; i++) {
        Color color = FindColorForPillar(arr[i].second);
        DrawRectangleV(Vector2{(float)i*Barwidth, (float)GetScreenHeight() - arr[i].first}, Vector2{Barwidth, (float)arr[i].first}, color);
    }
}

void RandomizeArray(vector<pair<int,int>> &arr) {
    for(int i = 0; i < numberOfPillars; i++) {
        arr[i] = {GetRandomValue(30, MinWindowWidth - 140), NORMAL};
    }
    shouldRandomizeArray = false;
}

void showMenuScreen() {
    float font = (2.5 * GetScreenWidth()) / 100;
    char SortText[] = "SORTING";
    float tmp = MeasureText(SortText, font) / 2;
    Sorting_Options_Button(tmp, SortText);
    if(sortingMenuOpen) Sorting_Selection_Buttons();
}

void Sorting_Options_Button(float size, char SortText[]) {
    Color color = sortingMenuOpen ? BLUE : GREEN;
    Button(GetScreenWidth() / 2 - size, GetScreenHeight()/20, SortText, color, sortingMenuOpen);
}

void Sorting_Selection_Buttons() {
    if (sortingInProgress) return;
    float font = (2.5 * GetScreenHeight()) / 100;
    float startX = GetScreenWidth() / 2 - 2 * MeasureText("Bubble Sort", font);
    float y = GetScreenHeight()/20 + font * 2;

    char Bubble[] = "Bubble Sort";
    char Insertion[] = "Insertion Sort";
    char Quick[] = "Quick Sort";
    char Merge[] = "Merge Sort";

    // Only close the menu if the user actually selects a new algorithm
    bool prevBubble = selectedBubble;
    bool prevInsertion = selectedInsertion;
    bool prevQuick = selectedQuick;
    bool prevMerge = selectedMerge;

    Button(startX, y, Bubble, GREEN, selectedBubble);
    if (!prevBubble && selectedBubble) { sortingMenuOpen = false; selectedInsertion = selectedQuick = selectedMerge = false; }

    startX += MeasureText(Bubble, font) + 20;
    Button(startX, y, Insertion, GREEN, selectedInsertion);
    if (!prevInsertion && selectedInsertion) { sortingMenuOpen = false; selectedBubble = selectedQuick = selectedMerge = false; }

    startX += MeasureText(Insertion, font) + 20;
    Button(startX, y, Quick, GREEN, selectedQuick);
    if (!prevQuick && selectedQuick) { sortingMenuOpen = false; selectedBubble = selectedInsertion = selectedMerge = false; }

    startX += MeasureText(Quick, font) + 20;
    Button(startX, y, Merge, GREEN, selectedMerge);
    if (!prevMerge && selectedMerge) { sortingMenuOpen = false; selectedBubble = selectedInsertion = selectedQuick = false; }
}

void showStartOptions() {
    float font = (3.0 * GetScreenHeight()) / 100;
    char StartText[] = "Start Sorting!";
    float tmp = (27 * GetScreenWidth()) / 100;
    Start_Button(tmp, font, StartText);

    tmp += MeasureText(StartText, font) + 75;
    char StopText[] = "Stop Sorting!";
    Stop_Button(tmp, font, StopText);

    tmp += MeasureText(StopText, font) + 75;
    char RandomizeArrayText[] = "Randomize Array!";
    Button(tmp, GetScreenHeight()/20 + font*2, RandomizeArrayText, DARKGRAY, shouldRandomizeArray);

    addSpeed = subSpeed = false;

    tmp = (84.2 * GetScreenWidth()) / 100;
    char TimeButtonText[] = "Speed";
    Button(tmp, GetScreenHeight()/20 + font*2, TimeButtonText, BLUE, NormalSpeed);
    if (NormalSpeed) changeSpeed('/', sortingSpeed);

    tmp += MeasureText(TimeButtonText, font) + 20;
    char SpeedPlusButtonText[] = "+";
    Button(tmp, GetScreenHeight()/20 + font*2, SpeedPlusButtonText, ORANGE, addSpeed);
    if (addSpeed) changeSpeed('+', sortingSpeed);

    tmp += MeasureText(SpeedPlusButtonText, font) + 20;
    char SpeedMinusButtonText[] = "-";
    Button(tmp, GetScreenHeight()/20 + font*2, SpeedMinusButtonText, ORANGE, subSpeed);
    if (subSpeed) changeSpeed('-', sortingSpeed);

    addSize = subSize = NormalSize = false;

    tmp = (5 * GetScreenWidth()) / 100;
    char SizeButtonText[] = "Size";
    Button(tmp, GetScreenHeight()/20 + font*2, SizeButtonText, BLUE, NormalSize);
    if (NormalSize) changeSize('/', numberOfPillars);

    tmp += MeasureText(SizeButtonText, font) + 30;
    char SizePlusButton[] = "+";
    Button(tmp, GetScreenHeight()/20 + font*2, SizePlusButton, ORANGE, addSize);
    if (addSize) changeSize('+', numberOfPillars);

    tmp += MeasureText(SizePlusButton, font) + 20;
    char SizeMinusButton[] = "-";
    Button(tmp, GetScreenHeight()/20 + font*2, SizeMinusButton, ORANGE, subSize);
    if (subSize) changeSize('-', numberOfPillars);
}

void Start_Button(float size, float font, char Text[]) {
    Color color = ShouldStartSorting ? BLUE : GREEN;
    Button(size, GetScreenHeight()/20 + font * 2, Text, color, ShouldStartSorting);
}

void Stop_Button(float size, float font, char Text[]) {
    Color color = stopSorting ? RED : ORANGE;
    Button(size, GetScreenHeight()/20 + font * 2, Text, color, stopSorting);
}

void changeSpeed(char operation, int &value){
    switch (operation) {
        case  '-' : value = value - 10 > 0 ? value - 10 : value; break;
        case '+' : value += 10; break;
        default: value = 61; break;
    }
}

void changeSize(char operation, int &value){
    switch (operation) {
        case '-': if (value > 5){ value -= 5; for (int i = 0; i < 5; i++) arr.pop_back(); } break;
        case '+': value += 5; for (int i = 0; i < 5; i++) arr.push_back({GetRandomValue(40, MinWindowWidth-80), NORMAL}); break;
        default:
            while (numberOfPillars > 50){ numberOfPillars--; arr.pop_back(); }
            while (numberOfPillars < 50){ numberOfPillars++; arr.push_back({GetRandomValue(40, MinWindowWidth-80), NORMAL}); }
            break;
    }
    for (int i = 0; i < numberOfPillars; i++) arr[i].second = NORMAL;
}

Color FindColorForPillar(int pillarState){
    switch (pillarState) {
        case SELECTED : return GREEN;
        case SORTED : return DARKGREEN;
        default: return WHITE;
    }
}

void Button(float x, float y, char *Text, Color color, bool &state){
    float font = (2.5 * GetScreenHeight()) / 100;
    Rectangle r1 = { .x = x, .y = y, .width = (float) MeasureText(Text, font), .height = (float) font };
    if(CheckCollisionPointRec(GetMousePosition(), r1)){
        DrawText(Text, x , y, font, RED);
        if(IsMouseButtonPressed(0)) state = !state;
    } else {
        DrawText(Text, x, y, font, color);
    }
}

// ✅ Draw refresh-friendly highlight
void Highlight(int index, int state) {
    arr[index].second = state;
    this_thread::sleep_for(chrono::milliseconds(1000 / sortingSpeed));
    // PostEmptyEvent(); // Forces raylib to refresh frame (optional)
}

// ✅ Sorting Functions Below...
// (No changes needed in BubbleSort, QuickSort, etc. — they now work correctly with Highlight refresh)

void BubbleSort() {
    for (int i = 0; i < arr.size() - 1 && !stopSorting; i++) {
        for (int j = 0; j < arr.size() - i - 1 && !stopSorting; j++) {
            Highlight(j, SELECTED);
            Highlight(j+1, SELECTED);
            if (arr[j].first > arr[j+1].first) swap(arr[j], arr[j+1]);
            arr[j].second = NORMAL;
            arr[j+1].second = NORMAL;
        }
        arr[arr.size() - i - 1].second = SORTED;
    }
    if (!stopSorting) for (auto &p : arr) p.second = SORTED;
}

void InsertionSort() {
    for (int i = 1; i < arr.size() && !stopSorting; i++) {
        int key = arr[i].first;
        int j = i - 1;
        while (j >= 0 && arr[j].first > key && !stopSorting) {
            arr[j + 1].first = arr[j].first;
            Highlight(j + 1, SELECTED);
            Highlight(j, NORMAL);
            j--;
        }
        arr[j + 1].first = key;
        Highlight(j + 1, SORTED);
    }
    if (!stopSorting) for (auto &p : arr) p.second = SORTED;
}

int Partition(int low, int high) {
    int pivot = arr[high].first;
    int i = low - 1;
    for (int j = low; j < high && !stopSorting; j++) {
        if (arr[j].first < pivot) {
            i++;
            swap(arr[i], arr[j]);
            Highlight(i, SELECTED);
            Highlight(j, SELECTED);
        }
    }
    swap(arr[i + 1], arr[high]);
    Highlight(i+1, SORTED);
    return i + 1;
}

void QuickSort(int low, int high) {
    if (low < high && !stopSorting) {
        int pi = Partition(low, high);
        QuickSort(low, pi - 1);
        QuickSort(pi + 1, high);
    }
    if (!stopSorting) for (int i = low; i <= high; i++) arr[i].second = SORTED;
}

void Merge(int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i].first;
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j].first;

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2 && !stopSorting) {
        if (L[i] <= R[j]) arr[k++].first = L[i++];
        else arr[k++].first = R[j++];
        Highlight(k - 1, SELECTED);
    }
    while (i < n1 && !stopSorting) { arr[k++].first = L[i++]; Highlight(k-1, SELECTED); }
    while (j < n2 && !stopSorting) { arr[k++].first = R[j++]; Highlight(k-1, SELECTED); }
}

void MergeSort(int l, int r) {
    if (l < r && !stopSorting) {
        int m = l + (r - l) / 2;
        MergeSort(l, m);
        MergeSort(m + 1, r);
        Merge(l, m, r);
    }
    if (!stopSorting) for (int i = l; i <= r; i++) arr[i].second = SORTED;
}
