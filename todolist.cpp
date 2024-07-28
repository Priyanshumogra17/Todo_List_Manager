#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

struct Task {
    string description;
    string category;
    int priority;
    string dueDate;

    bool isOverdue() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        ostringstream todayStream;
        todayStream << 1900 + ltm->tm_year << "-"
                    << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-"
                    << setfill('0') << setw(2) << ltm->tm_mday;
        return dueDate < todayStream.str();  // Check if the due date is in the past
    }
};

vector<Task> tasks;
vector<string> categories = {"Work", "Personal", "Study", "Shopping", "Other"};

void loadTasks(const string &filename) {
    ifstream file(filename);
    if (!file) return;

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        Task task;
        getline(ss, task.description, '|');
        getline(ss, task.category, '|');
        ss >> task.priority;
        ss.ignore();
        getline(ss, task.dueDate);
        tasks.push_back(task);
    }
}

void saveTasks(const string &filename) {
    ofstream file(filename);
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto &task = tasks[i];
        file << task.description << "|" << task.category << "|" 
             << task.priority << "|" << task.dueDate << endl;
    }
}

void addTask() {
    Task task;
    cout << "Enter task description: ";
    cin.ignore();
    getline(cin, task.description);

    cout << "Choose a category: " << endl;
    for (size_t i = 0; i < categories.size(); ++i) {
        cout << i + 1 << ". " << categories[i] << endl;
    }
    int categoryChoice;
    cin >> categoryChoice;
    if (categoryChoice > 0 && categoryChoice <= categories.size()) {
        task.category = categories[categoryChoice - 1];
    } else {
        cout << "Invalid choice, defaulting to 'Other'" << endl;
        task.category = "Other";
    }

    cout << "Enter priority (1-5): ";
    cin >> task.priority;
    cin.ignore();

    cout << "Enter due date (YYYY-MM-DD) or leave empty if not applicable: ";
    getline(cin, task.dueDate);

    tasks.push_back(task);
}

void sortTasksByPriority() {
    sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) {
        return a.priority < b.priority;  // Sort in ascending order
    });
}

void listTasks() {
    sortTasksByPriority();  // Sort tasks by priority before listing
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto &task = tasks[i];
        cout << "Task " << i + 1 << ":" << endl;
        cout << "Description: " << task.description << endl;
        cout << "Category: " << task.category << endl;
        cout << "Priority: " << task.priority << endl;
        if (!task.dueDate.empty()) {
            cout << "Due Date: " << task.dueDate << endl;
        } else {
            cout << "Due Date: None" << endl;
        }
        cout << "---------------------------" << endl;
    }
}

void removeTask() {
    int index;
    cout << "Enter task index to remove: ";
    cin >> index;
    if (index > 0 && index <= tasks.size()) {
        tasks.erase(tasks.begin() + index - 1);
    } else {
        cout << "Invalid index!" << endl;
    }
}

void remindPastDueTasks() {
    cout << "Past Due Tasks:" << endl;
    bool hasOverdue = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        const Task &task = tasks[i];
        if (task.isOverdue()) {
            cout << " - " << task.description << " (Due: " << task.dueDate << ")" << endl;
            hasOverdue = true;
        }
    }
    if (!hasOverdue) {
        cout << "No past due tasks." << endl;
    }
}

void displayMenu() {
    cout << "1. Add Task" << endl;
    cout << "2. List Tasks by Priority" << endl;
    cout << "3. Remove Task" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose an option: ";
}

int main() {
    const string filename = "tasks.txt";
    loadTasks(filename);

    int choice;
    do {
        displayMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                addTask();
                remindPastDueTasks();  // Remind about overdue tasks after adding a new task
                break;
            case 2:
                listTasks();
                remindPastDueTasks();  // Remind about overdue tasks after listing
                break;
            case 3:
                removeTask();
                break;
            case 4:
                saveTasks(filename);
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 4);

    return 0;
}