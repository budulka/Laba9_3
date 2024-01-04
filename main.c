
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

enum MenuButtons {
    CreateFileButton,
    ReadAFile,
    DeleteFileButton,
    AppendButton,
    ShowInfoButton,
    EditButton,
    SortButton,
    InsertButton,
    RemoveButton,
    ExitButton,
    InfoButton,
    WeirdButton
};

typedef struct  Button
{
    enum MenuButtons ButtonIndex;
    char* name;
} Button;

typedef struct {
    int area;
    int population;
    char name[32];
} Region;

void initializeButtons();
void chooseButton(enum MenuButtons button);
void updateUIMenu();
void readKeyDirection();
void createFile();
void deleteFile();
void readAFile();
void appendData();
void editData();
void sortData();
void removeData();
void showData();
void insertData();
void inputFileName(char*);
void updateRegion(Region*);
void createArray(Region**, FILE*, int*);
void updateFiles(FILE*, FILE*, char*);
void activateFunctionForButton(enum MenuButtons button);

char* ButtonNames[16] = { "Create a file",
                          "Read a File",
                          "Delete a file",
                          "Append data" ,
                          "Read data",
                          "Edit data",
                          "Sort data",
                          "Insert data",
                          "Remove data" };

int ChosenButton = CreateFileButton;
Button Buttons[9];

int main() {
    initializeButtons();
    while (1) {
        updateUIMenu();
        readKeyDirection();
        chooseButton(ChosenButton);
    }
}

void readKeyDirection()
{
    int key = _getch();
    switch (key) {
        case 97:
            ChosenButton--;
            if (ChosenButton == -1)
                ChosenButton = 8;
            break;
        case 100:
            ChosenButton++;
            if (ChosenButton == 9)
                ChosenButton = 0;
            break;
        case 115:
            ChosenButton += 3;
            if (ChosenButton > 8) ChosenButton -= 9;
            break;
        case 119:
            ChosenButton -= 3;
            if (ChosenButton < 0) ChosenButton = 9 + ChosenButton;
            break;
        case 13:
            activateFunctionForButton(ChosenButton);
            break;
        default:
            break;
    }
}

void initializeButtons()
{
    size_t countOfButtons = sizeof(Buttons) / sizeof(Buttons[0]);
    Buttons[0].name = "->CreateFile";
    for (int i = 1; i < countOfButtons; i++)
    {
        Buttons[i].ButtonIndex = i;
        Buttons[i].name = ButtonNames[i];
    }
}

void chooseButton(enum MenuButtons button)
{
    size_t countOfButtons = sizeof(Buttons) / sizeof(Buttons[0]);
    for (int i = 0; i < (int)countOfButtons; i++) Buttons[i].name = ButtonNames[i];
    char newFileName[32] = "->";
    strncat(newFileName, ButtonNames[button], sizeof(newFileName) - strlen(newFileName) - 1);
    Buttons[button].name = _strdup(newFileName);
}

void activateFunctionForButton(enum MenuButtons button)
{
    switch (button) {
        case CreateFileButton:
            createFile();
            break;
        case ReadAFile:
            readAFile();
            break;
        case DeleteFileButton:
            deleteFile();
            break;
        case AppendButton:
            appendData();
            break;
        case ShowInfoButton:
            showData();
            break;
        case EditButton:
            editData();
            break;
        case SortButton:
            sortData();
            break;
        case InsertButton:
            insertData();
            break;
        case RemoveButton:
            removeData();
            break;
    }
    /*
    if (button == CreateFileButton)
        createFile();
    else if (button == ReadAFile) {
        readAFile();
    }
    else if (button == DeleteFileButton) {
        deleteFile();
    }
    else if (button == AppendButton) {
        appendData();
    }
    else if (button == ShowInfoButton) {
        showData();
    }
    else if (button == EditButton) {
        editData();
    }
    else if (button == SortButton) {
        sortData();
    }
    else if(button == InsertButton) {
        insertData();
    }
    else if(button == RemoveButton) {
        removeData();
    }
     */
}

void updateUIMenu()
{
    system("cls");
    printf("________________________________________________________________________\n");
    printf("|\t%s\t|\t%s\t|\t%s\t|\n", Buttons[0].name, Buttons[1].name, Buttons[2].name);
    printf("------------------------------------------------------------------------\n");
    printf("|\t%s\t|\t%s\t|\t%s\t|\n", Buttons[3].name, Buttons[4].name, Buttons[5].name);
    printf("------------------------------------------------------------------------\n");
    printf("|\t%s\t|\t%s\t|\t%s\t|\n", Buttons[6].name, Buttons[7].name, Buttons[8].name);
    printf("------------------------------------------------------------------------\n");
}



void createFile() {
    FILE *fileToCreate;
    char fileName[16];
    inputFileName(fileName);
    if (fopen(fileName, "r") != NULL) {
        printf("Such file already exists");
    } else {
        fileToCreate = fopen(fileName, "w");
        printf("The file was created");
    }
    fclose(fileToCreate);
    getch();
}

void deleteFile()
{
    char fileToDelete[16];
    inputFileName(fileToDelete);
    fflush(stdin);
    int res = remove(fileToDelete);
    if (res == 0) {
        printf("The file %s was deleted", fileToDelete);
    } else {
        printf("Unable to delete that file!");
    }
    getch();
}

void readAFile() {
    char fileNameToRead[16];
    Region newRegion;
    inputFileName(fileNameToRead);
    FILE* fileToRead;
    if (fopen(fileNameToRead, "r") == NULL) {
     printf("Such file does not exist");
     getch();
    } else {
        fileToRead = fopen(fileNameToRead, "r");
        while (fscanf(fileToRead, "Name: %64[^\n]\nArea: %d\nPopulation: %d\n", newRegion.name, &newRegion.area, &newRegion.population) != EOF)
            printf("Name: %s\nArea: %d\nPopulation: %d\n", newRegion.name, newRegion.area, newRegion.population);
        fclose(fileToRead);
    }
    getch();
}

void appendData() {
    char fileNameToAppend[16];
    inputFileName(fileNameToAppend);
    FILE* fileToAppend = fopen(fileNameToAppend,"r");
    if (fileToAppend == NULL) {
        printf("Such file does not exist!");
        getch();
    } else {
        Region newRegion;
        updateRegion(&newRegion);
        fileToAppend = fopen(fileNameToAppend, "a");
        fprintf(fileToAppend, "Name: %s\nArea: %d\nPopulation: %d\n", newRegion.name, newRegion.area,
                newRegion.population);
        fclose(fileToAppend);
        printf("The file was updated!");
        getch();
    }
}

void editData() {
    char fileToEdit[16];
    FILE *file, *temp;
    inputFileName(fileToEdit);
    file = fopen(fileToEdit, "r");
    if (file == NULL) {
        printf("Such file does not exist!");
        getch();
    } else {
        Region newRegion;
        char regionName[16];
        printf("What region do you want to edit?: ");
        scanf("%15s", regionName);
        temp = fopen("temp", "w");
        short i = 0;
        while (fscanf(file, "Name: %15[^\n]\nArea: %d\nPopulation: %d\n", newRegion.name, &newRegion.area, &newRegion.population) != EOF) {
            if (strcmp(newRegion.name, regionName) == 0) {
                printf("Enter new area for the region: ");
                scanf("%d", &newRegion.area);
                printf("Enter new population for the region: ");
                scanf("%d", &newRegion.population);
                i = 1;
            }
            fprintf(temp, "Name: %s\nArea: %d\nPopulation: %d\n", newRegion.name, newRegion.area, newRegion.population);
        }
        if (i == 0) {
            printf("Such region does not exist!");
            getch();
        }
        updateFiles(temp, file, fileToEdit);
        getch();
    }
}

void showData() {
    char fileToRead[16];
    char regionName[16];
    Region region;
    inputFileName(fileToRead);
    FILE* file = fopen(fileToRead, "r");
    printf("What region do you want to find out about?:");
    scanf("%15s", regionName);
    while (fscanf(file, "Name: %s\nArea: %d\nPopulation: %d\n", region.name, &region.area, &region.population) != EOF) {
        if (strcmp(region.name, regionName) == 0) {
            break;
        }
    }
    printf("Name: %s\nArea: %d\nPopulation: %d\n", region.name, region.area, region.population);
    fclose(file);
    getch();
}

void sortData() {
    FILE* file, *temp;
    char fileToRead[16];
    inputFileName(fileToRead);
    int n = 0;
    Region* regions = (Region*)malloc(1 * sizeof(Region));
    file = fopen(fileToRead, "r");
    createArray(&regions, file, &n);
    fclose(file);
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(regions[i].name, regions[j].name) > 0) {
                Region tmp = regions[i];
                regions[i] = regions[j];
                regions[j] = tmp;
            }
        }
    }
    temp = fopen("temp", "w");
    for (int i = 0; i < n; ++i) {
        fprintf(temp, "Name: %s\nArea: %d\nPopulation: %d\n", regions[i].name, regions[i].area, regions[i].population);
    }
    free(regions);
    updateFiles(temp, file, fileToRead);
    printf("The data was sorted!");
    getch();
}

void insertData() {
    char fileToRead[16];
    Region toInsert;
    inputFileName(fileToRead);
    FILE* file, *temp;
    int n = 0;
    Region* regions = (Region*)malloc(1 * sizeof(Region));
    file = fopen(fileToRead, "r");
    createArray(&regions, file, &n);
    updateRegion(&toInsert);
    temp = fopen("temp", "w");
    int flag = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(regions[i].name, toInsert.name) > 0 && flag == 0) {
            fprintf(temp, "Name: %s\nArea: %d\nPopulation: %d\n", toInsert.name, toInsert.area, toInsert.population);
            flag = 1;
        }
        fprintf(temp, "Name: %s\nArea: %d\nPopulation: %d\n", regions[i].name, regions[i].area, regions[i].population);
    }
    if (flag == 0) {
        fprintf(temp, "Name: %s\nArea: %d\nPopulation: %d\n", toInsert.name, toInsert.area, toInsert.population);
    }
    free(regions);
    updateFiles(temp, file, fileToRead);
    getch();
}

void removeData() {
    char fileToEdit[16];
    FILE *file, *temp;
    inputFileName(fileToEdit);
    file = fopen(fileToEdit, "r");
    if (file == NULL) {
        printf("Such file does not exist!");
        getch();
    } else {
        Region newRegion;
        char regionName[16];
        printf("What region do you want to edit?: ");
        scanf("%15s", regionName);
        temp = fopen("temp", "w");
        while (fscanf(file, "Name: %15[^\n]\nArea: %d\nPopulation: %d\n", newRegion.name, &newRegion.area, &newRegion.population) != EOF) {
            if (strcmp(newRegion.name, regionName) != 0) {
                fprintf(temp, "Name: %s\nArea: %d\nPopulation: %d\n", newRegion.name, newRegion.area, newRegion.population);
            }
        }
        printf("The data was deleted!");
        updateFiles(temp, file, fileToEdit);
        getch();
    }
}

void updateFiles(FILE*temp, FILE*file, char* filename) {
    fclose(temp);
    fclose(file);
    remove(filename);
    rename("temp", filename);
}

void updateRegion(Region* newRegion) {
    printf("\n\nEnter the name of the region: ");
    scanf("%15s", newRegion->name);
    printf("Enter the area of the region: ");
    scanf("%d", &newRegion->area);
    printf("Enter the population of the region: ");
    scanf("%d", &newRegion->population);
}

void createArray(Region** regions, FILE* file, int* n) {
    Region region;
    while (fscanf(file, "Name: %s\nArea: %d\nPopulation: %d\n", region.name, &region.area, &region.population) != EOF) {
        *regions = (Region*)realloc(*regions, (*n + 1) * sizeof(Region));
        strcpy((*regions)[*n].name, region.name);
        (*regions)[*n].area = region.area;
        (*regions)[*n].population = region.population;
        (*n)++;
    }
}

void inputFileName(char*string) {
    printf("Enter file name:");
    int a = scanf("%15s", string);
    while (a != 1 || getchar() != '\n') {
        printf("You name is too long!");
        fflush(stdin);
    }
}