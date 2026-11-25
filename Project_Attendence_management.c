#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define RED               "\033[1;31m"
#define GREEN             "\033[1;32m"
#define YELLOW            "\033[1;33m"
#define BLUE              "\033[1;34m"
#define MAGENTA           "\033[1;35m"
#define CYAN              "\033[1;36m"
#define WHITE             "\033[1;37m"
#define GOLD              "\033[1;38;5;220m"
#define SKY_BLUE          "\033[1;38;5;117m"
#define SILVER            "\033[1;38;5;250m"
#define GRAY              "\033[0;37m"
#define BRIGHT_RED        "\033[1;91m"
#define BRIGHT_GREEN      "\033[1;92m"
#define BRIGHT_CYAN       "\033[1;96m"
#define ORANGE            "\033[1;38;5;208m"
#define INDIGO            "\033[1;38;5;99m"
#define VIOLET            "\033[1;38;5;177m"

#define BG_RED            "\033[41;1;37m"
#define BG_GREEN          "\033[42;1;30m"
#define BG_YELLOW         "\033[43;1;30m"
#define BG_BLUE           "\033[44;1;37m"
#define BG_MAGENTA        "\033[45;1;37m"
#define BG_CYAN           "\033[46;1;30m"
#define BG_WHITE          "\033[47;1;30m"
#define BG_GRAY           "\033[100;1;97m"
#define BG_BRIGHT_RED     "\033[101;1;30m"
#define BG_BRIGHT_GREEN   "\033[102;1;30m"
#define BG_BRIGHT_YELLOW  "\033[103;1;30m"
#define BG_BRIGHT_BLUE    "\033[104;1;30m"
#define BG_BRIGHT_MAGENTA "\033[105;1;30m"
#define BG_BRIGHT_CYAN    "\033[106;1;30m"

#define BG_ORANGE         "\033[48;5;208;1;30m"
#define BG_GOLD           "\033[48;5;220;1;30m"
#define BG_LIME           "\033[48;5;118;1;30m"
#define BG_MINT           "\033[48;5;121;1;30m"
#define BG_TEAL           "\033[48;5;30;1;97m"
#define BG_SKY_BLUE       "\033[48;5;117;1;30m"
#define BG_NAVY           "\033[48;5;18;1;97m"
#define BG_INDIGO         "\033[48;5;54;1;97m"
#define BG_VIOLET         "\033[48;5;129;1;97m"
#define BG_PURPLE         "\033[48;5;93;1;97m"
#define BG_PINK           "\033[48;5;205;1;30m"
#define BG_PEACH          "\033[48;5;216;1;30m"
#define BG_MAROON         "\033[48;5;52;1;97m"
#define BG_BROWN          "\033[48;5;94;1;97m"
#define BG_CHARCOAL       "\033[48;5;238;1;97m"

#define RESET             "\033[0m"

struct Student {
    char dept[20];
    char course[20];
    int rollNo;
    char name[50];
};

struct Attendance {
    char date[15];
    int month;
    int year;
    int classNo;
    char dept[20];
    char course[20];
    int rollNo;
    char status;
    char markedBy[30];
};

struct User {
    char username[30];
    char password[30];
    char role[15];
};

int getValidInt(const char prompt[], int min, int max) {
    int val;
    char extra;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1) {
            int extraChars = 0;
            while ((extra = getchar()) != '\n' && extra != EOF) {
                if (!isspace(extra)) extraChars = 1;
            }
            if (!extraChars && val >= min && val <= max) {
                return val;
            }
        } else {
            while (getchar() != '\n');
        }
        printf(RED "Invalid input! Please enter a valid number." RESET "\n");
    }
}

int getValidDay(int month, int year) {
    int maxDays = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDays = 30;
    } else if (month == 2) {
        int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        maxDays = isLeap ? 29 : 28;
    }
    return getValidInt(GOLD "Enter Day: " RESET, 1, maxDays);
}

void getValidString(const char prompt[], char dest[], int maxLen) {
    char buffer[200];
    while (1) {
        printf("%s", prompt);
        scanf(" %199[^\n]", buffer);
        while (getchar() != '\n');

        if (strchr(buffer, ',') != NULL) {
            printf(RED "Commas (,) are not allowed! Please re-enter." RESET "\n");
        } else if ((int)strlen(buffer) >= maxLen) {
            printf(RED "Too long! Please enter a shorter value." RESET "\n");
        } else {
            strcpy(dest, buffer);
            break;
        }
    }
}

void toUpperStr(char str[]) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int canAccessRecord(const char markedBy[], const char currentUser[], const char currentRole[]) {
    if (strcmp(currentRole, "ADMIN") == 0) {
        return 1;
    }
    return strcmp(markedBy, currentUser) == 0;
}

void filescreation() {
    FILE *fp = fopen("students.csv", "r");
    if (fp == NULL) {
        fp = fopen("students.csv", "w");
        if (fp != NULL) {
            fprintf(fp, "Department,Course,RollNo,Name\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }

    fp = fopen("attendance.csv", "r");
    if (fp == NULL) {
        fp = fopen("attendance.csv", "w");
        if (fp != NULL) {
            fprintf(fp, "Date,Month,Year,ClassNo,Department,Course,RollNo,Status,MarkedBy\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }

    fp = fopen("users.csv", "r");
    if (fp == NULL) {
        fp = fopen("users.csv", "w");
        if (fp != NULL) {
            fprintf(fp, "Username,Password,Role\n");
            fprintf(fp, "admin,admin123,ADMIN\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }
}

void registerstudent(const char currentRole[]) {
    system("cls");
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf(BG_RED " Access Denied! Only the ADMIN can register students. " RESET "\n");
        return;
    }

    struct Student s, temp;
    char line[200];
    int count = 0;

    printf(BG_CYAN " --- REGISTER STUDENTS --- " RESET "\n");
    getValidString(GOLD "Enter Department: " RESET, s.dept, sizeof(s.dept));
    toUpperStr(s.dept);

    getValidString(GOLD "Enter Course: " RESET, s.course, sizeof(s.course));
    toUpperStr(s.course);

    printf(GRAY "(Enter 0 as Roll Number when finished)" RESET "\n");

    while (1) {
        printf("\n");
        s.rollNo = getValidInt(GOLD "Enter Roll Number: " RESET, 0, 99999);

        if (s.rollNo == 0) {
            break;
        }

        int duplicate = 0;
        FILE *fp = fopen("students.csv", "r");
        if (fp != NULL) {
            fgets(line, sizeof(line), fp);
            while (fgets(line, sizeof(line), fp)) {
                if (sscanf(line, "%[^,],%[^,],%d,%[^\n]", temp.dept, temp.course, &temp.rollNo, temp.name) == 4) {
                    if (strcmp(temp.dept, s.dept) == 0 && strcmp(temp.course, s.course) == 0 && temp.rollNo == s.rollNo) {
                        duplicate = 1;
                        break;
                    }
                }
            }
            fclose(fp);
        }

        if (duplicate) {
            printf(YELLOW "Roll Number %d already exists in %s - %s! Try another." RESET "\n", s.rollNo, s.dept, s.course);
            continue;
        }

        getValidString(GOLD "Enter Student Name: " RESET, s.name, sizeof(s.name));

        fp = fopen("students.csv", "a");
        if (fp == NULL) {
            printf(RED "Error: Close students.csv in Excel first!" RESET "\n");
            return;
        }
        fprintf(fp, "%s,%s,%d,%s\n", s.dept, s.course, s.rollNo, s.name);
        fclose(fp);

        count++;
        printf(GREEN "Saved: %s (Roll %d | %s - %s)" RESET "\n", s.name, s.rollNo, s.dept, s.course);
    }

    system("cls");
    printf(BG_GREEN " Total %d student(s) registered in %s - %s! " RESET "\n", count, s.dept, s.course);
}

void markattendance(const char currentUser[]) {
    system("cls");
    char dept[20], course[20], date[15], line[200], status;
    int day, month, year, classNo, found = 0;
    struct Student s;
    struct Attendance a;

    printf(BG_CYAN " --- MARK CLASS ATTENDANCE [%s] --- " RESET "\n", currentUser);
    getValidString(GOLD "Enter Department: " RESET, dept, sizeof(dept));
    toUpperStr(dept);

    getValidString(GOLD "Enter Course: " RESET, course, sizeof(course));
    toUpperStr(course);

    year = getValidInt(GOLD "Enter Year: " RESET, 2026, 2030);
    month = getValidInt(GOLD "Enter Month: " RESET, 1, 12);
    day = getValidDay(month, year);
    sprintf(date, "%02d-%02d-%04d", day, month, year);

    classNo = getValidInt(GOLD "Enter Class / Period Number: " RESET, 1, 10);

    FILE *fpAtt = fopen("attendance.csv", "r");
    if (fpAtt != NULL) {
        fgets(line, sizeof(line), fpAtt);
        while (fgets(line, sizeof(line), fpAtt)) {
            if (sscanf(line, "%[^,],%d,%d,%d,%[^,],%[^,],%d,%c,%[^\n]",
                       a.date, &a.month, &a.year, &a.classNo, a.dept, a.course, &a.rollNo, &a.status, a.markedBy) == 9) {
                if (strcmp(a.date, date) == 0 && a.classNo == classNo &&
                    strcmp(a.dept, dept) == 0 && strcmp(a.course, course) == 0 &&
                    strcmp(a.markedBy, currentUser) == 0) {
                    system("cls");
                    printf(BG_YELLOW " You already marked Class %d attendance for %s - %s on %s! " RESET "\n",
                           classNo, dept, course, date);
                    fclose(fpAtt);
                    return;
                }
            }
        }
        fclose(fpAtt);
    }

    FILE *fpStu = fopen("students.csv", "r");
    fpAtt = fopen("attendance.csv", "a");
    if (fpStu == NULL || fpAtt == NULL) {
        printf(RED "Error opening CSV files!" RESET "\n");
        if (fpStu) fclose(fpStu);
        if (fpAtt) fclose(fpAtt);
        return;
    }

    fgets(line, sizeof(line), fpStu);
    printf("\n" CYAN "Mark Class %d Attendance (" GREEN "P = Present" CYAN ", " RED "A = Absent" CYAN "):" RESET "\n", classNo);
    while (fgets(line, sizeof(line), fpStu)) {
        if (sscanf(line, "%[^,],%[^,],%d,%[^\n]", s.dept, s.course, &s.rollNo, s.name) == 4) {
            if (strcmp(s.dept, dept) == 0 && strcmp(s.course, course) == 0) {
                found = 1;
                while (1) {
                    char extra;
                    printf(SKY_BLUE "Roll %d" RESET " - " WHITE "%s" RESET ": ", s.rollNo, s.name);
                    scanf(" %c", &status);
                    status = toupper(status);
                    int extraChars = 0;
                    while ((extra = getchar()) != '\n' && extra != EOF) {
                        if (!isspace(extra)) extraChars = 1;
                    }

                    if (!extraChars && (status == 'P' || status == 'A')) {
                        break;
                    }
                    printf(RED "Invalid! Enter P or A only." RESET "\n");
                }
                fprintf(fpAtt, "%s,%d,%d,%d,%s,%s,%d,%c,%s\n",
                        date, month, year, classNo, dept, course, s.rollNo, status, currentUser);
            }
        }
    }
    fclose(fpStu);
    fclose(fpAtt);

    system("cls");
    if (found) printf(BG_GREEN " Class %d Attendance Saved by %s for %s - %s on %s! " RESET "\n",
                      classNo, currentUser, dept, course, date);
    else printf(BG_RED " No students found in %s - %s! " RESET "\n", dept, course);
}

void alterstudentname(const char currentRole[]) {
    system("cls");
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf(BG_RED " Access Denied! Only the ADMIN can alter student names. " RESET "\n");
        return;
    }

    char dept[20], course[20], newName[50], line[200];
    int rollNo, updated = 0;
    struct Student s;

    printf(BG_BLUE " --- ALTER STUDENT NAME --- " RESET "\n");
    getValidString(GOLD "Enter Department: " RESET, dept, sizeof(dept));
    toUpperStr(dept);

    getValidString(GOLD "Enter Course: " RESET, course, sizeof(course));
    toUpperStr(course);

    rollNo = getValidInt(GOLD "Enter Roll Number: " RESET, 1, 99999);

    FILE *fp = fopen("students.csv", "r");
    FILE *temp = fopen("temp_students.csv", "w");
    if (fp == NULL || temp == NULL) {
        printf(RED "Error opening student records!" RESET "\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    if (fgets(line, sizeof(line), fp)) {
        fprintf(temp, "%s", line);
    }

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^,],%[^,],%d,%[^\n]", s.dept, s.course, &s.rollNo, s.name) == 4) {
            if (strcmp(s.dept, dept) == 0 && strcmp(s.course, course) == 0 && s.rollNo == rollNo) {
                printf(CYAN "Current Name: " WHITE "%s" RESET "\n", s.name);
                getValidString(GOLD "Enter New Name: " RESET, newName, sizeof(newName));
                fprintf(temp, "%s,%s,%d,%s\n", s.dept, s.course, s.rollNo, newName);
                updated = 1;
            } else {
                fprintf(temp, "%s", line);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    if (updated) {
        remove("students.csv");
        rename("temp_students.csv", "students.csv");
        system("cls");
        printf(BG_GREEN " Student name updated to '%s' for Roll %d (%s - %s)! " RESET "\n", newName, rollNo, dept, course);
    } else {
        remove("temp_students.csv");
        system("cls");
        printf(BG_RED " Student Roll %d not found in %s - %s! " RESET "\n", rollNo, dept, course);
    }
}

void alterattendance(const char currentUser[], const char currentRole[]) {
    system("cls");
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf(BG_RED " Access Denied! Only the ADMIN can alter attendance records. " RESET "\n");
        return;
    }

    char dept[20], course[20], date[15], line[200], newStatus, targetTeacher[30];
    int rollNo, day, month, year, classNo, updated = 0;
    struct Attendance a;

    printf(BG_BLUE " --- ALTER STUDENT ATTENDANCE --- " RESET "\n");
    getValidString(GOLD "Enter Department: " RESET, dept, sizeof(dept));
    toUpperStr(dept);

    getValidString(GOLD "Enter Course: " RESET, course, sizeof(course));
    toUpperStr(course);

    rollNo = getValidInt(GOLD "Enter Roll Number: " RESET, 1, 99999);
    year = getValidInt(GOLD "Enter Year: " RESET, 2026, 2030);
    month = getValidInt(GOLD "Enter Month: " RESET, 1, 12);
    day = getValidDay(month, year);
    sprintf(date, "%02d-%02d-%04d", day, month, year);
    classNo = getValidInt(GOLD "Enter Class / Period Number: " RESET, 1, 10);
    getValidString(GOLD "Enter Username who marked it: " RESET, targetTeacher, sizeof(targetTeacher));

    FILE *fp = fopen("attendance.csv", "r");
    FILE *temp = fopen("temp_attendance.csv", "w");
    if (fp == NULL || temp == NULL) {
        printf(RED "Error opening attendance records!" RESET "\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    if (fgets(line, sizeof(line), fp)) {
        fprintf(temp, "%s", line);
    }

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^,],%d,%d,%d,%[^,],%[^,],%d,%c,%[^\n]",
                   a.date, &a.month, &a.year, &a.classNo, a.dept, a.course, &a.rollNo, &a.status, a.markedBy) == 9) {
            if (strcmp(a.dept, dept) == 0 && strcmp(a.course, course) == 0 &&
                a.rollNo == rollNo && strcmp(a.date, date) == 0 &&
                a.classNo == classNo && strcmp(a.markedBy, targetTeacher) == 0) {
                printf(CYAN "Current Status on %s Class %d (by %s): %s%c" RESET "\n",
                       date, classNo, a.markedBy, (a.status == 'P' ? GREEN : RED), a.status);
                while (1) {
                    char extra;
                    printf(GOLD "Enter New Status (" GREEN "P" GOLD "/" RED "A" GOLD "): " RESET);
                    scanf(" %c", &newStatus);
                    newStatus = toupper(newStatus);

                    int extraChars = 0;
                    while ((extra = getchar()) != '\n' && extra != EOF) {
                        if (!isspace(extra)) extraChars = 1;
                    }

                    if (!extraChars && (newStatus == 'P' || newStatus == 'A')) {
                        break;
                    }
                    printf(RED "Invalid! Enter P or A only." RESET "\n");
                }
                fprintf(temp, "%s,%d,%d,%d,%s,%s,%d,%c,%s\n",
                        a.date, a.month, a.year, a.classNo, a.dept, a.course, a.rollNo, newStatus, a.markedBy);
                updated = 1;
            } else {
                fprintf(temp, "%s", line);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    if (updated) {
        remove("attendance.csv");
        rename("temp_attendance.csv", "attendance.csv");
        system("cls");
        printf(BG_GREEN " Class %d attendance updated to '%c' for Roll %d (%s - %s) on %s! " RESET "\n",
               classNo, newStatus, rollNo, dept, course, date);
    } else {
        remove("temp_attendance.csv");
        system("cls");
        printf(BG_RED " No Class %d attendance record found for Roll %d (%s - %s) on %s by %s! " RESET "\n",
               classNo, rollNo, dept, course, date, targetTeacher);
    }
}

void monthlypercentage(const char currentUser[], const char currentRole[]) {
    system("cls");
    char dept[20], course[20], line[200];
    int month, year, foundAny = 0;
    struct Student s;
    struct Attendance a;

    printf(BG_CYAN " --- MONTHLY ATTENDANCE PERCENTAGE --- " RESET "\n");
    getValidString(GOLD "Enter Department: " RESET, dept, sizeof(dept));
    toUpperStr(dept);

    getValidString(GOLD "Enter Course: " RESET, course, sizeof(course));
    toUpperStr(course);

    year = getValidInt(GOLD "Enter Year: " RESET, 2026, 2030);
    month = getValidInt(GOLD "Enter Month: " RESET, 1, 12);

    system("cls");
    if (strcmp(currentRole, "ADMIN") == 0) {
        printf(BG_BLUE " Monthly Class Report (ALL USERS): %s - %s (%02d-%04d) " RESET "\n", dept, course, month, year);
    } else {
        printf(BG_BLUE " Monthly Class Report [%s]: %s - %s (%02d-%04d) " RESET "\n", currentUser, dept, course, month, year);
    }
    printf(GOLD "RollNo   Name                 Attended   Classes   Percentage" RESET "\n");
    printf(GRAY "-------------------------------------------------------------" RESET "\n");

    FILE *fpStu = fopen("students.csv", "r");
    if (fpStu == NULL) return;
    fgets(line, sizeof(line), fpStu);

    while (fgets(line, sizeof(line), fpStu)) {
        if (sscanf(line, "%[^,],%[^,],%d,%[^\n]", s.dept, s.course, &s.rollNo, s.name) == 4) {
            if (strcmp(s.dept, dept) == 0 && strcmp(s.course, course) == 0) {
                int total = 0, present = 0;
                char attLine[200];
                FILE *fpAtt = fopen("attendance.csv", "r");
                if (fpAtt != NULL) {
                    fgets(attLine, sizeof(attLine), fpAtt);
                    while (fgets(attLine, sizeof(attLine), fpAtt)) {
                        if (sscanf(attLine, "%[^,],%d,%d,%d,%[^,],%[^,],%d,%c,%[^\n]",
                                   a.date, &a.month, &a.year, &a.classNo, a.dept, a.course, &a.rollNo, &a.status, a.markedBy) == 9) {
                            if (strcmp(a.dept, dept) == 0 && strcmp(a.course, course) == 0 &&
                                a.rollNo == s.rollNo && a.month == month && a.year == year &&
                                canAccessRecord(a.markedBy, currentUser, currentRole)) {
                                total++;
                                if (a.status == 'P') present++;
                            }
                        }
                    }
                    fclose(fpAtt);
                }

                if (total > 0) {
                    foundAny = 1;
                    float pct = (present * 100.0) / total;
                    if (pct >= 75.0)
                        printf(SKY_BLUE "%-8d " WHITE "%-20s " BRIGHT_GREEN "%-10d " SILVER "%-9d " GREEN "%.2f%%" RESET "\n", s.rollNo, s.name, present, total, pct);
                    else
                        printf(SKY_BLUE "%-8d " WHITE "%-20s " YELLOW "%-10d " SILVER "%-9d " RED "%.2f%%" RESET "\n", s.rollNo, s.name, present, total, pct);
                }
            }
        }
    }
    fclose(fpStu);

    if (!foundAny) {
        printf(YELLOW "No class attendance records found for %s - %s in %02d-%04d." RESET "\n", dept, course, month, year);
    }
}

void absentreport(const char currentUser[], const char currentRole[]) {
    system("cls");
    char dept[20], course[20], date[15], line[200];
    int day, month, year, classNo, count = 0;
    struct Attendance a;

    printf(BG_CYAN " --- CLASS ABSENT REPORT --- " RESET "\n");
    getValidString(GOLD "Enter Department: " RESET, dept, sizeof(dept));
    toUpperStr(dept);

    getValidString(GOLD "Enter Course: " RESET, course, sizeof(course));
    toUpperStr(course);

    year = getValidInt(GOLD "Enter Year: " RESET, 2026, 2030);
    month = getValidInt(GOLD "Enter Month: " RESET, 1, 12);
    day = getValidDay(month, year);
    sprintf(date, "%02d-%02d-%04d", day, month, year);
    classNo = getValidInt(GOLD "Enter Class / Period Number (0 for ALL classes): " RESET, 0, 10);

    system("cls");
    if (classNo == 0) {
        printf(BG_RED " Absent Roll Numbers on %s - ALL Classes (%s - %s): " RESET "\n", date, dept, course);
    } else {
        printf(BG_RED " Absent Roll Numbers on %s - Class %d (%s - %s): " RESET "\n", date, classNo, dept, course);
    }

    FILE *fp = fopen("attendance.csv", "r");
    if (fp == NULL) return;
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^,],%d,%d,%d,%[^,],%[^,],%d,%c,%[^\n]",
                   a.date, &a.month, &a.year, &a.classNo, a.dept, a.course, &a.rollNo, &a.status, a.markedBy) == 9) {
            int classMatch = (classNo == 0 || a.classNo == classNo);
            if (strcmp(a.dept, dept) == 0 && strcmp(a.course, course) == 0 &&
                strcmp(a.date, date) == 0 && classMatch && a.status == 'A' &&
                canAccessRecord(a.markedBy, currentUser, currentRole)) {
                if (strcmp(currentRole, "ADMIN") == 0) {
                    printf(BRIGHT_RED "- Roll No: %d " CYAN "[Class %d] " GRAY "(Marked by: %s)" RESET "\n",
                           a.rollNo, a.classNo, a.markedBy);
                } else {
                    printf(BRIGHT_RED "- Roll No: %d " CYAN "[Class %d]" RESET "\n", a.rollNo, a.classNo);
                }
                count++;
            }
        }
    }
    fclose(fp);

    if (count == 0) printf(GREEN "No absentees found (or attendance not marked for this class/date)." RESET "\n");
}

void attendancesummary(const char currentUser[], const char currentRole[]) {
    system("cls");
    char dept[20], course[20], line[200];
    struct Student s;
    struct Attendance a;
    int safe = 0, shortage = 0;

    printf(BG_CYAN " --- OVERALL ATTENDANCE SUMMARY --- " RESET "\n");
    getValidString(GOLD "Enter Department: " RESET, dept, sizeof(dept));
    toUpperStr(dept);

    getValidString(GOLD "Enter Course: " RESET, course, sizeof(course));
    toUpperStr(course);

    system("cls");
    if (strcmp(currentRole, "ADMIN") == 0) {
        printf(BG_BLUE " Overall Summary (ALL USERS): %s - %s " RESET "\n", dept, course);
    } else {
        printf(BG_BLUE " Overall Summary [%s]: %s - %s " RESET "\n", currentUser, dept, course);
    }
    printf(GOLD "RollNo   Name                 Present   Classes   Overall %%" RESET "\n");
    printf(GRAY "-------------------------------------------------------------" RESET "\n");

    FILE *fpStu = fopen("students.csv", "r");
    if (fpStu == NULL) return;
    fgets(line, sizeof(line), fpStu);

    while (fgets(line, sizeof(line), fpStu)) {
        if (sscanf(line, "%[^,],%[^,],%d,%[^\n]", s.dept, s.course, &s.rollNo, s.name) == 4) {
            if (strcmp(s.dept, dept) == 0 && strcmp(s.course, course) == 0) {
                int total = 0, present = 0;
                char attLine[200];
                FILE *fpAtt = fopen("attendance.csv", "r");
                if (fpAtt != NULL) {
                    fgets(attLine, sizeof(attLine), fpAtt);
                    while (fgets(attLine, sizeof(attLine), fpAtt)) {
                        if (sscanf(attLine, "%[^,],%d,%d,%d,%[^,],%[^,],%d,%c,%[^\n]",
                                   a.date, &a.month, &a.year, &a.classNo, a.dept, a.course, &a.rollNo, &a.status, a.markedBy) == 9) {
                            if (strcmp(a.dept, dept) == 0 && strcmp(a.course, course) == 0 &&
                                a.rollNo == s.rollNo && canAccessRecord(a.markedBy, currentUser, currentRole)) {
                                total++;
                                if (a.status == 'P') present++;
                            }
                        }
                    }
                    fclose(fpAtt);
                }
                if (total > 0) {
                    float pct = (present * 100.0) / total;
                    if (pct >= 75.0) {
                        safe++;
                        printf(SKY_BLUE "%-8d " WHITE "%-20s " BRIGHT_GREEN "%-9d " SILVER "%-9d " GREEN "%.2f%%" RESET "\n", s.rollNo, s.name, present, total, pct);
                    } else {
                        shortage++;
                        printf(SKY_BLUE "%-8d " WHITE "%-20s " YELLOW "%-9d " SILVER "%-9d " RED "%.2f%%" RESET "\n", s.rollNo, s.name, present, total, pct);
                    }
                }
            }
        }
    }
    fclose(fpStu);

    printf(GRAY "-------------------------------------------------------------" RESET "\n");
    printf(GREEN "Above 75%% (Safe): %d" RESET " " GRAY "|" RESET " " RED "Below 75%% (Shortage): %d" RESET "\n", safe, shortage);
}

int loginUser(char currentUser[], char currentRole[]) {
    char inputUser[30], inputPass[30], line[200];
    struct User u;
    int attempts = 3;

    while (attempts > 0) {
        printf("\n" BG_BLUE " --- SYSTEM LOGIN (%d attempts left) --- " RESET "\n", attempts);
        getValidString(GOLD "Username: " RESET, inputUser, sizeof(inputUser));
        getValidString(GOLD "Password: " RESET, inputPass, sizeof(inputPass));

        FILE *fp = fopen("users.csv", "r");
        if (fp != NULL) {
            fgets(line, sizeof(line), fp);
            while (fgets(line, sizeof(line), fp)) {
                if (sscanf(line, "%[^,],%[^,],%[^\n]", u.username, u.password, u.role) == 3) {
                    if (strcmp(u.username, inputUser) == 0 && strcmp(u.password, inputPass) == 0) {
                        strcpy(currentUser, u.username);
                        strcpy(currentRole, u.role);
                        fclose(fp);
                        system("cls");
                        printf(BG_GREEN " Login Successful! Welcome %s (%s). " RESET "\n", u.username, u.role);
                        return 1;
                    }
                }
            }
            fclose(fp);
        }

        attempts--;
        system("cls");
        printf(BG_RED " Access Denied! Invalid username or password. " RESET "\n");
    }
    return 0;
}

void addTeacher(const char currentRole[]) {
    system("cls");
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf(BG_RED " Access Denied! Only the ADMIN can create Teacher accounts. " RESET "\n");
        return;
    }

    struct User u, temp;
    char line[200];

    printf(BG_MAGENTA " --- CREATE TEACHER ACCOUNT --- " RESET "\n");
    getValidString(GOLD "Enter Teacher Username: " RESET, u.username, sizeof(u.username));

    FILE *fp = fopen("users.csv", "r");
    if (fp != NULL) {
        fgets(line, sizeof(line), fp);
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "%[^,],%[^,],%[^\n]", temp.username, temp.password, temp.role) == 3) {
                if (strcmp(temp.username, u.username) == 0) {
                    printf(RED "Username '%s' already exists!" RESET "\n", u.username);
                    fclose(fp);
                    return;
                }
            }
        }
        fclose(fp);
    }

    getValidString(GOLD "Enter Teacher Password: " RESET, u.password, sizeof(u.password));
    strcpy(u.role, "TEACHER");

    fp = fopen("users.csv", "a");
    if (fp != NULL) {
        fprintf(fp, "%s,%s,%s\n", u.username, u.password, u.role);
        fclose(fp);
        printf(BG_GREEN " Teacher account '%s' created successfully! " RESET "\n", u.username);
    }
}

void attendancecalendar(const char currentUser[], const char currentRole[]) {
    system("cls");
    char dept[20], course[20], line[200], filterUser[30];
    int month, year;
    struct Attendance a;
    int taken[32] = {0};

    printf(BG_TEAL "  MONTHLY ATTENDANCE CALENDAR  " RESET "\n");
    getValidString(GOLD "Enter Department: " RESET, dept, sizeof(dept));
    toUpperStr(dept);

    getValidString(GOLD "Enter Course: " RESET, course, sizeof(course));
    toUpperStr(course);

    year = getValidInt(GOLD "Enter Year: " RESET, 2026, 2030);
    month = getValidInt(GOLD "Enter Month: " RESET, 1, 12);

    if (strcmp(currentRole, "ADMIN") == 0) {
        getValidString(GOLD "Filter by Username (0 or type ALL): " RESET, filterUser, sizeof(filterUser));
    } else {
        strcpy(filterUser, currentUser);
    }

    FILE *fp = fopen("attendance.csv", "r");
    if (fp != NULL) {
        fgets(line, sizeof(line), fp);

        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "%[^,],%d,%d,%d,%[^,],%[^,],%d,%c,%[^\n]", a.date, &a.month, &a.year, &a.classNo, a.dept, a.course, &a.rollNo, &a.status, a.markedBy) == 9) {
                int userMatch = (strcmp(currentRole, "ADMIN") == 0 && strcmp(filterUser, "ALL") == 0) ||
                                (strcmp(a.markedBy, filterUser) == 0);

                if (strcmp(a.dept, dept) == 0 && strcmp(a.course, course) == 0 && a.month == month && a.year == year && userMatch) {
                    int day = 0;
                    sscanf(a.date, "%d", &day);
                    if (day >= 1 && day <= 31) {
                        taken[day] = 1;
                    }
                }
            }
        }
        fclose(fp);
    }

    int totalDays = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        totalDays = 30;
    } else if (month == 2) {
        int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        totalDays = isLeap ? 29 : 28;
    }

    system("cls");
    printf(BG_BLUE "  ATTENDANCE DATES [%s]: %s - %s (%02d-%04d)  " RESET "\n\n", filterUser, dept, course, month, year);

    int count = 0;
    for (int d = 1; d <= totalDays; d++) {
        if (taken[d] == 1) {
            printf(BG_GREEN " %02d " RESET "  ", d);
            count++;
        } else {
            printf(WHITE " %02d   " RESET, d);
        }

        if (d % 7 == 0) {
            printf("\n\n");
        }
    }

    printf("\n" GRAY "-----------------------------------------" RESET "\n");
    printf(BG_GREEN " DD " RESET " = Taken (" GREEN "%d days" RESET ")  |  " WHITE "DD = Not Taken" RESET "\n", count);
}

int main() {
    int choice;
    char currentUser[30];
    char currentRole[15];
    system("cls");
    printf(BG_SKY_BLUE " Welcome to the Attendance Management System " RESET "\n");
    filescreation();

    if (!loginUser(currentUser, currentRole)) {
        printf(BG_RED " Too many failed attempts. Exiting... " RESET "\n");
        system("pause");
        return 0;
    }

    while (1) {
        system("pause");
        system("cls");
        printf("\n" BG_BLUE " --- ATTENDANCE SYSTEM: %s [%s] --- " RESET "\n", currentUser, currentRole);

        if (strcmp(currentRole, "ADMIN") == 0) {
            printf(RED         "1. Register Student (Admin Only)" RESET "\n");
            printf(ORANGE      "2. Mark Class Attendance" RESET "\n");
            printf(YELLOW      "3. Alter Student Name (Admin Only)" RESET "\n");
            printf(GREEN       "4. Alter Student Attendance (Admin Only)" RESET "\n");
            printf(BLUE        "5. Monthly Percentage" RESET "\n");
            printf(INDIGO      "6. Absent Report" RESET "\n");
            printf(VIOLET      "7. Attendance Summary" RESET "\n");
            printf(BRIGHT_CYAN "8. Attendance Calendar" RESET "\n");
            printf(YELLOW      "9. Create Teacher Account (Admin Only)" RESET "\n");
            printf(RED         "10. Exit" RESET "\n");

            choice = getValidInt(GOLD "Choice: " RESET, 1, 10);

            switch (choice) {
                case 1: registerstudent(currentRole); break;
                case 2: markattendance(currentUser); break;
                case 3: alterstudentname(currentRole); break;
                case 4: alterattendance(currentUser, currentRole); break;
                case 5: monthlypercentage(currentUser, currentRole); break;
                case 6: absentreport(currentUser, currentRole); break;
                case 7: attendancesummary(currentUser, currentRole); break;
                case 8: attendancecalendar(currentUser, currentRole); break;
                case 9: addTeacher(currentRole); break;
                case 10: system("cls"); return 0;
            }
        } else {
            printf(RED         "1. Mark Class Attendance" RESET "\n");
            printf(ORANGE      "2. Monthly Percentage" RESET "\n");
            printf(YELLOW      "3. Absent Report" RESET "\n");
            printf(GREEN       "4. Attendance Summary" RESET "\n");
            printf(BLUE        "5. Attendance Calendar" RESET "\n");
            printf(RED         "6. Exit" RESET "\n");

            choice = getValidInt(GOLD "Choice: " RESET, 1, 6);

            switch (choice) {
                case 1: markattendance(currentUser); break;
                case 2: monthlypercentage(currentUser, currentRole); break;
                case 3: absentreport(currentUser, currentRole); break;
                case 4: attendancesummary(currentUser, currentRole); break;
                case 5: attendancecalendar(currentUser, currentRole); break;
                case 6: system("cls"); return 0;
            }
        }
    }
}