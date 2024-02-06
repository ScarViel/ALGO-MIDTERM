
#include <iostream>

using namespace std;
const int MAX_SIZE = 100;
const int MAX_PROCESSES = 100;

// Structure to represent a process
struct Process {
    int id;
    int arrivalTime;
    int burstTime; // Original burst time
    int remainingTime; // Remaining time to be executed
    int priority;
    int finishTime;
    int turnaroundTime;
    int waitingTime;
    bool completed;
    ////
    int proc;            
    int arrival;    
    int burst;      
    int remain;
    int start;
};


// Function to find the process with the highest priority at a given time
int findHighestPriorityProcess(Process processes[], int n, int currentTime) {
    int highestPriority = -1;
    int selectedProcess = -1;

    for (int i = 0; i < n; ++i) {
        if (!processes[i].completed && processes[i].arrivalTime <= currentTime) {
            if (highestPriority == -1 || processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                selectedProcess = i;
            }
        }
    }
    return selectedProcess;
}

// Function to perform Priority Scheduling - Preemptive
void priorityScheduling(Process processes[], int n, ofstream& outputFile) {
    int currentTime = 0;
    int completedProcesses = 0;
    int executedProcess[MAX_SIZE]; // Array to store executed process at each time
    int executionTime[MAX_SIZE];   // Array to store the time at which each process is executed

    // Initialize arrays
    for (int i = 0; i < MAX_SIZE; ++i) {
        executedProcess[i] = -1;
        executionTime[i] = -1;
    }

    // Continue until all processes are completed
    while (completedProcesses < n) {
        int processIndex = findHighestPriorityProcess(processes, n, currentTime);

        if (processIndex != -1) {
            processes[processIndex].remainingTime--;

            if (processes[processIndex].remainingTime == 0) {
                processes[processIndex].completed = true;
                processes[processIndex].finishTime = currentTime + 1;
                processes[processIndex].turnaroundTime = processes[processIndex].finishTime - processes[processIndex].arrivalTime;
                processes[processIndex].waitingTime = processes[processIndex].turnaroundTime - processes[processIndex].burstTime;
                completedProcesses++;
            }

            // Store executed process and execution time
            executedProcess[currentTime] = processIndex;
            executionTime[currentTime] = currentTime;

            // Move to the next time slot
            currentTime++;
        } else {
            // No process to execute, move to the next arrival time
            int nextArrivalTime = INT_MAX;
            for (int i = 0; i < n; ++i) {
                if (!processes[i].completed && processes[i].arrivalTime > currentTime && processes[i].arrivalTime < nextArrivalTime) {
                    nextArrivalTime = processes[i].arrivalTime;
                }
            }
            currentTime = nextArrivalTime;
        }
    }

    // Declare an array to store the time slots
    int timeSlots[MAX_SIZE];

    // Initialize lastExecutionIndex to keep track of the index in the timeSlots array
    int lastExecutionIndex = 0;

    // Display Gantt Chart
    outputFile << "\nGantt Chart:\n";
    int lastExecutedProcess = -1;  // To keep track of the last executed process
    int lastExecutionTime = -1;  // To keep track of the last execution time
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (executedProcess[i] != -1 && executedProcess[i] != lastExecutedProcess) {
            if (lastExecutionTime != -1) {
                // Store the last execution time in the timeSlots array
                timeSlots[lastExecutionIndex++] = lastExecutionTime;
            }
            outputFile << "|---P" << processes[executedProcess[i]].id << "---";
            lastExecutedProcess = executedProcess[i];
            lastExecutionTime = i;
        }
    }

    // Store the last execution time in the timeSlots array
    if (lastExecutionTime != -1) {
        timeSlots[lastExecutionIndex++] = lastExecutionTime;
    }

    int lastProcessFinishTime = lastExecutionTime + processes[lastExecutedProcess].burstTime; // Calculate the completion time
    timeSlots[lastExecutionIndex++] = lastProcessFinishTime;

    // Display the processes
    outputFile << "|" << endl;

    // Display the time slots
    for (int i = 0; i < lastExecutionIndex; ++i) {
        int whiteSpace = timeSlots[i];
        if (whiteSpace < 10) {
            outputFile << timeSlots[i];
            outputFile << "        "; // 8 white spaces
        } else if (whiteSpace >= 10) {
            outputFile << timeSlots[i];
            outputFile << "       ";  // 7 white spaces
        }
    }
    outputFile << endl;
}

// Function to write process details to a file
void writeProcessDetailsToFile(Process processes[], int n, ofstream& outputFile) {
    outputFile << "\nProcess Details:\n";
    outputFile << "+----+---------------+---------------+--------------+-----------------+-------------------+----------------+\n";
    outputFile << "| ID | Arrival Time  |   Burst Time  |   Priority   |   Finish Time   |  Turnaround Time  |  Waiting Time  |\n";
    for (int i = 0; i < n; ++i) {
        outputFile << "+----+---------------+---------------+--------------+-----------------+-------------------+----------------+\n";

        outputFile << "|  " << processes[i].id << " |       " << processes[i].arrivalTime << "       |       " << processes[i].burstTime 
             << "\t     |      " << processes[i].priority << "\t    |        " << processes[i].finishTime << "\t      |  \t" 
             << processes[i].turnaroundTime << "\t  |\t   " << (processes[i].waitingTime < 0 ? 0 : processes[i].waitingTime) << "\t   |\n";
    }
    outputFile << "+----+---------------+---------------+--------------+-----------------+-------------------+----------------+\n";
}

///////
void srt(Process* p, int n, int time) {
    int done = 0;
    int shortest = -1;
    int stime = 999999;
    
    for (int i = 0; i < n; i++) {
        if (p[i].arrival <= time && p[i].remain < stime && p[i].remain > 0) {
            shortest = i;
            stime = p[i].remain;
        }
    }
    
    if (shortest == -1) {
        time++;
        srt(p, n, time);
        return;
    }
    
    if (p[shortest].remain == p[shortest].burst) {
        p[shortest].start = time;
    }
    
    time++;
    p[shortest].remain--;

    
    if (p[shortest].remain == 0) {
        done++;
        cout << "\nP" << p[shortest].proc << " finished executing at " << time;
        int turn = time - p[shortest].arrival;
    	int wait = p[shortest].start - p[shortest].arrival;
        cout << "\nTurnaround time is " << turn << "ms";
        cout << "\nWaiting time is " << wait << "ms" << endl;
    }
    
    if (done != n) {
        srt(p, n, time);
    }
}
////////
// Function to maintain the ready queue recursively
void queueUpdationRecursive(int queue[], int timer, int arrival[], int n, int currentIndex, int maxProccessIndex) {
    if (currentIndex < n) {
        if (arrival[currentIndex] <= timer && maxProccessIndex < currentIndex) {
            maxProccessIndex = currentIndex;
        }
        queueUpdationRecursive(queue, timer, arrival, n, currentIndex + 1, maxProccessIndex);
    } else {
        for (int i = 0; i < n; i++) {
            if (queue[i] == 0) {
                queue[i] = maxProccessIndex + 1;
                break;
            }
        }
    }
}

void checkNewArrivalRecursive(int timer, int arrival[], int n, int maxProccessIndex, int queue[]) {
    if (timer <= arrival[n - 1]) {
        queueUpdationRecursive(queue, timer, arrival, n, maxProccessIndex + 1, maxProccessIndex);
    }
}

void queueMaintainenceRecursive(int queue[], int n, int currentIndex) {
    if (currentIndex < n - 1 && queue[currentIndex + 1] != 0) {
        int temp = queue[currentIndex];
        queue[currentIndex] = queue[currentIndex + 1];
        queue[currentIndex + 1] = temp;
        queueMaintainenceRecursive(queue, n, currentIndex + 1);
    }
}

void displayGanttChart(int ganttChart[MAX_PROCESSES][2], int executionCount) {
    cout << "\nGantt Chart:\n";
    cout << "---------------------------------\n";
    cout << "| Process ID | Execution Time   |\n";
    cout << "---------------------------------\n";
    for (int i = 0; i < executionCount; ++i) {
        cout << "|     " << ganttChart[i][0] << "      |         " << ganttChart[i][1] << "         |\n";
    }
    cout << "---------------------------------\n";
}
////////
void SRTF()
{
   int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    Process processes[MAX_SIZE];

    // Input the process details
    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cout << "Enter arrival time for process P" << processes[i].id << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter burst time for process P" << processes[i].id << ": ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime; // Initialize remaining time
        cout << "Enter priority for process P" << processes[i].id << ": ";
        cin >> processes[i].priority;
        processes[i].completed = false;
    }

    // Sort processes based on arrival time (for Gantt Chart)
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                swap(processes[j], processes[j + 1]);
            }
        }
    }

    // Perform Priority Scheduling - Preemptive
    ofstream outputFile("output.txt");
    priorityScheduling(processes, n, outputFile);
    writeProcessDetailsToFile(processes, n, outputFile);
    outputFile.close();

    cout << "Process details and Gantt chart saved to output.txt\n";
};
//////////
void PS()
{
   int num;
	cout<< "Enter number of processes: "; cin >> num;
	Process* process = new Process[num];
	
	for(int i = 0; i < num; i++){
		process[i].proc = i +1;
		cout << "\nP" << process[i].proc << " arrival time: "; cin >> process[i].arrival;
		cout << "P" << process[i].proc << " burst time: "; cin >> process[i].burst;
		process[i].remain = process[i].burst;
	}
	
	srt(process, num, 0); 
    
};
//////////
void RR()
{
    int n, tq, timer = 0, maxProccessIndex = 0;
    float avgWait = 0, avgTT = 0;
    cout << "\nEnter the time quanta : ";
    cin >> tq;
    cout << "\nEnter the number of processes : ";
    cin >> n;
    int arrival[n], burst[n], wait[n], turn[n], queue[n], temp_burst[n];
    bool complete[n];

    cout << "\nEnter the arrival time of the processes : ";
    for (int i = 0; i < n; i++)
        cin >> arrival[i];
    cin.ignore();
    cout << "\nEnter the burst time of the processes : ";
    for (int i = 0; i < n; i++) {
        cin >> burst[i];
        temp_burst[i] = burst[i];
    }

    for (int i = 0; i < n; i++) {
        complete[i] = false;
        queue[i] = 0;
    }
    while (timer < arrival[0])
        timer++;
    queue[0] = 1;

    int ganttChart[MAX_PROCESSES][2]; // Process ID and Execution Time
    int executionCount = 0;

    while (true) {
        bool flag = true;
        for (int i = 0; i < n; i++) {
            if (temp_burst[i] != 0) {
                flag = false;
                break;
            }
        }
        if (flag)
            break;

        for (int i = 0; (i < n) && (queue[i] != 0); i++) {
            int ctr = 0;
            while ((ctr < tq) && (temp_burst[queue[0] - 1] > 0)) {
                temp_burst[queue[0] - 1] -= 1;
                timer += 1;
                ctr++;

                // Update Gantt chart
                ganttChart[executionCount][0] = queue[0];
                ganttChart[executionCount][1] = timer;
                executionCount++;

                checkNewArrivalRecursive(timer, arrival, n, maxProccessIndex, queue);
            }

            if ((temp_burst[queue[0] - 1] == 0) && (complete[queue[0] - 1] == false)) {
                // Corrected the calculation of turnaround time
                turn[queue[0] - 1] = timer - arrival[queue[0] - 1];
                complete[queue[0] - 1] = true;
            }

            bool idle = true;
            if (queue[n - 1] == 0) {
                for (int i = 0; i < n && queue[i] != 0; i++) {
                    if (complete[queue[i] - 1] == false) {
                        idle = false;
                    }
                }
            } else
                idle = false;

            if (idle) {
                timer++;
                checkNewArrivalRecursive(timer, arrival, n, maxProccessIndex, queue);
            }

            queueMaintainenceRecursive(queue, n, 0);
        }
    }

    for (int i = 0; i < n; i++) {
        wait[i] = turn[i] - burst[i];
    }

    cout << "\nProgram No.\tArrival Time\tBurst Time\tWait Time\tTurnAround Time"
         << endl;
    for (int i = 0; i < n; i++) {
        cout << i + 1 << "\t\t" << arrival[i] << "\t\t"
             << burst[i] << "\t\t" << wait[i] << "\t\t" << turn[i] << endl;
    }
    for (int i = 0; i < n; i++) {
        avgWait += wait[i];
        avgTT += turn[i];
    }
    cout << "\nAverage wait time : " << (avgWait / n)
         << "\nAverage Turn Around Time : " << (avgTT / n);

    displayGanttChart(ganttChart, executionCount);
    
};


int main()
{
    
    
            int choice;
            bool loop=false;
            while (loop==false)
            {
            cout<<"\t\tMENU"<<endl;
            cout<<"\t[1]:Shortest Remaining Time First"<<endl;
            cout<<"\t[2]:Prioty Scheduling"<<endl;
            cout<<"\t[3]:Round Robin"<<endl;
            cout<<"\t[4:Exit\n\t";
            cin>>choice;
            switch(choice)
            {
            case 1:
            SRTF();
            break;
            case 2:
            PS();
            break;
            case 3:
            RR();
            break;
            case 4:
            exit(0);
            break;
            default:
            loop=false;
            cout<<"Please try again....\n\n\n";
            
            }
    }

}
