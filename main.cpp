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
void priorityScheduling(Process processes[], int n, int currentTime, int completedProcesses = 0) {
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

        // Recursively call the function for the next time slot
        priorityScheduling(processes, n, currentTime + 1, completedProcesses);
    } else {
        // No process to execute, move to the next time slot
        currentTime++;

        // Check if all processes are completed
        if (completedProcesses == n) {
            return;
        }

        // Continue scheduling for the next time slot
        priorityScheduling(processes, n, currentTime, completedProcesses);
    }
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
void PS()
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
    cout << "\nGantt Chart:\n";
    priorityScheduling(processes, n, 0);

    // Display process details
    cout << "\nProcess Details:\n";
    cout << "+----+---------------+---------------+--------------+-----------------+-------------------+----------------+" << endl;
    cout << "| ID | Arrival Time  |   Burst Time  |   Priority   |   Finish Time   |  Turnaround Time  |  Waiting Time  |\n";
    for (int i = 0; i < n; ++i) {
        cout << "+----+---------------+---------------+--------------+-----------------+-------------------+----------------+" << endl;

        cout << "| " << processes[i].id << "  |       " << processes[i].arrivalTime << "       |       " << processes[i].burstTime 
        << "       |      " << processes[i].priority << "       |        " << processes[i].finishTime << "\t      |\t        " 
        << processes[i].turnaroundTime << "\t  |\t  " << (processes[i].waitingTime < 0 ? 0 : processes[i].waitingTime) << "\t   |" << endl;
    }
    cout << "+----+---------------+---------------+--------------+-----------------+-------------------+----------------+" << endl;
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
            cout<<"\t[4:Exit\n";
            cout<<"\tChoice:";
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
