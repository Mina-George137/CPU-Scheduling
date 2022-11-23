#include <iostream>
#include <stdio.h>
#include "process.h"
#include "algo_type.h"
#include <sstream>
#include <bits/stdc++.h>
#include <cstring>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;

map<int, string> algorithm;

vector<Process> processes;
vector<Process> readyQueue;

string mode, algoType, lI, np; // mode of output if trace or stats, algorithmType , lastInstance char

int lastInstance, numberOfProcesses, quantum = 0, rrFlag;
vector<AlgoType> algoT;

float meanTurnAround, meanNormTurnAround;

bool compareArrival(Process p1, Process p2)
{
    return p1.arrivalTime < p2.arrivalTime;
}

bool compareService(Process p1, Process p2)
{
    return p1.serviceTime < p2.serviceTime;
}

float MeanTurnAround()
{
    float mean = 0.0;
    for (int i = 0; i < processes.size(); i++)
    {
        mean += processes[i].turnaroundTime;
    }
    mean /= processes.size();
    return mean;
}

float NormalizeMeanTurnAround()
{
    float mean = 0.0;
    for (int i = 0; i < processes.size(); i++)
    {
        mean += processes[i].normaTAT;
    }
    mean /= processes.size();
    return mean;
}
// Send 0 at quantum in order to not to print it
void outputFile(string algoName, int quantum1)
{
    int spaces = 6;
    if (mode == "stats")
    {
        meanTurnAround = MeanTurnAround();
        meanNormTurnAround = NormalizeMeanTurnAround();
        sort(processes.begin(), processes.end(), compareArrival);
        if (quantum1)
        {
            cout << algoName << "-" << quantum1 << endl;
        }
        else
        {
            cout << algoName << endl;
        }

        cout << "Process    |";
        for (int i = 0; i < processes.size(); i++)
        {
            cout << "  " << processes[i].name << "  "
                 << "|";
        }
        cout << endl;
        cout << "Arrival    |";
        for (int i = 0; i < processes.size(); i++)
        {
            cout << "  " << processes[i].arrivalTime << "  "
                 << "|";
        }
        cout << endl;

        cout << "Service    |";
        for (int i = 0; i < processes.size(); i++)
        {
            cout << "  " << processes[i].serviceTime << "  "
                 << "|";
        }
        cout << " Mean|";
        cout << endl;

        cout << "Finish     |";
        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].finishTime > 9)
            {
                cout << " " << processes[i].finishTime << "  "
                     << "|";
            }
            else
            {
                cout << "  " << processes[i].finishTime << "  "
                     << "|";
            }
        }
        cout << "-----|";
        cout << endl;

        cout << "Turnaround |";
        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].turnaroundTime > 9)
            {
                cout << " " << processes[i].turnaroundTime << "  "
                     << "|";
            }
            else
            {
                cout << "  " << processes[i].turnaroundTime << "  "
                     << "|";
            }
        }

        if (meanTurnAround >= 10)
        {
            cout << fixed << setprecision(2) << meanTurnAround << "|";
            cout << endl;
        }
        else
        {
            cout << " " << fixed << setprecision(2) << meanTurnAround << "|";
            cout << endl;
        }

        cout << "NormTurn   |";
        for (int i = 0; i < processes.size(); i++)
        {
            cout << " " << processes[i].normaTAT << "|";
        }
        if (meanNormTurnAround >= 10)
        {
            cout << fixed << setprecision(2) << meanNormTurnAround << "|";
            cout << endl;
        }
        else
        {
            cout << " " << fixed << setprecision(2) << meanNormTurnAround << "|";
            cout << endl;
        }
        cout << endl;
    }
    else
    {
        if (quantum1 && algoName != "Aging")
        {
            cout << algoName << '-' << quantum1;
            spaces = spaces - (quantum1 / 10 >= 10 ? 3 : quantum1 / 10 >= 1 ? 2 : 1) - algoName.length() - 2;
        }
        else
        {
            sort(processes.begin(), processes.end(), compareArrival);
            spaces = spaces - algoName.length() - 1;
            cout << algoName;
        }

        for (int i = 0; i < spaces; i++)
        {
            cout << " ";
        }
        int maximumFinishTime = 0;
        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].finishTime > maximumFinishTime)
            {
                maximumFinishTime = processes[i].finishTime;
            }
        }
        for (int i = 0; i <= maximumFinishTime; i++)
        {
            cout << " " << i % 10;
        }
        cout << "\n";
        for (int i = 0; i <= maximumFinishTime * 2 + 6; i++)
        {
            cout << "-";
        }
        cout << "-\n";
        for (int i = 0; i < processes.size(); i++)
        {
            cout << processes[i].name;
            for (int k = processes[i].name.length(); k < 6; k++)
            {
                cout << " ";
            }
            for (int j = 0; j < stoi(lI); j++)
            {
                if (j >= processes[i].arrivalTime && j < processes[i].finishTime && processes[i].trace.size() != 0)
                {
                    cout << "|" << processes[i].trace[0];
                    processes[i].trace.erase(processes[i].trace.begin());
                }
                else
                {
                    cout << "| ";
                }
            }
            cout << "|\n";
        }
        for (int i = 0; i <= maximumFinishTime * 2 + 6; i++)
        {
            cout << "-";
        }
        cout << "-\n";
        cout << endl;
    }
}

void FCFS()
{
    // waiting time of 1st process is always 0
    processes[0].waitingTime = 0;
    processes[0].finishTime = processes[0].serviceTime;

    for (int i = 0; i < processes[0].serviceTime; i++)
    {
        processes[0].trace.push_back('*');
    }

    // CALCULATE WAITING TIME
    for (int i = 1; i < processes.size(); i++)
    {
        processes[i].waitingTime = 0;
        processes[i].finishTime = 0;
        for (int j = 0; j < i; j++)
        {
            processes[i].waitingTime += processes[j].serviceTime;
            processes[i].finishTime += processes[j].serviceTime;
        }
        processes[i].waitingTime -= processes[i].arrivalTime;
        processes[i].finishTime += processes[i].serviceTime;
    }

    // CALCULATE TURNAROUND TIME
    for (int i = 0; i < processes.size(); i++)
    {
        processes[i].turnaroundTime = processes[i].serviceTime + processes[i].waitingTime;
    }

    // CALCULATE NORMALIZED-TURNAROUND TIME
    for (int i = 0; i < processes.size(); i++)
    {
        int j = 0;
        processes[i].normaTAT = (float)processes[i].turnaroundTime / processes[i].serviceTime;
        while (j < processes[i].waitingTime)
        {
            processes[i].trace.push_back('.');
            j++;
        }
        j = 0;
        while (j < processes[i].serviceTime)
        {
            processes[i].trace.push_back('*');
            j++;
        }
    }
}

void RR(int quantumT)
{
    vector<Process> readyQueue;
    Process currentProcess;
    int i, j, flag = 1, currentProcessOut = 0;
    for (i = 0; i < stoi(lI); i++)
    {
        for (j = 0; j < processes.size(); j++)
        {
            if (processes[j].arrivalTime == i)
            {
                readyQueue.push_back(processes[j]);
            }
        }
        if (currentProcessOut == 1)
        {
            readyQueue.push_back(currentProcess);
            currentProcessOut = 0;
        }
        readyQueue[0].remainingServiceTime -= 1;
        readyQueue[0].trace.push_back('*');
        for (j = 1; j < readyQueue.size(); j++)
        {
            readyQueue[j].waitingTime++;
            readyQueue[j].trace.push_back('.');
        }
        if (readyQueue[0].remainingServiceTime == 0)
        {
            readyQueue[0].finishTime = i + 1;
            for (j = 0; j < processes.size(); j++)
            {
                if (processes[j].name == readyQueue[0].name)
                {
                    processes[j] = readyQueue[0];
                }
            }
            readyQueue.erase(readyQueue.begin());
            flag = 1;
            continue;
        }
        if (flag < quantumT)
        {
            flag++;
        }
        else if (flag == quantumT)
        {
            flag = 1;
            currentProcess = readyQueue[0];
            readyQueue.erase(readyQueue.begin());
            currentProcessOut = 1;
        }
    }
    for (i = 0; i < processes.size(); i++)
    {
        processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
    }

    for (i = 0; i < processes.size(); i++)
    {
        processes[i].normaTAT = (float)processes[i].turnaroundTime / processes[i].serviceTime;
    }
}

void SPN()
{
    sort(processes.begin(), processes.end(), compareService); // sort all process according to their service time
    sort(processes.begin(), processes.end(), compareArrival); // sort all process according to their arrival time
    int cTime = 0, i;
    int arr[processes.size()];
    for (i = 0; i < processes.size(); i++)
    {
        int j = i;
        while (cTime > processes[j].arrivalTime && j != processes.size())
        {
            j++;
        }
        sort(processes.begin() + i, processes.begin() + j, compareService);
        arr[i] = cTime;
        cTime += processes[i].serviceTime;
    }
    arr[i] = cTime;
    for (int i = 0; i < processes.size(); i++)
    {
        processes[i].finishTime = arr[i + 1];
        processes[i].turnaroundTime = arr[i] - processes[i].arrivalTime + processes[i].serviceTime;
        processes[i].waitingTime = arr[i] - processes[i].arrivalTime;
    }
    for (int i = 0; i < processes.size(); i++)
    {
        processes[i].normaTAT = (float)processes[i].turnaroundTime / processes[i].serviceTime;
        int j = 0;
        while (j < processes[i].waitingTime)
        {
            processes[i].trace.push_back('.');
            j++;
        }
        j = 0;
        while (j < processes[i].serviceTime)
        {
            processes[i].trace.push_back('*');
            j++;
        }
    }
}

void SRT()
{
    vector<Process> readyQueue;
    Process currentProcess;
    int i, j, shortestRemaninigTime = 0;
    for (i = 0; i < stoi(lI); i++)
    {
        for (j = 0; j < processes.size(); j++)
        {
            if (processes[j].arrivalTime == i)
            {
                readyQueue.push_back(processes[j]);
            }
        }
        currentProcess = readyQueue[0];
        shortestRemaninigTime = 0;
        for (j = 0; j < readyQueue.size(); j++)
        {
            if (currentProcess.remainingServiceTime > readyQueue[j].remainingServiceTime)
            {
                currentProcess = readyQueue[j];
                shortestRemaninigTime = j;
            }
        }
        readyQueue[shortestRemaninigTime].remainingServiceTime--;
        readyQueue[shortestRemaninigTime].trace.push_back('*');
        for (j = 0; j < readyQueue.size(); j++)
        {
            if (j != shortestRemaninigTime)
            {
                readyQueue[j].waitingTime += 1;
                readyQueue[j].trace.push_back('.');
            }
        }
        if (readyQueue[shortestRemaninigTime].remainingServiceTime == 0)
        {
            readyQueue[shortestRemaninigTime].finishTime = i + 1;
            for (j = 0; j < processes.size(); j++)
            {
                if (processes[j].name == readyQueue[shortestRemaninigTime].name)
                {
                    processes[j] = readyQueue[shortestRemaninigTime];
                }
            }
            readyQueue.erase(readyQueue.begin() + shortestRemaninigTime);
        }
    }
    for (i = 0; i < processes.size(); i++)
    {
        processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
    }

    for (i = 0; i < processes.size(); i++)
    {
        processes[i].normaTAT = (float)processes[i].turnaroundTime / processes[i].serviceTime;
    }
}

void HRRN()
{
    float hrr = -999, tempHRR;
    int ta, sumService = 0, location;
    sort(processes.begin(), processes.end(), compareArrival);

    for (int i = 0; i < processes.size(); i++)
    {
        sumService += processes[i].serviceTime;
    }

    for (ta = processes[0].arrivalTime; ta < sumService;)
    {
        hrr = INT_MIN;
        for (int j = 0; j < processes.size(); j++)
        {
            if ((ta >= processes[j].arrivalTime) && (processes[j].is_complete == 0))
            {
                tempHRR = ((ta - processes[j].arrivalTime) + processes[j].serviceTime) / processes[j].serviceTime;
                if (tempHRR > hrr)
                {
                    location = j;
                    hrr = tempHRR;
                }
            }
        }
        ta = ta + processes[location].serviceTime;
        processes[location].finishTime = ta;
        processes[location].waitingTime = processes[location].finishTime - processes[location].arrivalTime - processes[location].serviceTime;
        processes[location].turnaroundTime = processes[location].finishTime - processes[location].arrivalTime;
        processes[location].is_complete = 1;
    }
    for (int i = 0; i < processes.size(); i++)
    {
        processes[i].normaTAT = (float)processes[i].turnaroundTime / processes[i].serviceTime;
        int j = 0;
        while (j < processes[i].waitingTime)
        {
            processes[i].trace.push_back('.');
            j++;
        }
        j = 0;
        while (j < processes[i].serviceTime)
        {
            processes[i].trace.push_back('*');
            j++;
        }
    }
}

void FBi(int quantumFbi)
{
    // vectors initializations
    vector<vector<Process>> queues;

    vector<Process> queue1;
    vector<Process> queue2;
    vector<Process> queue3;
    vector<Process> queue4;
    vector<Process> queue5;

    queues.push_back(queue1);
    queues.push_back(queue2);
    queues.push_back(queue3);
    queues.push_back(queue4);
    queues.push_back(queue5);

    int i, j, currentQueue = 0, quantumFlag = 1, numberOfProcesses = 0, switchProcess = 0;
    vector<Process> completedProcesses;
    Process currentProcess;

    for (i = 0; i < stoi(lI); i++)
    {
        for (j = 0; j < processes.size(); j++)
        {
            if (processes[j].arrivalTime == i)
            {
                queues[0].push_back(processes[j]);
                numberOfProcesses++;
            }
        }
        // checker to whether change the currrent Process or not
        if (i != 0)
        {
            if (quantumFlag < pow(quantumFbi, currentQueue) && currentProcess.remainingServiceTime > 0)
            {
                quantumFlag++;
            }
            else if (currentProcess.remainingServiceTime > 0 && numberOfProcesses > 1 && quantumFlag == pow(quantumFbi, currentQueue))
            {
                currentQueue = currentQueue == 4 ? 4 : currentQueue + 1;
                queues[currentQueue].push_back(currentProcess);
                switchProcess = 1;
            }
            else if (currentProcess.remainingServiceTime == 0)
            {
                currentProcess.finishTime = i;
                completedProcesses.push_back(currentProcess);
                switchProcess = 1;
            }
        }
        // when i = 0 then put current process to first process
        else
        {
            currentProcess = queues[0][0];
            queues[0].erase(queues[0].begin());
            currentQueue = 0;
        }

        if (switchProcess)
        {
            for (j = 0; j < 5; j++)
            {
                if (queues[j].size() != 0)
                {
                    currentProcess = queues[j][0];
                    queues[j].erase(queues[j].begin());
                    currentQueue = j;
                    break;
                }
            }
            quantumFlag = 1;
            switchProcess = 0;
        }
        // execute
        currentProcess.remainingServiceTime--;
        currentProcess.trace.push_back('*');
        // wait
        for (j = 0; j < 5; j++)
        {
            for (int k = 0; k < queues[j].size(); k++)
            {
                queues[j][k].waitingTime++;
                queues[j][k].trace.push_back('.');
            }
        }
    }
    currentProcess.finishTime = i;
    completedProcesses.push_back(currentProcess);
    for (i = 0; i < processes.size(); i++)
    {
        for (j = 0; j < completedProcesses.size(); j++)
        {
            if (processes[i].name == completedProcesses[j].name)
            {
                processes[i] = completedProcesses[j];
            }
        }
    }
    // calculating the turnaround time and normalized turn around time
    for (i = 0; i < processes.size(); i++)
    {
        processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
    }

    for (i = 0; i < processes.size(); i++)
    {
        processes[i].normaTAT = (float)processes[i].turnaroundTime / processes[i].serviceTime;
    }
}

void aging(int quantum)
{
    // we will suppose that the service time is infinity
    vector<Process> readyQueue;
    int i, j;
    Process currentProcess;
    int isCurrentProcessActive = 0, maxPriorityProcess = 0, quantumFlag = 1;
    for (i = 0; i < stoi(lI); i++)
    {
        maxPriorityProcess = 0;
        for (j = 0; j < processes.size(); j++)
        {
            if (processes[j].arrivalTime == i)
            {
                readyQueue.push_back(processes[j]);
            }
        }
        // waiting processes
        for (j = 0; j < readyQueue.size(); j++)
        {
            readyQueue[j].serviceTime++;
            readyQueue[j].waitingTime++;
            readyQueue[j].trace.push_back('.');
        }
        if (isCurrentProcessActive && quantumFlag == quantum)
        {
            for (j = 0; j < processes.size(); j++)
            {
                if (processes[j].name == currentProcess.name)
                {
                    currentProcess.serviceTime = processes[j].serviceTime;
                    currentProcess.waitingTime = processes[j].waitingTime;
                }
            }
            readyQueue.push_back(currentProcess);
            isCurrentProcessActive = 0;
        }
        // choose the highest priority and then give it to the processor
        if (quantumFlag == quantum || i == 0)
        {
            for (j = 0; j < readyQueue.size(); j++)
            {
                if (readyQueue[maxPriorityProcess].serviceTime < readyQueue[j].serviceTime)
                {
                    maxPriorityProcess = j;
                }
                else if (readyQueue[maxPriorityProcess].serviceTime == readyQueue[j].serviceTime)
                {
                    if (readyQueue[maxPriorityProcess].waitingTime < readyQueue[j].waitingTime)
                    {
                        maxPriorityProcess = j;
                    }
                }
            }
            currentProcess = readyQueue[maxPriorityProcess];
            readyQueue.erase(readyQueue.begin() + maxPriorityProcess);
            isCurrentProcessActive = 1;
            currentProcess.trace.push_back('*');
            quantumFlag = 1;
        }
        // execute
        else if (quantumFlag < quantum)
        {
            currentProcess.trace.push_back('*');
            quantumFlag++;
        }
    }
    // copying all data to processes inorder to be printed correctly
    readyQueue.push_back(currentProcess);
    for (j = 0; j < processes.size(); j++)
    {
        for (i = 0; i < readyQueue.size(); i++)
        {
            if (processes[j].name == readyQueue[i].name)
            {
                processes[j] = readyQueue[i];
                processes[j].finishTime = stoi(lI);
            }
        }
    }
}

void cleanProcesses()
{
    for (int i = 0; i < processes.size(); i++)
    {
        processes[i].remainingServiceTime = processes[i].serviceTime;
        processes[i].waitingTime = 0;
        processes[i].trace.erase(processes[i].trace.begin(), processes[i].trace.end());
    }
}

int readingInput()
{
    vector<string> tokens;
    string schedulingAlgos;
    string schedulingAlgo;

    getline(std::cin, mode);
    getline(std::cin, schedulingAlgos);
    stringstream check1(schedulingAlgos);

    while (getline(check1, schedulingAlgo, ','))
    {
        tokens.push_back(schedulingAlgo);
    }
    // reading quantums if any
    for (int i = 0; i < tokens.size(); i++)
    {
        algoType = tokens[i];
        if (algoType.find("-") != string::npos)
        {
            int n = algoType.length();
            char x[n + 1];
            strcpy(x, algoType.c_str());
            AlgoType a1(stoi(&x[0]), stoi(&x[2]));
            /* algoT.push_back(stoi(&x[0]));
             quantum = stoi(&x[2]);*/
            algoT.push_back(a1);
        }
        else
        {
            AlgoType a1(stoi(algoType), 0);
            algoT.push_back(a1);
        }
    }
    // getting last instance of the program
    getline(std::cin, lI);
    lastInstance = stoi(lI);

    getline(std::cin, np);
    numberOfProcesses = stoi(np);

    vector<string> str;
    // initializing processes
    for (int i = 0; i < numberOfProcesses; i++)
    {
        Process p;

        string lineOfProcess;
        getline(std::cin, lineOfProcess);
        stringstream streamLine(lineOfProcess);
        string temp;
        while (getline(streamLine, temp, ','))
        {
            str.push_back(temp);
        }
        p.name = str[0];
        p.arrivalTime = stoi(str[1]);
        p.serviceTime = stoi(str[2]);
        p.remainingServiceTime = p.serviceTime;
        p.finishTime = p.is_complete = p.normaTAT = p.waitingTime = p.normaTAT = 0;
        processes.push_back(p);
        str.clear();
    }
    // executing
    for (int j = 0; j < algoT.size(); j++)
    {
        cleanProcesses();
        switch (algoT[j].algoNumber)
        {
        case 1:
            FCFS();
            outputFile("FCFS", 0);
            break;
        case 2:
            // rrFlag = 1;
            RR(algoT[j].quantum);
            outputFile("RR", algoT[j].quantum);
            break;
        case 3:
            SPN();
            outputFile("SPN", 0);
            break;
        case 4:
            SRT();
            outputFile("SRT", 0);
            break;
        case 5:
            HRRN();
            outputFile("HRRN", 0);
            break;
        case 6:
            FBi(1);
            outputFile("FB-1", 0);
            break;
        case 7:
            FBi(2);
            outputFile("FB-2i", 0);
            break;
        case 8:
            aging(algoT[j].quantum);
            outputFile("Aging", algoT[j].quantum);
            break;
        }
    }
    return 0;
}

int main()
{
    readingInput();

    return 0;
}
