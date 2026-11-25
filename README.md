# Student Attendance Management System (C)

A console-based Student Attendance Management System written in pure C, featuring **CSV file persistence**, **Role-Based Access Control (RBAC)**, **per-class/period tracking**, and an **ANSI-colored terminal UI**.

---

## Features

### 1. Role-Based Access Control (RBAC)
* **Admin (`ADMIN`)**: Full system privileges—register students, create teacher accounts, alter student names, alter past attendance records, mark attendance, and view reports across all teachers or filtered by a specific user.
* **Teacher (`TEACHER`)**: Restricted menu—mark attendance for assigned classes and view reports/calendars strictly for the classes they marked.

### 2. Academic & Period Hierarchy
* Organizes records cleanly by **Department → Course → Roll Number**.
* Tracks attendance per **Class / Period Number (`1–10`)** on any valid calendar date (including automatic leap-year validation).
* Blocks duplicate attendance submissions for the same class, date, department, course, and teacher.

### 3. Reports & Visual Monthly Calendar
* **Monthly Percentage Report**: Calculates attended vs. total classes for a specific month and highlights students in Green ($\ge 75\%$) or Red ($< 75\%$).
* **Absent Report**: Lists absent roll numbers for a specific class period or across all periods on a given date.
* **Overall Attendance Summary**: Displays cumulative attendance percentages and counts total students in the Safe ($\ge 75\%$) vs. Shortage ($< 75\%$) categories.
* **Monthly Attendance Calendar**: Renders a 7-column calendar grid highlighting dates where attendance was taken.

### 4. Defensive Input Validation & CSV Integrity
* Custom input handlers (`getValidInt`, `getValidDay`, `getValidString`) flush leftover buffer characters to prevent infinite loops.
* Automatically blocks commas (`,`) in user inputs to preserve CSV column alignment.
* Uses safe temporary file rewriting (`remove` & `rename`) when updating existing records.

---

## CSV Database Schema

On first run, the program automatically generates three CSV files in the working directory:

| File | Columns |
| :--- | :--- |
| `users.csv` | `Username,Password,Role` |
| `students.csv` | `Department,Course,RollNo,Name` |
| `attendance.csv` | `Date,Month,Year,ClassNo,Department,Course,RollNo,Status,MarkedBy` |

---

## Getting Started

### Prerequisites
* A C compiler such as **GCC** (MinGW on Windows, GCC on Linux/macOS).
* A terminal supporting ANSI escape codes (Windows Terminal, PowerShell, VS Code Terminal, or Linux/macOS Terminal).

### Compilation & Execution
```bash
gcc Project_Attendance_management.c -o attendance
./attendance
