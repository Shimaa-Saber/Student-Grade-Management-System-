#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

struct Subject {
    char sub_name[50];
    float score;
};

typedef struct Subject Subject;

struct Student {
    int id;
    char name[50];
    Subject* subjects;
    int subj_count;
    float avgScore;
    char grade;
};

typedef struct Student Student;

COORD coord = {0, 0};

void gotoxy(int x, int y) {
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int ForgC) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

Student* Stus[100];
int StudentCount = 0;

float CalculateAverage(Student* student) {
    if (student->subj_count == 0) return 0;

    float sum = 0;
    for (int i = 0; i < student->subj_count; i++) {
        sum += student->subjects[i].score;
    }
    return sum / student->subj_count;
}

char AssignGrade(float average) {
    if (average >= 90) return 'A';
    else if (average >= 80) return 'B';
    else if (average >= 70) return 'C';
    else if (average >= 60) return 'D';
    return 'F';
}

void AddStudent() {
    system("cls");
    printf("=== Add New Student ===\n\n");

    Student* s = (Student*)malloc(sizeof(Student));
    if (!s) {
        printf("Error: Unable to allocate memory for student.\n");
        return;
    }

   int isDuplicate = 0;
    do {
        printf("Enter Student ID: ");
        scanf("%d", &s->id);

        isDuplicate = 0;
        for (int i = 0; i < StudentCount; i++) {
            if (Stus[i]->id == s->id) {
                isDuplicate = 1;
                printf("Error: Student ID %d already exists. Please enter a unique ID.\n", s->id);
                break;
            }
        }
    } while (isDuplicate);
    printf("Enter Student Name: ");
    scanf("%49s", s->name);

    printf("Enter the number of subjects for %s: ", s->name);
    scanf("%d", &s->subj_count);

    s->subjects = (Subject*)malloc(s->subj_count * sizeof(Subject));
    if (!s->subjects) {
        printf("Error: Unable to allocate memory for subjects.\n");
        free(s);
        return;
    }

    for (int i = 0; i < s->subj_count; i++) {
        printf("\nEnter the name of subject %d: ", i + 1);
        scanf("%49s", s->subjects[i].sub_name);
        printf("Enter the score for %s: ", s->subjects[i].sub_name);
        scanf("%f", &s->subjects[i].score);
    }

    s->avgScore = CalculateAverage(s);
    s->grade = AssignGrade(s->avgScore);
    Stus[StudentCount++] = s;

    printf("\nStudent added successfully!\n");
    printf("==========================\n");

    printf("==========================\n");
    system("pause");
}

void UpdateStudent() {
    if (StudentCount == 0) {
        printf("No students available to edit.\n");
        system("pause");
        return;
    }

    int id, found = 0;
    printf("\nEnter the Student ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < StudentCount; i++) {
        if (Stus[i]->id == id) {
            found = 1;

            printf("\nStudent Found:\n");
            printf("ID: %d\nName: %s\n", Stus[i]->id, Stus[i]->name);

            printf("\nWhat would you like to update?\n");
            printf("1. Name\n2. Scores of Subjects\nEnter your choice: ");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                printf("\nEnter the new name: ");
                scanf("%49s", Stus[i]->name);
                printf("Name updated successfully!\n");
            }
            else if (choice == 2) {
                printf("\nUpdating scores for %s...\n", Stus[i]->name);
                while (1) {
                    printf("\nSubjects and Scores:\n");
                    for (int j = 0; j < Stus[i]->subj_count; j++) {
                        printf("%d. %s: %.2f\n", j + 1, Stus[i]->subjects[j].sub_name, Stus[i]->subjects[j].score);
                    }

                    printf("\nEnter the number of the subject to update (or 0 to finish): ");
                    int subj_choice;
                    scanf("%d", &subj_choice);

                    if (subj_choice == 0) {
                        printf("Finished updating scores.\n");
                        break;
                    }

                    if (subj_choice < 1 || subj_choice > Stus[i]->subj_count) {
                        printf("Invalid choice. Please try again.\n");
                    }
                    else {
                        int index = subj_choice - 1;
                        printf("Enter the new score for %s: ", Stus[i]->subjects[index].sub_name);
                        scanf("%f", &Stus[i]->subjects[index].score);
                        printf("Score for %s updated successfully!\n", Stus[i]->subjects[index].sub_name);
                    }
                }
            }
            else {
                printf("Invalid choice. No updates made.\n");
            }

            Stus[i]->avgScore = CalculateAverage(Stus[i]);
            Stus[i]->grade = AssignGrade(Stus[i]->avgScore);
            printf("\nUpdated Details:\n");
            printf("Average Score: %.2f\nGrade: %c\n", Stus[i]->avgScore, Stus[i]->grade);
           return;
        }

    }

    if(!found){

      printf("Student with ID %d not found.\n", id);
    }

}



void DisplayStudents() {
    system("cls");
    printf("=== List of Students ===\n\n");

    if (StudentCount == 0) {
        printf("No students to display!\n");
    } else {
        for (int i = 0; i < StudentCount; i++) {
            printf("Student #%d\n", i + 1);
            printf("ID: %d\n", Stus[i]->id);
            printf("Name: %s\n", Stus[i]->name);
            printf("Average Score: %.2f\n", Stus[i]->avgScore);
            printf("Grade: %c\n", Stus[i]->grade);
            printf("------------------------\n");
        }
    }
    system("pause");
}

void DisplayHighAvgScore() {
    system("cls");
    printf("=== Student with Highest Average Score ===\n\n");

    if (StudentCount == 0) {
        printf("No students available.\n");
    } else {
        int index = 0;
        float highAvg = Stus[0]->avgScore;

        for (int i = 1; i < StudentCount; i++) {
            if (Stus[i]->avgScore > highAvg) {
                highAvg = Stus[i]->avgScore;
                index = i;
            }
        }

        printf("Student Name: %s\n", Stus[index]->name);
        printf("ID: %d\n", Stus[index]->id);
        printf("Average Score: %.2f\n", Stus[index]->avgScore);
        printf("Grade: %c\n", Stus[index]->grade);
    }
    printf("------------------------\n");
    system("pause");
}

void DisplayAllAvgScores() {
    system("cls");
    printf("=== Average Scores of All Students ===\n\n");

    if (StudentCount == 0) {
        printf("No students to display.\n");
    } else {
        for (int i = 0; i < StudentCount; i++) {
            printf("%s: %.2f\n", Stus[i]->name, Stus[i]->avgScore);
        }
    }
    printf("------------------------\n");
    system("pause");
}

void StudentsPassed() {
    system("cls");
    printf("=== Students Who Passed ===\n\n");

    int found = 0;
    if (StudentCount == 0) {
        printf("No students to display.\n");
    } else {
        for (int i = 0; i < StudentCount; i++) {
            if (Stus[i]->avgScore >= 60) {
                printf("ID: %d\n", Stus[i]->id);
                printf("Name: %s\n", Stus[i]->name);
                printf("Average Score: %.2f\n", Stus[i]->avgScore);
                printf("Grade: %c\n", Stus[i]->grade);
                printf("------------------------\n");
                found = 1;
            }
        }
        if (!found) {
            printf("No students passed.\n");
        }
    }
    system("pause");
}

void StudentsFailed() {
    system("cls");
    printf("=== Students Who Failed ===\n\n");

    int found = 0;
    if (StudentCount == 0) {
        printf("No students to display.\n");
    } else {
        for (int i = 0; i < StudentCount; i++) {
            if (Stus[i]->avgScore < 60) {
                printf("ID: %d\n", Stus[i]->id);
                printf("Name: %s\n", Stus[i]->name);
                printf("Average Score: %.2f\n", Stus[i]->avgScore);
                printf("Grade: %c\n", Stus[i]->grade);
                printf("------------------------\n");
                found = 1;
            }
        }
        if (!found) {
            printf("No students failed.\n");
        }
    }
    system("pause");
}

int main() {
    char menuOptions[9][100] = {
        "1- Add Student",
        "2- Update Student",
        "3- Display All Students",
        "4- Display Student with Highest Average Score",
        "5- Display Average Scores of All Students",
        "6- Display Passed Students",
        "7- Display Failed Students",
        "8- Exit"
    };

    int selectedColor = 0;
    char ch;

    while (1) {
        system("cls");
        gotoxy(0, 0);

         for (int i = 0; i < 8; i++) {
        if (i == selectedColor) {
            if (i == 7) {
                SetColor(12);
            } else {
                SetColor(10);
            }
            printf(">> %s\n", menuOptions[i]);
        } else {

                SetColor(15);

            printf("   %s\n", menuOptions[i]);
        }
    }
        ch = getch();
        if (ch == 13) {
            system("cls");
            switch (selectedColor) {
                case 0: AddStudent(); break;
                case 1: UpdateStudent();break;
                case 2: DisplayStudents(); break;
                case 3: DisplayHighAvgScore(); break;
                case 4: DisplayAllAvgScores(); break;
                case 5: StudentsPassed(); break;
                case 6: StudentsFailed(); break;
                case 7: printf("Exiting program...\n"); exit(0);
            }
        } else if (ch == -32) {
            ch = getch();
            if (ch == 72) {
                selectedColor = (selectedColor == 0) ? 7 : selectedColor - 1;
            } else if (ch == 80) {
                selectedColor = (selectedColor == 7) ? 0 : selectedColor + 1;
            }
        }
    }

    return 0;
}
